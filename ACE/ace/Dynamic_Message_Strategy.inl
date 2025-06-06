// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE unsigned long
ACE_Dynamic_Message_Strategy::static_bit_field_mask () const
{
  return static_bit_field_mask_;
}
  // get static bit field mask

ACE_INLINE void
ACE_Dynamic_Message_Strategy::static_bit_field_mask (unsigned long ul)
{
  static_bit_field_mask_ = ul;
}
  // set static bit field mask

ACE_INLINE unsigned long
ACE_Dynamic_Message_Strategy::static_bit_field_shift () const
{
  return static_bit_field_shift_;
}
  // get left shift value to make room for static bit field

ACE_INLINE void
ACE_Dynamic_Message_Strategy::static_bit_field_shift (unsigned long ul)
{
  static_bit_field_shift_ = ul;
}
  // set left shift value to make room for static bit field

ACE_INLINE unsigned long
ACE_Dynamic_Message_Strategy::dynamic_priority_max () const
{
  return dynamic_priority_max_;
}
  // get maximum supported priority value

ACE_INLINE void
ACE_Dynamic_Message_Strategy::dynamic_priority_max (unsigned long ul)
{
  // pending_shift_ depends on dynamic_priority_max_: for performance
  // reasons, the value in pending_shift_ is (re)calculated only when
  // dynamic_priority_max_ is initialized or changes, and is stored
  // as a class member rather than being a derived value.
  dynamic_priority_max_ = ul;
  pending_shift_ = ACE_Time_Value (0, static_cast<suseconds_t> (ul));
}
  // set maximum supported priority value

ACE_INLINE unsigned long
ACE_Dynamic_Message_Strategy::dynamic_priority_offset () const
{
  return dynamic_priority_offset_;
}
  // get offset for boundary between signed range and unsigned range

ACE_INLINE void
ACE_Dynamic_Message_Strategy::dynamic_priority_offset (unsigned long ul)
{
  // max_late_ and min_pending_ depend on dynamic_priority_offset_:
  // for performance reasons, the values in max_late_ and min_pending_
  // are (re)calculated only when dynamic_priority_offset_ is
  // initialized or changes, and are stored as a class member rather
  // than being derived each time one of their values is needed.
  dynamic_priority_offset_ = ul;
  max_late_ = ACE_Time_Value (0, static_cast<suseconds_t> (ul - 1));
  min_pending_ = ACE_Time_Value (0, static_cast<suseconds_t> (ul));
}
  // set offset for boundary between signed range and unsigned range

ACE_END_VERSIONED_NAMESPACE_DECL
