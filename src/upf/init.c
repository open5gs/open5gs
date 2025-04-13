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
//Modified by Fatemeh Shafiei Ardestani on 2025-04-06
#include "context.h"
#include "gtp-path.h"
#include "pfcp-path.h"
#include "metrics.h"
#include <pthread.h>
#include "ee_server.h"
#include "ee_client.h"
#include "publish_subscribe.h"

static ogs_thread_t *thread;
static void upf_main(void *data);

static int initialized = 0;

int upf_initialize(void)
{
    int rv;

#define APP_NAME "upf"
    rv = ogs_app_parse_local_conf(APP_NAME);
    if (rv != OGS_OK) return rv;

    upf_metrics_init();

    ogs_gtp_context_init(OGS_MAX_NUM_OF_GTPU_RESOURCE);
    ogs_pfcp_context_init();

    upf_context_init();
    upf_event_init();
    upf_gtp_init();

    rv = ogs_pfcp_xact_init();
    if (rv != OGS_OK) return rv;

    rv = ogs_gtp_context_parse_config(APP_NAME, "smf");
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_context_parse_config(APP_NAME, "smf");
    if (rv != OGS_OK) return rv;

    rv = ogs_metrics_context_parse_config(APP_NAME);
    if (rv != OGS_OK) return rv;

    rv = upf_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_pfcp_ue_pool_generate();
    if (rv != OGS_OK) return rv;

    ogs_metrics_context_open(ogs_metrics_self());

    rv = upf_pfcp_open();
    if (rv != OGS_OK) return rv;

    rv = upf_gtp_open();
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(upf_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;
    pthread_t ee_server_thread;
    if (pthread_create(&ee_server_thread, NULL, ee_http_server, NULL)) {
      fatemeh_log("Error creating thread for EE request\n");
      return 1;
    }
  fatemeh_log("server thread created\n");
//    pthread_join(server_thread, NULL);
    pthread_t report_thread;
    if (pthread_create(&report_thread, NULL, periodic_sending, NULL)) {
      fatemeh_log("Error creating thread for EE report\n");
    return 1;
    }
    pthread_join(report_thread, NULL);
//  pthread_t publisher_thread;
//  if (pthread_create(&publisher_thread, NULL, publisher, NULL)) {
//      ogs_warn("Error creating thread for EE request\n");
//      return 1;
//    }
//  pthread_join(publisher_thread, NULL);

    return OGS_OK;
}

void upf_terminate(void)
{
    if (!initialized) return;

    upf_event_term();

    ogs_thread_destroy(thread);

    upf_pfcp_close();
    upf_gtp_close();

    ogs_metrics_context_close(ogs_metrics_self());

    upf_context_final();

    ogs_pfcp_context_final();
    ogs_gtp_context_final();

    ogs_pfcp_xact_final();

    upf_gtp_final();
    upf_event_final();

    upf_metrics_final();
}

static void upf_main(void *data)
{
    ogs_fsm_t upf_sm;
    int rv;

    ogs_fsm_init(&upf_sm, upf_state_initial, upf_state_final, 0);

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
            upf_event_t *e = NULL;

            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
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
}
