/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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
#include "ngap-path.h"
#include "sbcap-path.h"
#include "init.h"

static ogs_thread_t *thread;
static void pwsiws_main(void *data);
static int initialized = 0;

int pwsiws_initialize(void)
{
    int rv;
#define APP_NAME "pwsiws"
    rv = ogs_app_parse_local_conf(APP_NAME);
    if (rv != OGS_OK) return rv;

    pwsiws_context_init();

    /* Install SBI log domain before any SBI server is started */
    /* Only install if not already installed to avoid duplication warnings */
    if (!ogs_log_find_domain("sbi"))
        ogs_log_install_domain(&__ogs_sbi_domain, "sbi", ogs_core()->log.level);
    if (!ogs_log_find_domain("pwsiws"))
        ogs_log_install_domain(&__pwsiws_log_domain, "pwsiws", ogs_core()->log.level);

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    /* Use AMF NF type as a base since PWS-IWS interfaces with AMF */
    ogs_sbi_context_init(OpenAPI_nf_type_AMF);
    //rv = ogs_sbi_context_parse_config(APP_NAME, NULL, NULL);
    //if (rv != OGS_OK) return rv;

    rv = pwsiws_context_parse_config(); // Only for non-SBI config
    if (rv != OGS_OK) return rv;

    rv = pwsiws_context_nf_info();
    if (rv != OGS_OK) return rv;

    rv = pwsiws_sbi_open();
    if (rv != OGS_OK) return rv;

    rv = sbcap_open();
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(pwsiws_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;
    return OGS_OK;
}

static ogs_timer_t *t_termination_holding = NULL;

static void event_termination(void)
{
    /* Gracefully shutdown the server by sending GOAWAY to each session. */
    pwsiws_sbi_close();
    sbcap_close();

    /* Starting holding timer */
    t_termination_holding = ogs_timer_add(ogs_app()->timer_mgr, NULL, NULL);
    ogs_assert(t_termination_holding);
#define TERMINATION_HOLDING_TIME ogs_time_from_msec(300)
    ogs_timer_start(t_termination_holding, TERMINATION_HOLDING_TIME);

    /* Sending termination event to the queue */
    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
}

void pwsiws_terminate(void)
{
    if (!initialized) return;
    event_termination();
    ogs_thread_destroy(thread);
    ogs_timer_delete(t_termination_holding);
    pwsiws_context_final();
}

static void pwsiws_main(void *data)
{
    int rv;
    for (;;) {
        ogs_pollset_poll(ogs_app()->pollset,
                ogs_timer_mgr_next(ogs_app()->timer_mgr));
        ogs_timer_mgr_expire(ogs_app()->timer_mgr);
        for (;;) {
            void *e = NULL;
            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);
            if (rv == OGS_DONE)
                goto done;
            if (rv == OGS_RETRY)
                break;
        }
    }
done:
    ;
} 