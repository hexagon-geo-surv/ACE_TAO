/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    InconsistentTypeCode.h
//
// = AUTHOR
//
// ******  Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO ORB and the TAO IDL Compiler have been developed by Washington
// University Computer Science's Distributed Object Computing Group.
//
// Information on TAO is available at
//                 http://www.cs.wustl.edu/~schmidt/TAO.html
//
// Modified by Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IDL_INCONSISTENTTYPECODEC_H
#include "ace/pre.h"
#define TAO_IDL_INCONSISTENTTYPECODEC_H

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/Exception.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if !defined (_CORBA_ORB_INCONSISTENTTYPECODE_CH_)
#define _CORBA_ORB_INCONSISTENTTYPECODE_CH_

class TAO_Export CORBA_ORB_InconsistentTypeCode : public CORBA::UserException
{
  // = TITLE
  //   Exception class generated by the TAO IDL compiler.
  //
  // = DESCRIPTION
  //   This exception is thrown in the ORB create_dyn_any(TypeCode) factory
  //   function if the typecode argument is neither a basic type nor a
  //   sequence, enum, array, struct or union.
public:
  CORBA_ORB_InconsistentTypeCode (void);
  // default ctor
  CORBA_ORB_InconsistentTypeCode (const CORBA_ORB_InconsistentTypeCode &);
  // copy ctor
  ~CORBA_ORB_InconsistentTypeCode (void);
  // dtor

  CORBA_ORB_InconsistentTypeCode &operator= (const CORBA_ORB_InconsistentTypeCode &);

  virtual void _raise (void);

  virtual void _tao_encode (TAO_OutputCDR &cdr,
                            CORBA::Environment &) const;
  virtual void _tao_decode (TAO_InputCDR &cdr,
                            CORBA::Environment &);

  static CORBA_ORB_InconsistentTypeCode *_narrow (CORBA::Exception *);

  // = TAO extension
  static CORBA::Exception *_alloc (void);

};

#endif /* end #if !defined */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_MINIMUM_CORBA */

#include "ace/post.h"
#endif /* TAO_IDL_INCONSISTENTTYPECODEC_H */
