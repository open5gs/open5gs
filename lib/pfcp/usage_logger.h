/*
 * Copyright (C) 2023 by Ryan Dimsey <ryan@omnitouch.com.au>
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

#ifndef USAGE_LOGGER_CONTEXT_H
#define USAGE_LOGGER_CONTEXT_H

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

enum
{
    FILENAME_MAX_LEN = 32,
    ORIGIN_STR_MAX_LEN = 32,
    IMSI_STR_MAX_LEN = 16,
    APN_STR_MAX_LEN = 16
};

typedef struct
{
    char imsi[IMSI_STR_MAX_LEN];
    char apn[APN_STR_MAX_LEN];
    uint8_t qci;
    uint64_t octets_in;
    uint64_t octets_out;
    // int events;
    // int tracking_area;
} UsageLoggerData;

typedef struct
{
    /* User should set the two fields,
     * origin specifies what is generating
     * the log files. E.g. "SGW-01" */
    bool enabled;
    char origin[ORIGIN_STR_MAX_LEN];

    /* The following are to be used 
     * internally by the module and
     * shouldn't be directly written
     * to */
    char filename[FILENAME_MAX_LEN];
    time_t file_start_time;
    time_t file_end_time;
    uint64_t file_period_sec;
} UsageLoggerState;

bool log_usage_data(UsageLoggerState *state, time_t current_epoch_sec, UsageLoggerData data);

#endif /* USAGE_LOGGER_CONTEXT_H */