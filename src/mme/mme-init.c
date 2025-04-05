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

#include "ogs-sctp.h"
#include "ogs-gtp.h"

#include "mme-context.h"
#include "mme-sm.h"
#include "mme-event.h"
#include "mme-timer.h"

#include "mme-fd-path.h"
#include "s1ap-path.h"
#include "sgsap-path.h"
#include "mme-gtp-path.h"
#include "mme-redis.h"
#include "metrics.h"

static ogs_thread_t *thread;
static void mme_main(void *data);

static int initialized = 0;
static ogs_timer_t *t_redis_init = NULL;
static void redis_init_retry(void *data);


static void redis_init_retry(void *data)
{
    int rv;

    rv = mme_redis_init();
    if (rv != OGS_OK) {
        ogs_warn("Redis initialization failed, retrying in 10 seconds...");
        ogs_timer_start(t_redis_init, ogs_time_from_sec(10));
    } else {
        ogs_info("Redis initialization successful");
        ogs_timer_delete(t_redis_init);
        t_redis_init = NULL;
    }
}


int mme_initialize(void)
{
    int rv;

#define APP_NAME "mme"
    rv = ogs_app_parse_local_conf(APP_NAME);
    if (rv != OGS_OK) return rv;

    mme_metrics_init();

    ogs_gtp_context_init(OGS_MAX_NUM_OF_GTPU_RESOURCE);
    mme_context_init();

    rv = ogs_gtp_xact_init();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = ogs_gtp_context_parse_config(APP_NAME, "sgwc");
    if (rv != OGS_OK) return rv;

    rv = ogs_metrics_context_parse_config(APP_NAME);
    if (rv != OGS_OK) return rv;

    rv = mme_context_parse_config();
    if (rv != OGS_OK) return rv;

    ogs_metrics_context_open(ogs_metrics_self());

    rv = mme_fd_init();
    if (rv != OGS_OK) return OGS_ERROR;

    rv = mme_gtp_open();
    if (rv != OGS_OK) return OGS_ERROR;

    rv = sgsap_open();
    if (rv != OGS_OK) return OGS_ERROR;

    rv = s1ap_open();
    if (rv != OGS_OK) return OGS_ERROR;

    rv = mme_redis_init();
    if (rv != OGS_OK) {
        ogs_warn("Redis initialization failed, starting retry mechanism...");

        if (!t_redis_init) {
            t_redis_init = ogs_timer_add(ogs_app()->timer_mgr, redis_init_retry, NULL);
            ogs_assert(t_redis_init);
        }
        /* Start timer for first retry */
        ogs_timer_start(t_redis_init, ogs_time_from_sec(10));
    }

    thread = ogs_thread_create(mme_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

void mme_terminate(void)
{
    if (!initialized) return;

    mme_event_term();

    ogs_thread_destroy(thread);

    mme_gtp_close();
    sgsap_close();
    s1ap_close();

    ogs_metrics_context_close(ogs_metrics_self());

    mme_fd_final();

    mme_context_final();

    ogs_gtp_context_final();

    ogs_gtp_xact_final();

    mme_metrics_final();

    mme_redis_final();
}

static void mme_main(void *data)
{
    ogs_fsm_t mme_sm;
    int rv;

    ogs_fsm_init(&mme_sm, mme_state_initial, mme_state_final, 0);

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
            mme_event_t *e = NULL;

            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&mme_sm, e);
            mme_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&mme_sm, 0);
}
