/*
 * Copyright (C) 2023-2025 by Sukchan Lee <acetcom@gmail.com>
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
#include "sbi-path.h"

static ogs_thread_t *thread;
static void sepp_main(void *data);

static int initialized = 0;

int sepp_initialize(void)
{
    int rv;

#define APP_NAME "sepp"
    rv = ogs_app_parse_local_conf(APP_NAME);
    if (rv != OGS_OK) return rv;

    ogs_sbi_context_init(OpenAPI_nf_type_SEPP);
    sepp_context_init();

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_sbi_context_parse_config(APP_NAME, "nrf", "scp");
    if (rv != OGS_OK) return rv;

    rv = sepp_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = sepp_sbi_open();
    if (rv != 0) return OGS_ERROR;

    thread = ogs_thread_create(sepp_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void sepp_terminate(void)
{
    if (!initialized) return;

    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
    ogs_thread_destroy(thread);

    sepp_sbi_close();

    sepp_context_final();
    ogs_sbi_context_final();
}

static void sepp_main(void *data)
{
    ogs_fsm_t sepp_sm;
    sepp_node_t *sepp_node = NULL;
    int rv;

    ogs_fsm_init(&sepp_sm, sepp_state_initial, sepp_state_final, 0);

    for ( ;; ) {
        ogs_pollset_poll(ogs_app()->pollset,
                ogs_timer_mgr_next(ogs_app()->timer_mgr));

        /*
         * After ogs_pollset_poll(), ogs_timer_mgr_expire() must be called.
         *
         * The reason is why ogs_timer_mgr_next() can get the current value
         * when ogs_timer_stop() is called internally in ogs_timer_mgr_expire().
         *
         * You should not use event-queue before ogs_timer_mgr_expire().
         * In this case, ogs_timer_mgr_expire() does not work
         * because 'if rv == OGS_DONE' statement is exiting and
         * not calling ogs_timer_mgr_expire().
         */
        ogs_timer_mgr_expire(ogs_app()->timer_mgr);

        for ( ;; ) {
            sepp_event_t *e = NULL;

            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&sepp_sm, e);
            ogs_event_free(e);
        }
    }
done:
    ogs_sbi_nf_fsm_fini_all();

    /* Send N32 termination to each peer SEPP. */
    ogs_list_for_each(&sepp_self()->peer_list, sepp_node)
        sepp_handshake_fsm_fini(sepp_node);

    ogs_sbi_graceful_shutdown();
    ogs_fsm_fini(&sepp_sm, 0);
}
