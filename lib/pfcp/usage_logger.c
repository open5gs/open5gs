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

#include "usage_logger.h"
#include <string.h>

static bool file_elapsed(UsageLoggerState const *state, time_t current_epoch_sec);
static void refresh_state(UsageLoggerState *state, time_t current_epoch_sec);
static void get_time_string(time_t time, char *buf, size_t buf_sz);
static bool create_new_file(UsageLoggerState const *state);
static bool step(UsageLoggerState *state, time_t current_epoch_sec);

bool log_usage_data(UsageLoggerState *state, time_t current_epoch_sec, UsageLoggerData data)
{
    bool result = false;
    bool stepped = step(state, current_epoch_sec);

    if (stepped) {
        FILE *fptr = fopen(state->filename, "a");

        if (NULL != fptr)
        {
            int fprint_result = fprintf(
                fptr,
                "%s,%s,%u,%lu,%lu\n",
                data.imsi,
                data.apn,
                data.qci,
                data.octets_in,
                data.octets_out);

            int fclose_result = fclose(fptr);

            if ((0 < fprint_result) &&
                (0 == fclose_result))
            {
                result = true;
            }
        }
    }

    return result;
}

static bool step(UsageLoggerState *state, time_t current_epoch_sec)
{
    if (NULL != state)
    {
        if (file_elapsed(state, current_epoch_sec))
        {
            refresh_state(state, current_epoch_sec);
            create_new_file(state);
        }
        return true;
    }
    return false;
}

static bool file_elapsed(UsageLoggerState const *state, time_t current_epoch_sec)
{
    return state->file_end_time <= current_epoch_sec;
}

static void refresh_state(UsageLoggerState *state, time_t current_epoch_sec)
{
    /* Generate and set filename */
    snprintf(state->filename, FILENAME_MAX_LEN, "%lu", current_epoch_sec);

    /* Set the file capture window */
    state->file_start_time = current_epoch_sec;
    state->file_end_time = state->file_start_time + state->file_period_sec;
}

static bool create_new_file(UsageLoggerState const *state)
{
    enum
    {
        CAPTURE_TIME_MAX_SZ = 16
    };

    bool result = false;
    char file_capture_time_start[CAPTURE_TIME_MAX_SZ] = "";
    char file_capture_time_end[CAPTURE_TIME_MAX_SZ] = "";
    FILE *fptr = fopen(state->filename, "wb");

    get_time_string(state->file_end_time, file_capture_time_end, CAPTURE_TIME_MAX_SZ);
    get_time_string(state->file_start_time, file_capture_time_start, CAPTURE_TIME_MAX_SZ);

    if (NULL != fptr)
    {
        int fprint_result = fprintf(
            fptr,
            "# SWG CDR File:\n"
            "# File Start Time: %s\n"
            "# File End Time: %s\n"
            "# Origin: %s\n"
            "# imsi,apn,qci,octets_in,octets_out\n",
            file_capture_time_start,
            file_capture_time_end,
            state->origin);

        int fclose_result = fclose(fptr);

        if ((0 < fprint_result) &&
            (0 == fclose_result))
        {
            result = true;
        }
    }

    return result;
}

static void get_time_string(time_t time, char *buf, size_t buf_sz)
{
    struct tm *tmp = localtime(&time);
    snprintf(buf, buf_sz, "%02d:%02d:%02d", tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
}