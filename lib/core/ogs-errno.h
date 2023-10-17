/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_ERRNO_H
#define OGS_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)

typedef DWORD ogs_err_t;

#define OGS_EPERM                   ERROR_ACCESS_DENIED
#define OGS_ENOMEM                  ERROR_NOT_ENOUGH_MEMORY
#define OGS_EACCES                  ERROR_ACCESS_DENIED
#define OGS_EEXIST                  ERROR_ALREADY_EXISTS
#define OGS_EEXIST_FILE             ERROR_FILE_EXISTS
#define OGS_ECONNRESET              WSAECONNRESET
#define OGS_ETIMEDOUT               WSAETIMEDOUT
#define OGS_ECONNREFUSED            WSAECONNREFUSED
#define OGS_EBADF                   WSAEBADF
#define OGS_EAGAIN                  WSAEWOULDBLOCK

#define ogs_errno                   GetLastError()
#define ogs_set_errno(err)          SetLastError(err)
#define ogs_socket_errno            WSAGetLastError()
#define ogs_set_socket_errno(err)   WSASetLastError(err)

#else

typedef int ogs_err_t;

#define OGS_EPERM                   EPERM
#define OGS_ENOMEM                  ENOMEM
#define OGS_EACCES                  EACCES
#define OGS_EEXIST                  EEXIST
#define OGS_EEXIST_FILE             EEXIST
#define OGS_ECONNRESET              ECONNRESET
#define OGS_ETIMEDOUT               ETIMEDOUT
#define OGS_ECONNREFUSED            ECONNREFUSED
#define OGS_EBADF                   EBADF
#if (__hpux__)
#define OGS_EAGAIN                  EWOULDBLOCK
#else
#define OGS_EAGAIN                  EAGAIN
#endif

#define ogs_errno                   errno
#define ogs_socket_errno            errno
#define ogs_set_errno(err)          errno = err
#define ogs_set_socket_errno(err)   errno = err

#endif

#define OGS_OK           0
#define OGS_ERROR       -1
#define OGS_RETRY       -2
#define OGS_TIMEUP      -3
#define OGS_DONE        -4
#define OGS_NOTFOUND    -5

char *ogs_strerror(ogs_err_t err, char *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* OGS_ERRNO_H */
