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

static ogs_diam_stats_t self;
static pthread_t fd_stats_th = (pthread_t)NULL;

static void * diam_stats_worker(void * arg);

int ogs_diam_stats_init(int mode)
{
    memset(&self, 0, sizeof(ogs_diam_stats_t));

    self.mode = mode;
    self.duration = 60;       /* 60 seconds */

    CHECK_POSIX( pthread_mutex_init(&self.stats_lock, NULL) );

    return 0;
}

void ogs_diam_stats_final()
{
    CHECK_FCT_DO( fd_thr_term(&fd_stats_th), );
    CHECK_POSIX_DO( pthread_mutex_destroy(&self.stats_lock), );
}

ogs_diam_stats_t* ogs_diam_stats_self()
{
    return &self;
}

int ogs_diam_stats_start()
{
    /* Start the statistics thread */
    CHECK_POSIX( pthread_create(&fd_stats_th, NULL, diam_stats_worker, NULL) );

    return 0;
}

/* Function to display statistics periodically */
static void * diam_stats_worker(void * arg)
{
    struct timespec start, now;
    struct fd_stats copy;

    /* Get the start time */
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &start), );

    /* Now, loop until canceled */
    while (1) {
        /* Display statistics every XX seconds */
        sleep(self.duration);

        /* Now, get the current stats */
        CHECK_POSIX_DO( pthread_mutex_lock(&self.stats_lock), );
        memcpy(&copy, &self.stats, sizeof(struct fd_stats));
        CHECK_POSIX_DO( pthread_mutex_unlock(&self.stats_lock), );

        /* Get the current execution time */
        CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &now), );

        /* Now, display everything */
        ogs_trace("------- fd statistics ---------");
        if (now.tv_nsec >= start.tv_nsec)
        {
            ogs_trace(" Executing for: %d.%06ld sec",
                    (int)(now.tv_sec - start.tv_sec),
                    (long)(now.tv_nsec - start.tv_nsec) / 1000);
        }
        else
        {
            ogs_trace(" Executing for: %d.%06ld sec",
                    (int)(now.tv_sec - 1 - start.tv_sec),
                    (long)(now.tv_nsec + 1000000000 - start.tv_nsec) / 1000);
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
    }

    return NULL; /* never called */
}

