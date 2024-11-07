/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "pcrf-context.h"
#include "pcrf-fd-path.h"
#include "pcrf-sm.h"
#include "metrics.h"

static ogs_thread_t *thread;
static void pcrf_main(void *data);

static int initialized = 0;

int pcrf_initialize(void)
{
    int rv;

#define APP_NAME "pcrf"
    rv = ogs_app_parse_local_conf(APP_NAME);
    if (rv != OGS_OK) return rv;

    pcrf_metrics_init();

    pcrf_context_init();
    pcrf_event_init();

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_metrics_context_parse_config(APP_NAME);
    if (rv != OGS_OK) return rv;

    rv = pcrf_context_parse_config();
    if (rv != OGS_OK) return rv;

    ogs_metrics_context_open(ogs_metrics_self());

    if (ogs_app()->db_uri) {
        rv = ogs_dbi_init(ogs_app()->db_uri);
        if (rv != OGS_OK) return rv;
    }

    rv = pcrf_fd_init();
    if (rv != OGS_OK) return OGS_ERROR;

    thread = ogs_thread_create(pcrf_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void pcrf_terminate(void)
{
    if (!initialized) return;

    pcrf_event_term();
    ogs_thread_destroy(thread);
    ogs_metrics_context_close(ogs_metrics_self());

    pcrf_fd_final();

    if (ogs_app()->db_uri) {
        ogs_dbi_final();
    }

    pcrf_context_final();
    pcrf_event_final();
    pcrf_metrics_final();

    return;
}

static void pcrf_main(void *data)
{
    ogs_fsm_t pcrf_sm;
    int rv;

    ogs_fsm_init(&pcrf_sm, pcrf_state_initial, pcrf_state_final, 0);

    for ( ;; ) {
        ogs_pollset_poll(ogs_app()->pollset,
                ogs_timer_mgr_next(ogs_app()->timer_mgr));

        /*
         * After ogs_pollset_poll(), ogs_timer_mgr_expire() must be called.
         *
         * The reason is why ogs_timer_mgr_next() can get the corrent value
         * when ogs_timer_stop() is called internally in ogs_timer_mgr_expire().
         *
         * You should not use event-queue before ogs_timer_mgr_expire().
         * In this case, ogs_timer_mgr_expire() does not work
         * because 'if rv == OGS_DONE' statement is exiting and
         * not calling ogs_timer_mgr_expire().
         */
        ogs_timer_mgr_expire(ogs_app()->timer_mgr);

        for ( ;; ) {
            pcrf_event_t *e = NULL;

            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&pcrf_sm, e);
            pcrf_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&pcrf_sm, 0);
}
