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

#include "pgw-context.h"
#include "pgw-event.h"
#include "pgw-sm.h"

#include "pgw-fd-path.h"

static ogs_thread_t *thread;
static void pgw_main(void *data);

static int initialized = 0;

int pgw_initialize()
{
    int rv;

    pgw_context_init();
    pgw_event_init();

    rv = ogs_gtp_xact_init(pgw_self()->timer_mgr, 512);
    if (rv != OGS_OK) return rv;

    rv = pgw_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_config()->logger.domain, ogs_config()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = pgw_ue_pool_generate();
    if (rv != OGS_OK) return rv;

    rv = pgw_fd_init();
    if (rv != 0) return OGS_ERROR;

    thread = ogs_thread_create(pgw_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void pgw_terminate(void)
{
    if (!initialized) return;

    pgw_event_term();

    ogs_thread_destroy(thread);

    pgw_fd_final();

    pgw_context_final();

    ogs_gtp_xact_final();

    pgw_event_final();
}

static void pgw_main(void *data)
{
    ogs_fsm_t pgw_sm;
    int rv;

    ogs_fsm_create(&pgw_sm, pgw_state_initial, pgw_state_final);
    ogs_fsm_init(&pgw_sm, 0);

    for ( ;; ) {
        ogs_pollset_poll(pgw_self()->pollset,
                ogs_timer_mgr_next(pgw_self()->timer_mgr));

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
        ogs_timer_mgr_expire(pgw_self()->timer_mgr);

        for ( ;; ) {
            pgw_event_t *e = NULL;

            rv = ogs_queue_trypop(pgw_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&pgw_sm, e);
            pgw_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&pgw_sm, 0);
    ogs_fsm_delete(&pgw_sm);
}
