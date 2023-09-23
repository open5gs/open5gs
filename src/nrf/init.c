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

#include "sbi-path.h"

static ogs_thread_t *thread;
static void nrf_main(void *data);
static int initialized = 0;

/******************************************************************
 * Functionality: Initialize the NRF (Network Function Repository) system.
 * Input: None
 * Input Type: N/A (No inputs required)
 * Output: Return status (an integer indicating success or failure)
 * Output Type: int (Integer representing the return status)
 ********************************************************************/


int nrf_initialize(void)
{
    int rv;
    // Initialize the SBI (Service Bus Interface) context for NRF.
    ogs_sbi_context_init(OpenAPI_nf_type_NRF);
    // Initialize the NRF context.
    nrf_context_init();
    rv = ogs_sbi_context_parse_config("nrf", NULL, "scp");
    if (rv != OGS_OK) return rv;

    rv = nrf_context_parse_config();
    if (rv != OGS_OK) return rv;

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    rv = nrf_sbi_open();
    if (rv != OGS_OK) return rv;

    thread = ogs_thread_create(nrf_main, NULL);
    if (!thread) return OGS_ERROR;

    initialized = 1;

    return OGS_OK;
}

static ogs_timer_t *t_termination_holding = NULL; // creates a variable with type of ogs_timer_t for usage in event_termination





     /******************************************************************
         * this function is for ending a Daemon(background process) , wait up to 300 seconds (starts a timer), sends termination to pollset
         * input
         * input type : void
         * output
         * output type
        ********************************************************************/

static void event_termination(void)
{
    /*
     * Add business-login during Daemon termination
     */

    /* Start holding timer */
    t_termination_holding = ogs_timer_add(ogs_app()->timer_mgr, NULL, NULL);
    ogs_assert(t_termination_holding); // is a check for making sure that termamation timer has a valid value
#define TERMINATION_HOLDING_TIME ogs_time_from_msec(300)
    ogs_timer_start(t_termination_holding, TERMINATION_HOLDING_TIME);

    /* Sending termination event to the queue */
    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
}


        /******************************************************************
         * functionality: Terminate the NRF (Network Function Repository) system.
         * input : None 
         * input type : N/A (No inputs required)
         * output: None
         * output type: N/A (No output)
        ********************************************************************/


void nrf_terminate(void)
{
        // Check if the NRF system has been initialized; if not, do nothing and return.
    if (!initialized) return;

    /* Daemon terminating */
    event_termination();
    ogs_thread_destroy(thread);
    ogs_timer_delete(t_termination_holding);

    nrf_sbi_close();

    nrf_context_final();
    ogs_sbi_context_final();
}

/******************************************************************
 * Functionality: Main loop of the NRF (Network Function Repository) system.
 * Input: data (Unused parameter, can be NULL)
 * Input Type: void* (Pointer to any data, but unused in this function)
 * Output: None
 * Output Type: N/A (No output)
 ********************************************************************/



static void nrf_main(void *data)
{
        // Initialize the NRF state machine.
    ogs_fsm_t nrf_sm;
    int rv;

    ogs_fsm_init(&nrf_sm, nrf_state_initial, nrf_state_final, 0);

    for ( ;; ) {
                // Poll for events in the application's pollset, considering timers.
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

        /******************************************************************
         * functionality
         * input
         * input type
         * output
         * output type
        ********************************************************************/
        ogs_timer_mgr_expire(ogs_app()->timer_mgr);

        for ( ;; ) {
            nrf_event_t *e = NULL;  //jyfuyfh

            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);

            if (rv == OGS_DONE)
                goto done;

            if (rv == OGS_RETRY)
                break;

            ogs_assert(e);
            ogs_fsm_dispatch(&nrf_sm, e);
            ogs_event_free(e);
        }
    }
done:

    ogs_fsm_fini(&nrf_sm, 0);
}
