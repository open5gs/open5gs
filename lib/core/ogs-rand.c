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

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#if HAVE_SYS_RANDOM_H
#include <sys/random.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ogs-core.h"

void ogs_random(void *buf, size_t buflen)
{
#ifdef _WIN32
    void arc4random_buf(void *buf, size_t n);
    arc4random_buf(buf, buflen);
#elif defined(HAVE_GETRANDOM)
    int rc = getrandom(buf, buflen, GRND_NONBLOCK);
    if (rc < 0) {
        ogs_log_message(OGS_LOG_FATAL, ogs_errno, "getrandom() failed");
        ogs_assert_if_reached();
    }
#elif defined(HAVE_ARC4RANDOM_BUF)
    arc4random_buf(buf, buflen);
#elif defined(OGS_DEV_RANDOM)
    int fd = -1;

    /* On BSD/OS 4.1, /dev/random gives out 8 bytes at a time, then
     * gives EOF, so reading 'buflen' bytes may require opening the
     * device several times. */
    do {
        int rc;

        if (fd == -1)
            if ((fd = open(OGS_DEV_RANDOM, O_RDONLY)) == -1) {
                ogs_log_message(OGS_LOG_FATAL, ogs_errno, "open() failed");
                ogs_assert_if_reached();
            }
        
        do {
            rc = read(fd, buf, buflen);
        } while (rc == -1 && errno == EINTR);

        if (rc < 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_errno, "read() failed");
            close(fd);
            return;
        }
        else if (rc == 0) {
            close(fd);
            fd = -1; /* force open() again */
        }
        else {
			buf = (unsigned char *)buf + rc;
            buflen -= rc;
        }
    } while (buflen > 0);
    
    close(fd);
#else
#error cannot find a method which finds a pseudo-random number
#endif
}

uint32_t ogs_random32(void)
{
#ifdef _WIN32
    uint32_t arc4random(void);
    return arc4random();
#elif defined(HAVE_ARC4RANDOM_BUF)
    return arc4random();
#else
    uint32_t rand32;

    ogs_random(&rand32, sizeof(rand32));
    return rand32;
#endif
}

#ifdef _WIN32
#define ARC4RANDOM_NO_INCLUDES
#define ARC4_LOCK_() do {} while(0)
#define ARC4_UNLOCK_() do {} while(0)

#define ARC4RANDOM_UINT32 uint32_t
#define ARC4RANDOM_NOSTIR
#define ARC4RANDOM_NOADDRANDOM
#define ARC4RANDOM_NOUNIFORM

/**
 * Stolen from libevent library.
 * https://github.com/libevent/libevent/blob/master/evutil.c
 */
void *(*volatile evutil_memset_volatile_)(void *, int, size_t) = memset;

static void evutil_memclear_(void *mem, size_t len)
{
	evutil_memset_volatile_(mem, 0, len);
}

#include "arc4random.c"
#endif

