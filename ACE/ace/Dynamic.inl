// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
ACE_Dynamic::set ()
{
  // ACE_TRACE ("ACE_Dynamic::set");
  this->is_dynamic_ = true;
}

ACE_INLINE bool
ACE_Dynamic::is_dynamic ()
{
  // ACE_TRACE ("ACE_Dynamic::is_dynamic");
  return this->is_dynamic_;
}

ACE_INLINE void
ACE_Dynamic::reset ()
{
  // ACE_TRACE ("ACE_Dynamic::reset");
  this->is_dynamic_ = false;
}

ACE_END_VERSIONED_NAMESPACE_DECL
