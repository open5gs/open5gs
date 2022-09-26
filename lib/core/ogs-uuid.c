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

/*
 * Copyright (C) 2019-2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "core-config-private.h"

#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#include "ogs-core.h"

#define NODE_LENGTH 6

static int uuid_state_seqnum;
static unsigned char uuid_state_node[NODE_LENGTH] = { 0 };

static void get_random_info(unsigned char node[NODE_LENGTH])
{
    ogs_random(node, NODE_LENGTH);
}

/* This implementation generates a random node ID instead of a
   system-dependent call to get IEEE node ID. This is also more secure:
   we aren't passing out our MAC address. */
static void get_pseudo_node_identifier(unsigned char *node)
{
    get_random_info(node);
    node[0] |= 0x01; /* this designates a random multicast node ID */
}

/* true_random -- generate a crypto-quality random number. */
static int true_random(void)
{
    unsigned char buf[2];

    ogs_random(buf, 2);
    return (buf[0] << 8) | buf[1];
}

static void init_state(void)
{
    uuid_state_seqnum = true_random();
    get_pseudo_node_identifier(uuid_state_node);
}

static void get_system_time(uint64_t *uuid_time)
{
    struct timeval tv;

    /* ### fix this call to be more portable? */
    ogs_gettimeofday(&tv);
    *uuid_time = ogs_time_from_sec(tv.tv_sec) + tv.tv_usec;

    /* Offset between UUID formatted times and Unix formatted times.
       UUID UTC base time is October 15, 1582.
       Unix base time is January 1, 1970.      */
    *uuid_time = (*uuid_time * 10) + 0x01B21DD213814000;
}

static void get_current_time(uint64_t *timestamp)
{
    /* ### this needs to be made thread-safe! */

    uint64_t time_now;
    static uint64_t time_last = 0;
    static uint64_t fudge = 0;

    get_system_time(&time_now);
        
    /* if clock reading changed since last UUID generated... */
    if (time_last != time_now) {
        /* The clock reading has changed since the last UUID was generated.
           Reset the fudge factor. if we are generating them too fast, then
           the fudge may need to be reset to something greater than zero. */
        if (time_last + fudge > time_now)
            fudge = time_last + fudge - time_now + 1;
        else
            fudge = 0;
        time_last = time_now;
    }
    else {
        /* We generated two really fast. Bump the fudge factor. */
        ++fudge;
    }

    *timestamp = time_now + fudge;
}

void ogs_uuid_get(ogs_uuid_t *uuid)
{
    uint64_t timestamp;
    unsigned char *d = NULL;
    int version = 4;

    ogs_assert(uuid);
    d = uuid->data;

    if (!uuid_state_node[0])
        init_state();

    get_current_time(&timestamp);

    /* time_low, uint32 */
    d[3] = (unsigned char)timestamp;
    d[2] = (unsigned char)(timestamp >> 8);
    d[1] = (unsigned char)(timestamp >> 16);
    d[0] = (unsigned char)(timestamp >> 24);

    /* time_mid, uint16 */
    d[5] = (unsigned char)(timestamp >> 32);
    d[4] = (unsigned char)(timestamp >> 40);

    /* Set the four most significant bits (bits 12 through 15) of the
     * time_hi_and_version field to the 4-bit version number from
     * Section 4.1.3. */
    d[7] = (unsigned char)(timestamp >> 48);
    d[6] = (unsigned char)(((timestamp >> 56) & 0x0F) | (version << 4));

    /* Set the two most significant bits (bits 6 and 7) of the
     * clock_seq_hi_and_reserved to zero and one, respectively. */
    d[8] = (unsigned char)(((uuid_state_seqnum >> 8) & 0x3F) | 0x80);

    /* clock_seq_low, uint8 */
    d[9] = (unsigned char)uuid_state_seqnum;

    /* node, byte[6] */
    memcpy(&d[10], uuid_state_node, NODE_LENGTH);
}

void ogs_uuid_format(char *buffer, const ogs_uuid_t *uuid)
{
    const unsigned char *d = uuid->data;

    ogs_snprintf(buffer, OGS_UUID_FORMATTED_LENGTH + 1,
                    "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-"
                    "%02x%02x%02x%02x%02x%02x",
                    d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7],
                    d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15]);
}

/* convert a pair of hex digits to an integer value [0,255] */
#if 'A' == 65
static unsigned char parse_hexpair(const char *s)
{
    int result;
    int temp;

    result = s[0] - '0';
    if (result > 48)
        result = (result - 39) << 4;
    else if (result > 16)
        result = (result - 7) << 4;
    else
        result = result << 4;

    temp = s[1] - '0';
    if (temp > 48)
        result |= temp - 39;
    else if (temp > 16)
        result |= temp - 7;
    else
        result |= temp;

    return (unsigned char)result;
}
#else
static unsigned char parse_hexpair(const char *s)
{
    int result;

    if (isdigit(*s)) {
        result = (*s - '0') << 4;
    }
    else {
        if (isupper(*s)) {
            result = (*s - 'A' + 10) << 4;
        }
        else {
            result = (*s - 'a' + 10) << 4;
        }
    }

    ++s;
    if (isdigit(*s)) {
        result |= (*s - '0');
    }
    else {
        if (isupper(*s)) {
            result |= (*s - 'A' + 10);
        }
        else {
            result |= (*s - 'a' + 10);
        }
    }

    return (unsigned char)result;
}
#endif

int ogs_uuid_parse(ogs_uuid_t *uuid, const char *uuid_str)
{
    int i;
    unsigned char *d = uuid->data;

    for (i = 0; i < 36; ++i) {
        char c = uuid_str[i];
        if (!isxdigit(c) &&
            !(c == '-' && (i == 8 || i == 13 || i == 18 || i == 23)))
            /* ### need a better value */
            return OGS_ERROR;
    }
    if (uuid_str[36] != '\0') {
        /* ### need a better value */
        return OGS_ERROR;
    }

    d[0] = parse_hexpair(&uuid_str[0]);
    d[1] = parse_hexpair(&uuid_str[2]);
    d[2] = parse_hexpair(&uuid_str[4]);
    d[3] = parse_hexpair(&uuid_str[6]);

    d[4] = parse_hexpair(&uuid_str[9]);
    d[5] = parse_hexpair(&uuid_str[11]);

    d[6] = parse_hexpair(&uuid_str[14]);
    d[7] = parse_hexpair(&uuid_str[16]);

    d[8] = parse_hexpair(&uuid_str[19]);
    d[9] = parse_hexpair(&uuid_str[21]);

    for (i = 6; i--;)
        d[10 + i] = parse_hexpair(&uuid_str[i*2+24]);

    return OGS_OK;
}
