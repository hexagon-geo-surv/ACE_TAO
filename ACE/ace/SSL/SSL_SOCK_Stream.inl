// -*- C++ -*-
#include "ace/OS_NS_errno.h"
#include "ace/Truncate.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
ACE_SSL_SOCK_Stream::set_handle (ACE_HANDLE fd)
{
  if (this->ssl_ == 0 || fd == ACE_INVALID_HANDLE)
    {
      this->ACE_SSL_SOCK::set_handle (ACE_INVALID_HANDLE);
      return;
    }
  else
    {
      (void) ::SSL_set_fd (this->ssl_, (int) fd);
      this->ACE_SSL_SOCK::set_handle (fd);
      this->stream_.set_handle (fd);
    }
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::send_i (const void *buf,
                             size_t n,
                             int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_i");

  // NOTE: Caller must provide thread-synchronization.

  // No send flags are supported in SSL.
  if (flags != 0)
    {
      ACE_NOTSUP_RETURN (-1);
    }

  int const bytes_sent = ::SSL_write (this->ssl_,
                                      static_cast<const char *> (buf),
                                      ACE_Utils::truncate_cast<int> (n));

  switch (::SSL_get_error (this->ssl_, bytes_sent))
    {
    case SSL_ERROR_NONE:
      return bytes_sent;

    case SSL_ERROR_WANT_READ:
    case SSL_ERROR_WANT_WRITE:
      errno = EWOULDBLOCK;

      return -1;

    case SSL_ERROR_ZERO_RETURN:
      // The peer has notified us that it is shutting down via the SSL
      // "close_notify" message so we need to shutdown, too.
      (void) ::SSL_shutdown (this->ssl_);

      return bytes_sent;

    case SSL_ERROR_SYSCALL:
      if (bytes_sent == 0)
        // An EOF occurred but the SSL "close_notify" message was not
        // sent.  This is a protocol error, but we ignore it.
        return 0;

      // If not an EOF, then fall through to "default" case.

      // On some platforms (e.g. MS Windows) OpenSSL does not store
      // the last error in errno so explicitly do so.
      ACE_OS::set_errno_to_last_error ();

      break;

    default:
      // Reset errno to prevent previous values (e.g. EWOULDBLOCK)
      // from being associated with fatal SSL errors.
      errno = 0;

      ACE_SSL_Context::report_error ();

      break;
    }

  return -1;
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t n,
                           int flags) const
{
  return this->send_i (buf, n, flags);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv_i (void *buf,
                             size_t n,
                             int flags,
                             const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_i");

  // NOTE: Caller must provide thread-synchronization.
  int bytes_read = 0;
  ACE_HANDLE const handle = this->get_handle ();

  // Value for current I/O mode (blocking/non-blocking)
  int val = 0;

  if (timeout != 0)
    ACE::record_and_set_non_blocking_mode (handle,
                                           val);

  // Only block to wait for I/O ready with a timeout if all on-hand data
  // has been consumed. If there is remaining data in the SSL buffers
  // the socket won't "select" even though SSL_read would complete.
  // See "SSL and TLS" by Rescorla section 8.9 for more info.
  bool peeking = false;
  bool retry = false;
  if (flags)
    {
      if (ACE_BIT_ENABLED (flags, MSG_PEEK))
        {
          peeking = true;
        }
      else
        {
          ACE_NOTSUP_RETURN (-1);
        }
    }

  do
    {
      retry = false;
      if (peeking)
        {
          bytes_read = ::SSL_peek (this->ssl_,
                                   static_cast<char *> (buf),
                                   ACE_Utils::truncate_cast<int> (n));
        }
      else
        {
          bytes_read = ::SSL_read (this->ssl_,
                                   static_cast<char *> (buf),
                                   ACE_Utils::truncate_cast<int> (n));
        }

      int const status = ::SSL_get_error (this->ssl_, bytes_read);
      int substat = 0;
      switch (status)
        {
        case SSL_ERROR_NONE:
          break;

        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
          if (timeout == 0)
            {
              errno = EWOULDBLOCK;
              bytes_read = -1;
              break;
            }
          substat = ACE::handle_ready (handle,
                                       timeout,
                                       status == SSL_ERROR_WANT_READ,
                                       status == SSL_ERROR_WANT_WRITE,
                                       false);
          if (substat == 1)   // Now ready to proceed
            {
              retry = true;
              break;
            }
          bytes_read = -1;
          if (substat == 0)
            errno = ETIME;
          break;

        case SSL_ERROR_ZERO_RETURN:
          bytes_read = 0;

          // The peer has notified us that it is shutting down via the SSL
          // "close_notify" message so we need to shutdown, too.
          (void) ::SSL_shutdown (this->ssl_);

          break;

        case SSL_ERROR_SYSCALL:
          if (bytes_read == 0)
            // An EOF occurred but the SSL "close_notify" message was not
            // sent.  This is a protocol error, but we ignore it.
            break;

          // On some platforms (e.g. MS Windows) OpenSSL does not store
          // the last error in errno so explicitly do so.
          ACE_OS::set_errno_to_last_error ();

          break;

        default:
          // Reset errno to prevent previous values (e.g. EWOULDBLOCK)
          // from being associated with a fatal SSL error.
          bytes_read = -1;
          errno = 0;

          ACE_SSL_Context::report_error ();

          break;
        }
    } while (retry);

  if (timeout != 0)
    ACE::restore_non_blocking_mode (handle, val);
  return bytes_read;
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           int flags) const
{
  return this->recv_i (buf, n, flags, 0);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");

  return this->send_i (buf, n, 0);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  return this->recv_i (buf, n, 0, 0);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t len,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");
  return this->send (buf, len, 0, timeout);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");
  return this->recv (buf, n, 0, timeout);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv_n (void *buf, int buf_size) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_n");
  return this->recv_n (buf, buf_size, 0);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv_n (void *buf,
                             size_t len,
                             const ACE_Time_Value *timeout,
                             size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_n");
  return this->recv_n (buf, len, 0, timeout, bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::send_n (const void *buf, int len) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");
  return this->send_n (buf, len, 0);
}

ACE_INLINE ssize_t
ACE_SSL_SOCK_Stream::send_n (const void *buf,
                             size_t len,
                             const ACE_Time_Value *timeout,
                             size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");
  return this->send_n (buf, len, 0, timeout, bytes_transferred);
}

ACE_INLINE int
ACE_SSL_SOCK_Stream::close_reader ()
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close_reader");
  return this->stream_.close_reader ();
}

ACE_INLINE int
ACE_SSL_SOCK_Stream::close_writer ()
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close_writer");
  return this->stream_.close_writer ();
}

ACE_INLINE int
ACE_SSL_SOCK_Stream::close ()
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close");

  if (this->ssl_ == 0 || this->get_handle () == ACE_INVALID_HANDLE)
    return 0;  // SSL_SOCK_Stream was never opened.

  // SSL_shutdown() returns 1 on successful shutdown of the SSL
  // connection, not 0.
  int const status = ::SSL_shutdown (this->ssl_);

  switch (::SSL_get_error (this->ssl_, status))
    {
    case SSL_ERROR_NONE:
      // Reset the SSL object to allow another connection to be made
      // using this ACE_SSL_SOCK_Stream instance.  This prevents the
      // previous SSL session state from being associated with the new
      // SSL session/connection.
      ::SSL_clear (this->ssl_);
      this->set_handle (ACE_INVALID_HANDLE);
      return this->stream_.close ();

    case SSL_ERROR_WANT_READ:
    case SSL_ERROR_WANT_WRITE:
      errno = EWOULDBLOCK;
      break;

    case SSL_ERROR_SSL:
    case SSL_ERROR_SYSCALL:
    default:
      ACE_SSL_Context::report_error ();

      this->set_handle (ACE_INVALID_HANDLE);
      ACE_Errno_Guard error (errno);   // Save/restore errno
      this->stream_.close ();
      break;
    }

  return -1;
}

ACE_INLINE ACE_SOCK_Stream &
ACE_SSL_SOCK_Stream::peer ()
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::peer");
  return this->stream_;
}

ACE_INLINE SSL *
ACE_SSL_SOCK_Stream::ssl () const
{
  return this->ssl_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
