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

static ogs_diam_stats_ctx_t self;

static void diam_stats_timer_cb(void *data);

int ogs_diam_stats_init(int mode, const ogs_diam_config_stats_t *config)
{
    ogs_assert(config);
    memset(&self, 0, sizeof(ogs_diam_stats_ctx_t));

    self.mode = mode;
    self.poll.t_interval = config->interval_sec > 0 ?
                            ogs_time_from_sec(config->interval_sec) :
                            ogs_time_from_sec(60); /* default 60 seconds */
    self.poll.timer = ogs_timer_add(ogs_app()->timer_mgr,
                diam_stats_timer_cb, 0);
        ogs_assert(self.poll.timer);
    if (config->priv_stats_size > 0) {
        self.priv_stats_size = config->priv_stats_size;
        self.priv_stats = ogs_calloc(1, self.priv_stats_size);
        self.priv_stats_copy = ogs_calloc(1, self.priv_stats_size);
        ogs_assert(self.priv_stats);
    }

    CHECK_POSIX( pthread_mutex_init(&self.stats_lock, NULL) );

    return 0;
}

void ogs_diam_stats_final()
{
    if (self.poll.timer)
        ogs_timer_delete(self.poll.timer);
    self.poll.timer = NULL;
    if (self.priv_stats) {
        ogs_free(self.priv_stats);
        ogs_free(self.priv_stats_copy);
        self.priv_stats = NULL;
        self.priv_stats_copy = NULL;
        self.priv_stats_size = 0;
    }
}

ogs_diam_stats_ctx_t* ogs_diam_stats_self()
{
    return &self;
}

int ogs_diam_stats_start()
{
    /* Get the start time */
    self.poll.t_start = ogs_get_monotonic_time();
    /* Start the statistics timer */
    self.poll.t_prev = self.poll.t_start;
    ogs_timer_start(self.poll.timer, self.poll.t_interval);
    ogs_info("Polling freeDiameter stats every %lld usecs", (long long)self.poll.t_interval);

    return 0;
}

void ogs_diam_stats_update_cb_register(ogs_diam_stats_update_cb cb)
{
    self.update_cb = cb;
}

void ogs_diam_stats_update_cb_unregister(void)
{
    self.update_cb = NULL;
}

static void ogs_diam_stats_log(const ogs_diam_stats_t *stats, ogs_time_t elapsed)
{
    ogs_trace("------- fd statistics ---------");
    ogs_trace(" Executing for: %llu.%06llu sec",
              (unsigned long long)ogs_time_sec(elapsed),
              (unsigned long long)ogs_time_usec(elapsed));

    if (self.mode & FD_MODE_SERVER) {
        ogs_trace(" Server: %llu message(s) echoed",
                stats->nb_echoed);
    }
    if (self.mode & FD_MODE_CLIENT) {
        ogs_trace(" Client:");
        ogs_trace("   %llu message(s) sent", stats->nb_sent);
        ogs_trace("   %llu error(s) received", stats->nb_errs);
        ogs_trace("   %llu answer(s) received", stats->nb_recv);
        ogs_trace("     fastest: %ld.%06ld sec.",
                stats->shortest / 1000000, stats->shortest % 1000000);
        ogs_trace("     slowest: %ld.%06ld sec.",
                stats->longest / 1000000, stats->longest % 1000000);
        ogs_trace("     Average: %ld.%06ld sec.",
                stats->avg / 1000000, stats->avg % 1000000);
    }
    ogs_trace("-------------------------------------");
}

/* Function to display statistics periodically */
static void diam_stats_timer_cb(void *data)
{
    ogs_time_t now, since_start, since_prev, next_run;
    ogs_diam_stats_t copy;

    /* Now, get the current stats */
    CHECK_POSIX_DO( pthread_mutex_lock(&self.stats_lock), );
    memcpy(&copy, &self.stats, sizeof(ogs_diam_stats_t));
    if (self.priv_stats_copy)
        memcpy(self.priv_stats_copy, self.priv_stats, self.priv_stats_size);
    CHECK_POSIX_DO( pthread_mutex_unlock(&self.stats_lock), );

    /* Get the current execution time */
    now = ogs_get_monotonic_time();
    since_start = now - self.poll.t_start;

    /* Now, display everything */
    ogs_diam_stats_log(&copy, since_start);

    /* Trigger user callback: */
    if (self.update_cb)
        self.update_cb(&copy, self.priv_stats_copy);

    /* Re-schedule timer: */
    now = ogs_get_monotonic_time();
    since_prev = now - self.poll.t_prev;
    /* Avoid increasing drift: */
    if (since_prev > self.poll.t_interval) {
        if (since_prev - self.poll.t_interval >= self.poll.t_interval)
            next_run = 1; /* 0 not accepted by ogs_timer_start() */
        else
            next_run = self.poll.t_interval - (since_prev - self.poll.t_interval);
    } else {
        next_run = self.poll.t_interval;
    }
    self.poll.t_prev = now;
    ogs_timer_start(self.poll.timer, next_run);
}

