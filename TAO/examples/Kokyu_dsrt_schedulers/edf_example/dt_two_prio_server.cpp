//$Id$ 

#include "test1_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "EDF_Scheduler.h"
#include "Task_Stats.h"
#include "orbsvcs/orbsvcs/Time_Utilities.h"

#include <dsui.h>

ACE_RCSID(MT_Server, server, "server.cpp,v 1.3 2003/10/14 05:57:01 jwillemsen Exp")

  const char *ior_output_file = "test1.ior";

int nthreads = 1;
int enable_dynamic_scheduling = 1;
const CORBA::Short max_importance = 100;
int enable_yield = 1;
int workload1 = 2;
int period1 = 3;
int niteration1 = 5;
int enable_rand = 0;

class Worker_c : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker_c (CORBA::ORB_ptr orb,
            RTScheduling::Current_ptr current,
            EDF_Scheduler* scheduler,
            long importance,
            CORBA::Long server_load,
            CORBA::Long period,
            CORBA::Long niteration,
            int worker_id);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb

  RTScheduling::Current_var scheduler_current_;
  EDF_Scheduler* scheduler_;
  long importance_;
  CORBA::Long server_load_;
  CORBA::Long period_;
  CORBA::Long niteration_;
  int sleep_time_;
  unsigned int m_id;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        enable_yield = 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
			   "-n (thread num)"
                           "-s (disable yield)"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (CORBA::ORB_ptr orb);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

Task_Stats task_stats;

int
main (int argc, char *argv[])
{

  ds_control ds_cntrl ("DT_Oneway_Server", "dt_enable.dsui");

  ACE_DEBUG((LM_DEBUG,"FISRT LINE\n"));
  EDF_Scheduler* scheduler = 0;
  RTScheduling::Current_var current;
  long flags;
  int sched_policy = ACE_SCHED_RR;
  int sched_scope = ACE_SCOPE_THREAD;

  if (sched_policy == ACE_SCHED_RR)
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  else
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;

  task_stats.init (100000);

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      if (enable_dynamic_scheduling)
        {
          CORBA::Object_ptr manager_obj =
            orb->resolve_initial_references ("RTSchedulerManager"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type;
          if (enable_yield)
            {
              disp_impl_type = Kokyu::DSRT_CV_BASED;
            }
          else
            {
              disp_impl_type = Kokyu::DSRT_OS_BASED;
            }

          ACE_NEW_RETURN (scheduler,
                          EDF_Scheduler (orb.in (),
                                         disp_impl_type,
                                         sched_policy,
                                         sched_scope), -1);

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          current  =
            RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      Simple_Server1_i server_impl (orb.in (),
                                    current.in (),
                                    task_stats,
                                    enable_yield);


      Simple_Server1_var server =
        server_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      CPULoad::calibrate(10);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /*
      int importance=0;
              Worker_c worker1 (orb.in (),
              current.in (),
              scheduler,
              importance,
              workload1,
              period1,
              niteration1,
              3);

              if (worker1.activate (flags, 1, 0, ACE_Sched_Params::priority_max(sched_policy, sched_scope)) != 0)
              {
              ACE_ERROR ((LM_ERROR,
              "(%t|%T) cannot activate worker thread.\n"));
              }
      */
      //      TAO_debug_level = 1;
      Worker worker (orb.in ());

      if (worker.activate (flags,
                           nthreads,
                           0,
                           ACE_Sched_Params::priority_max(sched_policy,
                                                          sched_scope)) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate threads in RT class.",
                      "Trying to activate in non-RT class\n"));

          flags = THR_NEW_LWP | THR_JOINABLE | THR_BOUND;
          if (worker.activate (flags, nthreads) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot activate server threads\n"),
                                1);
            }
        }

      //      worker1.wait ();

      worker.wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      ACE_DEBUG ((LM_DEBUG, "shutting down scheduler\n"));
      scheduler->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "Exiting main...\n"));
  //  task_stats.dump_samples ("timeline.txt",
  //                            "Time\t\tGUID",
  //                            ACE_High_Res_Timer::global_scale_factor ());
  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_Time_Value tv(5000);

  ACE_TRY
    {
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): Worker thread exiting...\n"));
  return 0;
}

//--------------------------------------------------------------
Worker_c::Worker_c (CORBA::ORB_ptr orb,
                    RTScheduling::Current_ptr current,
                    EDF_Scheduler* scheduler,
                    long importance,
                    CORBA::Long server_load,
                    CORBA::Long period,
                    CORBA::Long niteration,
                    int worker_id)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     importance_ (importance),
     server_load_ (server_load),
     period_(period),
     niteration_(niteration),
     m_id (worker_id)
  //     sleep_time_ (sleep_time)
{
}

int
Worker_c::svc (void)
{
  /* MEASURE: Worker start time */
  //  DSUI_EVENT_LOG (WORKER_GROUP_FAM, WORKER_STARTED, m_id, 0, NULL);

  ACE_DECLARE_NEW_CORBA_ENV;
  const char * name = 0;
  
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):about to sleep for %d sec\n", sleep_time_));
  ACE_OS::sleep (17);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T):woke up from sleep for %d sec\n", sleep_time_));
 
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("getprio not supported\n")
                     ));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n")
                      ACE_TEXT ("thr_getprio failed")));
        }
    }

  //  ACE_DEBUG ((LM_DEBUG, "(%t|%T) worker activated with prio %d AND iteration is %d\n", prio,niteration_));

  EDF_Scheduling::SchedulingParameter sched_param;
  CORBA::Policy_var sched_param_policy;
  CORBA::Policy_var implicit_sched_param;
  double rand2=0.0;

  if (enable_dynamic_scheduling)
    {
      sched_param.importance = importance_;
      ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (period_,0) );
      sched_param.period = period_*10000000;
      sched_param.task_id = 4;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;

      /* MEASURE: Start of scheduling segment */
      //      DSUI_EVENT_LOG (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, m_id, 0, NULL);
      //      ACE_DEBUG ((LM_DEBUG, "(%t|%T):before begin_sched_segment\n"));

      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      /* MEASURE: End of scheduling segment */
      //      DSUI_EVENT_LOG (WORKER_GROUP_FAM, END_SCHED_SEGMENT, m_id, 0, NULL);
      //      ACE_DEBUG ((LM_DEBUG, "(%t|%T):after begin_sched_segment\n"));
    }

  ACE_Time_Value start_t, repair_t;
  repair_t=ACE_Time_Value(0,0);

  timeval tv;

  tv.tv_sec = server_load_-1;
  tv.tv_usec = 800000;

  for(int i=0;i<niteration_;i++)
    {

      ACE_DEBUG ((LM_DEBUG, "(%t|%T): Local Task begin to run!\n"));
      if(i>0 && enable_dynamic_scheduling)
        {
          /*      if(enable_rand)
                  ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                  sched_param.deadline +
                  ACE_Time_Value (period_,0) -
                  ACE_Time_Value (0,rand) );
                  else

                  ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                  sched_param.deadline +
                  ACE_Time_Value (period_,0) );
          */
          sched_param.deadline = sched_param.deadline+period_*10000000;
          sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

          //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
          implicit_sched_param = sched_param_policy;
          //      DSUI_EVENT_LOG (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_BEGIN, m_id, 0, NULL);
          scheduler_current_->update_scheduling_segment(name,
                                                        sched_param_policy.in (),
                                                        implicit_sched_param.in ()
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          //      DSUI_EVENT_LOG (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_END, m_id, 0, NULL);
        }

      if (i==0)
        start_t =  ACE_OS::gettimeofday ();
      else {
        repair_t = start_t+ACE_Time_Value(period_*i,0)-ACE_OS::gettimeofday ();
      }


      CPULoad::run(tv);

      scheduler_->kokyu_dispatcher_->update_schedule (*(scheduler_current_->id ()),
                                                      Kokyu::BLOCK);
      rand2 = 0.1*rand()/RAND_MAX;
      if(enable_rand)
        {
          int sleep_t = period_*1000000-((int)(period_*rand2*1000000))+repair_t.sec()*1000000+repair_t.usec();
          if(sleep_t > 0)
            {
              ACE_DEBUG((LM_DEBUG,"NOW I AM GOING TO SLEEP FOR %d.\n",
                         (int)(period_*1000000-period_*rand2*1000000)));
              usleep(sleep_t);
            }
          else
            {
              ACE_DEBUG((LM_DEBUG,"NOW I AM GOING TO SLEEP FOR %d\n", 0));
            }
        }
      else
        {
          ACE_Time_Value current = ACE_OS::gettimeofday ();
          int sleep_t = sched_param.deadline/10-current.sec()*1000000-current.usec();
          ACE_DEBUG((LM_DEBUG,"(%t|%T)NOW I AM GOING TO SLEEP FOR %d\n", sleep_t));

          usleep(sleep_t);
        }
    }
  if (enable_dynamic_scheduling)
    {
      scheduler_current_->end_scheduling_segment (name);
      ACE_CHECK_RETURN (-1);
    }

  ACE_DEBUG ((LM_DEBUG, "client worker thread (%t) done\n"));

  return 0;
}

