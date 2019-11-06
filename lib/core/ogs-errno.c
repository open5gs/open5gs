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

#include "core-config-private.h"

#include "ogs-core.h"

char *ogs_strerror(ogs_err_t err, char *buf, size_t size)
{
#if defined(_WIN32)
	/*
     * The following code is stolen from APR Library
     * http://svn.apache.org/repos/asf/apr/apr/trunk/misc/unix/errorcodes.c
     */
    size_t len = 0, i;
    LPTSTR msg = (LPTSTR)buf;
    len = FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            err,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), /* Default Language */
            msg,
            (DWORD)(size/sizeof(TCHAR)),
            NULL);

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
        /* Windows didn't provide us with a message.  Even stuff like */
        ogs_snprintf(buf, size, "Unrecognized Win32 error code %d", (int)err);
    }

    return buf;
#elif defined(HAVE_STRERROR_R)

#if defined(STRERROR_R_CHAR_P)
    return strerror_r(err, buf, size);
#else
    int ret = strerror_r(err, buf, size);

    if (ret == 0)
        return buf;
    else
        return NULL;
#endif

#else
#error TODO
#endif
}
