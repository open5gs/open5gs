/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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

#include "sbi-path.h"
#include "db-handler.h"
#include "cdr-path.h"
#include "notify-path.h"
#include "metrics.h"

static ogs_thread_t *thread;
static void chf_main(void *data);
static int initialized = 0;

int chf_initialize(void)
{
    int rv;

#define APP_NAME "chf"
    rv = ogs_app_parse_local_conf(APP_NAME);
    if (rv != OGS_OK) return rv;

    chf_metrics_init();

    ogs_sbi_context_init(OpenAPI_nf_type_CHF);
    chf_context_init();

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_sbi_context_parse_config(APP_NAME, "nrf", "scp");
    if (rv != OGS_OK) return rv;

    rv = ogs_metrics_context_parse_config(APP_NAME);
    if (rv != OGS_OK) return rv;

    rv = chf_context_parse_config();
    if (rv != OGS_OK) return rv;

    ogs_metrics_context_open(ogs_metrics_self());

    if (ogs_app()->db_uri) {
        rv = ogs_dbi_init(ogs_app()->db_uri);
        if (rv != OGS_OK) return rv;

        rv = chf_db_init();
        if (rv != OGS_OK) return rv;
    }

    chf_cdr_init();

    /* After chf_db_init(): the poll timer reads chf.notifications. */
    chf_notify_init();

    rv = chf_sbi_open();
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(chf_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void chf_terminate(void)
{
    if (!initialized) return;

    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
    ogs_thread_destroy(thread);

    chf_sbi_close();

    chf_notify_final();

    chf_cdr_final();

    if (ogs_app()->db_uri) {
        chf_db_final();
        ogs_dbi_final();
    }

    chf_context_final();
    ogs_sbi_context_final();

    ogs_metrics_context_close(ogs_metrics_self());
    chf_metrics_final();
}

static void chf_main(void *data)
{
    ogs_fsm_t chf_sm;
    int rv;

    ogs_fsm_init(&chf_sm, chf_state_initial, chf_state_final, 0);

    for ( ;; ) {
        ogs_pollset_poll(ogs_app()->pollset,
                ogs_timer_mgr_next(ogs_app()->timer_mgr));

        ogs_timer_mgr_expire(ogs_app()->timer_mgr);

        for ( ;; ) {
            chf_event_t *e = NULL;

            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&chf_sm, e);
            ogs_event_free(e);
        }
    }
done:
    ogs_sbi_nf_fsm_fini_all();
    ogs_sbi_graceful_shutdown();
    ogs_fsm_fini(&chf_sm, 0);
}
