// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html


#if !defined (_PORTABLEINTERCEPTOR_INTERCEPTOR___CI_)
#define _PORTABLEINTERCEPTOR_INTERCEPTOR___CI_


#endif /* end #if !defined */

// *************************************************************
// Inline operations for exception PortableInterceptor::ForwardRequest
// *************************************************************

// *************************************************************
// Inline operations for exception PortableInterceptor::InvalidSlot
// *************************************************************


#if !defined (_PORTABLEINTERCEPTOR_CURRENT___CI_)
#define _PORTABLEINTERCEPTOR_CURRENT___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_REQUESTINFO___CI_)
#define _PORTABLEINTERCEPTOR_REQUESTINFO___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_CLIENTREQUESTINFO___CI_)
#define _PORTABLEINTERCEPTOR_CLIENTREQUESTINFO___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_SERVERREQUESTINFO___CI_)
#define _PORTABLEINTERCEPTOR_SERVERREQUESTINFO___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_CLIENTREQUESTINTERCEPTOR___CI_)
#define _PORTABLEINTERCEPTOR_CLIENTREQUESTINTERCEPTOR___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_SERVERREQUESTINTERCEPTOR___CI_)
#define _PORTABLEINTERCEPTOR_SERVERREQUESTINTERCEPTOR___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_IORINFO___CI_)
#define _PORTABLEINTERCEPTOR_IORINFO___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_IORINTERCEPTOR___CI_)
#define _PORTABLEINTERCEPTOR_IORINTERCEPTOR___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_POLICYFACTORY___CI_)
#define _PORTABLEINTERCEPTOR_POLICYFACTORY___CI_


#endif /* end #if !defined */

// *************************************************************
// Inline operations for exception PortableInterceptor::ORBInitInfo::DuplicateName
// *************************************************************

// *************************************************************
// Inline operations for exception PortableInterceptor::ORBInitInfo::InvalidName
// *************************************************************


#if !defined (_PORTABLEINTERCEPTOR_ORBINITINFO___CI_)
#define _PORTABLEINTERCEPTOR_ORBINITINFO___CI_


#endif /* end #if !defined */


#if !defined (_PORTABLEINTERCEPTOR_ORBINITIALIZER___CI_)
#define _PORTABLEINTERCEPTOR_ORBINITIALIZER___CI_


#endif /* end #if !defined */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const PortableInterceptor::ForwardRequest &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._rep_id ())
  {
    // now marshal the members (if any)
    if (
      (strm << _tao_aggregate.forward.in ()) &&
      (strm << CORBA::Any::from_boolean (_tao_aggregate.permanent))
    )
      return 1;
    else
      return 0;
  }
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm,PortableInterceptor::ForwardRequest &_tao_aggregate)
{
  // now marshal the members
  if (
    (strm >> _tao_aggregate.forward.out ()) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.permanent))
  )
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const PortableInterceptor::InvalidSlot &_tao_aggregate)
{
  // first marshal the repository ID
  if (strm << _tao_aggregate._rep_id ())
    return 1;
  else
    return 0;
}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &,PortableInterceptor::InvalidSlot&)
{
  return 1;
}

