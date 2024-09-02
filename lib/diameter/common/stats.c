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

#include "ogs-diameter-common.h"
#include "ogs-app.h"

static ogs_diam_stats_t self;

static void diam_stats_timer_cb(void *data);

int ogs_diam_stats_init(int mode)
{
    memset(&self, 0, sizeof(ogs_diam_stats_t));

    self.mode = mode;
    self.poll.t_interval = ogs_time_from_sec(60); /* 60 seconds */
    self.poll.timer = ogs_timer_add(ogs_app()->timer_mgr,
                diam_stats_timer_cb, 0);
        ogs_assert(self.poll.timer);

    CHECK_POSIX( pthread_mutex_init(&self.stats_lock, NULL) );

    return 0;
}

void ogs_diam_stats_final()
{
    if (self.poll.timer)
        ogs_timer_delete(self.poll.timer);
    self.poll.timer = NULL;
}

ogs_diam_stats_t* ogs_diam_stats_self()
{
    return &self;
}

int ogs_diam_stats_start()
{
    /* Get the start time */
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &self.poll.ts_start), );
    /* Start the statistics timer */
    ogs_timer_start(self.poll.timer, self.poll.t_interval);

    return 0;
}

/* Function to display statistics periodically */
static void diam_stats_timer_cb(void *data)
{
    struct timespec now;
    struct fd_stats copy;

    /* Now, get the current stats */
    CHECK_POSIX_DO( pthread_mutex_lock(&self.stats_lock), );
    memcpy(&copy, &self.stats, sizeof(struct fd_stats));
    CHECK_POSIX_DO( pthread_mutex_unlock(&self.stats_lock), );

    /* Get the current execution time */
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );

    /* Now, display everything */
    ogs_trace("------- fd statistics ---------");
    if (now.tv_nsec >= self.poll.ts_start.tv_nsec) {
        ogs_trace(" Executing for: %d.%06ld sec",
                (int)(now.tv_sec - self.poll.ts_start.tv_sec),
                (long)(now.tv_nsec - self.poll.ts_start.tv_nsec) / 1000);
    } else {
        ogs_trace(" Executing for: %d.%06ld sec",
                (int)(now.tv_sec - 1 - self.poll.ts_start.tv_sec),
                (long)(now.tv_nsec + 1000000000 - self.poll.ts_start.tv_nsec) / 1000);
    }

    if (self.mode & FD_MODE_SERVER) {
        ogs_trace(" Server: %llu message(s) echoed",
                copy.nb_echoed);
    }
    if (self.mode & FD_MODE_CLIENT) {
        ogs_trace(" Client:");
        ogs_trace("   %llu message(s) sent", copy.nb_sent);
        ogs_trace("   %llu error(s) received", copy.nb_errs);
        ogs_trace("   %llu answer(s) received", copy.nb_recv);
        ogs_trace("     fastest: %ld.%06ld sec.",
                copy.shortest / 1000000, copy.shortest % 1000000);
        ogs_trace("     slowest: %ld.%06ld sec.",
                copy.longest / 1000000, copy.longest % 1000000);
        ogs_trace("     Average: %ld.%06ld sec.",
                copy.avg / 1000000, copy.avg % 1000000);
    }
    ogs_trace("-------------------------------------");

    /* Re-schedule timer: */
    ogs_timer_start(self.poll.timer, self.poll.t_interval);
}

