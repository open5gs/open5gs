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
#include "event.h"
#include "upf-sm.h"

static ogs_thread_t *thread;
static void upf_main(void *data);

static int initialized = 0;

int upf_initialize()
{
    int rv;

    ogs_pfcp_context_init(OGS_MAX_NUM_OF_GTPU_RESOURCE);
    upf_context_init();
    upf_event_init();

    rv = ogs_pfcp_xact_init(upf_self()->timer_mgr, 512);
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_context_parse_config("upf", "smf");
    if (rv != OGS_OK) return rv;

    rv = upf_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_config()->logger.domain, ogs_config()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_ue_pool_generate();
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(upf_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void upf_terminate(void)
{
    if (!initialized) return;

    upf_event_term();

    ogs_thread_destroy(thread);

    upf_context_final();
    ogs_pfcp_context_final();

    ogs_pfcp_xact_final();

    upf_event_final();
}

static void upf_main(void *data)
{
    ogs_fsm_t upf_sm;
    int rv;

    ogs_fsm_create(&upf_sm, upf_state_initial, upf_state_final);
    ogs_fsm_init(&upf_sm, 0);

    for ( ;; ) {
        ogs_pollset_poll(upf_self()->pollset,
                ogs_timer_mgr_next(upf_self()->timer_mgr));

        /* Process the MESSAGE FIRST.
         *
         * For example, if UE Context Release Complete is received,
         * the MME_TIMER_UE_CONTEXT_RELEASE is first stopped */
        for ( ;; ) {
            upf_event_t *e = NULL;

            rv = ogs_queue_trypop(upf_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&upf_sm, e);
            upf_event_free(e);
        }

        ogs_timer_mgr_expire(upf_self()->timer_mgr);

        /* AND THEN, process the TIMER. */
        for ( ;; ) {
            upf_event_t *e = NULL;

            rv = ogs_queue_trypop(upf_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&upf_sm, e);
            upf_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&upf_sm, 0);
    ogs_fsm_delete(&upf_sm);
}
