#include "tao/PortableServer/Regular_POA.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Regular_POA.inl"
#endif /* ! __ACE_INLINE__ */

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Regular_POA::TAO_Regular_POA (const TAO_Root_POA::String &name,
                                  PortableServer::POAManager_ptr poa_manager,
                  const TAO_POA_Policy_Set &policies,
                  TAO_Root_POA *parent,
                  ACE_Lock &lock,
                  TAO_SYNCH_MUTEX &thread_lock,
                  TAO_ORB_Core &orb_core,
                  TAO_Object_Adapter *object_adapter)
  : TAO_Root_POA (name,
                  poa_manager,
                  policies,
                  parent,
                  lock,
                  thread_lock,
                  orb_core,
                  object_adapter),
     parent_ (parent)
{
}

void
TAO_Regular_POA::remove_from_parent_i ()
{
  // Remove POA from the parent
  if (this->parent_ != nullptr)
    {
      int const result = this->parent_->delete_child (this->name_);
      if (result != 0)
        {
          throw ::CORBA::OBJ_ADAPTER ();
        }
    }
}

CORBA::Boolean
TAO_Regular_POA::root () const
{
  return (parent_ == nullptr);
}

char
TAO_Regular_POA::root_key_type ()
{
  if (this->parent_ != nullptr)
    {
      return TAO_Root_POA::non_root_key_char ();
    }
  else
    {
      return TAO_Root_POA::root_key_type ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
