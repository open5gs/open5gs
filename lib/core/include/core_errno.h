/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CORE_ERRNO_H__
#define __CORE_ERRNO_H__

/**
 * @file core_errno.h
 * @brief CORE Error Codes
 */

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup core_errno Error Codes
 * @ingroup CORE
 * @{
 */

/**
 * Type for specifying an error or status code.
 */
typedef int status_t;

/**
 * Return a human readable string describing the specified error.
 * @param statcode The error code the get a string for.
 * @param buf A buffer to hold the error string.
 * @param bufsize Size of the buffer to hold the string.
 */
CORE_DECLARE(char *) core_strerror(status_t statcode, char *buf,
                                 size_t bufsize);

#if defined(DOXYGEN)
/**
 * @def FROM_OS_ERROR(os_err_type syserr)
 * Fold a platform specific error into an STATUS_T code.
 * @return STATUS_T
 * @param e The platform os error code.
 * @warning  macro implementation; the syserr argument may be evaluated
 *      multiple times.
 */
#define FROM_OS_ERROR(e) (e == 0 ? CORE_OK : e + OS_START_SYSERR)

/**
 * @def TO_OS_ERROR(STATUS_T statcode)
 * @return os_err_type
 * Fold an STATUS_T code back to the native platform defined error.
 * @param e The STATUS_T folded platform os error code.
 * @warning  macro implementation; the statcode argument may be evaluated
 *      multiple times.  If the statcode was not created by get_os_error
 *      or FROM_OS_ERROR, the results are undefined.
 */
#define TO_OS_ERROR(e) (e == 0 ? CORE_OK : e - OS_START_SYSERR)

/** @def get_os_error()
 * @return STATUS_T the last platform error, folded into STATUS_T, on most platforms
 * @remark This retrieves errno, or calls a GetLastError() style function, and
 *      folds it with FROM_OS_ERROR.  Some platforms (such as OS2) have no
 *      such mechanism, so this call may be unsupported.  Do NOT use this
 *      call for socket errors from socket, send, recv etc!
 */

/** @def set_os_error(e)
 * Reset the last platform error, unfolded from an STATUS_T, on some platforms
 * @param e The OS error folded in a prior call to FROM_OS_ERROR()
 * @warning This is a macro implementation; the statcode argument may be evaluated
 *      multiple times.  If the statcode was not created by get_os_error
 *      or FROM_OS_ERROR, the results are undefined.  This macro sets
 *      errno, or calls a SetLastError() style function, unfolding statcode
 *      with TO_OS_ERROR.  Some platforms (such as OS2) have no such
 *      mechanism, so this call may be unsupported.
 */

/** @def get_netos_error()
 * Return the last socket error, folded into STATUS_T, on all platforms
 * @remark This retrieves errno or calls a GetLastSocketError() style function,
 *      and folds it with FROM_OS_ERROR.
 */

/** @def set_netos_error(e)
 * Reset the last socket error, unfolded from an STATUS_T
 * @param e The socket error folded in a prior call to FROM_OS_ERROR()
 * @warning This is a macro implementation; the statcode argument may be evaluated
 *      multiple times.  If the statcode was not created by get_os_error
 *      or FROM_OS_ERROR, the results are undefined.  This macro sets
 *      errno, or calls a WSASetLastError() style function, unfolding
 *      socketcode with TO_OS_ERROR.
 */

#endif /* defined(DOXYGEN) */

/**
 * OS_START_ERROR is where the CORE specific error values start.
 */
#define OS_START_ERROR     20000
/**
 * OS_ERRSPACE_SIZE is the maximum number of errors you can fit
 *    into one of the error/status ranges below -- except for
 *    OS_START_USERERR, which see.
 */
#define OS_ERRSPACE_SIZE 50000
/**
 * UTIL_ERRSPACE_SIZE is the size of the space that is reserved for
 * use within core-util. This space is reserved above that used by CORE
 * internally.
 * @note This number MUST be smaller than OS_ERRSPACE_SIZE by a
 *       large enough amount that CORE has sufficient room for it's
 *       codes.
 */
#define UTIL_ERRSPACE_SIZE 20000
/**
 * OS_START_STATUS is where the CORE specific status codes start.
 */
#define OS_START_STATUS    (OS_START_ERROR + OS_ERRSPACE_SIZE)
/**
 * UTIL_START_STATUS is where CORE-Util starts defining it's
 * status codes.
 */
#define UTIL_START_STATUS   (OS_START_STATUS + \
                           (OS_ERRSPACE_SIZE - UTIL_ERRSPACE_SIZE))
/**
 * OS_START_USERERR are reserved for applications that use CORE that
 *     layer their own error codes along with CORE's.  Note that the
 *     error immediately following this one is set ten times farther
 *     away than usual, so that users of core have a lot of room in
 *     which to declare custom error codes.
 *
 * In general applications should try and create unique error codes. To try
 * and assist in finding suitable ranges of numbers to use, the following
 * ranges are known to be used by the listed applications. If your
 * application defines error codes please advise the range of numbers it
 * uses to dev@core.apache.org for inclusion in this list.
 *
 * Ranges shown are in relation to OS_START_USERERR
 *
 * Subversion - Defined ranges, of less than 100, at intervals of 5000
 *              starting at an offset of 5000, e.g.
 *               +5000 to 5100,  +10000 to 10100
 */
#define OS_START_USERERR    (OS_START_STATUS + OS_ERRSPACE_SIZE)
/**
 * OS_START_USEERR is obsolete, defined for compatibility only.
 * Use OS_START_USERERR instead.
 */
#define OS_START_USEERR     OS_START_USERERR
/**
 * OS_START_CANONERR is where CORE versions of errno values are defined
 *     on systems which don't have the corresponding errno.
 */
#define OS_START_CANONERR  (OS_START_USERERR \
                                 + (OS_ERRSPACE_SIZE * 10))
/**
 * OS_START_EAIERR folds EAI_ error codes from getaddrinfo() into
 *     STATUS_T values.
 */
#define OS_START_EAIERR    (OS_START_CANONERR + OS_ERRSPACE_SIZE)
/**
 * OS_START_SYSERR folds platform-specific system error values into
 *     STATUS_T values.
 */
#define OS_START_SYSERR    (OS_START_EAIERR + OS_ERRSPACE_SIZE)

/**
 * @defgroup CORE_ERROR_map CORE Error Space
 * <PRE>
 * The following attempts to show the relation of the various constants
 * used for mapping CORE Status codes.
 *
 *       0
 *
 *  20,000     OS_START_ERROR
 *
 *         + OS_ERRSPACE_SIZE (50,000)
 *
 *  70,000      OS_START_STATUS
 *
 *         + OS_ERRSPACE_SIZE - UTIL_ERRSPACE_SIZE (30,000)
 *
 * 100,000      UTIL_START_STATUS
 *
 *         + UTIL_ERRSPACE_SIZE (20,000)
 *
 * 120,000      OS_START_USERERR
 *
 *         + 10 x OS_ERRSPACE_SIZE (50,000 * 10)
 *
 * 620,000      OS_START_CANONERR
 *
 *         + OS_ERRSPACE_SIZE (50,000)
 *
 * 670,000      OS_START_EAIERR
 *
 *         + OS_ERRSPACE_SIZE (50,000)
 *
 * 720,000      OS_START_SYSERR
 *
 * </PRE>
 */

/** no error. */
#define CORE_OK     0
#define CORE_ERROR -1

/**
 * @defgroup CORE_Error CORE Error Values
 * <PRE>
 * <b>CORE ERROR VALUES</b>
 * CORE_ENOSTAT      CORE was unable to perform a stat on the file
 * CORE_ENOPOOL      CORE was not provided a pool with which to allocate memory
 * CORE_EBADDATE     CORE was given an invalid date
 * CORE_EINVALSOCK   CORE was given an invalid socket
 * CORE_ENOPROC      CORE was not given a process structure
 * CORE_ENOTIME      CORE was not given a time structure
 * CORE_ENODIR       CORE was not given a directory structure
 * CORE_ENOLOCK      CORE was not given a lock structure
 * CORE_ENOPOLL      CORE was not given a poll structure
 * CORE_ENOSOCKET    CORE was not given a socket
 * CORE_ENOTHREAD    CORE was not given a thread structure
 * CORE_ENOTHDKEY    CORE was not given a thread key structure
 * CORE_ENOSHMAVAIL  There is no more shared memory available
 * CORE_EDSOOPEN     CORE was unable to open the dso object.  For more
 *                  information call dso_error().
 * CORE_EGENERAL     General failure (specific information not available)
 * CORE_EBADIP       The specified IP address is invalid
 * CORE_EBADMASK     The specified netmask is invalid
 * CORE_ESYMNOTFOUND Could not find the requested symbol
 * CORE_ENOTENOUGHENTROPY Not enough entropy to continue
 * </PRE>
 *
 * <PRE>
 * <b>CORE STATUS VALUES</b>
 * CORE_INCHILD        Program is currently executing in the child
 * CORE_INPARENT       Program is currently executing in the parent
 * CORE_DETACH         The thread is detached
 * CORE_NOTDETACH      The thread is not detached
 * CORE_CHILD_DONE     The child has finished executing
 * CORE_CHILD_NOTDONE  The child has not finished executing
 * CORE_TIMEUP         The operation did not finish before the timeout
 * CORE_INCOMPLETE     The operation was incomplete although some processing
 *                    was performed and the results are partially valid
 * CORE_BADCH          Getopt found an option not in the option string
 * CORE_BADARG         Getopt found an option that is missing an argument
 *                    and an argument was specified in the option string
 * CORE_EOF            CORE has encountered the end of the file
 * CORE_NOTFOUND       CORE was unable to find the socket in the poll structure
 * CORE_ANONYMOUS      CORE is using anonymous shared memory
 * CORE_FILEBASED      CORE is using a file name as the key to the shared memory
 * CORE_KEYBASED       CORE is using a shared key as the key to the shared memory
 * CORE_EINIT          Ininitalizer value.  If no option has been found, but
 *                    the status variable requires a value, this should be used
 * CORE_ENOTIMPL       The CORE function has not been implemented on this
 *                    platform, either because nobody has gotten to it yet,
 *                    or the function is impossible on this platform.
 * CORE_EMISMATCH      Two passwords do not match.
 * CORE_EABSOLUTE      The given path was absolute.
 * CORE_ERELATIVE      The given path was relative.
 * CORE_EINCOMPLETE    The given path was neither relative nor absolute.
 * CORE_EABOVEROOT     The given path was above the root path.
 * CORE_EBUSY          The given lock was busy.
 * CORE_EPROC_UNKNOWN  The given process wasn't recognized by CORE
 * </PRE>
 * @{
 */
/** @see STATUS_IS_ENOSTAT */
#define CORE_ENOSTAT        (OS_START_ERROR + 1)
/** @see STATUS_IS_ENOPOOL */
#define CORE_ENOPOOL        (OS_START_ERROR + 2)
/* empty slot: +3 */
/** @see STATUS_IS_EBADDATE */
#define CORE_EBADDATE       (OS_START_ERROR + 4)
/** @see STATUS_IS_EINVALSOCK */
#define CORE_EINVALSOCK     (OS_START_ERROR + 5)
/** @see STATUS_IS_ENOPROC */
#define CORE_ENOPROC        (OS_START_ERROR + 6)
/** @see STATUS_IS_ENOTIME */
#define CORE_ENOTIME        (OS_START_ERROR + 7)
/** @see STATUS_IS_ENODIR */
#define CORE_ENODIR         (OS_START_ERROR + 8)
/** @see STATUS_IS_ENOLOCK */
#define CORE_ENOLOCK        (OS_START_ERROR + 9)
/** @see STATUS_IS_ENOPOLL */
#define CORE_ENOPOLL        (OS_START_ERROR + 10)
/** @see STATUS_IS_ENOSOCKET */
#define CORE_ENOSOCKET      (OS_START_ERROR + 11)
/** @see STATUS_IS_ENOTHREAD */
#define CORE_ENOTHREAD      (OS_START_ERROR + 12)
/** @see STATUS_IS_ENOTHDKEY */
#define CORE_ENOTHDKEY      (OS_START_ERROR + 13)
/** @see STATUS_IS_EGENERAL */
#define CORE_EGENERAL       (OS_START_ERROR + 14)
/** @see STATUS_IS_ENOSHMAVAIL */
#define CORE_ENOSHMAVAIL    (OS_START_ERROR + 15)
/** @see STATUS_IS_EBADIP */
#define CORE_EBADIP         (OS_START_ERROR + 16)
/** @see STATUS_IS_EBADMASK */
#define CORE_EBADMASK       (OS_START_ERROR + 17)
/* empty slot: +18 */
/** @see STATUS_IS_EDSOPEN */
#define CORE_EDSOOPEN       (OS_START_ERROR + 19)
/** @see STATUS_IS_EABSOLUTE */
#define CORE_EABSOLUTE      (OS_START_ERROR + 20)
/** @see STATUS_IS_ERELATIVE */
#define CORE_ERELATIVE      (OS_START_ERROR + 21)
/** @see STATUS_IS_EINCOMPLETE */
#define CORE_EINCOMPLETE    (OS_START_ERROR + 22)
/** @see STATUS_IS_EABOVEROOT */
#define CORE_EABOVEROOT     (OS_START_ERROR + 23)
/** @see STATUS_IS_EBADPATH */
#define CORE_EBADPATH       (OS_START_ERROR + 24)
/** @see STATUS_IS_EPATHWILD */
#define CORE_EPATHWILD      (OS_START_ERROR + 25)
/** @see STATUS_IS_ESYMNOTFOUND */
#define CORE_ESYMNOTFOUND   (OS_START_ERROR + 26)
/** @see STATUS_IS_EPROC_UNKNOWN */
#define CORE_EPROC_UNKNOWN  (OS_START_ERROR + 27)
/** @see STATUS_IS_ENOTENOUGHENTROPY */
#define CORE_ENOTENOUGHENTROPY (OS_START_ERROR + 28)
/** @} */

/**
 * @defgroup STATUS_IS Status Value Tests
 * @warning For any particular error condition, more than one of these tests
 *      may match. This is because platform-specific error codes may not
 *      always match the semantics of the POSIX codes these tests (and the
 *      corresponding CORE error codes) are named after. A notable example
 *      are the STATUS_IS_ENOENT and STATUS_IS_ENOTDIR tests on
 *      Win32 platforms. The programmer should always be aware of this and
 *      adjust the order of the tests accordingly.
 * @{
 */
/**
 * CORE was unable to perform a stat on the file
 * @warning always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_ENOSTAT(s)        ((s) == CORE_ENOSTAT)
/**
 * CORE was not provided a pool with which to allocate memory
 * @warning always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_ENOPOOL(s)        ((s) == CORE_ENOPOOL)
/** CORE was given an invalid date  */
#define STATUS_IS_EBADDATE(s)       ((s) == CORE_EBADDATE)
/** CORE was given an invalid socket */
#define STATUS_IS_EINVALSOCK(s)     ((s) == CORE_EINVALSOCK)
/** CORE was not given a process structure */
#define STATUS_IS_ENOPROC(s)        ((s) == CORE_ENOPROC)
/** CORE was not given a time structure */
#define STATUS_IS_ENOTIME(s)        ((s) == CORE_ENOTIME)
/** CORE was not given a directory structure */
#define STATUS_IS_ENODIR(s)         ((s) == CORE_ENODIR)
/** CORE was not given a lock structure */
#define STATUS_IS_ENOLOCK(s)        ((s) == CORE_ENOLOCK)
/** CORE was not given a poll structure */
#define STATUS_IS_ENOPOLL(s)        ((s) == CORE_ENOPOLL)
/** CORE was not given a socket */
#define STATUS_IS_ENOSOCKET(s)      ((s) == CORE_ENOSOCKET)
/** CORE was not given a thread structure */
#define STATUS_IS_ENOTHREAD(s)      ((s) == CORE_ENOTHREAD)
/** CORE was not given a thread key structure */
#define STATUS_IS_ENOTHDKEY(s)      ((s) == CORE_ENOTHDKEY)
/** Generic Error which can not be put into another spot */
#define STATUS_IS_EGENERAL(s)       ((s) == CORE_EGENERAL)
/** There is no more shared memory available */
#define STATUS_IS_ENOSHMAVAIL(s)    ((s) == CORE_ENOSHMAVAIL)
/** The specified IP address is invalid */
#define STATUS_IS_EBADIP(s)         ((s) == CORE_EBADIP)
/** The specified netmask is invalid */
#define STATUS_IS_EBADMASK(s)       ((s) == CORE_EBADMASK)
/* empty slot: +18 */
/**
 * CORE was unable to open the dso object.
 * For more information call dso_error().
 */
#if defined(WIN32)
#define STATUS_IS_EDSOOPEN(s)       ((s) == CORE_EDSOOPEN \
                       || TO_OS_ERROR(s) == ERROR_MOD_NOT_FOUND)
#else
#define STATUS_IS_EDSOOPEN(s)       ((s) == CORE_EDSOOPEN)
#endif
/** The given path was absolute. */
#define STATUS_IS_EABSOLUTE(s)      ((s) == CORE_EABSOLUTE)
/** The given path was relative. */
#define STATUS_IS_ERELATIVE(s)      ((s) == CORE_ERELATIVE)
/** The given path was neither relative nor absolute. */
#define STATUS_IS_EINCOMPLETE(s)    ((s) == CORE_EINCOMPLETE)
/** The given path was above the root path. */
#define STATUS_IS_EABOVEROOT(s)     ((s) == CORE_EABOVEROOT)
/** The given path was bad. */
#define STATUS_IS_EBADPATH(s)       ((s) == CORE_EBADPATH)
/** The given path contained wildcards. */
#define STATUS_IS_EPATHWILD(s)      ((s) == CORE_EPATHWILD)
/** Could not find the requested symbol.
 * For more information call dso_error().
 */
#if defined(WIN32)
#define STATUS_IS_ESYMNOTFOUND(s)   ((s) == CORE_ESYMNOTFOUND \
                       || TO_OS_ERROR(s) == ERROR_PROC_NOT_FOUND)
#else
#define STATUS_IS_ESYMNOTFOUND(s)   ((s) == CORE_ESYMNOTFOUND)
#endif
/** The given process was not recognized by CORE. */
#define STATUS_IS_EPROC_UNKNOWN(s)  ((s) == CORE_EPROC_UNKNOWN)
/** CORE could not gather enough entropy to continue. */
#define STATUS_IS_ENOTENOUGHENTROPY(s) ((s) == CORE_ENOTENOUGHENTROPY)

/** @} */

/**
 * @addtogroup CORE_Error
 * @{
 */
/** @see STATUS_IS_INCHILD */
#define CORE_INCHILD        (OS_START_STATUS + 1)
/** @see STATUS_IS_INPARENT */
#define CORE_INPARENT       (OS_START_STATUS + 2)
/** @see STATUS_IS_DETACH */
#define CORE_DETACH         (OS_START_STATUS + 3)
/** @see STATUS_IS_NOTDETACH */
#define CORE_NOTDETACH      (OS_START_STATUS + 4)
/** @see STATUS_IS_CHILD_DONE */
#define CORE_CHILD_DONE     (OS_START_STATUS + 5)
/** @see STATUS_IS_CHILD_NOTDONE */
#define CORE_CHILD_NOTDONE  (OS_START_STATUS + 6)
/** @see STATUS_IS_TIMEUP */
#define CORE_TIMEUP         (OS_START_STATUS + 7)
/** @see STATUS_IS_INCOMPLETE */
#define CORE_INCOMPLETE     (OS_START_STATUS + 8)
/* empty slot: +9 */
/* empty slot: +10 */
/* empty slot: +11 */
/** @see STATUS_IS_BADCH */
#define CORE_BADCH          (OS_START_STATUS + 12)
/** @see STATUS_IS_BADARG */
#define CORE_BADARG         (OS_START_STATUS + 13)
/** @see STATUS_IS_EOF */
#define CORE_EOF            (OS_START_STATUS + 14)
/** @see STATUS_IS_NOTFOUND */
#define CORE_NOTFOUND       (OS_START_STATUS + 15)
/* empty slot: +16 */
/* empty slot: +17 */
/* empty slot: +18 */
/** @see STATUS_IS_ANONYMOUS */
#define CORE_ANONYMOUS      (OS_START_STATUS + 19)
/** @see STATUS_IS_FILEBASED */
#define CORE_FILEBASED      (OS_START_STATUS + 20)
/** @see STATUS_IS_KEYBASED */
#define CORE_KEYBASED       (OS_START_STATUS + 21)
/** @see STATUS_IS_EINIT */
#define CORE_EINIT          (OS_START_STATUS + 22)
/** @see STATUS_IS_ENOTIMPL */
#define CORE_ENOTIMPL       (OS_START_STATUS + 23)
/** @see STATUS_IS_EMISMATCH */
#define CORE_EMISMATCH      (OS_START_STATUS + 24)
/** @see STATUS_IS_EBUSY */
#define CORE_EBUSY          (OS_START_STATUS + 25)
/** @} */

/**
 * @addtogroup STATUS_IS
 * @{
 */
/**
 * Program is currently executing in the child
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code */
#define STATUS_IS_INCHILD(s)        ((s) == CORE_INCHILD)
/**
 * Program is currently executing in the parent
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_INPARENT(s)       ((s) == CORE_INPARENT)
/**
 * The thread is detached
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_DETACH(s)         ((s) == CORE_DETACH)
/**
 * The thread is not detached
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_NOTDETACH(s)      ((s) == CORE_NOTDETACH)
/**
 * The child has finished executing
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_CHILD_DONE(s)     ((s) == CORE_CHILD_DONE)
/**
 * The child has not finished executing
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_CHILD_NOTDONE(s)  ((s) == CORE_CHILD_NOTDONE)
/**
 * The operation did not finish before the timeout
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_TIMEUP(s)         ((s) == CORE_TIMEUP)
/**
 * The operation was incomplete although some processing was performed
 * and the results are partially valid.
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_INCOMPLETE(s)     ((s) == CORE_INCOMPLETE)
/* empty slot: +9 */
/* empty slot: +10 */
/* empty slot: +11 */
/**
 * Getopt found an option not in the option string
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_BADCH(s)          ((s) == CORE_BADCH)
/**
 * Getopt found an option not in the option string and an argument was
 * specified in the option string
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_BADARG(s)         ((s) == CORE_BADARG)
/**
 * CORE has encountered the end of the file
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_EOF(s)            ((s) == CORE_EOF)
/**
 * CORE was unable to find the socket in the poll structure
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_NOTFOUND(s)       ((s) == CORE_NOTFOUND)
/* empty slot: +16 */
/* empty slot: +17 */
/* empty slot: +18 */
/**
 * CORE is using anonymous shared memory
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_ANONYMOUS(s)      ((s) == CORE_ANONYMOUS)
/**
 * CORE is using a file name as the key to the shared memory
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_FILEBASED(s)      ((s) == CORE_FILEBASED)
/**
 * CORE is using a shared key as the key to the shared memory
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_KEYBASED(s)       ((s) == CORE_KEYBASED)
/**
 * Ininitalizer value.  If no option has been found, but
 * the status variable requires a value, this should be used
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_EINIT(s)          ((s) == CORE_EINIT)
/**
 * The CORE function has not been implemented on this
 * platform, either because nobody has gotten to it yet,
 * or the function is impossible on this platform.
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_ENOTIMPL(s)       ((s) == CORE_ENOTIMPL)
/**
 * Two passwords do not match.
 * @warning
 * always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_EMISMATCH(s)      ((s) == CORE_EMISMATCH)
/**
 * The given lock was busy
 * @warning always use this test, as platform-specific variances may meet this
 * more than one error code
 */
#define STATUS_IS_EBUSY(s)          ((s) == CORE_EBUSY)

/** @} */

/**
 * @addtogroup CORE_Error CORE Error Values
 * @{
 */
/* CORE CANONICAL ERROR VALUES */
/** @see STATUS_IS_EACCES */
#ifdef EACCES
#define CORE_EACCES EACCES
#else
#define CORE_EACCES         (OS_START_CANONERR + 1)
#endif

/** @see STATUS_IS_EEXIST */
#ifdef EEXIST
#define CORE_EEXIST EEXIST
#else
#define CORE_EEXIST         (OS_START_CANONERR + 2)
#endif

/** @see STATUS_IS_ENAMETOOLONG */
#ifdef ENAMETOOLONG
#define CORE_ENAMETOOLONG ENAMETOOLONG
#else
#define CORE_ENAMETOOLONG   (OS_START_CANONERR + 3)
#endif

/** @see STATUS_IS_ENOENT */
#ifdef ENOENT
#define CORE_ENOENT ENOENT
#else
#define CORE_ENOENT         (OS_START_CANONERR + 4)
#endif

/** @see STATUS_IS_ENOTDIR */
#ifdef ENOTDIR
#define CORE_ENOTDIR ENOTDIR
#else
#define CORE_ENOTDIR        (OS_START_CANONERR + 5)
#endif

/** @see STATUS_IS_ENOSPC */
#ifdef ENOSPC
#define CORE_ENOSPC ENOSPC
#else
#define CORE_ENOSPC         (OS_START_CANONERR + 6)
#endif

/** @see STATUS_IS_ENOMEM */
#ifdef ENOMEM
#define CORE_ENOMEM ENOMEM
#else
#define CORE_ENOMEM         (OS_START_CANONERR + 7)
#endif

/** @see STATUS_IS_EMFILE */
#ifdef EMFILE
#define CORE_EMFILE EMFILE
#else
#define CORE_EMFILE         (OS_START_CANONERR + 8)
#endif

/** @see STATUS_IS_ENFILE */
#ifdef ENFILE
#define CORE_ENFILE ENFILE
#else
#define CORE_ENFILE         (OS_START_CANONERR + 9)
#endif

/** @see STATUS_IS_EBADF */
#ifdef EBADF
#define CORE_EBADF EBADF
#else
#define CORE_EBADF          (OS_START_CANONERR + 10)
#endif

/** @see STATUS_IS_EINVAL */
#ifdef EINVAL
#define CORE_EINVAL EINVAL
#else
#define CORE_EINVAL         (OS_START_CANONERR + 11)
#endif

/** @see STATUS_IS_ESPIPE */
#ifdef ESPIPE
#define CORE_ESPIPE ESPIPE
#else
#define CORE_ESPIPE         (OS_START_CANONERR + 12)
#endif

/**
 * @see STATUS_IS_EAGAIN
 * @warning use STATUS_IS_EAGAIN instead of just testing this value
 */
#ifdef EAGAIN
#define CORE_EAGAIN EAGAIN
#elif defined(EWOULDBLOCK)
#define CORE_EAGAIN EWOULDBLOCK
#else
#define CORE_EAGAIN         (OS_START_CANONERR + 13)
#endif

/** @see STATUS_IS_EINTR */
#ifdef EINTR
#define CORE_EINTR EINTR
#else
#define CORE_EINTR          (OS_START_CANONERR + 14)
#endif

/** @see STATUS_IS_ENOTSOCK */
#ifdef ENOTSOCK
#define CORE_ENOTSOCK ENOTSOCK
#else
#define CORE_ENOTSOCK       (OS_START_CANONERR + 15)
#endif

/** @see STATUS_IS_ECONNREFUSED */
#ifdef ECONNREFUSED
#define CORE_ECONNREFUSED ECONNREFUSED
#else
#define CORE_ECONNREFUSED   (OS_START_CANONERR + 16)
#endif

/** @see STATUS_IS_EINPROGRESS */
#ifdef EINPROGRESS
#define CORE_EINPROGRESS EINPROGRESS
#else
#define CORE_EINPROGRESS    (OS_START_CANONERR + 17)
#endif

/**
 * @see STATUS_IS_ECONNABORTED
 * @warning use STATUS_IS_ECONNABORTED instead of just testing this value
 */

#ifdef ECONNABORTED
#define CORE_ECONNABORTED ECONNABORTED
#else
#define CORE_ECONNABORTED   (OS_START_CANONERR + 18)
#endif

/** @see STATUS_IS_ECONNRESET */
#ifdef ECONNRESET
#define CORE_ECONNRESET ECONNRESET
#else
#define CORE_ECONNRESET     (OS_START_CANONERR + 19)
#endif

/** @see STATUS_IS_ETIMEDOUT
 *  @deprecated */
#ifdef ETIMEDOUT
#define CORE_ETIMEDOUT ETIMEDOUT
#else
#define CORE_ETIMEDOUT      (OS_START_CANONERR + 20)
#endif

/** @see STATUS_IS_EHOSTUNREACH */
#ifdef EHOSTUNREACH
#define CORE_EHOSTUNREACH EHOSTUNREACH
#else
#define CORE_EHOSTUNREACH   (OS_START_CANONERR + 21)
#endif

/** @see STATUS_IS_ENETUNREACH */
#ifdef ENETUNREACH
#define CORE_ENETUNREACH ENETUNREACH
#else
#define CORE_ENETUNREACH    (OS_START_CANONERR + 22)
#endif

/** @see STATUS_IS_EFTYPE */
#ifdef EFTYPE
#define CORE_EFTYPE EFTYPE
#else
#define CORE_EFTYPE        (OS_START_CANONERR + 23)
#endif

/** @see STATUS_IS_EPIPE */
#ifdef EPIPE
#define CORE_EPIPE EPIPE
#else
#define CORE_EPIPE         (OS_START_CANONERR + 24)
#endif

/** @see STATUS_IS_EXDEV */
#ifdef EXDEV
#define CORE_EXDEV EXDEV
#else
#define CORE_EXDEV         (OS_START_CANONERR + 25)
#endif

/** @see STATUS_IS_ENOTEMPTY */
#ifdef ENOTEMPTY
#define CORE_ENOTEMPTY ENOTEMPTY
#else
#define CORE_ENOTEMPTY     (OS_START_CANONERR + 26)
#endif

/** @see STATUS_IS_EAFNOSUPPORT */
#ifdef EAFNOSUPPORT
#define CORE_EAFNOSUPPORT EAFNOSUPPORT
#else
#define CORE_EAFNOSUPPORT  (OS_START_CANONERR + 27)
#endif

/** @} */

#if defined(OS2) && !defined(DOXYGEN)

#define FROM_OS_ERROR(e) (e == 0 ? CORE_OK : e + OS_START_SYSERR)
#define TO_OS_ERROR(e)   (e == 0 ? CORE_OK : e - OS_START_SYSERR)

#define INCL_DOSERRORS
#define INCL_DOS

/* Leave these undefined.
 * OS2 doesn't rely on the errno concept.
 * The API calls always return a result codes which
 * should be filtered through FROM_OS_ERROR().
 *
 * #define get_os_error()   (FROM_OS_ERROR(GetLastError()))
 * #define set_os_error(e)  (SetLastError(TO_OS_ERROR(e)))
 */

/* A special case, only socket calls require this;
 */
#define get_netos_error()   (FROM_OS_ERROR(errno))
#define set_netos_error(e)  (errno = TO_OS_ERROR(e))

/* And this needs to be greped away for good:
 */
#define OS2_STATUS(e) (FROM_OS_ERROR(e))

/* These can't sit in a private header, so in spite of the extra size,
 * they need to be made available here.
 */
#define SOCBASEERR              10000
#define SOCEPERM                (SOCBASEERR+1)             /* Not owner */
#define SOCESRCH                (SOCBASEERR+3)             /* No such process */
#define SOCEINTR                (SOCBASEERR+4)             /* Interrupted system call */
#define SOCENXIO                (SOCBASEERR+6)             /* No such device or address */
#define SOCEBADF                (SOCBASEERR+9)             /* Bad file number */
#define SOCEACCES               (SOCBASEERR+13)            /* Permission denied */
#define SOCEFAULT               (SOCBASEERR+14)            /* Bad address */
#define SOCEINVAL               (SOCBASEERR+22)            /* Invalid argument */
#define SOCEMFILE               (SOCBASEERR+24)            /* Too many open files */
#define SOCEPIPE                (SOCBASEERR+32)            /* Broken pipe */
#define SOCEOS2ERR              (SOCBASEERR+100)           /* OS/2 Error */
#define SOCEWOULDBLOCK          (SOCBASEERR+35)            /* Operation would block */
#define SOCEINPROGRESS          (SOCBASEERR+36)            /* Operation now in progress */
#define SOCEALREADY             (SOCBASEERR+37)            /* Operation already in progress */
#define SOCENOTSOCK             (SOCBASEERR+38)            /* Socket operation on non-socket */
#define SOCEDESTADDRREQ         (SOCBASEERR+39)            /* Destination address required */
#define SOCEMSGSIZE             (SOCBASEERR+40)            /* Message too long */
#define SOCEPROTOTYPE           (SOCBASEERR+41)            /* Protocol wrong type for socket */
#define SOCENOPROTOOPT          (SOCBASEERR+42)            /* Protocol not available */
#define SOCEPROTONOSUPPORT      (SOCBASEERR+43)            /* Protocol not supported */
#define SOCESOCKTNOSUPPORT      (SOCBASEERR+44)            /* Socket type not supported */
#define SOCEOPNOTSUPP           (SOCBASEERR+45)            /* Operation not supported on socket */
#define SOCEPFNOSUPPORT         (SOCBASEERR+46)            /* Protocol family not supported */
#define SOCEAFNOSUPPORT         (SOCBASEERR+47)            /* Address family not supported by protocol family */
#define SOCEADDRINUSE           (SOCBASEERR+48)            /* Address already in use */
#define SOCEADDRNOTAVAIL        (SOCBASEERR+49)            /* Can't assign requested address */
#define SOCENETDOWN             (SOCBASEERR+50)            /* Network is down */
#define SOCENETUNREACH          (SOCBASEERR+51)            /* Network is unreachable */
#define SOCENETRESET            (SOCBASEERR+52)            /* Network dropped connection on reset */
#define SOCECONNABORTED         (SOCBASEERR+53)            /* Software caused connection abort */
#define SOCECONNRESET           (SOCBASEERR+54)            /* Connection reset by peer */
#define SOCENOBUFS              (SOCBASEERR+55)            /* No buffer space available */
#define SOCEISCONN              (SOCBASEERR+56)            /* Socket is already connected */
#define SOCENOTCONN             (SOCBASEERR+57)            /* Socket is not connected */
#define SOCESHUTDOWN            (SOCBASEERR+58)            /* Can't send after socket shutdown */
#define SOCETOOMANYREFS         (SOCBASEERR+59)            /* Too many references: can't splice */
#define SOCETIMEDOUT            (SOCBASEERR+60)            /* Connection timed out */
#define SOCECONNREFUSED         (SOCBASEERR+61)            /* Connection refused */
#define SOCELOOP                (SOCBASEERR+62)            /* Too many levels of symbolic links */
#define SOCENAMETOOLONG         (SOCBASEERR+63)            /* File name too long */
#define SOCEHOSTDOWN            (SOCBASEERR+64)            /* Host is down */
#define SOCEHOSTUNREACH         (SOCBASEERR+65)            /* No route to host */
#define SOCENOTEMPTY            (SOCBASEERR+66)            /* Directory not empty */

/* CORE CANONICAL ERROR TESTS */
#define STATUS_IS_EACCES(s)         ((s) == CORE_EACCES \
                || (s) == OS_START_SYSERR + ERROR_ACCESS_DENIED \
                || (s) == OS_START_SYSERR + ERROR_SHARING_VIOLATION)
#define STATUS_IS_EEXIST(s)         ((s) == CORE_EEXIST \
                || (s) == OS_START_SYSERR + ERROR_OPEN_FAILED \
                || (s) == OS_START_SYSERR + ERROR_FILE_EXISTS \
                || (s) == OS_START_SYSERR + ERROR_ALREADY_EXISTS \
                || (s) == OS_START_SYSERR + ERROR_ACCESS_DENIED)
#define STATUS_IS_ENAMETOOLONG(s)   ((s) == CORE_ENAMETOOLONG \
                || (s) == OS_START_SYSERR + ERROR_FILENAME_EXCED_RANGE \
                || (s) == OS_START_SYSERR + SOCENAMETOOLONG)
#define STATUS_IS_ENOENT(s)         ((s) == CORE_ENOENT \
                || (s) == OS_START_SYSERR + ERROR_FILE_NOT_FOUND \
                || (s) == OS_START_SYSERR + ERROR_PATH_NOT_FOUND \
                || (s) == OS_START_SYSERR + ERROR_NO_MORE_FILES \
                || (s) == OS_START_SYSERR + ERROR_OPEN_FAILED)
#define STATUS_IS_ENOTDIR(s)        ((s) == CORE_ENOTDIR)
#define STATUS_IS_ENOSPC(s)         ((s) == CORE_ENOSPC \
                || (s) == OS_START_SYSERR + ERROR_DISK_FULL)
#define STATUS_IS_ENOMEM(s)         ((s) == CORE_ENOMEM)
#define STATUS_IS_EMFILE(s)         ((s) == CORE_EMFILE \
                || (s) == OS_START_SYSERR + ERROR_TOO_MANY_OPEN_FILES)
#define STATUS_IS_ENFILE(s)         ((s) == CORE_ENFILE)
#define STATUS_IS_EBADF(s)          ((s) == CORE_EBADF \
                || (s) == OS_START_SYSERR + ERROR_INVALID_HANDLE)
#define STATUS_IS_EINVAL(s)         ((s) == CORE_EINVAL \
                || (s) == OS_START_SYSERR + ERROR_INVALID_PARAMETER \
                || (s) == OS_START_SYSERR + ERROR_INVALID_FUNCTION)
#define STATUS_IS_ESPIPE(s)         ((s) == CORE_ESPIPE \
                || (s) == OS_START_SYSERR + ERROR_NEGATIVE_SEEK)
#define STATUS_IS_EAGAIN(s)         ((s) == CORE_EAGAIN \
                || (s) == OS_START_SYSERR + ERROR_NO_DATA \
                || (s) == OS_START_SYSERR + SOCEWOULDBLOCK \
                || (s) == OS_START_SYSERR + ERROR_LOCK_VIOLATION)
#define STATUS_IS_EINTR(s)          ((s) == CORE_EINTR \
                || (s) == OS_START_SYSERR + SOCEINTR)
#define STATUS_IS_ENOTSOCK(s)       ((s) == CORE_ENOTSOCK \
                || (s) == OS_START_SYSERR + SOCENOTSOCK)
#define STATUS_IS_ECONNREFUSED(s)   ((s) == CORE_ECONNREFUSED \
                || (s) == OS_START_SYSERR + SOCECONNREFUSED)
#define STATUS_IS_EINPROGRESS(s)    ((s) == CORE_EINPROGRESS \
                || (s) == OS_START_SYSERR + SOCEINPROGRESS)
#define STATUS_IS_ECONNABORTED(s)   ((s) == CORE_ECONNABORTED \
                || (s) == OS_START_SYSERR + SOCECONNABORTED)
#define STATUS_IS_ECONNRESET(s)     ((s) == CORE_ECONNRESET \
                || (s) == OS_START_SYSERR + SOCECONNRESET)
/* XXX deprecated */
#define STATUS_IS_ETIMEDOUT(s)         ((s) == CORE_ETIMEDOUT \
                || (s) == OS_START_SYSERR + SOCETIMEDOUT)
#undef STATUS_IS_TIMEUP
#define STATUS_IS_TIMEUP(s)         ((s) == CORE_TIMEUP \
                || (s) == OS_START_SYSERR + SOCETIMEDOUT)
#define STATUS_IS_EHOSTUNREACH(s)   ((s) == CORE_EHOSTUNREACH \
                || (s) == OS_START_SYSERR + SOCEHOSTUNREACH)
#define STATUS_IS_ENETUNREACH(s)    ((s) == CORE_ENETUNREACH \
                || (s) == OS_START_SYSERR + SOCENETUNREACH)
#define STATUS_IS_EFTYPE(s)         ((s) == CORE_EFTYPE)
#define STATUS_IS_EPIPE(s)          ((s) == CORE_EPIPE \
                || (s) == OS_START_SYSERR + ERROR_BROKEN_PIPE \
                || (s) == OS_START_SYSERR + SOCEPIPE)
#define STATUS_IS_EXDEV(s)          ((s) == CORE_EXDEV \
                || (s) == OS_START_SYSERR + ERROR_NOT_SAME_DEVICE)
#define STATUS_IS_ENOTEMPTY(s)      ((s) == CORE_ENOTEMPTY \
                || (s) == OS_START_SYSERR + ERROR_DIR_NOT_EMPTY \
                || (s) == OS_START_SYSERR + ERROR_ACCESS_DENIED)
#define STATUS_IS_EAFNOSUPPORT(s)   ((s) == CORE_AFNOSUPPORT \
                || (s) == OS_START_SYSERR + SOCEAFNOSUPPORT)

/*
    Sorry, too tired to wrap this up for OS2... feel free to
    fit the following into their best matches.

    { ERROR_NO_SIGNAL_SENT,     ESRCH           },
    { SOCEALREADY,              EALREADY        },
    { SOCEDESTADDRREQ,          EDESTADDRREQ    },
    { SOCEMSGSIZE,              EMSGSIZE        },
    { SOCEPROTOTYPE,            EPROTOTYPE      },
    { SOCENOPROTOOPT,           ENOPROTOOPT     },
    { SOCEPROTONOSUPPORT,       EPROTONOSUPPORT },
    { SOCESOCKTNOSUPPORT,       ESOCKTNOSUPPORT },
    { SOCEOPNOTSUPP,            EOPNOTSUPP      },
    { SOCEPFNOSUPPORT,          EPFNOSUPPORT    },
    { SOCEADDRINUSE,            EADDRINUSE      },
    { SOCEADDRNOTAVAIL,         EADDRNOTAVAIL   },
    { SOCENETDOWN,              ENETDOWN        },
    { SOCENETRESET,             ENETRESET       },
    { SOCENOBUFS,               ENOBUFS         },
    { SOCEISCONN,               EISCONN         },
    { SOCENOTCONN,              ENOTCONN        },
    { SOCESHUTDOWN,             ESHUTDOWN       },
    { SOCETOOMANYREFS,          ETOOMANYREFS    },
    { SOCELOOP,                 ELOOP           },
    { SOCEHOSTDOWN,             EHOSTDOWN       },
    { SOCENOTEMPTY,             ENOTEMPTY       },
    { SOCEPIPE,                 EPIPE           }
*/

#elif defined(WIN32) && !defined(DOXYGEN) /* !defined(OS2) */

#define FROM_OS_ERROR(e) (e == 0 ? CORE_OK : e + OS_START_SYSERR)
#define TO_OS_ERROR(e)   (e == 0 ? CORE_OK : e - OS_START_SYSERR)

#define get_os_error()   (FROM_OS_ERROR(GetLastError()))
#define set_os_error(e)  (SetLastError(TO_OS_ERROR(e)))

/* A special case, only socket calls require this:
 */
#define get_netos_error()   (FROM_OS_ERROR(WSAGetLastError()))
#define set_netos_error(e)   (WSASetLastError(TO_OS_ERROR(e)))

/* CORE CANONICAL ERROR TESTS */
#define STATUS_IS_EACCES(s)         ((s) == CORE_EACCES \
                || (s) == OS_START_SYSERR + ERROR_ACCESS_DENIED \
                || (s) == OS_START_SYSERR + ERROR_CANNOT_MAKE \
                || (s) == OS_START_SYSERR + ERROR_CURRENT_DIRECTORY \
                || (s) == OS_START_SYSERR + ERROR_DRIVE_LOCKED \
                || (s) == OS_START_SYSERR + ERROR_FAIL_I24 \
                || (s) == OS_START_SYSERR + ERROR_LOCK_VIOLATION \
                || (s) == OS_START_SYSERR + ERROR_LOCK_FAILED \
                || (s) == OS_START_SYSERR + ERROR_NOT_LOCKED \
                || (s) == OS_START_SYSERR + ERROR_NETWORK_ACCESS_DENIED \
                || (s) == OS_START_SYSERR + ERROR_SHARING_VIOLATION)
#define STATUS_IS_EEXIST(s)         ((s) == CORE_EEXIST \
                || (s) == OS_START_SYSERR + ERROR_FILE_EXISTS \
                || (s) == OS_START_SYSERR + ERROR_ALREADY_EXISTS)
#define STATUS_IS_ENAMETOOLONG(s)   ((s) == CORE_ENAMETOOLONG \
                || (s) == OS_START_SYSERR + ERROR_FILENAME_EXCED_RANGE \
                || (s) == OS_START_SYSERR + WSAENAMETOOLONG)
#define STATUS_IS_ENOENT(s)         ((s) == CORE_ENOENT \
                || (s) == OS_START_SYSERR + ERROR_FILE_NOT_FOUND \
                || (s) == OS_START_SYSERR + ERROR_PATH_NOT_FOUND \
                || (s) == OS_START_SYSERR + ERROR_OPEN_FAILED \
                || (s) == OS_START_SYSERR + ERROR_NO_MORE_FILES)
#define STATUS_IS_ENOTDIR(s)        ((s) == CORE_ENOTDIR \
                || (s) == OS_START_SYSERR + ERROR_PATH_NOT_FOUND \
                || (s) == OS_START_SYSERR + ERROR_BAD_NETPATH \
                || (s) == OS_START_SYSERR + ERROR_BAD_NET_NAME \
                || (s) == OS_START_SYSERR + ERROR_BAD_PATHNAME \
                || (s) == OS_START_SYSERR + ERROR_INVALID_DRIVE)
#define STATUS_IS_ENOSPC(s)         ((s) == CORE_ENOSPC \
                || (s) == OS_START_SYSERR + ERROR_DISK_FULL)
#define STATUS_IS_ENOMEM(s)         ((s) == CORE_ENOMEM \
                || (s) == OS_START_SYSERR + ERROR_ARENA_TRASHED \
                || (s) == OS_START_SYSERR + ERROR_NOT_ENOUGH_MEMORY \
                || (s) == OS_START_SYSERR + ERROR_INVALID_BLOCK \
                || (s) == OS_START_SYSERR + ERROR_NOT_ENOUGH_QUOTA \
                || (s) == OS_START_SYSERR + ERROR_OUTOFMEMORY)
#define STATUS_IS_EMFILE(s)         ((s) == CORE_EMFILE \
                || (s) == OS_START_SYSERR + ERROR_TOO_MANY_OPEN_FILES)
#define STATUS_IS_ENFILE(s)         ((s) == CORE_ENFILE)
#define STATUS_IS_EBADF(s)          ((s) == CORE_EBADF \
                || (s) == OS_START_SYSERR + ERROR_INVALID_HANDLE \
                || (s) == OS_START_SYSERR + ERROR_INVALID_TARGET_HANDLE)
#define STATUS_IS_EINVAL(s)         ((s) == CORE_EINVAL \
                || (s) == OS_START_SYSERR + ERROR_INVALID_ACCESS \
                || (s) == OS_START_SYSERR + ERROR_INVALID_DATA \
                || (s) == OS_START_SYSERR + ERROR_INVALID_FUNCTION \
                || (s) == OS_START_SYSERR + ERROR_INVALID_HANDLE \
                || (s) == OS_START_SYSERR + ERROR_INVALID_PARAMETER \
                || (s) == OS_START_SYSERR + ERROR_NEGATIVE_SEEK)
#define STATUS_IS_ESPIPE(s)         ((s) == CORE_ESPIPE \
                || (s) == OS_START_SYSERR + ERROR_SEEK_ON_DEVICE \
                || (s) == OS_START_SYSERR + ERROR_NEGATIVE_SEEK)
#define STATUS_IS_EAGAIN(s)         ((s) == CORE_EAGAIN \
                || (s) == OS_START_SYSERR + ERROR_NO_DATA \
                || (s) == OS_START_SYSERR + ERROR_NO_PROC_SLOTS \
                || (s) == OS_START_SYSERR + ERROR_NESTING_NOT_ALLOWED \
                || (s) == OS_START_SYSERR + ERROR_MAX_THRDS_REACHED \
                || (s) == OS_START_SYSERR + ERROR_LOCK_VIOLATION \
                || (s) == OS_START_SYSERR + WSAEWOULDBLOCK)
#define STATUS_IS_EINTR(s)          ((s) == CORE_EINTR \
                || (s) == OS_START_SYSERR + WSAEINTR)
#define STATUS_IS_ENOTSOCK(s)       ((s) == CORE_ENOTSOCK \
                || (s) == OS_START_SYSERR + WSAENOTSOCK)
#define STATUS_IS_ECONNREFUSED(s)   ((s) == CORE_ECONNREFUSED \
                || (s) == OS_START_SYSERR + WSAECONNREFUSED)
#define STATUS_IS_EINPROGRESS(s)    ((s) == CORE_EINPROGRESS \
                || (s) == OS_START_SYSERR + WSAEINPROGRESS)
#define STATUS_IS_ECONNABORTED(s)   ((s) == CORE_ECONNABORTED \
                || (s) == OS_START_SYSERR + WSAECONNABORTED)
#define STATUS_IS_ECONNRESET(s)     ((s) == CORE_ECONNRESET \
                || (s) == OS_START_SYSERR + ERROR_NETNAME_DELETED \
                || (s) == OS_START_SYSERR + WSAECONNRESET)
/* XXX deprecated */
#define STATUS_IS_ETIMEDOUT(s)         ((s) == CORE_ETIMEDOUT \
                || (s) == OS_START_SYSERR + WSAETIMEDOUT \
                || (s) == OS_START_SYSERR + WAIT_TIMEOUT)
#undef STATUS_IS_TIMEUP
#define STATUS_IS_TIMEUP(s)         ((s) == CORE_TIMEUP \
                || (s) == OS_START_SYSERR + WSAETIMEDOUT \
                || (s) == OS_START_SYSERR + WAIT_TIMEOUT)
#define STATUS_IS_EHOSTUNREACH(s)   ((s) == CORE_EHOSTUNREACH \
                || (s) == OS_START_SYSERR + WSAEHOSTUNREACH)
#define STATUS_IS_ENETUNREACH(s)    ((s) == CORE_ENETUNREACH \
                || (s) == OS_START_SYSERR + WSAENETUNREACH)
#define STATUS_IS_EFTYPE(s)         ((s) == CORE_EFTYPE \
                || (s) == OS_START_SYSERR + ERROR_EXE_MACHINE_TYPE_MISMATCH \
                || (s) == OS_START_SYSERR + ERROR_INVALID_DLL \
                || (s) == OS_START_SYSERR + ERROR_INVALID_MODULETYPE \
                || (s) == OS_START_SYSERR + ERROR_BAD_EXE_FORMAT \
                || (s) == OS_START_SYSERR + ERROR_INVALID_EXE_SIGNATURE \
                || (s) == OS_START_SYSERR + ERROR_FILE_CORRUPT \
                || (s) == OS_START_SYSERR + ERROR_BAD_FORMAT)
#define STATUS_IS_EPIPE(s)          ((s) == CORE_EPIPE \
                || (s) == OS_START_SYSERR + ERROR_BROKEN_PIPE)
#define STATUS_IS_EXDEV(s)          ((s) == CORE_EXDEV \
                || (s) == OS_START_SYSERR + ERROR_NOT_SAME_DEVICE)
#define STATUS_IS_ENOTEMPTY(s)      ((s) == CORE_ENOTEMPTY \
                || (s) == OS_START_SYSERR + ERROR_DIR_NOT_EMPTY)
#define STATUS_IS_EAFNOSUPPORT(s)   ((s) == CORE_EAFNOSUPPORT \
                || (s) == OS_START_SYSERR + WSAEAFNOSUPPORT)

#elif defined(NETWARE) && defined(USE_WINSOCK) && !defined(DOXYGEN) /* !defined(OS2) && !defined(WIN32) */

#define FROM_OS_ERROR(e) (e == 0 ? CORE_OK : e + OS_START_SYSERR)
#define TO_OS_ERROR(e)   (e == 0 ? CORE_OK : e - OS_START_SYSERR)

#define get_os_error()    (errno)
#define set_os_error(e)   (errno = (e))

/* A special case, only socket calls require this: */
#define get_netos_error()   (FROM_OS_ERROR(WSAGetLastError()))
#define set_netos_error(e)  (WSASetLastError(TO_OS_ERROR(e)))

/* CORE CANONICAL ERROR TESTS */
#define STATUS_IS_EACCES(s)         ((s) == CORE_EACCES)
#define STATUS_IS_EEXIST(s)         ((s) == CORE_EEXIST)
#define STATUS_IS_ENAMETOOLONG(s)   ((s) == CORE_ENAMETOOLONG)
#define STATUS_IS_ENOENT(s)         ((s) == CORE_ENOENT)
#define STATUS_IS_ENOTDIR(s)        ((s) == CORE_ENOTDIR)
#define STATUS_IS_ENOSPC(s)         ((s) == CORE_ENOSPC)
#define STATUS_IS_ENOMEM(s)         ((s) == CORE_ENOMEM)
#define STATUS_IS_EMFILE(s)         ((s) == CORE_EMFILE)
#define STATUS_IS_ENFILE(s)         ((s) == CORE_ENFILE)
#define STATUS_IS_EBADF(s)          ((s) == CORE_EBADF)
#define STATUS_IS_EINVAL(s)         ((s) == CORE_EINVAL)
#define STATUS_IS_ESPIPE(s)         ((s) == CORE_ESPIPE)

#define STATUS_IS_EAGAIN(s)         ((s) == CORE_EAGAIN \
                || (s) ==                       EWOULDBLOCK \
                || (s) == OS_START_SYSERR + WSAEWOULDBLOCK)
#define STATUS_IS_EINTR(s)          ((s) == CORE_EINTR \
                || (s) == OS_START_SYSERR + WSAEINTR)
#define STATUS_IS_ENOTSOCK(s)       ((s) == CORE_ENOTSOCK \
                || (s) == OS_START_SYSERR + WSAENOTSOCK)
#define STATUS_IS_ECONNREFUSED(s)   ((s) == CORE_ECONNREFUSED \
                || (s) == OS_START_SYSERR + WSAECONNREFUSED)
#define STATUS_IS_EINPROGRESS(s)    ((s) == CORE_EINPROGRESS \
                || (s) == OS_START_SYSERR + WSAEINPROGRESS)
#define STATUS_IS_ECONNABORTED(s)   ((s) == CORE_ECONNABORTED \
                || (s) == OS_START_SYSERR + WSAECONNABORTED)
#define STATUS_IS_ECONNRESET(s)     ((s) == CORE_ECONNRESET \
                || (s) == OS_START_SYSERR + WSAECONNRESET)
/* XXX deprecated */
#define STATUS_IS_ETIMEDOUT(s)       ((s) == CORE_ETIMEDOUT \
                || (s) == OS_START_SYSERR + WSAETIMEDOUT \
                || (s) == OS_START_SYSERR + WAIT_TIMEOUT)
#undef STATUS_IS_TIMEUP
#define STATUS_IS_TIMEUP(s)         ((s) == CORE_TIMEUP \
                || (s) == OS_START_SYSERR + WSAETIMEDOUT \
                || (s) == OS_START_SYSERR + WAIT_TIMEOUT)
#define STATUS_IS_EHOSTUNREACH(s)   ((s) == CORE_EHOSTUNREACH \
                || (s) == OS_START_SYSERR + WSAEHOSTUNREACH)
#define STATUS_IS_ENETUNREACH(s)    ((s) == CORE_ENETUNREACH \
                || (s) == OS_START_SYSERR + WSAENETUNREACH)
#define STATUS_IS_ENETDOWN(s)       ((s) == OS_START_SYSERR + WSAENETDOWN)
#define STATUS_IS_EFTYPE(s)         ((s) == CORE_EFTYPE)
#define STATUS_IS_EPIPE(s)          ((s) == CORE_EPIPE)
#define STATUS_IS_EXDEV(s)          ((s) == CORE_EXDEV)
#define STATUS_IS_ENOTEMPTY(s)      ((s) == CORE_ENOTEMPTY)
#define STATUS_IS_EAFNOSUPPORT(s)   ((s) == CORE_EAFNOSUPPORT \
                || (s) == OS_START_SYSERR + WSAEAFNOSUPPORT)

#else /* !defined(NETWARE) && !defined(OS2) && !defined(WIN32) */

/*
 *  os error codes are clib error codes
 */
#define FROM_OS_ERROR(e)  (e)
#define TO_OS_ERROR(e)    (e)

#define get_os_error()    (errno)
#define set_os_error(e)   (errno = (e))

/* A special case, only socket calls require this:
 */
#define get_netos_error() (errno)
#define set_netos_error(e) (errno = (e))

/**
 * @addtogroup STATUS_IS
 * @{
 */

/** permission denied */
#define STATUS_IS_EACCES(s)         ((s) == CORE_EACCES)
/** file exists */
#define STATUS_IS_EEXIST(s)         ((s) == CORE_EEXIST)
/** path name is too long */
#define STATUS_IS_ENAMETOOLONG(s)   ((s) == CORE_ENAMETOOLONG)
/**
 * no such file or directory
 * @remark
 * EMVSCATLG can be returned by the automounter on z/OS for
 * paths which do not exist.
 */
#ifdef EMVSCATLG
#define STATUS_IS_ENOENT(s)         ((s) == CORE_ENOENT \
                                      || (s) == EMVSCATLG)
#else
#define STATUS_IS_ENOENT(s)         ((s) == CORE_ENOENT)
#endif
/** not a directory */
#define STATUS_IS_ENOTDIR(s)        ((s) == CORE_ENOTDIR)
/** no space left on device */
#ifdef EDQUOT
#define STATUS_IS_ENOSPC(s)         ((s) == CORE_ENOSPC \
                                      || (s) == EDQUOT)
#else
#define STATUS_IS_ENOSPC(s)         ((s) == CORE_ENOSPC)
#endif
/** not enough memory */
#define STATUS_IS_ENOMEM(s)         ((s) == CORE_ENOMEM)
/** too many open files */
#define STATUS_IS_EMFILE(s)         ((s) == CORE_EMFILE)
/** file table overflow */
#define STATUS_IS_ENFILE(s)         ((s) == CORE_ENFILE)
/** bad file # */
#define STATUS_IS_EBADF(s)          ((s) == CORE_EBADF)
/** invalid argument */
#define STATUS_IS_EINVAL(s)         ((s) == CORE_EINVAL)
/** illegal seek */
#define STATUS_IS_ESPIPE(s)         ((s) == CORE_ESPIPE)

/** operation would block */
#if !defined(EWOULDBLOCK) || !defined(EAGAIN)
#define STATUS_IS_EAGAIN(s)         ((s) == CORE_EAGAIN)
#elif (EWOULDBLOCK == EAGAIN)
#define STATUS_IS_EAGAIN(s)         ((s) == CORE_EAGAIN)
#else
#define STATUS_IS_EAGAIN(s)         ((s) == CORE_EAGAIN \
                                      || (s) == EWOULDBLOCK)
#endif

/** interrupted system call */
#define STATUS_IS_EINTR(s)          ((s) == CORE_EINTR)
/** socket operation on a non-socket */
#define STATUS_IS_ENOTSOCK(s)       ((s) == CORE_ENOTSOCK)
/** Connection Refused */
#define STATUS_IS_ECONNREFUSED(s)   ((s) == CORE_ECONNREFUSED)
/** operation now in progress */
#define STATUS_IS_EINPROGRESS(s)    ((s) == CORE_EINPROGRESS)

/**
 * Software caused connection abort
 * @remark
 * EPROTO on certain older kernels really means ECONNABORTED, so we need to
 * ignore it for them.  See discussion in new-httpd archives nh.9701 & nh.9603
 *
 * There is potentially a bug in Solaris 2.x x<6, and other boxes that
 * implement tcp sockets in userland (i.e. on top of STREAMS).  On these
 * systems, EPROTO can actually result in a fatal loop.  See PR#981 for
 * example.  It's hard to handle both uses of EPROTO.
 */
#ifdef EPROTO
#define STATUS_IS_ECONNABORTED(s)    ((s) == CORE_ECONNABORTED \
                                       || (s) == EPROTO)
#else
#define STATUS_IS_ECONNABORTED(s)    ((s) == CORE_ECONNABORTED)
#endif

/** Connection Reset by peer */
#define STATUS_IS_ECONNRESET(s)      ((s) == CORE_ECONNRESET)
/** Operation timed out
 *  @deprecated */
#define STATUS_IS_ETIMEDOUT(s)      ((s) == CORE_ETIMEDOUT)
/** no route to host */
#define STATUS_IS_EHOSTUNREACH(s)    ((s) == CORE_EHOSTUNREACH)
/** network is unreachable */
#define STATUS_IS_ENETUNREACH(s)     ((s) == CORE_ENETUNREACH)
/** inappropiate file type or format */
#define STATUS_IS_EFTYPE(s)          ((s) == CORE_EFTYPE)
/** broken pipe */
#define STATUS_IS_EPIPE(s)           ((s) == CORE_EPIPE)
/** cross device link */
#define STATUS_IS_EXDEV(s)           ((s) == CORE_EXDEV)
/** Directory Not Empty */
#define STATUS_IS_ENOTEMPTY(s)       ((s) == CORE_ENOTEMPTY || \
                                          (s) == CORE_EEXIST)
/** Address Family not supported */
#define STATUS_IS_EAFNOSUPPORT(s)    ((s) == CORE_EAFNOSUPPORT)
/** @} */

#endif /* !defined(NETWARE) && !defined(OS2) && !defined(WIN32) */

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! __CORE_ERRNO_H__ */
