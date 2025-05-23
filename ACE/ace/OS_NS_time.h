// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_time.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_NS_TIME_H
#define ACE_OS_NS_TIME_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Basic_Types.h"
#include "ace/os_include/os_time.h"
#include "ace/OS_NS_errno.h"

#include /**/ "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Type-safe, and unsigned.
static constexpr ACE_UINT32 ACE_U_ONE_SECOND_IN_MSECS = 1000U;
static constexpr ACE_UINT32 ACE_U_ONE_SECOND_IN_USECS = 1000000U;
static constexpr ACE_UINT32 ACE_U_ONE_SECOND_IN_NSECS = 1000000000U;

/// Helper for the ACE_OS::timezone() function
/**
 * We put all the timezone stuff that used to be in ACE_OS::timezone()
 * here because on some platforms "timezone" is a macro.  Because of this,
 * the name ACE_OS::timezone will cause errors.  So in order to use the
 * macro as it is defined but also keep the name ACE_OS::timezone, we
 * use timezone first here in this inline function, and then undefine
 * timezone.
 */
inline long ace_timezone ()
{
#if defined (ACE_WIN32)
  TIME_ZONE_INFORMATION tz;
  GetTimeZoneInformation (&tz);
  return tz.Bias * 60;
#elif defined (ACE_HAS_TIMEZONE)
  // The XPG/POSIX specification requires that tzset() be called to
  // set the global variable <timezone>.
  ::tzset();
  return timezone;
#elif defined (ACE_HAS_TIMEZONE_GETTIMEOFDAY)
  // The XPG/POSIX specification does not require gettimeofday to
  // set the timezone struct (it leaves the behavior of passing a
  // non-null struct undefined).
  long result = 0;
  struct timeval time;
  struct timezone zone;
  ACE_UNUSED_ARG (result);
  ACE_OSCALL (::gettimeofday (&time, &zone), int, result);
  return zone.tz_minuteswest * 60;
#else
  ACE_NOTSUP_RETURN (0);
#endif
}

/*
 * We inline and undef some functions that may be implemented
 * as macros on some platforms. This way macro definitions will
 * be usable later as there is no way to save the macro definition
 * using the pre-processor.
 */
#if !defined (ACE_LACKS_ASCTIME_R)
inline char *ace_asctime_r_helper (const struct tm *t, char *buf)
{
#  if defined (asctime_r)
  return asctime_r (t, buf);
#    undef asctime_r
#  else
  return ::asctime_r (t, buf);
#  endif /* asctime_r */
}
#endif /* !ACE_LACKS_ASCTIME_R */

#if !defined (ACE_LACKS_GMTIME_R)
inline struct tm *ace_gmtime_r_helper (const time_t *clock, struct tm *res)
{
#  if defined (gmtime_r)
  return gmtime_r (clock, res);
#    undef gmtime_r
#  else
  return ::gmtime_r (clock, res);
#  endif /* gmtime_r */
}
#endif /* !ACE_LACKS_GMTIME_R */

#if !defined (ACE_LACKS_LOCALTIME_R)
inline struct tm *ace_localtime_r_helper (const time_t *clock, struct tm *res)
{
#  if defined (localtime_r)
  return localtime_r (clock, res);
#    undef localtime_r
#  else
  return ::localtime_r (clock, res);
#  endif /* localtime_r */
}
#endif /* !ACE_LACKS_LOCALTIME_R */

/// Helper for the ACE_OS::difftime() function
/**
 * We moved the difftime code that used to be in ACE_OS::difftime()
 * here because on some platforms "difftime" is a macro.  Because of this,
 * the name ACE_OS::difftime will cause errors.  So in order to use the
 * macro as it is defined but also keep the name ACE_OS::difftime, we
 * use difftime first here in this inline function, and then undefine
 * it.
 */
inline double ace_difftime (time_t t1, time_t t0)
{
#if defined (difftime)
  return difftime (t1, t0);
#  undef difftime
#else
  return ::difftime (t1, t0);
#endif
}

#if defined (ACE_WIN32)
typedef unsigned __int64 ACE_hrtime_t;
#elif defined (_TNS_R_TARGET)
typedef long long ACE_hrtime_t;
#else /* !ACE_WIN32 */
#  if defined (ACE_HAS_HI_RES_TIMER)
  /* hrtime_t is defined on systems (Suns) with ACE_HAS_HI_RES_TIMER */
  typedef hrtime_t ACE_hrtime_t;
#  else  /* ! ACE_HAS_HI_RES_TIMER */
  typedef ACE_UINT64 ACE_hrtime_t;
#  endif /* ! ACE_HAS_HI_RES_TIMER */
#endif /* ACE_WIN32 */

#define ACE_HRTIME_CONVERSION(VAL) (VAL)
#define ACE_HRTIME_TO_U64(VAL) (VAL)

namespace ACE_OS
{
  enum ACE_HRTimer_Op
  {
    ACE_HRTIMER_START = 0x0,  // Only use these if you can stand
    ACE_HRTIMER_INCR = 0x1,   // for interrupts to be disabled during
    ACE_HRTIMER_STOP = 0x2,   // the timed interval!!!!
    ACE_HRTIMER_GETTIME = 0xFFFF
  };

  //@{ @name A set of wrappers for operations on time.

  ACE_NAMESPACE_INLINE_FUNCTION
  char *asctime (const struct tm *tm);

  ACE_NAMESPACE_INLINE_FUNCTION
  char *asctime_r (const struct tm *tm,
                   char *buf, int buflen);

  ACE_NAMESPACE_INLINE_FUNCTION
  int clock_gettime (clockid_t,
                     struct timespec *);

  ACE_NAMESPACE_INLINE_FUNCTION
  int clock_settime (clockid_t,
                     const struct timespec *);

  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_TCHAR *ctime (const time_t *t);

  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_TCHAR *ctime_r (const time_t *clock, ACE_TCHAR *buf, int buflen);

  ACE_NAMESPACE_INLINE_FUNCTION
  double difftime (time_t t1,
                   time_t t0);

  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_hrtime_t gethrtime (const ACE_HRTimer_Op = ACE_HRTIMER_GETTIME);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct tm *gmtime (const time_t *clock);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct tm *gmtime_r (const time_t *clock,
                       struct tm *res);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct tm *localtime (const time_t *clock);

  extern ACE_Export
  struct tm *localtime_r (const time_t *clock,
                          struct tm *res);

#if defined (ACE_USES_ULONG_FOR_STAT_TIME)
  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_TCHAR *ctime (const unsigned long *t);

  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_TCHAR *ctime_r (const unsigned long *clock, ACE_TCHAR *buf, int buflen);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct tm *gmtime (const unsigned long *clock);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct tm *gmtime_r (const unsigned long *clock,
                       struct tm *res);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct tm *localtime (const unsigned long *clock);

  ACE_NAMESPACE_INLINE_FUNCTION
  struct tm *localtime_r (const unsigned long *clock,
                          struct tm *res);
#endif


  // Get the current time.
  extern ACE_Export
  time_t mktime (struct tm *timeptr);

  ACE_NAMESPACE_INLINE_FUNCTION
  int nanosleep (const struct timespec *requested,
                 struct timespec *remaining = nullptr);

  ACE_NAMESPACE_INLINE_FUNCTION
  size_t strftime (char *s,
                   size_t maxsize,
                   const char *format,
                   const struct tm *timeptr)
    ACE_GCC_FORMAT_ATTRIBUTE (strftime, 3, 0);

  /**
   * strptime wrapper. Note that the struct @a tm will always be set to
   * zero
   */
  ACE_NAMESPACE_INLINE_FUNCTION
  char *strptime (const char *buf,
                  const char *format,
                  struct tm *tm);

#if defined (ACE_LACKS_STRPTIME)
  extern ACE_Export
  char *strptime_emulation (const char *buf,
                            const char *format,
                            struct tm *tm);

  extern ACE_Export
  int strptime_getnum (const char *buf, int *num, int *bi,
                       int *fi, int min, int max);
#endif /* ACE_LACKS_STRPTIME  */

  ACE_NAMESPACE_INLINE_FUNCTION
  time_t time (time_t *tloc = nullptr);

  ACE_NAMESPACE_INLINE_FUNCTION
  long timezone ();

  // wrapper for time zone information.
  ACE_NAMESPACE_INLINE_FUNCTION
  void tzset ();

  //@}
} /* namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL

#if (defined (ACE_HAS_VERSIONED_NAMESPACE) && ACE_HAS_VERSIONED_NAMESPACE == 1) \
     && defined (__ghs__) \
     && defined (ACE_HAS_PENTIUM) \
     && !defined (ACE_WIN32)
#  define ACE_GETHRTIME_NAME ACE_PREPROC_CONCATENATE (ACE_, ACE_PREPROC_CONCATENATE (ACE_VERSIONED_NAMESPACE_NAME, _gethrtime))
#else
#  define ACE_GETHRTIME_NAME ACE_gethrtime
#endif  /* ACE_HAS_VERSIONED_NAMESPACE == 1 */


#if defined (ACE_HAS_INLINED_OSCALLS)
#  if defined (ACE_INLINE)
#    undef ACE_INLINE
#  endif /* ACE_INLINE */
#  define ACE_INLINE inline
#  include "ace/OS_NS_time.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */

#include /**/ "ace/post.h"
#endif /* ACE_OS_NS_TIME_H */
