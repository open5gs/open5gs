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

#include "core.h"
#include "core_lib.h"

#include "core_arch_network.h"

static char *stuffbuffer(char *buf, size_t bufsize, const char *s)
{
    strncpy(buf,s,bufsize);
    return buf;
}

static char *core_error_string(status_t statcode)
{
    switch (statcode) {
    case CORE_ENOPOOL:
        return "A new pool could not be created.";
    case CORE_EBADDATE:
        return "An invalid date has been provided";
    case CORE_EINVALSOCK:
        return "An invalid socket was returned";
    case CORE_ENOPROC:
        return "No process was provided and one was required.";
    case CORE_ENOTIME:
        return "No time was provided and one was required.";
    case CORE_ENODIR:
        return "No directory was provided and one was required.";
    case CORE_ENOLOCK:
        return "No lock was provided and one was required.";
    case CORE_ENOPOLL:
        return "No poll structure was provided and one was required.";
    case CORE_ENOSOCKET:
        return "No socket was provided and one was required.";
    case CORE_ENOTHREAD:
        return "No thread was provided and one was required.";
    case CORE_ENOTHDKEY:
        return "No thread key structure was provided and one was required.";
    case CORE_ENOSHMAVAIL:
        return "No shared memory is currently available";
    case CORE_EDSOOPEN:
        return "DSO load failed";
    case CORE_EBADIP:
        return "The specified IP address is invalid.";
    case CORE_EBADMASK:
        return "The specified network mask is invalid.";

    case CORE_INCHILD:
        return
        "Your code just forked, and you are currently executing in the "
        "child process";
    case CORE_INPARENT:
        return
        "Your code just forked, and you are currently executing in the "
        "parent process";
    case CORE_DETACH:
        return "The specified thread is detached";
    case CORE_NOTDETACH:
        return "The specified thread is not detached";
    case CORE_CHILD_DONE:
        return "The specified child process is done executing";
    case CORE_CHILD_NOTDONE:
        return "The specified child process is not done executing";
    case CORE_TIMEUP:
        return "The timeout specified has expired";
    case CORE_INCOMPLETE:
        return "Partial results are valid but processing is incomplete";
    case CORE_BADCH:
        return "Bad character specified on command line";
    case CORE_BADARG:
        return "Missing parameter for the specified command line option";
    case CORE_EOF:
        return "End of file found";
    case CORE_NOTFOUND:
        return "Could not find specified socket in poll list.";
    case CORE_ANONYMOUS:
        return "Shared memory is implemented anonymously";
    case CORE_FILEBASED:
        return "Shared memory is implemented using files";
    case CORE_KEYBASED:
        return "Shared memory is implemented using a key system";
    case CORE_EINIT:
        return
        "There is no error, this value signifies an initialized "
        "error code";
    case CORE_ENOTIMPL:
        return "This function has not been implemented on this platform";
    case CORE_EMISMATCH:
        return "passwords do not match";
    case CORE_EABSOLUTE:
        return "The given path is absolute";
    case CORE_ERELATIVE:
        return "The given path is relative";
    case CORE_EINCOMPLETE:
        return "The given path is incomplete";
    case CORE_EABOVEROOT:
        return "The given path was above the root path";
    case CORE_EBADPATH:
        return "The given path is misformatted or contained invalid characters";
    case CORE_EPATHWILD:
        return "The given path contained wildcard characters";
    case CORE_EPROC_UNKNOWN:
        return "The process is not recognized.";
    case CORE_EGENERAL:
        return "Internal error";
    default:
        return "Error string not specified yet";
    }
}


#ifdef OS2
#include <ctype.h>

int core_canonical_error(status_t err);

static char *core_os_strerror(char* buf, size_t bufsize, int err)
{
  char result[200];
  unsigned char message[HUGE_STRING_LEN];
  ULONG len;
  char *pos;
  int c;

  if (err >= 10000 && err < 12000) {  /* socket error codes */
      return stuffbuffer(buf, bufsize,
                         strerror(core_canonical_error(err+OS_START_SYSERR)));
  }
  else if (DosGetMessage(NULL, 0, message, HUGE_STRING_LEN, err,
             "OSO001.MSG", &len) == 0) {
      len--;
      message[len] = 0;
      pos = result;

      if (len >= sizeof(result))
        len = sizeof(result) - 1;

      for (c=0; c<len; c++) {
      /* skip multiple whitespace */
          while (core_isspace(message[c]) && core_isspace(message[c+1]))
              c++;
          *(pos++) = core_isspace(message[c]) ? ' ' : message[c];
      }

      *pos = 0;
  }
  else {
      sprintf(result, "OS/2 error %d", err);
  }

  /* Stuff the string into the caller supplied buffer, then return
   * a pointer to it.
   */
  return stuffbuffer(buf, bufsize, result);
}

#elif defined(WIN32) || (defined(NETWARE) && defined(USE_WINSOCK))

static const struct {
    status_t code;
    const char *msg;
} gaErrorList[] = {
    WSAEINTR,           "Interrupted system call",
    WSAEBADF,           "Bad file number",
    WSAEACCES,          "Permission denied",
    WSAEFAULT,          "Bad address",
    WSAEINVAL,          "Invalid argument",
    WSAEMFILE,          "Too many open sockets",
    WSAEWOULDBLOCK,     "Operation would block",
    WSAEINPROGRESS,     "Operation now in progress",
    WSAEALREADY,        "Operation already in progress",
    WSAENOTSOCK,        "Socket operation on non-socket",
    WSAEDESTADDRREQ,    "Destination address required",
    WSAEMSGSIZE,        "Message too long",
    WSAEPROTOTYPE,      "Protocol wrong type for socket",
    WSAENOPROTOOPT,     "Bad protocol option",
    WSAEPROTONOSUPPORT, "Protocol not supported",
    WSAESOCKTNOSUPPORT, "Socket type not supported",
    WSAEOPNOTSUPP,      "Operation not supported on socket",
    WSAEPFNOSUPPORT,    "Protocol family not supported",
    WSAEAFNOSUPPORT,    "Address family not supported",
    WSAEADDRINUSE,      "Address already in use",
    WSAEADDRNOTAVAIL,   "Can't assign requested address",
    WSAENETDOWN,        "Network is down",
    WSAENETUNREACH,     "Network is unreachable",
    WSAENETRESET,       "Net connection reset",
    WSAECONNABORTED,    "Software caused connection abort",
    WSAECONNRESET,      "Connection reset by peer",
    WSAENOBUFS,         "No buffer space available",
    WSAEISCONN,         "Socket is already connected",
    WSAENOTCONN,        "Socket is not connected",
    WSAESHUTDOWN,       "Can't send after socket shutdown",
    WSAETOOMANYREFS,    "Too many references, can't splice",
    WSAETIMEDOUT,       "Connection timed out",
    WSAECONNREFUSED,    "Connection refused",
    WSAELOOP,           "Too many levels of symbolic links",
    WSAENAMETOOLONG,    "File name too long",
    WSAEHOSTDOWN,       "Host is down",
    WSAEHOSTUNREACH,    "No route to host",
    WSAENOTEMPTY,       "Directory not empty",
    WSAEPROCLIM,        "Too many processes",
    WSAEUSERS,          "Too many users",
    WSAEDQUOT,          "Disc quota exceeded",
    WSAESTALE,          "Stale NFS file handle",
    WSAEREMOTE,         "Too many levels of remote in path",
    WSASYSNOTREADY,     "Network system is unavailable",
    WSAVERNOTSUPPORTED, "Winsock version out of range",
    WSANOTINITIALISED,  "WSAStartup not yet called",
    WSAEDISCON,         "Graceful shutdown in progress",
    WSAHOST_NOT_FOUND,  "Host not found",
    WSANO_DATA,         "No host data of that type was found",
    0,                  NULL
};


static char *core_os_strerror(char *buf, size_t bufsize, status_t errcode)
{
    size_t len=0, i;

#ifndef NETWARE
#ifndef _WIN32_WCE
    len = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM
                      | FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL,
                        errcode,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
                        buf,
                        (DWORD)bufsize,
                        NULL);
#else /* _WIN32_WCE speaks unicode */
     LPTSTR msg = (LPTSTR) buf;
     len = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
                       | FORMAT_MESSAGE_IGNORE_INSERTS,
                         NULL,
                         errcode,
                         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
                         msg,
                         (DWORD) (bufsize/sizeof(TCHAR)),
                         NULL);
     /* in-place convert to US-ASCII, substituting '?' for non ASCII   */
     for(i = 0; i <= len; i++) {
        if (msg[i] < 0x80 && msg[i] >= 0) {
            buf[i] = (char) msg[i];
        } else {
            buf[i] = '?';
        }
    }
#endif
#endif

    if (!len) {
        for (i = 0; gaErrorList[i].msg; ++i) {
            if (gaErrorList[i].code == errcode) {
                core_cpystrn(buf, gaErrorList[i].msg, bufsize);
                len = strlen(buf);
                break;
            }
        }
    }

    if (len) {
        /* FormatMessage put the message in the buffer, but it may
         * have embedded a newline (\r\n), and possible more than one.
         * Remove the newlines replacing them with a space. This is not
         * as visually perfect as moving all the remaining message over,
         * but more efficient.
         */
        i = len;
        while (i) {
            i--;
            if ((buf[i] == '\r') || (buf[i] == '\n'))
                buf[i] = ' ';
        }
    }
    else {
        /* Windows didn't provide us with a message.  Even stuff like                    * WSAECONNREFUSED won't get a message.
         */
        core_snprintf(buf, bufsize, "Unrecognized Win32 error code %d", errcode);
    }

    return buf;
}

#else
/* On Unix, core_os_strerror() handles error codes from the resolver
 * (h_errno).
 */
static char *core_os_strerror(char* buf, size_t bufsize, int err)
{
#ifdef HAVE_HSTRERROR
    return stuffbuffer(buf, bufsize, hstrerror(err));
#else /* HAVE_HSTRERROR */
    const char *msg;

    switch(err) {
    case HOST_NOT_FOUND:
        msg = "Unknown host";
        break;
#if defined(NO_DATA)
    case NO_DATA:
#if defined(NO_ADDRESS) && (NO_DATA != NO_ADDRESS)
    case NO_ADDRESS:
#endif
        msg = "No address for host";
        break;
#elif defined(NO_ADDRESS)
    case NO_ADDRESS:
        msg = "No address for host";
        break;
#endif /* NO_DATA */
    default:
        msg = "Unrecognized resolver error";
    }
    return stuffbuffer(buf, bufsize, msg);
#endif /* HAVE_STRERROR */
}
#endif

#if defined(HAVE_STRERROR_R) && defined(STRERROR_R_RC_INT) && !defined(BEOS)
/* AIX and Tru64 style */
static char *native_strerror(status_t statcode, char *buf,
                             size_t bufsize)
{
    if (strerror_r(statcode, buf, bufsize) < 0) {
        return stuffbuffer(buf, bufsize,
                           "CORE does not understand this error code");
    }
    else {
        return buf;
    }
}
#elif defined(HAVE_STRERROR_R)
/* glibc style */

/* BeOS has the function available, but it doesn't provide
 * the prototype publically (doh!), so to avoid a build warning
 * we add a suitable prototype here.
 */
#if defined(BEOS)
const char *strerror_r(status_t, char *, size_t);
#endif

static char *native_strerror(status_t statcode, char *buf,
                             size_t bufsize)
{
    const char *msg;

    buf[0] = '\0';
    msg = strerror_r(statcode, buf, bufsize);
    if (buf[0] == '\0') { /* libc didn't use our buffer */
        return stuffbuffer(buf, bufsize, msg);
    }
    else {
        return buf;
    }
}
#else
/* plain old strerror();
 * thread-safe on some platforms (e.g., Solaris, OS/390)
 */
static char *native_strerror(status_t statcode, char *buf,
                             size_t bufsize)
{
#ifdef _WIN32_WCE
    static char err[32];
    sprintf(err, "Native Error #%d", statcode);
    return stuffbuffer(buf, bufsize, err);
#else
    const char *err = strerror(statcode);
    if (err) {
        return stuffbuffer(buf, bufsize, err);
    } else {
        return stuffbuffer(buf, bufsize,
                           "CORE does not understand this error code");
    }
#endif
}
#endif

char * core_strerror(status_t statcode, char *buf,
                                 size_t bufsize)
{
    if (statcode < OS_START_ERROR) {
        return native_strerror(statcode, buf, bufsize);
    }
    else if (statcode < OS_START_USERERR) {
        return stuffbuffer(buf, bufsize, core_error_string(statcode));
    }
    else if (statcode < OS_START_EAIERR) {
        return stuffbuffer(buf, bufsize, "CORE does not understand this error code");
    }
    else if (statcode < OS_START_SYSERR) {
#if defined(HAVE_GAI_STRERROR)
        statcode -= OS_START_EAIERR;
#if defined(NEGATIVE_EAI)
        statcode = -statcode;
#endif
        return stuffbuffer(buf, bufsize, gai_strerror(statcode));
#else
        return stuffbuffer(buf, bufsize, "CORE does not understand this error code");
#endif
    }
    else {
        return core_os_strerror(buf, bufsize, statcode - OS_START_SYSERR);
    }
}

