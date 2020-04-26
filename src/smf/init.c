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
#include "smf-sm.h"

#include "fd-path.h"

static ogs_thread_t *thread;
static void smf_main(void *data);

static int initialized = 0;

int smf_initialize()
{
    int rv;

    ogs_pfcp_context_init(ogs_config()->max.upf * OGS_MAX_NUM_OF_GTPU_RESOURCE);
    smf_context_init();
    smf_event_init();

    rv = ogs_gtp_xact_init(smf_self()->timer_mgr, 512);
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_xact_init(smf_self()->timer_mgr, 512);
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_context_parse_config("smf", "upf");
    if (rv != OGS_OK) return rv;

    rv = smf_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_config()->logger.domain, ogs_config()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_ue_pool_generate();
    if (rv != OGS_OK) return rv;

    rv = smf_fd_init();
    if (rv != 0) return OGS_ERROR;

    thread = ogs_thread_create(smf_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void smf_terminate(void)
{
    if (!initialized) return;

    smf_event_term();

    ogs_thread_destroy(thread);

    smf_fd_final();

    smf_context_final();
    ogs_pfcp_context_final();

    ogs_pfcp_xact_final();
    ogs_gtp_xact_final();

    smf_event_final();
}

static void smf_main(void *data)
{
    ogs_fsm_t smf_sm;
    int rv;

    ogs_fsm_create(&smf_sm, smf_state_initial, smf_state_final);
    ogs_fsm_init(&smf_sm, 0);

    for ( ;; ) {
        ogs_pollset_poll(smf_self()->pollset,
                ogs_timer_mgr_next(smf_self()->timer_mgr));

        /* Process the MESSAGE FIRST.
         *
         * For example, if UE Context Release Complete is received,
         * the MME_TIMER_UE_CONTEXT_RELEASE is first stopped */
        for ( ;; ) {
            smf_event_t *e = NULL;

            rv = ogs_queue_trypop(smf_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&smf_sm, e);
            smf_event_free(e);
        }

        ogs_timer_mgr_expire(smf_self()->timer_mgr);

        /* AND THEN, process the TIMER. */
        for ( ;; ) {
            smf_event_t *e = NULL;

            rv = ogs_queue_trypop(smf_self()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&smf_sm, e);
            smf_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&smf_sm, 0);
    ogs_fsm_delete(&smf_sm);
}
