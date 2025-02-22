// -*- C++ -*-

/**
 *  @file RT_Transport_Descriptor_Property.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_RT_TRANSPORT_DESCRIPTOR_PROPERTY_H
#define TAO_RT_TRANSPORT_DESCRIPTOR_PROPERTY_H

#include /**/ "ace/pre.h"

#include "tao/RTCORBA/rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

#include "ace/Global_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_RT_Transport_Descriptor_Property
 *
 * @brief Base RT Transport Descriptor Property that can be <insert> ed into the TAO_RT_Transport_Descriptor class.
 */
class TAO_RTCORBA_Export TAO_RT_Transport_Descriptor_Property
{
  friend class TAO_RT_Transport_Descriptor;

public:
  /// Constructor
  TAO_RT_Transport_Descriptor_Property ();

  /// Destructor
  virtual ~TAO_RT_Transport_Descriptor_Property ();

  virtual TAO_RT_Transport_Descriptor_Property *duplicate () = 0;

  virtual CORBA::Boolean is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop) = 0;

protected:
  /// Properties can be chanined using the <next_> pointer;
  TAO_RT_Transport_Descriptor_Property* next_;

private:
  TAO_RT_Transport_Descriptor_Property (const TAO_RT_Transport_Descriptor_Property &) = delete;
  TAO_RT_Transport_Descriptor_Property (TAO_RT_Transport_Descriptor_Property &&) = delete;
  TAO_RT_Transport_Descriptor_Property & operator= (const TAO_RT_Transport_Descriptor_Property &) = delete;
  TAO_RT_Transport_Descriptor_Property & operator= (TAO_RT_Transport_Descriptor_Property &&) = delete;
};

/**
 * @class TAO_RT_Transport_Descriptor_Private_Connection_Property
 *
 * @brief Descriptor Property for Private Connections.
 *
 * Holds info necessary to identify private connections and
 * store/look them up in the Transport Cache.  (For description
 * of private connections see RTCORBA::PrivateTransportPolicy.)
 */
class TAO_RTCORBA_Export TAO_RT_Transport_Descriptor_Private_Connection_Property
  : public TAO_RT_Transport_Descriptor_Property
{
public:
  /// Constructor
  TAO_RT_Transport_Descriptor_Private_Connection_Property ();
  TAO_RT_Transport_Descriptor_Private_Connection_Property (long object_id);

  /// Destructor
  ~TAO_RT_Transport_Descriptor_Private_Connection_Property ();

  /// Init
  void init (long object_id);

  virtual TAO_RT_Transport_Descriptor_Property *duplicate ();

  virtual CORBA::Boolean is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop);

private:
  /**
   * Unique identifier of the object to which private connection
   * identified with this descriptor belongs.  The value of
   * @c object_id_ is the @c TAO_Stub* of the object.
   */
  long object_id_;
};

/*****************************************************************************/

/**
 * @class TAO_RT_Transport_Descriptor_Banded_Connection_Property
 *
 * @brief Descriptor Property for Banded Connections.
 *
 * This property holds the Band information necessary to identify a banded connection.
 */
class TAO_RTCORBA_Export TAO_RT_Transport_Descriptor_Banded_Connection_Property
  : public TAO_RT_Transport_Descriptor_Property
{
public:
  /// Constructor
  TAO_RT_Transport_Descriptor_Banded_Connection_Property ();
  TAO_RT_Transport_Descriptor_Banded_Connection_Property (CORBA::Short low_priority,
                                                          CORBA::Short high_priority);

  /// Destructor
  ~TAO_RT_Transport_Descriptor_Banded_Connection_Property ();

  /// Init
  void init (CORBA::Short low_priority, CORBA::Short high_priority);

  virtual TAO_RT_Transport_Descriptor_Property *duplicate ();

  virtual CORBA::Boolean is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop);

protected:
  /// The low priority of the Band.
  CORBA::Short low_priority_;

  /// The high priority of the Band.
  CORBA::Short high_priority_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

/*****************************************************************************/

#if defined (__ACE_INLINE__)
#include "tao/RTCORBA/RT_Transport_Descriptor_Property.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_RT_TRANSPORT_DESCRIPTOR_PROPERTY_H */
