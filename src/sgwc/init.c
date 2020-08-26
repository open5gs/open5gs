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

#include "context.h"

static ogs_thread_t *thread;
static void sgwc_main(void *data);

static int initialized = 0;

int sgwc_initialize()
{
    int rv;

    ogs_pfcp_context_init(ogs_app()->pool.nf * OGS_MAX_NUM_OF_GTPU_RESOURCE);
    sgwc_context_init();
    sgwc_event_init();

    rv = ogs_gtp_xact_init();
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_xact_init();
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_context_parse_config("sgwc", "sgwu");
    if (rv != OGS_OK) return rv;

    rv = sgwc_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(sgwc_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void sgwc_terminate(void)
{
    if (!initialized) return;

    sgwc_event_term();

    ogs_thread_destroy(thread);

    sgwc_context_final();
    ogs_pfcp_context_final();

    ogs_pfcp_xact_final();
    ogs_gtp_xact_final();

    sgwc_event_final();
}

static void sgwc_main(void *data)
{
    ogs_fsm_t sgwc_sm;
    int rv;

    ogs_fsm_create(&sgwc_sm, sgwc_state_initial, sgwc_state_final);
    ogs_fsm_init(&sgwc_sm, 0);

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
            sgwc_event_t *e = NULL;

            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&sgwc_sm, e);
            sgwc_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&sgwc_sm, 0);
    ogs_fsm_delete(&sgwc_sm);
}
