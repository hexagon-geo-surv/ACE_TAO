// $Id$
//
// ****              Code generated by the                 ****
// ****  Component Integrated ACE ORB (CIAO) CIDL Compiler ****
// CIAO has been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// CIDL Compiler has been developed by:
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about CIAO is available at:
//    http://www.dre.vanderbilt.edu/CIAO

#ifndef CIAO_GLUE_SESSION_CONTROLLER_SVNT_H
#define CIAO_GLUE_SESSION_CONTROLLER_SVNT_H

#include /**/ "ace/pre.h"

#include "ControllerEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Container_Base.h"
#include "ciao/Context_Impl_T.h"
#include "ciao/Servant_Impl_T.h"
#include "ciao/Home_Servant_Impl_T.h"

#include "ControllerS.h"

namespace CIAO_FACET_CIAO_RACE
{
  template <typename T>
  class Descriptors_Servant_T
  : public virtual POA_CIAO::RACE::Descriptors
  {
    public:
    Descriptors_Servant_T (
      ::CIAO::RACE::CCM_Descriptors_ptr executor,
      ::Components::CCMContext_ptr ctx);

    virtual ~Descriptors_Servant_T (void);

    virtual void
    register_string (
      const ::Deployment::PackageConfiguration & pcd,
      const ::Deployment::DeploymentPlan & plan
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( ::CORBA::SystemException));

    virtual void
    unregister_string (
      const char * UUID
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( ::CORBA::SystemException));

    // Get component implementation.
    virtual CORBA::Object_ptr
    _get_component (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( ::CORBA::SystemException));

    protected:
    // Facet executor.
    ::CIAO::RACE::CCM_Descriptors_var executor_;

    // Context object.
    ::Components::CCMContext_var ctx_;
  };

  typedef Descriptors_Servant_T<int> Descriptors_Servant;
}

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {
      class Controller_Servant;

      class CONTROLLER_SVNT_Export Controller_Context
        : public virtual CIAO::Context_Impl<
            ::CIAO::RACE::CCM_Controller_Context,
            Controller_Servant,
            ::CIAO::RACE::Controller,
            ::CIAO::RACE::Controller_var
          >
      {
        public:
        // We will allow the servant glue code we generate to access our state.
        friend class Controller_Servant;

        Controller_Context (
          ::Components::CCMHome_ptr h,
          ::CIAO::Session_Container *c,
          Controller_Servant *sv);

        virtual ~Controller_Context (void);

        // Operations for Controller receptacles and event sources,
        // defined in ::CIAO::RACE::CCM_Controller_Context.

        virtual ::CIAO::TargetManagerExt_ptr
        get_connection_target_mgr_ext (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual ::Deployment::TargetManager_ptr
        get_connection_target_mgr (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual ::CIAO::RACE::Execution_Time_Monitor_ptr
        get_connection_monitor (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        // CIAO-specific.

        static Controller_Context *
        _narrow (
          ::Components::SessionContext_ptr p
          ACE_ENV_ARG_DECL_WITH_DEFAULTS);

        protected:
        // Methods that manage this component's connections and consumers.

        virtual void
        connect_target_mgr_ext (
          ::CIAO::TargetManagerExt_ptr
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::AlreadyConnected,
                         ::Components::InvalidConnection));

        virtual ::CIAO::TargetManagerExt_ptr
        disconnect_target_mgr_ext (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::NoConnection));

        virtual void
        connect_target_mgr (
          ::Deployment::TargetManager_ptr
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::AlreadyConnected,
                         ::Components::InvalidConnection));

        virtual ::Deployment::TargetManager_ptr
        disconnect_target_mgr (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::NoConnection));

        virtual void
        connect_monitor (
          ::CIAO::RACE::Execution_Time_Monitor_ptr
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::AlreadyConnected,
                         ::Components::InvalidConnection));

        virtual ::CIAO::RACE::Execution_Time_Monitor_ptr
        disconnect_monitor (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::NoConnection));

        protected:
        // Simplex target_mgr_ext connection.
        ::CIAO::TargetManagerExt_var
        ciao_uses_target_mgr_ext_;

        // Simplex target_mgr connection.
        ::Deployment::TargetManager_var
        ciao_uses_target_mgr_;

        // Simplex monitor connection.
        ::CIAO::RACE::Execution_Time_Monitor_var
        ciao_uses_monitor_;
      };

      class CONTROLLER_SVNT_Export Controller_Servant
        : public virtual CIAO::Servant_Impl<
            POA_CIAO::RACE::Controller,
            ::CIAO::RACE::CCM_Controller,
            Controller_Context
          >
      {
        public:

        typedef ::CIAO::RACE::CCM_Controller _exec_type;

        Controller_Servant (
          ::CIAO::RACE::CCM_Controller_ptr executor,
          ::Components::CCMHome_ptr h,
          const char *ins_name,
          ::CIAO::Home_Servant_Impl_Base *hs,
          ::CIAO::Session_Container *c,
          ::CIAO::REC_POL_MAP &rec_pol_map);

        virtual ~Controller_Servant (void);

        virtual void
        set_attributes (
          const ::Components::ConfigValues &descr
          ACE_ENV_ARG_DECL);

        // Supported operations.

        virtual void
        start_poller (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual void
        stop_poller (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual void
        start_controller (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual void
        stop_controller (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        // Public port operations.

        virtual void
        connect_target_mgr_ext (
          ::CIAO::TargetManagerExt_ptr c
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::AlreadyConnected,
                         ::Components::InvalidConnection));

        virtual ::CIAO::TargetManagerExt_ptr
        disconnect_target_mgr_ext (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::NoConnection));

        virtual ::CIAO::TargetManagerExt_ptr
        get_connection_target_mgr_ext (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual void
        connect_target_mgr (
          ::Deployment::TargetManager_ptr c
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::AlreadyConnected,
                         ::Components::InvalidConnection));

        virtual ::Deployment::TargetManager_ptr
        disconnect_target_mgr (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::NoConnection));

        virtual ::Deployment::TargetManager_ptr
        get_connection_target_mgr (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual void
        connect_monitor (
          ::CIAO::RACE::Execution_Time_Monitor_ptr c
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::AlreadyConnected,
                         ::Components::InvalidConnection));

        virtual ::CIAO::RACE::Execution_Time_Monitor_ptr
        disconnect_monitor (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::NoConnection));

        virtual ::CIAO::RACE::Execution_Time_Monitor_ptr
        get_connection_monitor (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual ::CIAO::RACE::Descriptors_ptr
        provide_descriptors (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        // Component attribute operations.

        virtual ::CORBA::Long
        sampling_period (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        // Operations for Receptacles interface.

        virtual ::Components::Cookie *
        connect (
          const char *name,
          CORBA::Object_ptr connection
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::InvalidName,
                         ::Components::InvalidConnection,
                         ::Components::AlreadyConnected,
                         ::Components::ExceededConnectionLimit));

        virtual CORBA::Object_ptr
        disconnect (
          const char *name,
          ::Components::Cookie *ck
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::InvalidName,
                         ::Components::InvalidConnection,
                         ::Components::CookieRequired,
                         ::Components::NoConnection));

        virtual ::Components::ReceptacleDescriptions *
        get_all_receptacles (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        // Operations for Events interface.

        virtual ::Components::Cookie *
        subscribe (
          const char *publisher_name,
          ::Components::EventConsumerBase_ptr subscriber
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::InvalidName,
                         ::Components::InvalidConnection,
                         ::Components::ExceededConnectionLimit));

        virtual ::Components::EventConsumerBase_ptr
        unsubscribe (
          const char *publisher_name,
          ::Components::Cookie *ck
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::InvalidName,
                         ::Components::InvalidConnection));

        virtual void
        connect_consumer (
          const char *emitter_name,
          ::Components::EventConsumerBase_ptr consumer
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::InvalidName,
                         ::Components::AlreadyConnected,
                         ::Components::InvalidConnection));

        virtual ::Components::EventConsumerBase_ptr
        disconnect_consumer (
          const char *source_name
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException,
                         ::Components::InvalidName,
                         ::Components::NoConnection));

        virtual ::Components::PublisherDescriptions *
        get_all_publishers (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        virtual ::Components::EmitterDescriptions *
        get_all_emitters (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        // CIAO specific operations on the servant. 
        CORBA::Object_ptr
        get_facet_executor (
          const char *name
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        private:

        ::CIAO::RACE::Descriptors_var
        provide_descriptors_;

        const char *ins_name_;

        private:

        void
        populate_port_tables (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));

        ::CORBA::Object_ptr
        provide_descriptors_i (
          ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC (( ::CORBA::SystemException));
      };

      class CONTROLLER_SVNT_Export Controller_Home_Servant
        : public virtual
            ::CIAO::Home_Servant_Impl<
                ::POA_CIAO::RACE::Controller_Home,
                ::CIAO::RACE::CCM_Controller_Home,
                Controller_Servant
              >
      {
        public:

        Controller_Home_Servant (
          ::CIAO::RACE::CCM_Controller_Home_ptr exe,
          const char *ins_name,
          ::CIAO::Session_Container *c,
          ::CIAO::REC_POL_MAP &rec_pol_map);

        virtual ~Controller_Home_Servant (void);

        // Home operations.
        // Home factory and finder operations.

        // Attribute operations.
      };

      extern "C" CONTROLLER_SVNT_Export ::PortableServer::Servant
      create_CIAO_RACE_Controller_Home_Servant (
        ::Components::HomeExecutorBase_ptr p,
        CIAO::Session_Container *c,
        const char *ins_name,
        ::CIAO::REC_POL_MAP &rec_pol_map
        ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_GLUE_SESSION_CONTROLLER_SVNT_H */

