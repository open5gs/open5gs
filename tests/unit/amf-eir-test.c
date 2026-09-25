/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "amf/n5geir-handler.h"
#include "core/abts.h"

/* Exercise the production decisions without UE pools or SBI transport. */
static void eir_test_check_wanted(abts_case *tc, void *data)
{
    amf_ue_t *amf_ue;
    bool saved_enabled = amf_self()->eir.enabled;

    amf_ue = ogs_calloc(1, sizeof(*amf_ue));
    ABTS_PTR_NOTNULL(tc, amf_ue);

    amf_self()->eir.enabled = true;
    amf_ue->nas.message_type = OGS_NAS_5GS_REGISTRATION_REQUEST;
    amf_ue->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;
    ABTS_TRUE(tc, amf_n5geir_eic_check_wanted(amf_ue));
    amf_ue->nas.registration.value =
        OGS_NAS_5GS_REGISTRATION_TYPE_MOBILITY_UPDATING;
    ABTS_TRUE(tc, amf_n5geir_eic_check_wanted(amf_ue));
    amf_ue->nas.registration.value =
        OGS_NAS_5GS_REGISTRATION_TYPE_PERIODIC_UPDATING;
    ABTS_TRUE(tc, amf_n5geir_eic_check_wanted(amf_ue));
    amf_ue->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_EMERGENCY;
    ABTS_TRUE(tc, !amf_n5geir_eic_check_wanted(amf_ue));

    amf_ue->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;
    amf_ue->nas.message_type = OGS_NAS_5GS_SERVICE_REQUEST;
    ABTS_TRUE(tc, !amf_n5geir_eic_check_wanted(amf_ue));

    amf_ue->nas.message_type = OGS_NAS_5GS_REGISTRATION_REQUEST;
    amf_self()->eir.enabled = false;
    ABTS_TRUE(tc, !amf_n5geir_eic_check_wanted(amf_ue));

    amf_self()->eir.enabled = saved_enabled;
    ogs_free(amf_ue);
}

static void eir_test_equipment_status(abts_case *tc, void *data)
{
    amf_ue_t *amf_ue;
    ogs_sbi_message_t message;
    OpenAPI_eir_response_data_t response;
    ogs_eir_action_e saved_unknown = amf_self()->eir.unknown_action;
    ogs_eir_action_e saved_failure = amf_self()->eir.failure_action;

    amf_ue = ogs_calloc(1, sizeof(*amf_ue));
    ABTS_PTR_NOTNULL(tc, amf_ue);
    amf_ue->supi = "imsi-001010000000001";
    amf_ue->pei = "imeisv-4901542032375186";
    memset(&message, 0, sizeof(message));
    memset(&response, 0, sizeof(response));
    message.res_status = OGS_SBI_HTTP_STATUS_OK;
    message.EirResponseData = &response;

    amf_self()->eir.unknown_action = OGS_EIR_ACTION_REJECT;
    amf_self()->eir.failure_action = OGS_EIR_ACTION_REJECT;
    response.status = OpenAPI_equipment_status_WHITELISTED;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_REQUEST_ACCEPTED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));
    response.status = OpenAPI_equipment_status_GREYLISTED;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_REQUEST_ACCEPTED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));

    response.status = OpenAPI_equipment_status_BLACKLISTED;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_ILLEGAL_ME,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));
    amf_self()->eir.unknown_action = OGS_EIR_ACTION_ALLOW;
    amf_self()->eir.failure_action = OGS_EIR_ACTION_ALLOW;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_ILLEGAL_ME,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));

    amf_self()->eir.unknown_action = saved_unknown;
    amf_self()->eir.failure_action = saved_failure;
    ogs_free(amf_ue);
}

static void eir_test_unknown_and_failure(abts_case *tc, void *data)
{
    amf_ue_t *amf_ue;
    ogs_sbi_message_t message;
    OpenAPI_problem_details_t problem;
    ogs_eir_action_e saved_unknown = amf_self()->eir.unknown_action;
    ogs_eir_action_e saved_failure = amf_self()->eir.failure_action;

    amf_ue = ogs_calloc(1, sizeof(*amf_ue));
    ABTS_PTR_NOTNULL(tc, amf_ue);
    amf_ue->supi = "imsi-001010000000001";
    amf_ue->pei = "imeisv-4901542032375186";
    memset(&message, 0, sizeof(message));
    memset(&problem, 0, sizeof(problem));
    message.res_status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
    message.ProblemDetails = &problem;
    problem.cause = "ERROR_EQUIPMENT_UNKNOWN";

    /* A recognized unknown-equipment response ignores failure_action. */
    amf_self()->eir.unknown_action = OGS_EIR_ACTION_ALLOW;
    amf_self()->eir.failure_action = OGS_EIR_ACTION_REJECT;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_REQUEST_ACCEPTED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));
    amf_self()->eir.unknown_action = OGS_EIR_ACTION_REJECT;
    amf_self()->eir.failure_action = OGS_EIR_ACTION_ALLOW;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));

    /* Other HTTP errors, missing data and timeouts use failure_action. */
    message.ProblemDetails = NULL;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_REQUEST_ACCEPTED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));
    amf_self()->eir.failure_action = OGS_EIR_ACTION_REJECT;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));

    message.res_status = OGS_SBI_HTTP_STATUS_OK;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
            amf_n5geir_eic_failure_cause());
    amf_self()->eir.failure_action = OGS_EIR_ACTION_ALLOW;
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_REQUEST_ACCEPTED,
            amf_n5geir_eic_handle_equipment_status(amf_ue, &message));
    ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_REQUEST_ACCEPTED,
            amf_n5geir_eic_failure_cause());

    amf_self()->eir.unknown_action = saved_unknown;
    amf_self()->eir.failure_action = saved_failure;
    ogs_free(amf_ue);
}

abts_suite *test_amf_eir(abts_suite *suite)
{
    ogs_log_level_e saved_log_level;

    suite = ADD_SUITE(suite)

    if (!ogs_log_find_domain("amf"))
        ogs_log_install_domain(
                &__amf_log_domain, "amf", ogs_core()->log.level);
    saved_log_level = ogs_log_get_domain_level(__amf_log_domain);
    ogs_log_set_domain_level(__amf_log_domain, OGS_LOG_NONE);

    abts_run_test(suite, eir_test_check_wanted, NULL);
    abts_run_test(suite, eir_test_equipment_status, NULL);
    abts_run_test(suite, eir_test_unknown_and_failure, NULL);

    ogs_log_set_domain_level(__amf_log_domain, saved_log_level);
    return suite;
}
