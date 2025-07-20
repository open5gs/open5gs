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

#include "binding.h"
#include "sbi-path.h"
#include "gn-handler.h"
#include "gx-handler.h"
#include "gy-handler.h"
#include "n4-handler.h"
#include "s5c-handler.h"
#include "nnrf-handler.h"
#include "nsmf-handler.h"
#include "nudm-handler.h"
#include "npcf-handler.h"
#include "namf-handler.h"
#include "gsm-handler.h"
#include "ngap-handler.h"
#include "gtp-path.h"
#include "pfcp-path.h"
#include "ngap-path.h"
#include "fd-path.h"
#include "local-path.h"

static uint8_t gtp_cause_from_diameter(uint8_t gtp_version,
        const uint32_t dia_err, const uint32_t *dia_exp_err)
{
    switch (gtp_version) {
    case 1:
        switch (dia_err) {
        case OGS_DIAM_UNKNOWN_SESSION_ID:
            return OGS_GTP1_CAUSE_APN_ACCESS_DENIED;
        }
        break;
    case 2:
        switch (dia_err) {
        case OGS_DIAM_UNKNOWN_SESSION_ID:
            return OGS_GTP2_CAUSE_APN_ACCESS_DENIED_NO_SUBSCRIPTION;
        }
        break;
    }

    ogs_error("Unexpected Diameter Result Code %d/%d, defaulting to severe "
              "network failure",
              dia_err, dia_exp_err ? *dia_exp_err : -1);
    switch (gtp_version) {
    case 1:
        return OGS_GTP1_CAUSE_USER_AUTHENTICATION_FAILED;
    case 2:
    default:
        return OGS_GTP2_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER;
    }
}

static void send_gtp_create_err_msg(const smf_sess_t *sess,
        ogs_gtp_xact_t *gtp_xact, uint8_t gtp_cause)
{
    if (gtp_xact->gtp_version == 1)
        ogs_gtp1_send_error_message(gtp_xact, sess->sgw_s5c_teid,
            OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE, gtp_cause);
    else
        ogs_gtp2_send_error_message(gtp_xact, sess->sgw_s5c_teid,
            OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, gtp_cause);
}

static void send_gtp_delete_err_msg(const smf_sess_t *sess,
        ogs_gtp_xact_t *gtp_xact, uint8_t gtp_cause)
{
    if (gtp_xact->gtp_version == 1)
        ogs_gtp1_send_error_message(gtp_xact, sess->sgw_s5c_teid,
            OGS_GTP1_DELETE_PDP_CONTEXT_RESPONSE_TYPE, gtp_cause);
    else
        ogs_gtp2_send_error_message(gtp_xact, sess->sgw_s5c_teid,
            OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE, gtp_cause);
}

static bool send_ccr_init_req_gx_gy(smf_sess_t *sess, ogs_gtp_xact_t *gtp_xact)
{
    int use_gy = smf_use_gy_iface();

    if (use_gy == -1) {
        ogs_error("No Gy Diameter Peer");
        /* TODO: drop Gx connection here,
         * possibly move to another "releasing" state! */
        uint8_t gtp_cause = (gtp_xact->gtp_version == 1) ?
                OGS_GTP1_CAUSE_NO_RESOURCES_AVAILABLE :
                OGS_GTP2_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER;
        send_gtp_create_err_msg(sess, gtp_xact, gtp_cause);
        return false;
    }

    sess->sm_data.gx_ccr_init_in_flight = true;
    smf_gx_send_ccr(
            sess, gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID,
            OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST);

    if (use_gy == 1) {
        /* Gy is available,
         * set up session for the bearer before accepting it towards the UE */
        sess->sm_data.gy_ccr_init_in_flight = true;
        smf_gy_send_ccr(
                sess, gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID,
                OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST);
    }
    return true;
}

static bool send_ccr_termination_req_gx_gy_s6b(
        smf_sess_t *sess, ogs_gtp_xact_t *gtp_xact)
{
    /* TODO: we should take into account here whether "sess" has an active Gy
       session created, not whether one was supposedly created as per policy */
    int use_gy = smf_use_gy_iface();

    if (use_gy == -1) {
        ogs_error("No Gy Diameter Peer");
        /* TODO: drop Gx connection here,
         * possibly move to another "releasing" state! */
        uint8_t gtp_cause = (gtp_xact->gtp_version == 1) ?
                OGS_GTP1_CAUSE_NO_RESOURCES_AVAILABLE :
                OGS_GTP2_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER;
        send_gtp_delete_err_msg(sess, gtp_xact, gtp_cause);
        return false;
    }

    if (sess->gtp_rat_type == OGS_GTP2_RAT_TYPE_WLAN) {
        sess->sm_data.s6b_str_in_flight = true;
        smf_s6b_send_str(sess, gtp_xact,
            OGS_DIAM_TERMINATION_CAUSE_DIAMETER_LOGOUT);
    }

    sess->sm_data.gx_ccr_term_in_flight = true;
    smf_gx_send_ccr(
            sess, gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID,
            OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST);

    if (use_gy == 1) {
        /* Gy is available,
         * set up session for the bearer before accepting it towards the UE */
        sess->sm_data.gy_ccr_term_in_flight = true;
        smf_gy_send_ccr(
                sess, gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID,
                OGS_DIAM_GY_CC_REQUEST_TYPE_TERMINATION_REQUEST);
    }
    return true;
}

void smf_gsm_state_initial(ogs_fsm_t *s, smf_event_t *e)
{
    int rv;
    char *strerror = NULL;
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;

    ogs_gtp1_message_t *gtp1_message = NULL;
    ogs_gtp2_message_t *gtp2_message = NULL;
    uint8_t gtp1_cause, gtp2_cause;
    ogs_gtp_xact_t *gtp_xact = NULL;

    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        /* reset state: */
        sess->sm_data.s6b_aar_in_flight = false;
        sess->sm_data.gx_ccr_init_in_flight = false;
        sess->sm_data.gy_ccr_init_in_flight = false;
        sess->sm_data.s6b_aaa_err = ER_DIAMETER_SUCCESS;
        sess->sm_data.gx_cca_init_err = ER_DIAMETER_SUCCESS;
        sess->sm_data.gy_cca_init_err = ER_DIAMETER_SUCCESS;
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case SMF_EVT_GN_MESSAGE:
        gtp1_message = e->gtp1_message;
        ogs_assert(gtp1_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gtp1_message->h.type) {
        case OGS_GTP1_CREATE_PDP_CONTEXT_REQUEST_TYPE:
            gtp1_cause = smf_gn_handle_create_pdp_context_request(sess,
                            gtp_xact,
                            &e->gtp1_message->create_pdp_context_request);
            if (gtp1_cause != OGS_GTP1_CAUSE_REQUEST_ACCEPTED) {
                send_gtp_create_err_msg(sess, gtp_xact, gtp1_cause);
                return;
            }
            if (send_ccr_init_req_gx_gy(sess, gtp_xact) == true)
                OGS_FSM_TRAN(s, smf_gsm_state_wait_epc_auth_initial);
        }
        break;

    case SMF_EVT_S5C_MESSAGE:
        gtp2_message = e->gtp2_message;
        ogs_assert(gtp2_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gtp2_message->h.type) {
        case OGS_GTP2_CREATE_SESSION_REQUEST_TYPE:
            gtp2_cause = smf_s5c_handle_create_session_request(sess,
                            gtp_xact,
                            &e->gtp2_message->create_session_request);
            if (gtp2_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                send_gtp_create_err_msg(sess, gtp_xact, gtp2_cause);
                return;
            }
            switch (sess->gtp_rat_type) {
            case OGS_GTP2_RAT_TYPE_EUTRAN:
                if (send_ccr_init_req_gx_gy(sess, gtp_xact) == true)
                    OGS_FSM_TRAN(s, smf_gsm_state_wait_epc_auth_initial);
                break;
            case OGS_GTP2_RAT_TYPE_WLAN:
                smf_s6b_send_aar(sess, gtp_xact);
                sess->sm_data.s6b_aar_in_flight = true;
                OGS_FSM_TRAN(s, smf_gsm_state_wait_epc_auth_initial);
                /* Gx/Gy Init Req is done after s6b AAR + AAA */
                break;
            default:
                ogs_error("Unknown RAT Type [%d]", sess->gtp_rat_type);
                ogs_assert_if_reached();
            }
            break;

        default:
            ogs_error("Not implemented(type:%d)", gtp2_message->h.type);
        }
        break;

    case OGS_EVENT_SBI_SERVER:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);
        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXTS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                DEFAULT
                    if (smf_nsmf_handle_create_sm_context(
                            sess, stream, sbi_message) == false) {
                        ogs_error("smf_nsmf_handle_create_sm_context() failed");
                        OGS_FSM_TRAN(s, smf_gsm_state_exception);
                        break;
                    }

                    if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                        ogs_info("[%s:%d] SMContextCreate HR Roaming in V-SMF",
                                smf_ue->supi, sess->psi);
                        OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_establishment);
                    }
                END
                break;
            CASE(OGS_SBI_RESOURCE_NAME_PDU_SESSIONS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                DEFAULT
                    if (smf_nsmf_handle_create_data_in_hsmf(
                            sess, stream, sbi_message) == false) {
                        ogs_error(
                                "smf_nsmf_handle_create_pdu_session_in_hsmf() "
                                "failed");
                        OGS_FSM_TRAN(s, smf_gsm_state_exception);
                        break;
                    }
                    OGS_FSM_TRAN(s,
                            smf_gsm_state_wait_5gc_sm_policy_association);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                        "Invalid resource name",
                        sbi_message->h.resource.component[0], NULL));
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                    "Invalid API name", sbi_message->h.service.name,
                    NULL));
            OGS_FSM_TRAN(s, smf_gsm_state_exception);
        END
        break;

    case SMF_EVT_5GSM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);
        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        switch (nas_message->gsm.h.message_type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
            rv = gsm_handle_pdu_session_establishment_request(sess, stream,
                    &nas_message->gsm.pdu_session_establishment_request);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NAS message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
                break;
            }

            OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_sm_policy_association);
            break;

        default:
            strerror = ogs_msprintf("Unknown message [%d]",
                    nas_message->gsm.h.message_type);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror,
                    NULL, NULL));
            ogs_free(strerror);

            OGS_FSM_TRAN(s, smf_gsm_state_exception);
        }
        break;
    default:
        ogs_error("No handler for event %s", smf_event_get_name(e));
        break;
    }
}

void smf_gsm_state_wait_epc_auth_initial(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sess_t *sess = NULL;

    ogs_diam_s6b_message_t *s6b_message = NULL;
    ogs_diam_gy_message_t *gy_message = NULL;
    ogs_diam_gx_message_t *gx_message = NULL;
    uint32_t diam_err;
    bool need_gy_terminate = false;

    ogs_gtp_xact_t *gtp_xact = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case SMF_EVT_S6B_MESSAGE:
        s6b_message = e->s6b_message;
        ogs_assert(s6b_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(s6b_message->cmd_code) {
        case OGS_DIAM_S6B_CMD_AUTHENTICATION_AUTHORIZATION:
            sess->sm_data.s6b_aar_in_flight = false;
            sess->sm_data.s6b_aaa_err = s6b_message->result_code;
            if (s6b_message->result_code == ER_DIAMETER_SUCCESS) {
                send_ccr_init_req_gx_gy(sess, gtp_xact);
                return;
            }
            goto test_can_proceed;
        }
        break;

    case SMF_EVT_GX_MESSAGE:
        gx_message = e->gx_message;
        ogs_assert(gx_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gx_message->cmd_code) {
        case OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL:
            switch(gx_message->cc_request_type) {
            case OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST:
                ogs_assert(gtp_xact);
                diam_err = smf_gx_handle_cca_initial_request(sess,
                                gx_message, gtp_xact);
                sess->sm_data.gx_ccr_init_in_flight = false;
                sess->sm_data.gx_cca_init_err = diam_err;
                goto test_can_proceed;
            }
            break;
        }
        break;

    case SMF_EVT_GY_MESSAGE:
        gy_message = e->gy_message;
        ogs_assert(gy_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gy_message->cmd_code) {
        case OGS_DIAM_GY_CMD_CODE_CREDIT_CONTROL:
            switch(gy_message->cc_request_type) {
            case OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST:
                ogs_assert(gtp_xact);
                diam_err = smf_gy_handle_cca_initial_request(sess,
                                gy_message, gtp_xact, &need_gy_terminate);
                sess->sm_data.gy_ccr_init_in_flight = false;
                sess->sm_data.gy_cca_init_err = diam_err;
                goto test_can_proceed;
            }
            break;
        }
        break;
    }
    return;

test_can_proceed:
    /* First wait for both Gx and Gy requests to be done: */
    if (!sess->sm_data.s6b_aar_in_flight &&
        !sess->sm_data.gx_ccr_init_in_flight &&
        !sess->sm_data.gy_ccr_init_in_flight) {
        diam_err = ER_DIAMETER_SUCCESS;
        if (sess->sm_data.s6b_aaa_err != ER_DIAMETER_SUCCESS)
            diam_err = sess->sm_data.s6b_aaa_err;
        if (sess->sm_data.gx_cca_init_err != ER_DIAMETER_SUCCESS)
            diam_err = sess->sm_data.gx_cca_init_err;
        if (sess->sm_data.gy_cca_init_err != ER_DIAMETER_SUCCESS)
            diam_err = sess->sm_data.gy_cca_init_err;

        if (diam_err == ER_DIAMETER_SUCCESS) {
            OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_establishment);
            ogs_assert(gtp_xact);
            ogs_assert(OGS_OK ==
                smf_epc_pfcp_send_session_establishment_request(
                    sess,
                    gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID, 0));
        } else {
            /* Tear down Gx/Gy session if its sm_data.*init_err == ER_DIAMETER_SUCCESS */
            if (sess->sm_data.gx_cca_init_err == ER_DIAMETER_SUCCESS) {
                sess->sm_data.gx_ccr_term_in_flight = true;
                smf_gx_send_ccr(
                    sess, gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID,
                    OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST);
            }
            if (smf_use_gy_iface() == 1 &&
                (sess->sm_data.gy_cca_init_err == ER_DIAMETER_SUCCESS || need_gy_terminate)) {
                sess->sm_data.gy_ccr_term_in_flight = true;
                smf_gy_send_ccr(
                    sess, gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID,
                    OGS_DIAM_GY_CC_REQUEST_TYPE_TERMINATION_REQUEST);
            }
            uint8_t gtp_cause = gtp_cause_from_diameter(
                                    gtp_xact->gtp_version, diam_err, NULL);
            send_gtp_create_err_msg(sess, gtp_xact, gtp_cause);
        }
    }
}

void smf_gsm_state_wait_5gc_sm_policy_association(ogs_fsm_t *s, smf_event_t *e)
{
    char *strerror = NULL;
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                    stream_id <= OGS_MAX_POOL_ID);

            stream = ogs_sbi_stream_find_by_id(stream_id);
            if (!stream) {
                ogs_error("STREAM has already been removed [%d]", stream_id);
                break;
            }

            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                    strerror = ogs_msprintf("[%s:%d] HTTP response error [%d]",
                            smf_ue->supi, sess->psi, sbi_message->res_status);
                    ogs_assert(strerror);

                    ogs_error("%s", strerror);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(
                            stream, sbi_message->res_status,
                            sbi_message, strerror, NULL,
                            (sbi_message->ProblemDetails) ?
                                    sbi_message->ProblemDetails->cause : NULL));
                    ogs_free(strerror);

                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                }

                if (smf_nudm_sdm_handle_get(
                            sess, stream, sbi_message) == false) {
                    ogs_error("smf_nudm_sdm_handle_get() failed");
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                if ((sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) &&
                    (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED)) {
                    strerror = ogs_msprintf("[%s:%d] HTTP response error [%d]",
                            smf_ue->supi, sess->psi, sbi_message->res_status);
                    ogs_assert(strerror);

                    ogs_error("%s", strerror);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(
                            stream, sbi_message->res_status,
                            sbi_message, strerror, NULL, NULL));
                    ogs_free(strerror);

                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                }

                if (smf_nudm_sdm_handle_subscription(
                        sess, stream, sbi_message) == false) {
                    ogs_error("smf_nudm_sdm_handle_subscription() failed");
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                }
                break;

            DEFAULT
                strerror = ogs_msprintf("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[1]);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        sbi_message, strerror, NULL, NULL));
                ogs_free(strerror);

                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            /*
             * TS23.502
             * 4.3.2.2 UE Requested PDU Session Establishment
             *
             * 4.3.2.2.1 Non-roaming and Roaming with Local Breakout
             *
             * By here, SMF has already sent a response to AMF in
             * smf_nudm_sdm_handle_get. Therefore, 'stream = e->h.sbi.data'
             * should not be used here. Instead of sending additional error
             * replies to AMF over 'stream', SMF should send a
             * Namf_Communication_N1N2MessageTransfer request by transitioning
             * into smf_gsm_state_5gc_n1_n2_reject. This is according to
             *
             * 11.  ...
             * If the PDU session establishment failed anywhere between step 5
             * and step 11, then the Namf_Communication_N1N2MessageTransfer
             * request shall include the N1 SM container with a PDU Session
             * Establishment Reject message ...
             *
             * 4.3.2.2.2 Home-routed Roaming
             * However, Home Routed Roaming requires a stream.
             * This is because we need the stream
             * when we send the Nsmf_PDUSession_Create Response in step 13.
             *
             * 13. H-SMF to V-SMF: Nsmf_PDUSession_Create Response ...
             */
            stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            if (stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID)
                stream = ogs_sbi_stream_find_by_id(stream_id);

            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICIES)
                if (sbi_message->h.resource.component[1]) {
                    ogs_error("[%s:%d] Unknown resource name [%s]",
                            smf_ue->supi, sess->psi,
                            sbi_message->h.resource.component[1]);
                    OGS_FSM_TRAN(s, smf_gsm_state_5gc_n1_n2_reject);
                } else if (sbi_message->res_status !=
                           OGS_SBI_HTTP_STATUS_CREATED) {
                    ogs_error("[%s:%d] HTTP response error [%d]",
                            smf_ue->supi, sess->psi,
                            sbi_message->res_status);
                    OGS_FSM_TRAN(s, smf_gsm_state_5gc_n1_n2_reject);
                } else if (smf_npcf_smpolicycontrol_handle_create(
                        sess, stream, sbi_message) == true) {
                    OGS_FSM_TRAN(s,
                        &smf_gsm_state_wait_pfcp_establishment);
                } else {
                    ogs_error(
                        "smf_npcf_smpolicycontrol_handle_create() failed");
                    OGS_FSM_TRAN(s, smf_gsm_state_5gc_n1_n2_reject);
                }
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                OGS_FSM_TRAN(s, smf_gsm_state_5gc_n1_n2_reject);
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                    smf_ue->supi, sess->psi, sbi_message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    default:
        ogs_error("No handler for event %s", smf_event_get_name(e));
        break;
    }
}


void smf_gsm_state_wait_pfcp_establishment(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sess_t *sess = NULL;
    uint8_t pfcp_cause, gtp_cause;
    smf_n1_n2_message_transfer_param_t param;

    ogs_pfcp_xact_t *pfcp_xact = NULL;
    ogs_pfcp_message_t *pfcp_message = NULL;
    int rv;

    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case SMF_EVT_N4_MESSAGE:
        pfcp_xact = ogs_pfcp_xact_find_by_id(e->pfcp_xact_id);
        ogs_assert(pfcp_xact);
        pfcp_message = e->pfcp_message;
        ogs_assert(pfcp_message);

        if (pfcp_xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                pfcp_xact->assoc_stream_id <= OGS_MAX_POOL_ID)
            stream = ogs_sbi_stream_find_by_id(pfcp_xact->assoc_stream_id);

        switch (pfcp_message->h.type) {
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            if (pfcp_xact->epc) {
                ogs_gtp_xact_t *gtp_xact =
                    ogs_gtp_xact_find_by_id(pfcp_xact->assoc_xact_id);
                ogs_assert(gtp_xact);

                pfcp_cause = smf_epc_n4_handle_session_establishment_response(
                        sess, pfcp_xact,
                        &pfcp_message->pfcp_session_establishment_response);
                if (pfcp_cause != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
                    /* FIXME: tear down Gy and Gx */
                    gtp_cause = gtp_cause_from_pfcp(
                                    pfcp_cause, gtp_xact->gtp_version);
                    send_gtp_create_err_msg(sess, gtp_xact, gtp_cause);
                    return;
                }

                switch (gtp_xact->gtp_version) {
                case 1:
                    rv = smf_gtp1_send_create_pdp_context_response(
                            sess, gtp_xact);
                    break;
                case 2:
                    rv = smf_gtp2_send_create_session_response(
                            sess, gtp_xact);
                    break;
                default:
                    rv = OGS_ERROR;
                    break;
                }
                /* If no CreatePDPCtxResp can be sent,
                 * then tear down the session: */
                if (rv != OGS_OK) {
                    OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
                    return;
                }

                if (sess->gtp_rat_type == OGS_GTP2_RAT_TYPE_WLAN) {
                    /*
                     * TS23.214
                     * 6.3.1.7 Procedures with modification of bearer
                     * p50
                     * 2.  ...
                     * For "PGW/MME initiated bearer deactivation procedure",
                     * PGW-C shall indicate PGW-U to stop counting and stop
                     * forwarding downlink packets for the affected bearer(s).
                     */
                    ogs_assert(OGS_OK ==
                        smf_epc_pfcp_send_deactivation(sess,
                            OGS_GTP2_CAUSE_RAT_CHANGED_FROM_3GPP_TO_NON_3GPP));
                }
                smf_bearer_binding(sess);
            } else {
                int r = 0;
                pfcp_cause = smf_5gc_n4_handle_session_establishment_response(
                        sess, pfcp_xact,
                        &pfcp_message->pfcp_session_establishment_response);
                if (pfcp_cause != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
                    OGS_FSM_TRAN(s, smf_gsm_state_5gc_n1_n2_reject);
                    return;
                }
                if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                    r = smf_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                            smf_nsmf_pdusession_build_create_data,
                            sess, NULL, 0, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else if (HOME_ROUTED_ROAMING_IN_HSMF(sess)) {
                    r = smf_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NUDM_UECM, NULL,
                            smf_nudm_uecm_build_registration,
                            sess, stream,
                            SMF_UECM_STATE_REGISTERED_HR,
                            NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else {
                    memset(&param, 0, sizeof(param));
                    param.state = SMF_UE_REQUESTED_PDU_SESSION_ESTABLISHMENT;
                    param.n1smbuf =
                        gsm_build_pdu_session_establishment_accept(sess);
                    if (!param.n1smbuf) {
                        ogs_error(
                                "gsm_build_pdu_session_establishment_accept() "
                                "failed");
                        OGS_FSM_TRAN(s, smf_gsm_state_5gc_n1_n2_reject);
                        return;
                    }
                    param.n2smbuf =
                        ngap_build_pdu_session_resource_setup_request_transfer(
                                sess);
                    ogs_assert(param.n2smbuf);
                    smf_namf_comm_send_n1_n2_message_transfer(
                            sess, NULL, &param);
                }
            }

            OGS_FSM_TRAN(s, smf_gsm_state_operational);
            break;

        default:
            ogs_error("cannot handle PFCP message type[%d]",
                    pfcp_message->h.type);
        }
        break;

    case SMF_EVT_N4_TIMER:
        switch (e->h.timer_id) {
        case SMF_TIMER_PFCP_NO_ESTABLISHMENT_RESPONSE:
            OGS_FSM_TRAN(s, smf_gsm_state_5gc_n1_n2_reject);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("Unknown event [%s]", smf_event_get_name(e));
    }
}

void smf_gsm_state_operational(ogs_fsm_t *s, smf_event_t *e)
{
    int r, rv, ngap_state;
    char *strerror = NULL;
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    smf_bearer_t *qos_flow = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_pfcp_xact_t *pfcp_xact = NULL;
    ogs_pfcp_message_t *pfcp_message = NULL;
    uint8_t pfcp_cause;

    ogs_diam_gy_message_t *gy_message = NULL;
    uint32_t diam_err;

    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_gtp_xact_t *gtp_xact = NULL;
    ogs_gtp1_message_t *gtp1_message = NULL;
    ogs_gtp2_message_t *gtp2_message = NULL;
    uint8_t gtp1_cause, gtp2_cause;
    bool rc, release;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case SMF_EVT_GN_MESSAGE:
        gtp1_message = e->gtp1_message;
        ogs_assert(gtp1_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gtp1_message->h.type) {
        case OGS_GTP1_DELETE_PDP_CONTEXT_REQUEST_TYPE:
            gtp1_cause = smf_gn_handle_delete_pdp_context_request(sess,
                            gtp_xact,
                            &gtp1_message->delete_pdp_context_request);
            if (gtp1_cause != OGS_GTP1_CAUSE_REQUEST_ACCEPTED) {
                ogs_gtp1_send_error_message(gtp_xact, sess->sgw_s5c_teid,
                        OGS_GTP1_DELETE_PDP_CONTEXT_RESPONSE_TYPE, gtp1_cause);
                return;
            }
            OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
        }
        break;

    case SMF_EVT_S5C_MESSAGE:
        gtp2_message = e->gtp2_message;
        ogs_assert(gtp2_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gtp2_message->h.type) {
        case OGS_GTP2_DELETE_SESSION_REQUEST_TYPE:
            gtp2_cause = smf_s5c_handle_delete_session_request(
                            sess, gtp_xact,
                            &gtp2_message->delete_session_request);
            if (gtp2_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                ogs_gtp2_send_error_message(gtp_xact, sess->sgw_s5c_teid,
                        OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE, gtp2_cause);
                return;
            }
            OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
            break;
        case OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE:
            release = smf_s5c_handle_delete_bearer_response(
                sess, gtp_xact, &e->gtp2_message->delete_bearer_response);
            if (release) {
                OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
            }
            break;

        default:
            ogs_error("Not implemented(type:%d)", gtp2_message->h.type);
        }
        break;

    case SMF_EVT_N4_MESSAGE:
        pfcp_xact = ogs_pfcp_xact_find_by_id(e->pfcp_xact_id);
        ogs_assert(pfcp_xact);
        pfcp_message = e->pfcp_message;
        ogs_assert(pfcp_message);

        switch (pfcp_message->h.type) {
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            if ((pfcp_xact->create_flags &
                        OGS_PFCP_CREATE_RESTORATION_INDICATION)) {
                ogs_pfcp_session_establishment_response_t *rsp = NULL;
                ogs_pfcp_f_seid_t *up_f_seid = NULL;

                rsp = &pfcp_message->pfcp_session_establishment_response;
                if (rsp->up_f_seid.presence == 0) {
                    ogs_error("No UP F-SEID");
                    break;
                }
                up_f_seid = rsp->up_f_seid.data;
                ogs_assert(up_f_seid);
                sess->upf_n4_seid = be64toh(up_f_seid->seid);
            } else {
                ogs_error("cannot handle PFCP Session Establishment Response");
            }
            break;

        case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
            ogs_error("EPC Session Released by Error Indication");
            OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
            break;

        case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE:
            pfcp_cause = smf_n4_handle_session_report_request(sess, pfcp_xact,
                            &pfcp_message->pfcp_session_report_request);
            if (pfcp_cause != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
                OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
            }
            break;

        default:
            ogs_error("cannot handle PFCP message type[%d]",
                    pfcp_message->h.type);
        }
        break;

    case SMF_EVT_GY_MESSAGE:
        gy_message = e->gy_message;
        ogs_assert(gy_message);
        pfcp_xact = ogs_pfcp_xact_find_by_id(e->pfcp_xact_id);

        switch(gy_message->cmd_code) {
        case OGS_DIAM_GY_CMD_CODE_CREDIT_CONTROL:
            switch (gy_message->cc_request_type) {
            case OGS_DIAM_GY_CC_REQUEST_TYPE_UPDATE_REQUEST:
                ogs_assert(pfcp_xact);
                diam_err = smf_gy_handle_cca_update_request(
                        sess, gy_message, pfcp_xact);
                if (diam_err != ER_DIAMETER_SUCCESS)
                    OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
                break;
            }
            break;
        }
        break;

    case OGS_EVENT_SBI_SERVER:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXTS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                    smf_nsmf_handle_update_sm_context(
                            sess, stream, sbi_message);
                    break;
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
    /*
     * Network-requested PDU Session Release
     *
     * 1.  V*: smf_nsmf_handle_release_sm_context
     * 2.  V*: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *         OGS_PFCP_MODIFY_DEACTIVATE
     * 3.  V*: OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 4.  V: smf_nsmf_pdusession_build_release_data
     * 5.  H: smf_nsmf_handle_release_data_in_hsmf
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 7.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 8.  H: ogs_sbi_send_http_status_no_content
     * 9.  H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V: smf_nsmf_handle_release_data_in_hsmf
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V*: ogs_sbi_send_http_status_no_content
     * 16. V*: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                    smf_nsmf_handle_release_sm_context(
                            sess, stream, sbi_message);

                    if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                        ogs_assert(OGS_OK ==
                            smf_5gc_pfcp_send_all_pdr_modification_request(
                                sess, stream,
                                OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                                OGS_PFCP_MODIFY_UL_ONLY|
                                OGS_PFCP_MODIFY_DEACTIVATE,
                                OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT,
                                0));
                    } else {
                        e->h.sbi.state =
                            OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT;
                        OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
                    }
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_PDU_SESSIONS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                    rc = smf_nsmf_handle_update_data_in_hsmf(
                            sess, stream, sbi_message);
                    if (rc == true) {
                        switch (sess->nsmf_param.request_indication) {
                        case OpenAPI_request_indication_UE_REQ_PDU_SES_MOD:
                            if (sess->nsmf_param.up_cnx_state ==
                                    OpenAPI_up_cnx_state_DEACTIVATED) {
    /*
     * UE-requested PDU Session Modification(DEACTIVATED)
     *
     * For Home Routed Roaming, delegate PFCP deactivation to H-SMF by
     * sending UP_CNX_STATE=DEACTIVATED via HsmfUpdateData.
     *
     * 1.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 2.  V: smf_nsmf_pdusession_build_hsmf_update_data
     *        SMF_UPDATE_STATE_HR_DEACTIVATED
     * 3.  H*: smf_nsmf_handle_update_data_in_hsmf
     * 4.  H*: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 5.  H*: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_DL_ONLY|
     *         OGS_PFCP_MODIFY_DEACTIVATE
     * 6.  H: ogs_sbi_send_http_status_no_content
     * 7.  V: case SMF_UPDATE_STATE_HR_DEACTIVATED:
     * 8.  V: smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *          OpenAPI_up_cnx_state_DEACTIVATED)
     */
                                ogs_assert(OGS_OK ==
                                    smf_5gc_pfcp_send_all_pdr_modification_request(
                                        sess, stream,
                                        OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                                        OGS_PFCP_MODIFY_DL_ONLY|
                                        OGS_PFCP_MODIFY_DEACTIVATE, 0, 0));
                            } else if (sess->nsmf_param.up_cnx_state ==
                                    OpenAPI_up_cnx_state_ACTIVATING) {
    /*
     * UE-requested PDU Session Modification(ACTIVATING)
     *
     * 1.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 2.  V: smf_nsmf_pdusession_build_hsmf_update_data
     *        SMF_UPDATE_STATE_HR_ACTIVATING
     * 3.  H*: smf_nsmf_handle_update_data_in_hsmf
     * 4.  H*: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 5.  H*: ogs_sbi_send_http_status_no_content
     * 6.  V: ngap_build_pdu_session_resource_setup_request_transfer
     * 7.  V: smf_sbi_send_sm_context_updated_data(
     *          OpenAPI_up_cnx_state_ACTIVATING,
     *          OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ, n2smbuf)
     */
                                ogs_assert(true ==
                                        ogs_sbi_send_http_status_no_content(
                                            stream));
                            } else if (sess->nsmf_param.up_cnx_state ==
                                    OpenAPI_up_cnx_state_ACTIVATED) {
    /*
     * UE-requested PDU Session Modification(ACTIVATED)
     *
     * 1.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 2.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_DL_ONLY|
     *        OGS_PFCP_MODIFY_OUTER_HEADER_REMOVAL|OGS_PFCP_MODIFY_ACTIVATE
     * 3.  V: if (sess->up_cnx_state == OpenAPI_up_cnx_state_ACTIVATING)
     *           pfcp_flags |= OGS_PFCP_MODIFY_FROM_ACTIVATING;
     * 4.  V: flags & OGS_PFCP_MODIFY_FROM_ACTIVATING ?
     *           SMF_UPDATE_STATE_HR_ACTIVATED_FROM_ACTIVATING :
     *           SMF_UPDATE_STATE_HR_ACTIVATED_FROM_NON_ACTIVATING,
     * 5.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 6.  H: smf_nsmf_handle_update_data_in_hsmf
     * 7.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 8.  H*: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_DL_ONLY|
     *         OGS_PFCP_MODIFY_ACTIVATE
     * 9.  H: ogs_sbi_send_http_status_no_content
     * 10. V: case SMF_UPDATE_STATE_HR_ACTIVATED_FROM_ACTIVATING:
     *           sess->up_cnx_state = OpenAPI_up_cnx_state_ACTIVATED;
     *           smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *               OpenAPI_up_cnx_state_ACTIVATED);
     *        case SMF_UPDATE_STATE_HR_ACTIVATED_FROM_NON_ACTIVATING:
     *           ogs_sbi_send_http_status_no_content
     */
                                bool far_update = false;

                                if (memcmp(
                                        &sess->nsmf_param.dl_ip,
                                        &sess->remote_dl_ip,
                                        sizeof(sess->nsmf_param.dl_ip)) != 0 ||
                                    sess->nsmf_param.dl_teid !=
                                        sess->remote_dl_teid)
                                    far_update = true;

                                ogs_list_for_each(
                                        &sess->bearer_list, qos_flow) {
                                    ogs_pfcp_far_t *dl_far = qos_flow->dl_far;
                                    ogs_assert(dl_far);

                                    if (dl_far->apply_action !=
                                            OGS_PFCP_APPLY_ACTION_FORW)
                                        far_update = true;

                                    dl_far->apply_action =
                                        OGS_PFCP_APPLY_ACTION_FORW;
                                    ogs_assert(OGS_OK ==
                                        ogs_pfcp_ip_to_outer_header_creation(
                                            &sess->remote_dl_ip,
                                            &dl_far->outer_header_creation,
                                            &dl_far->outer_header_creation_len)
                                        );
                                    dl_far->outer_header_creation.teid =
                                        sess->remote_dl_teid;
                                }

                                if (far_update) {
                                    ogs_assert(OGS_OK ==
                                        smf_5gc_pfcp_send_all_pdr_modification_request(
                                            sess, stream,
                                            OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                                            OGS_PFCP_MODIFY_DL_ONLY|
                                            OGS_PFCP_MODIFY_ACTIVATE, 0, 0));
                                } else {
                                    ogs_assert(true ==
                                            ogs_sbi_send_http_status_no_content(
                                                stream));
                                }
                            } else if (sess->nsmf_param.pfcp_flags) {
                                ogs_assert(true ==
                                        ogs_sbi_send_http_status_no_content(
                                            stream));

                                ogs_assert(OGS_OK ==
                                        smf_5gc_pfcp_send_qos_flow_list_modification_request(
                                            sess, NULL,
                                            OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                                            OGS_PFCP_MODIFY_UE_REQUESTED|
                                            sess->nsmf_param.pfcp_flags, 0));
                            } else {
                                ogs_fatal("Invalid [upCnxState:%d]"
                                        "[pfcp_flags:0x%llx]",
                                        sess->nsmf_param.up_cnx_state,
                                        (long long)sess->nsmf_param.pfcp_flags);
                                ogs_assert_if_reached();
                            }
                            break;
                        case OpenAPI_request_indication_UE_REQ_PDU_SES_REL:
                        case OpenAPI_request_indication_NW_REQ_PDU_SES_REL:
                            if (sess->nsmf_param.request_indication ==
                                    OpenAPI_request_indication_UE_REQ_PDU_SES_REL) {
    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H*: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H*: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H*: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H*: ogs_sbi_send_http_status_no_content
     * 8.  H*: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V: smf_nsmf_handle_update_data_in_vsmf
     * 12. V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ngap_build_pdu_session_resource_release_command_transfer+
     *        gsm_build_pdu_session_release_command
     * 16  V: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V: ogs_sbi_send_http_status_no_content(stream)
     * 18. V: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
     *        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H: case OGS_EVENT_SBI_CLIENT:
     * 22. H: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V: case OGS_EVENT_SBI_SERVER:
     * 26. V: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V: ogs_sbi_send_http_status_no_content+
     *        smf_sbi_send_sm_context_status_notify
     * 28. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                                e->h.sbi.state =
                                    OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED;
                            } else if (sess->nsmf_param.request_indication ==
                                    OpenAPI_request_indication_NW_REQ_PDU_SES_REL) {
    /*
     * Network-requested PDU Session Release(DUPLICATED)
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT,
     * 3.  V: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H*: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H*: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 6.  H*: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 7.  H*: ogs_sbi_send_http_status_no_content
     * 8.  H*: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 13. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 14. V: ogs_sbi_send_http_status_no_content
     * 15. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                                e->h.sbi.state =
                                OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT;
                            }
    /*
     * TS23.502 clause 4.3.4.3 UE or network requested PDU Session Release
     * for Home-routed Roaming.
     *
     * In step 1a/1f, upon receiving Nsmf_PDUSession_Update from V-SMF,
     * H-SMF SHALL immediately send the Update Response, then issue PFCP
     * Session Deletion. This ordering is per the standard, even for duplicate
     * sessions, and may overlap with AMF’s concurrent Create Session.
     *
     * 1a. (UE initiated release)
     * 1f. This step is the same as step 1f in clause 4.3.4.2,
     *     with the addition that:
     *  - the V-SMF initiates N4 Session Modification to instruct the V-UPF
     *    to stop forwarding uplink traffic; and
     *  - the V-SMF invokes the Nsmf_PDUSession_Update Request towards H-SMF.
     *    (OpenAPI_cause_REL_DUE_TO_DUPLICATE_SESSION_ID);
     */
                            ogs_assert(true ==
                                    ogs_sbi_send_http_status_no_content(
                                        stream));

                            OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
                            break;
                        default:
                            ogs_error("Unknown request_indication:%d",
                                    sess->nsmf_param.request_indication);
                        }
                    } else {
                        ogs_error("smf_nsmf_handle_update_data_in_hsmf() "
                                "failed");
                    }
                    break;
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
    /*
     * Network-requested PDU Session Release
     *
     * 1.  V: smf_nsmf_handle_release_sm_context
     * 2.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 3.  V: OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 4.  V: smf_nsmf_pdusession_build_release_data
     * 5.  H*: smf_nsmf_handle_release_data_in_hsmf
     * 6.  H*: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 7.  H*: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 8.  H: ogs_sbi_send_http_status_no_content
     * 9.  H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V*: smf_nsmf_handle_release_data_in_hsmf
     * 13. V*: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 14. V*: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ogs_sbi_send_http_status_no_content
     * 16. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                    smf_nsmf_handle_release_data_in_hsmf(
                            sess, stream, sbi_message);
                    e->h.sbi.state =
                        OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT;
                    OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
    /*
     * Network-requested PDU Session Modification
     *
     * 1.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_MOD
     *        QOS_RULE_CODE_FROM_PFCP_FLAGS
     *        QOS_RULE_FLOW_DESCRIPTION_CODE_FROM_PFCP_FLAGS
     * 2.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 3.  V*: smf_nsmf_handle_update_data_in_vsmf
     * 4.  V: gsm_build_pdu_session_modification_command+
     *        ngap_build_pdu_session_resource_modify_request_transfer
     * 5.  V: OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP
     *        if (sess->up_cnx_state == OpenAPI_up_cnx_state_ACTIVATING)
     *            sess->up_cnx_state = OpenAPI_up_cnx_state_ACTIVATED;
     *            smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *                  OpenAPI_up_cnx_state_ACTIVATED)
     *        else
     *            ogs_sbi_send_http_status_no_content(stream)
     * 6.  V: ogs_sbi_send_http_status_no_content(stream)
     *        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
     *        ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_modify_stream));
     * 7.  V: case OGS_EVENT_SBI_CLIENT
     *        CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 8.  H: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
     *        OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE
     */
                    rc = smf_nsmf_handle_update_data_in_vsmf(
                            sess, stream, sbi_message);

                    if (rc == true) {
                        switch (sess->nsmf_param.request_indication) {
                        case OpenAPI_request_indication_UE_REQ_PDU_SES_MOD:
                        case OpenAPI_request_indication_NW_REQ_PDU_SES_MOD:
                            break;
                        case OpenAPI_request_indication_UE_REQ_PDU_SES_REL:
                        case OpenAPI_request_indication_NW_REQ_PDU_SES_REL:
    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V*: smf_nsmf_handle_update_data_in_vsmf
     * 12. V*: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V*: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V*: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ngap_build_pdu_session_resource_release_command_transfer+
     *        gsm_build_pdu_session_release_command
     * 16  V: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V: ogs_sbi_send_http_status_no_content(stream)
     * 18. V: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
              case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H: case OGS_EVENT_SBI_CLIENT:
     * 22. H: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V: case OGS_EVENT_SBI_SERVER:
     * 26. V: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V: ogs_sbi_send_http_status_no_content+
     *        smf_sbi_send_sm_context_status_notify
     * 28. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                            e->h.sbi.state =
                                OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED;
                            OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
                            break;
                        default:
                            ogs_error("Unknown request_indication:%d",
                                    sess->nsmf_param.request_indication);
                        }
                    } else {
                        ogs_error("smf_nsmf_handle_update_data_in_vsmf() "
                                "failed");
                    }
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                            sbi_message->h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                        "Invalid resource name [%s]",
                        sbi_message->h.resource.component[0], NULL));
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                    "Invalid API name", sbi_message->h.service.name,
                    NULL));
            OGS_FSM_TRAN(s, smf_gsm_state_exception);
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        if (stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID)
            stream = ogs_sbi_stream_find_by_id(stream_id);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                smf_namf_comm_handle_n1_n2_message_transfer(
                        sess, stream, e->h.sbi.state, sbi_message);
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            if (stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID)
                stream = ogs_sbi_stream_find_by_id(stream_id);

            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_PDU_SESSIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
/*
 * PFCP delete triggers are defined in lib/pfcp/xact.h (values 1–7).
 * To avoid overlap with OGS_PFCP_DELETE_TRIGGER_*, SMF states use:
 *   - UPDATE_STATE_BASE at 0x10–0x14
 *   - UECM_STATE_BASE   at 0x20–0x23
 * HR flag is bit 7 (0x80).
 */
                        switch (e->h.sbi.state) {
                        case OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED:
                            if (sess->amf_to_vsmf_release_stream_id >=
                                    OGS_MIN_POOL_ID &&
                                sess->amf_to_vsmf_release_stream_id <=
                                    OGS_MAX_POOL_ID)
                                ogs_error("UE requested release stream ID [%d]"
                                        "has not been used yet",
                                        sess->amf_to_vsmf_release_stream_id);
                            /* Store Stream ID */
                            sess->amf_to_vsmf_release_stream_id =
                                ogs_sbi_id_from_stream(stream);
                            break;
                        case OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT:
    /*
     * Network-requested PDU Session Release(DUPLICATED)
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT,
     * 3.  V: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V*: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 13. V*: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 14. V: ogs_sbi_send_http_status_no_content
     * 15. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                            OGS_FSM_TRAN(&sess->sm,
                                    &smf_gsm_state_wait_pfcp_deletion);
                            break;
                        case SMF_UPDATE_STATE_DEACTIVATED:
    /*
     * UE-requested PDU Session Modification(DEACTIVATED)
     *
     * For Home Routed Roaming, delegate PFCP deactivation to H-SMF by
     * sending UP_CNX_STATE=DEACTIVATED via HsmfUpdateData.
     *
     * 1.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 2.  V: smf_nsmf_pdusession_build_hsmf_update_data
     *        SMF_UPDATE_STATE_HR_DEACTIVATED
     * 3.  H: smf_nsmf_handle_update_data_in_hsmf
     * 4.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 5.  H: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_DL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 6.  H: ogs_sbi_send_http_status_no_content
     * 7.  V*: case SMF_UPDATE_STATE_HR_DEACTIVATED:
     * 8.  V*: smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *           OpenAPI_up_cnx_state_DEACTIVATED)
     */
                            smf_sbi_send_sm_context_updated_data_up_cnx_state(
                                    sess, stream,
                                    OpenAPI_up_cnx_state_DEACTIVATED);
                            break;
                        case SMF_UPDATE_STATE_ACTIVATING:
    /*
     * UE-requested PDU Session Modification(ACTIVATING)
     *
     * 1.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 2.  V: smf_nsmf_pdusession_build_hsmf_update_data
     *        SMF_UPDATE_STATE_HR_ACTIVATING
     * 3.  H: smf_nsmf_handle_update_data_in_hsmf
     * 4.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 5.  H: ogs_sbi_send_http_status_no_content
     * 6.  V*: ngap_build_pdu_session_resource_setup_request_transfer
     * 7.  V*: smf_sbi_send_sm_context_updated_data(
     *           OpenAPI_up_cnx_state_ACTIVATING,
     *           OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ, n2smbuf)
     */
                            n2smbuf =
                                ngap_build_pdu_session_resource_setup_request_transfer(sess);
                            ogs_assert(n2smbuf);

                            smf_sbi_send_sm_context_updated_data(
                                    sess, stream,
                                    OpenAPI_up_cnx_state_ACTIVATING, 0,
                                    NULL,
                                    OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ,
                                    n2smbuf);
                            break;
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_ACTIVATING:
                            sess->up_cnx_state =
                                OpenAPI_up_cnx_state_ACTIVATED;
                            smf_sbi_send_sm_context_updated_data_up_cnx_state(
                                    sess, stream,
                                    OpenAPI_up_cnx_state_ACTIVATED);
                            break;
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_NON_ACTIVATING:
                            ogs_assert(true ==
                                    ogs_sbi_send_http_status_no_content(
                                        stream));
                            break;
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_XN_HANDOVER:
                            n2smbuf =
                                ngap_build_path_switch_request_ack_transfer(
                                        sess);
                            ogs_assert(n2smbuf);

                            smf_sbi_send_sm_context_updated_data_n2smbuf(
                                    sess, stream,
                                    OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ_ACK,
                                    n2smbuf);
                            break;
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_N2_HANDOVER:
                            if (smf_sess_have_indirect_data_forwarding(sess) ==
                                    true) {
                                ogs_assert(OGS_OK ==
                                    smf_5gc_pfcp_send_all_pdr_modification_request(
                                        sess, stream,
                                        OGS_PFCP_MODIFY_INDIRECT|
                                        OGS_PFCP_MODIFY_REMOVE,
                                        0,
                                        ogs_local_conf()->
                                            time.handover.duration));
                            }

                            smf_sbi_send_sm_context_updated_data_ho_state(
                                    sess, stream, OpenAPI_ho_state_COMPLETED);
                            break;
                        case SMF_UPDATE_STATE_UE_REQ_MOD:
                            if (sess->amf_to_vsmf_modify_stream_id >=
                                    OGS_MIN_POOL_ID &&
                                sess->amf_to_vsmf_modify_stream_id <=
                                    OGS_MAX_POOL_ID)
                                ogs_error("UE requested modification stream ID "
                                        "[%d] has not been used yet",
                                        sess->amf_to_vsmf_modify_stream_id);
                            sess->amf_to_vsmf_modify_stream_id =
                                ogs_sbi_id_from_stream(stream);
                            break;
                        default:
                            ogs_fatal("Unknown state [0x%x]", e->h.sbi.state);
                            ogs_assert_if_reached();
                        }
                        break;
                    CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                        if (e->h.sbi.state ==
                                OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT)
                            OGS_FSM_TRAN(&sess->sm,
                                    &smf_gsm_state_wait_pfcp_deletion);
                        break;
                    DEFAULT
                        if (smf_nsmf_handle_created_data_in_vsmf(
                                    sess, sbi_message) == false) {
                            ogs_error("[%s:%d] create_pdu_session "
                                    "failed() [%d]",
                                    smf_ue->supi, sess->psi,
                                    sbi_message->res_status);
                            OGS_FSM_TRAN(s, smf_gsm_state_exception);
                        }
                    END
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]",
                            sbi_message->h.method);
                    ogs_assert_if_reached();
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
    /*
     * Network-requested PDU Session Modification
     *
     * 1.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_MOD
     *        QOS_RULE_CODE_FROM_PFCP_FLAGS
     *        QOS_RULE_FLOW_DESCRIPTION_CODE_FROM_PFCP_FLAGS
     * 2.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 3.  V: smf_nsmf_handle_update_data_in_vsmf
     * 4.  V: gsm_build_pdu_session_modification_command+
     *        ngap_build_pdu_session_resource_modify_request_transfer
     * 5.  V: OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP
     *        if (sess->up_cnx_state == OpenAPI_up_cnx_state_ACTIVATING)
     *            sess->up_cnx_state = OpenAPI_up_cnx_state_ACTIVATED;
     *            smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *                  OpenAPI_up_cnx_state_ACTIVATED)
     *        else
     *            ogs_sbi_send_http_status_no_content(stream)
     * 6.  V: ogs_sbi_send_http_status_no_content(stream)
     *         OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
     *         ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_modify_stream));
     * 7.  V*: case OGS_EVENT_SBI_CLIENT
     *         CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 8.  H*: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
     *         OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE
     */
                    switch (e->h.sbi.state) {
                    case SMF_CREATE_STATE_NONE:
                        ogs_list_for_each_entry(
                                &sess->qos_flow_to_modify_list,
                                qos_flow, to_modify_node) {
                            ogs_pfcp_far_t *dl_far = qos_flow->dl_far;
                            ogs_assert(dl_far);

                            dl_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
                            ogs_assert(OGS_OK ==
                                ogs_pfcp_ip_to_outer_header_creation(
                                    &sess->remote_dl_ip,
                                    &dl_far->outer_header_creation,
                                    &dl_far->outer_header_creation_len));
                            dl_far->outer_header_creation.teid =
                                sess->remote_dl_teid;
                        }

                        ogs_assert(OGS_OK ==
                                smf_5gc_pfcp_send_qos_flow_list_modification_request(
                                    sess, NULL,
                                    OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                                    OGS_PFCP_MODIFY_DL_ONLY|
                                    OGS_PFCP_MODIFY_ACTIVATE, 0));
                        break;
                    case SMF_UPDATE_STATE_NONE:
                        /* Nothing to do */
                        break;
                    case SMF_REMOVE_STATE_NONE:
                        /* Nothing to do */
                        break;
                    default:
                        ogs_fatal("Unknown state [%d]", e->h.sbi.state);
                        ogs_assert_if_reached();
                    }
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                            sbi_message->h.resource.component[0]);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                    smf_ue->supi, sess->psi, sbi_message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    case SMF_EVT_5GSM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);
        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        switch (nas_message->gsm.h.message_type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
            if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                sess->nsmf_param.request_indication =
                    OpenAPI_request_indication_UE_REQ_PDU_SES_MOD;

                r = smf_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                        smf_nsmf_pdusession_build_hsmf_update_data,
                        sess, stream, SMF_UPDATE_STATE_UE_REQ_MOD, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                rv = gsm_handle_pdu_session_modification_request(sess, stream,
                        &nas_message->gsm.pdu_session_modification_request);
                if (rv != OGS_OK) {
                    ogs_error("[%s:%d] Cannot handle NAS message",
                            smf_ue->supi, sess->psi);
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                }
            }
            break;

        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
            ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
            if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
    /*
     * Network-requested PDU Session Modification
     *
     * 1.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_MOD
     *        QOS_RULE_CODE_FROM_PFCP_FLAGS
     *        QOS_RULE_FLOW_DESCRIPTION_CODE_FROM_PFCP_FLAGS
     * 2.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 3.  V: smf_nsmf_handle_update_data_in_vsmf
     * 4.  V: gsm_build_pdu_session_modification_command+
     *        ngap_build_pdu_session_resource_modify_request_transfer
     * 5.  V: OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP
     *        if (sess->up_cnx_state == OpenAPI_up_cnx_state_ACTIVATING)
     *            sess->up_cnx_state = OpenAPI_up_cnx_state_ACTIVATED;
     *            smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *                  OpenAPI_up_cnx_state_ACTIVATED)
     *        else
     *            ogs_sbi_send_http_status_no_content(stream)
     * 6.  V*: ogs_sbi_send_http_status_no_content(stream)
     *         OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
     *         ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_modify_stream));
     * 7.  V: case OGS_EVENT_SBI_CLIENT
     *        CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 8.  H: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
     *        OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE
     */
                ogs_sbi_stream_t *vsmf_to_hsmf_modify_stream = NULL;
                if (sess->vsmf_to_hsmf_modify_stream_id >= OGS_MIN_POOL_ID &&
                    sess->vsmf_to_hsmf_modify_stream_id <= OGS_MAX_POOL_ID)
                    vsmf_to_hsmf_modify_stream =
                        ogs_sbi_stream_find_by_id(
                                sess->vsmf_to_hsmf_modify_stream_id);

                if (vsmf_to_hsmf_modify_stream) {
                    ogs_assert(true ==
                            ogs_sbi_send_http_status_no_content(
                                vsmf_to_hsmf_modify_stream));
                    sess->vsmf_to_hsmf_modify_stream_id = OGS_INVALID_POOL_ID;
                }
            }
            break;

        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
            memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

            sess->nsmf_param.gsm_cause =
                OGS_5GSM_CAUSE_REGULAR_DEACTIVATION;
            sess->nsmf_param.ngap_cause.group = NGAP_Cause_PR_nas;
            sess->nsmf_param.ngap_cause.value =
                NGAP_CauseNas_normal_release;

            e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED;

            if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                ogs_assert(OGS_OK ==
                    smf_5gc_pfcp_send_all_pdr_modification_request(
                        sess, stream,
                        OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                        OGS_PFCP_MODIFY_UL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE,
                        e->h.sbi.state, 0));
            } else {
                OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);
            }
            break;

        default:
            strerror = ogs_msprintf("Unknown message [%d]",
                    nas_message->gsm.h.message_type);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror,
                    NULL, NULL));
            ogs_free(strerror);
        }
        break;

    case SMF_EVT_NGAP_MESSAGE:
        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        ogs_assert(e->ngap.type);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        switch (e->ngap.type) {
        case OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP:
            rv = ngap_handle_pdu_session_resource_setup_response_transfer(
                    sess, stream, pkbuf);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NGAP message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OpenAPI_n2_sm_info_type_PDU_RES_SETUP_FAIL:
            rv = ngap_handle_pdu_session_resource_setup_unsuccessful_transfer(
                    sess, stream, pkbuf);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NGAP message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP:
    /*
     * Network-requested PDU Session Modification
     *
     * 1.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_MOD
     *        QOS_RULE_CODE_FROM_PFCP_FLAGS
     *        QOS_RULE_FLOW_DESCRIPTION_CODE_FROM_PFCP_FLAGS
     * 2.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 3.  V: smf_nsmf_handle_update_data_in_vsmf
     * 4.  V: gsm_build_pdu_session_modification_command+
     *        ngap_build_pdu_session_resource_modify_request_transfer
     * 5.  V*: OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP
     *        if (sess->up_cnx_state == OpenAPI_up_cnx_state_ACTIVATING)
     *            sess->up_cnx_state = OpenAPI_up_cnx_state_ACTIVATED;
     *            smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *                  OpenAPI_up_cnx_state_ACTIVATED)
     *        else
     *            ogs_sbi_send_http_status_no_content(stream)
     * 6.  V: ogs_sbi_send_http_status_no_content(stream)
     *        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
     *        ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_modify_stream));
     * 7.  V: case OGS_EVENT_SBI_CLIENT
     *        CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 8.  H: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
     *        OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE
     */
            rv = ngap_handle_pdu_session_resource_modify_response_transfer(
                    sess, stream, pkbuf);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NGAP message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
            ngap_state = sess->ngap_state.pdu_session_resource_release;

            if (ngap_state == SMF_NGAP_STATE_NONE) {
                strerror = ogs_msprintf(
                        "[%s:%d] No PDUSessionResourceReleaseRequest",
                        smf_ue->supi, sess->psi);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror,
                        NULL, NULL));
                ogs_free(strerror);

                OGS_FSM_TRAN(s, smf_gsm_state_exception);

            } else if (ngap_state ==
                    SMF_NGAP_STATE_ERROR_INDICATION_RECEIVED_FROM_5G_AN) {
                smf_n1_n2_message_transfer_param_t param;

                ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));

                memset(&param, 0, sizeof(param));
                param.state = SMF_NETWORK_TRIGGERED_SERVICE_REQUEST;
                param.n2smbuf =
                    ngap_build_pdu_session_resource_setup_request_transfer(
                            sess);
                ogs_assert(param.n2smbuf);

                param.n1n2_failure_txf_notif_uri = true;

                smf_namf_comm_send_n1_n2_message_transfer(sess, NULL, &param);
            } else {
                ogs_fatal("Invalid state [%d]", ngap_state);
                ogs_assert_if_reached();
            }
            break;

        case OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ:
            rv = ngap_handle_path_switch_request_transfer(sess, stream, pkbuf);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NGAP message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OpenAPI_n2_sm_info_type_HANDOVER_REQUIRED:
            rv = ngap_handle_handover_required_transfer(sess, stream, pkbuf);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NGAP message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OpenAPI_n2_sm_info_type_HANDOVER_REQ_ACK:
            rv = ngap_handle_handover_request_ack(sess, stream, pkbuf);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NGAP message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        default:
            ogs_error("Unknown message[%d]", e->ngap.type);
        }
        break;

    case SMF_EVT_SESSION_RELEASE:
        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        ogs_info("[%s:%d] Session Release [PFCP-Delete-Trigger:%d]",
            smf_ue->supi, sess->psi, e->h.sbi.state);

        if (e->h.sbi.state == OGS_PFCP_DELETE_TRIGGER_SMF_INITIATED) {
            if (HOME_ROUTED_ROAMING_IN_HSMF(sess)) {
                memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

                /* Network Requested PDU Session Release */
                sess->nsmf_param.request_indication =
                    OpenAPI_request_indication_NW_REQ_PDU_SES_REL;

                r = smf_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                        smf_nsmf_pdusession_build_vsmf_update_data,
                        sess, NULL, e->h.sbi.state, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

            } else {
                smf_n1_n2_message_transfer_param_t param;

                memset(&param, 0, sizeof(param));
                param.state = SMF_UE_OR_NETWORK_REQUESTED_PDU_SESSION_RELEASE;
                sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
                param.n1smbuf = gsm_build_pdu_session_release_command(
                    sess, OGS_5GSM_CAUSE_REACTIVATION_REQUESTED);
                ogs_assert(param.n1smbuf);

                param.n2smbuf =
                    ngap_build_pdu_session_resource_release_command_transfer(
                        sess, SMF_NGAP_STATE_DELETE_TRIGGER_SMF_INITIATED,
                        NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release);
                ogs_assert(param.n2smbuf);

    /*
     * Skip_ind is not used when changing the PDU Session Anchor.
     * param.skip_ind = false;
     *
     * TS23.502
     * 4.3.4 PDU Session Release
     * 4.3.4.2 UE or network requested PDU Session Release for Non-Roaming
     * and Roaming with Local Breakout
     *
     * 3b. ...
     *
     * The "skip indicator" tells the AMF whether it may skip sending
     * the N1 SM container to the UE (e.g. when the UE is in CM-IDLE state).
     * SMF includes the "skip indicator"
     * in the Namf_Communication_N1N2MessageTransfer
     * except when the procedure is triggered to change PDU Session Anchor
     * of a PDU Session with SSC mode 2.
     *
     * Related Issue #2396
     */

                smf_namf_comm_send_n1_n2_message_transfer(sess, NULL, &param);
            }

            OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);

        } else {

            OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion);

        }
        break;

    default:
        ogs_error("Unknown event [%s]", smf_event_get_name(e));
    }
}

void smf_gsm_state_wait_pfcp_deletion(ogs_fsm_t *s, smf_event_t *e)
{
    int status;

    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_pfcp_xact_t *pfcp_xact = NULL;
    ogs_pfcp_message_t *pfcp_message = NULL;

    uint8_t pfcp_cause, gtp_cause;
    ogs_gtp_xact_t *gtp_xact = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        /* Since `pfcp_xact->epc` is not available,
         * we'll use `sess->epc` */
        if (sess->epc) {
            /* EPC */
            gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);
            ogs_assert(OGS_OK ==
                smf_epc_pfcp_send_session_deletion_request(
                    sess, gtp_xact ? gtp_xact->id : OGS_INVALID_POOL_ID));
        } else {
            /* 5GC */
            stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            if (stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID)
                stream = ogs_sbi_stream_find_by_id(stream_id);

            ogs_assert(OGS_OK ==
                smf_5gc_pfcp_send_session_deletion_request(
                    sess, stream, e->h.sbi.state));
        }
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case SMF_EVT_GN_MESSAGE:
    case SMF_EVT_S5C_MESSAGE:
        break; /* ignore */

    case SMF_EVT_N4_MESSAGE:
        pfcp_xact = ogs_pfcp_xact_find_by_id(e->pfcp_xact_id);
        ogs_assert(pfcp_xact);
        pfcp_message = e->pfcp_message;
        ogs_assert(pfcp_message);

        switch (pfcp_message->h.type) {
        case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
            if (pfcp_xact->epc) {
                gtp_xact = ogs_gtp_xact_find_by_id(pfcp_xact->assoc_xact_id);

                pfcp_cause = smf_epc_n4_handle_session_deletion_response(
                            sess, pfcp_xact,
                            &pfcp_message->pfcp_session_deletion_response);
                if (pfcp_cause != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
                    /* FIXME: tear down Gy and Gx */
                    ogs_assert(gtp_xact);
                    gtp_cause = gtp_cause_from_pfcp(
                            pfcp_cause, gtp_xact->gtp_version);
                    send_gtp_delete_err_msg(sess, gtp_xact, gtp_cause);
                    break;
                }
                if (send_ccr_termination_req_gx_gy_s6b(
                            sess, gtp_xact) == true)
                    OGS_FSM_TRAN(s, smf_gsm_state_wait_epc_auth_release);
                /* else: free session? */
            } else {
                int r, trigger;

                if (pfcp_xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                        pfcp_xact->assoc_stream_id <= OGS_MAX_POOL_ID)
                    stream = ogs_sbi_stream_find_by_id(
                            pfcp_xact->assoc_stream_id);

                trigger = pfcp_xact->delete_trigger;
                ogs_assert(trigger);

                ogs_pfcp_xact_commit(pfcp_xact);

                status = smf_5gc_n4_handle_session_deletion_response(
                            sess, stream, trigger,
                            &pfcp_message->pfcp_session_deletion_response);
                if (status != OGS_SBI_HTTP_STATUS_OK) {
                    ogs_error(
                        "[%d] smf_5gc_n4_handle_session_deletion_response() "
                        "failed", trigger);

                    OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
                    break;
                }

                if (trigger == OGS_PFCP_DELETE_TRIGGER_LOCAL_INITIATED) {

                    ogs_error("OLD Session Released");
                    OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);

                } else if (trigger == OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED) {
                    ogs_pkbuf_t *n1smbuf = NULL, *n2smbuf = NULL;

                    if (HOME_ROUTED_ROAMING_IN_HSMF(sess)) {
    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H*: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H*: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V: smf_nsmf_handle_update_data_in_vsmf
     * 12. V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ngap_build_pdu_session_resource_release_command_transfer+
     *        gsm_build_pdu_session_release_command
     * 16  V: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V: ogs_sbi_send_http_status_no_content(stream)
     * 18. V: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
              case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H: case OGS_EVENT_SBI_CLIENT:
     * 22. H: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V: case OGS_EVENT_SBI_SERVER:
     * 26. V: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V: ogs_sbi_send_http_status_no_content+
     *        smf_sbi_send_sm_context_status_notify
     * 28. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */

                        if (sess->nsmf_param.request_indication !=
                                OpenAPI_request_indication_UE_REQ_PDU_SES_REL) {
                            ogs_fatal("Invalid request_indication = %d",
                                    sess->nsmf_param.request_indication);
                            ogs_assert_if_reached();
                        }

                        r = smf_sbi_discover_and_send(
                                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                                smf_nsmf_pdusession_build_vsmf_update_data,
                                sess, NULL, trigger, NULL);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                    } else if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V: smf_nsmf_handle_update_data_in_vsmf
     * 12. V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V*: ngap_build_pdu_session_resource_release_command_transfer+
     *         gsm_build_pdu_session_release_command
     * 16  V*: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V: ogs_sbi_send_http_status_no_content(stream)
     * 18. V: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
              case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H: case OGS_EVENT_SBI_CLIENT:
     * 22. H: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V: case OGS_EVENT_SBI_SERVER:
     * 26. V: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V: ogs_sbi_send_http_status_no_content+
     *        smf_sbi_send_sm_context_status_notify
     * 28. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */

                        smf_n1_n2_message_transfer_param_t param;
                        ogs_sbi_stream_t *amf_to_vsmf_release_stream = NULL;

                        if (sess->amf_to_vsmf_release_stream_id >=
                                OGS_MIN_POOL_ID &&
                            sess->amf_to_vsmf_release_stream_id <=
                                OGS_MAX_POOL_ID)
                            amf_to_vsmf_release_stream =
                                ogs_sbi_stream_find_by_id(
                                        sess->amf_to_vsmf_release_stream_id);

                        if (amf_to_vsmf_release_stream) {
                            ogs_assert(true ==
                                    ogs_sbi_send_http_status_no_content(
                                        amf_to_vsmf_release_stream));
                            sess->amf_to_vsmf_release_stream_id =
                                OGS_INVALID_POOL_ID;
                        }

                        memset(&param, 0, sizeof(param));
                        param.state =
                            SMF_UE_OR_NETWORK_REQUESTED_PDU_SESSION_RELEASE;
                        param.n1smbuf = gsm_build_pdu_session_release_command(
                            sess, OGS_5GSM_CAUSE_REACTIVATION_REQUESTED);
                        ogs_assert(param.n1smbuf);

                        param.n2smbuf =
                            ngap_build_pdu_session_resource_release_command_transfer(
                                sess,
                                SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED,
                                NGAP_Cause_PR_nas,
                                NGAP_CauseNas_normal_release);
                        ogs_assert(param.n2smbuf);

        /*
         * Skip_ind is not used when changing the PDU Session Anchor.
         * param.skip_ind = false;
         *
         * TS23.502
         * 4.3.4 PDU Session Release
         * 4.3.4.2 UE or network requested PDU Session Release for Non-Roaming
         * and Roaming with Local Breakout
         *
         * 3b. ...
         *
         * The "skip indicator" tells the AMF whether it may skip sending
         * the N1 SM container to the UE (e.g. when the UE is in CM-IDLE state).
         * SMF includes the "skip indicator"
         * in the Namf_Communication_N1N2MessageTransfer
         * except when the procedure is triggered to change PDU Session Anchor
         * of a PDU Session with SSC mode 2.
         *
         * Related Issue #2396
         */

                        smf_namf_comm_send_n1_n2_message_transfer(
                                sess, stream, &param);

                        OGS_FSM_TRAN(&sess->sm,
                                smf_gsm_state_wait_5gc_n1_n2_release);

                    } else {

                        n1smbuf = gsm_build_pdu_session_release_command(
                                sess, OGS_5GSM_CAUSE_REGULAR_DEACTIVATION);
                        ogs_assert(n1smbuf);

                        n2smbuf =
                            ngap_build_pdu_session_resource_release_command_transfer(
                                    sess,
                                    SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED,
                                    NGAP_Cause_PR_nas,
                                    NGAP_CauseNas_normal_release);
                        ogs_assert(n2smbuf);

                        ogs_assert(stream);
                        smf_sbi_send_sm_context_updated_data_n1_n2_message(
                                sess, stream, n1smbuf,
                                OpenAPI_n2_sm_info_type_PDU_RES_REL_CMD,
                                n2smbuf);
                    }

                    OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);

                } else if (trigger ==
                            OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT) {

                    if (HOME_ROUTED_ROAMING_IN_HSMF(sess)) {
    /*
     * Network-requested PDU Session Release(DUPLICATED)
     *
     * TS23.502 clause 4.3.4.3 UE or network requested PDU Session Release
     * for Home-routed Roaming.
     *
     * In step 1f, upon receiving Nsmf_PDUSession_Update from V-SMF, H-SMF
     * SHALL immediately send the Update Response, then issue PFCP Session
     * Deletion. This ordering is per the standard, even for duplicate
     * sessions, and may overlap with AMF’s concurrent Create Session.
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT,
     * 3.  V: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H*: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                 SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H*: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 13. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 14. V: ogs_sbi_send_http_status_no_content
     * 15. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                        r = smf_sbi_cleanup_session(
                                sess, NULL,
                                SMF_UECM_STATE_DEREG_BY_AMF_HR,
                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        OGS_FSM_TRAN(s,
                                smf_gsm_state_5gc_session_will_deregister);

                    } else if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {

    /*
     * Network-requested PDU Session Release(DUPLICATED)
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT,
     * 3.  V: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 13. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 14. V*: ogs_sbi_send_http_status_no_content
     * 15. V*: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                        ogs_assert(true ==
                                ogs_sbi_send_http_status_no_content(stream));
                        OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);

                    } else {

                        r = smf_sbi_cleanup_session(
                                sess, stream,
                                SMF_UECM_STATE_DEREG_BY_AMF,
                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        OGS_FSM_TRAN(s,
                                smf_gsm_state_5gc_session_will_deregister);
                    }

                } else if (trigger ==
                            OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT) {

                    if (HOME_ROUTED_ROAMING_IN_HSMF(sess)) {
    /*
     * Network-requested PDU Session Release
     *
     * TS23.502
     * 4.3.4.3 UE or network requested PDU Session Release
     *         for Home-routed Roaming
     *
     * 1b. (Serving network initiated release)
     *
     * 1.  V*: smf_nsmf_handle_release_sm_context
     * 2.  V*: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *         OGS_PFCP_MODIFY_DEACTIVATE
     * 3.  V*: OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 4.  V: smf_nsmf_pdusession_build_release_data
     * 5.  H: smf_nsmf_handle_release_data_in_hsmf
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 7.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 8.  H*: ogs_sbi_send_http_status_no_content
     * 9.  H*: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                 SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H*: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H*: SMF_SESS_CLEAR(sess)
     * 12. V: smf_nsmf_handle_release_data_in_hsmf
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V*: ogs_sbi_send_http_status_no_content
     * 16. V*: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                        ogs_assert(true ==
                                ogs_sbi_send_http_status_no_content(stream));

                        r = smf_sbi_cleanup_session(
                                sess, NULL,
                                SMF_UECM_STATE_DEREG_BY_AMF_HR,
                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        OGS_FSM_TRAN(s,
                                smf_gsm_state_5gc_session_will_deregister);

                    } else if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {

    /*
     * Network-requested PDU Session Release
     *
     * 1.  V: smf_nsmf_handle_release_sm_context
     * 2.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 3.  V: OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 4.  V: smf_nsmf_pdusession_build_release_data
     * 5.  H: smf_nsmf_handle_release_data_in_hsmf
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 7.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 8.  H: ogs_sbi_send_http_status_no_content
     * 9.  H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V: smf_nsmf_handle_release_data_in_hsmf
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V*: ogs_sbi_send_http_status_no_content
     * 16. V*: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                        ogs_assert(true ==
                                ogs_sbi_send_http_status_no_content(stream));
                        OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);

                    } else {

                        r = smf_sbi_cleanup_session(
                                sess, stream,
                                SMF_UECM_STATE_DEREG_BY_AMF,
                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        OGS_FSM_TRAN(s,
                                smf_gsm_state_5gc_session_will_deregister);
                    }

                } else if (trigger == OGS_PFCP_DELETE_TRIGGER_PCF_INITIATED) {

                    if (HOME_ROUTED_ROAMING_IN_HSMF(sess)) {

                        ogs_assert(true ==
                                ogs_sbi_send_http_status_no_content(stream));

                    } else {

                        smf_n1_n2_message_transfer_param_t param;

                        memset(&param, 0, sizeof(param));
                        param.state = SMF_UE_OR_NETWORK_REQUESTED_PDU_SESSION_RELEASE;
                        param.n2smbuf = ngap_build_pdu_session_resource_release_command_transfer(
                                sess,
                                SMF_NGAP_STATE_DELETE_TRIGGER_PCF_INITIATED,
                                NGAP_Cause_PR_nas,
                                NGAP_CauseNas_normal_release);
                        ogs_assert(param.n2smbuf);

                        param.skip_ind = true;

                        smf_namf_comm_send_n1_n2_message_transfer(
                                sess, NULL, &param);
                    }

                    OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
                } else {
                    ogs_fatal("Unknown trigger [%d]", trigger);
                    ogs_assert_if_reached();
                }
            }
            break;

        default:
            ogs_error("cannot handle PFCP message type[%d]",
                    pfcp_message->h.type);
        }
        break;
    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            ogs_pkbuf_t *n1smbuf = NULL;

            stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                    stream_id <= OGS_MAX_POOL_ID);

            stream = ogs_sbi_stream_find_by_id(stream_id);
            if (!stream) {
                ogs_error("STREAM has already been removed [%d]", stream_id);
                break;
            }

            ogs_error("[%s:%d] state [%d] res_status [%d]",
                smf_ue->supi, sess->psi,
                e->h.sbi.state, sbi_message->res_status);

            n1smbuf = gsm_build_pdu_session_release_reject(sess,
                OGS_5GSM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE);
            ogs_assert(n1smbuf);

            smf_sbi_send_sm_context_update_error_n1_n2_message(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    n1smbuf, OpenAPI_n2_sm_info_type_NULL, NULL);
            break;
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            if (stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID)
                stream = ogs_sbi_stream_find_by_id(stream_id);

            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_PDU_SESSIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                        switch (e->h.sbi.state) {
                        case OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED:
/*
 * Handle unexpected arrival of Nsmf_PDUSession_UpdateSMContext Response
 * (step 1a) after PFCP deletion triggered by UE (step 3a).
 * In Home-routed Roaming (TS 23.502 Section 4.3.4.3) SMContext Response is
 * expected before SMContext Request (step 3a), but races can invert order,
 * causing smf_gsm_state_wait_pfcp_deletion on PFCP release (steps 4a/4b).
 * Without handling the delayed Response here, no handler exists and a crash
 * results. Storing the stream ID lets the Response be processed safely.
 */
                            if (sess->amf_to_vsmf_release_stream_id >=
                                    OGS_MIN_POOL_ID &&
                                sess->amf_to_vsmf_release_stream_id <=
                                    OGS_MAX_POOL_ID)
                                ogs_error("UE requested release stream ID [%d]"
                                        "has not been used yet",
                                        sess->amf_to_vsmf_release_stream_id);
                            /* Store Stream ID */
                            sess->amf_to_vsmf_release_stream_id =
                                ogs_sbi_id_from_stream(stream);
                            break;
/*
 * Assume PDU session establishment and deregistration occur simultaneously.
 * In Non-Roaming or LBO scenarios, no issues arise. However, in Home Routed
 * Roaming, the following problem may occur:
 *
 * The final step of PDU session establishment is activation via the
 * PDUSessionResourceSetupResponse. At this point, the V-SMF sends a
 * /pdu-sessions/{xx}/modify request to the H-SMF. Meanwhile, the deregistration
 * process advances the state to smf_gsm_state_wait_pfcp_deletion as part
 * of the PFCP deletion procedure.
 *
 * When the H-SMF then sends the OGS_EVENT_SBI_CLIENT event, a corresponding
 * handler is required to process this state. The code below implements this
 * handler to address the race condition.
 */
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_ACTIVATING:
                            sess->up_cnx_state =
                                OpenAPI_up_cnx_state_ACTIVATED;
                            smf_sbi_send_sm_context_updated_data_up_cnx_state(
                                    sess, stream,
                                    OpenAPI_up_cnx_state_ACTIVATED);
                            break;
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_NON_ACTIVATING:
                            ogs_assert(true ==
                                    ogs_sbi_send_http_status_no_content(
                                        stream));
                            break;
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_XN_HANDOVER:
                            n2smbuf =
                                ngap_build_path_switch_request_ack_transfer(
                                        sess);
                            ogs_assert(n2smbuf);

                            smf_sbi_send_sm_context_updated_data_n2smbuf(
                                    sess, stream,
                                    OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ_ACK,
                                    n2smbuf);
                            break;
                        case SMF_UPDATE_STATE_ACTIVATED_FROM_N2_HANDOVER:
                            if (smf_sess_have_indirect_data_forwarding(sess) ==
                                    true) {
                                ogs_assert(OGS_OK ==
                                    smf_5gc_pfcp_send_all_pdr_modification_request(
                                        sess, stream,
                                        OGS_PFCP_MODIFY_INDIRECT|
                                        OGS_PFCP_MODIFY_REMOVE,
                                        0,
                                        ogs_local_conf()->
                                            time.handover.duration));
                            }

                            smf_sbi_send_sm_context_updated_data_ho_state(
                                    sess, stream, OpenAPI_ho_state_COMPLETED);
                            break;
                        default:
                            ogs_fatal("Unknown state [0x%x]", e->h.sbi.state);
                            ogs_assert_if_reached();
                        }
                        break;
                    DEFAULT
                        ogs_error("Invalid resource name [%s]",
                                    sbi_message->h.resource.component[2]);
                        ogs_assert_if_reached();
                    END
                    break;
                DEFAULT
                    ogs_error("Invalid HTTP method [%s]",
                            sbi_message->h.method);
                    ogs_assert_if_reached();
                END
                break;
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                            sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                ogs_error("[%s:%d] Ignore SBI message "
                        "state [%d] res_status [%d]",
                    smf_ue->supi, sess->psi,
                    e->h.sbi.state, sbi_message->res_status);
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                    smf_ue->supi, sess->psi, sbi_message->h.service.name);
            ogs_assert_if_reached();
        END
        break;
    }
}

void smf_gsm_state_wait_epc_auth_release(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sess_t *sess = NULL;

    ogs_diam_gx_message_t *gx_message = NULL;
    ogs_diam_gy_message_t *gy_message = NULL;
    ogs_diam_s6b_message_t *s6b_message = NULL;
    uint32_t diam_err;

    ogs_gtp_xact_t *gtp_xact = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        /* reset state: */
        sess->sm_data.gx_cca_term_err = ER_DIAMETER_SUCCESS;
        sess->sm_data.gy_cca_term_err = ER_DIAMETER_SUCCESS;
        sess->sm_data.s6b_sta_err = ER_DIAMETER_SUCCESS;
        break;

    case SMF_EVT_GN_MESSAGE:
    case SMF_EVT_S5C_MESSAGE:
    case SMF_EVT_N4_MESSAGE:
        break; /* ignore */

    case SMF_EVT_GX_MESSAGE:
        gx_message = e->gx_message;
        ogs_assert(gx_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gx_message->cmd_code) {
        case OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL:
            switch(gx_message->cc_request_type) {
            case OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST:
                diam_err = smf_gx_handle_cca_termination_request(sess,
                                gx_message, gtp_xact);
                sess->sm_data.gx_ccr_term_in_flight = false;
                sess->sm_data.gx_cca_term_err = diam_err;
                goto test_can_proceed;
            }
            break;
        }
        break;

    case SMF_EVT_GY_MESSAGE:
        gy_message = e->gy_message;
        ogs_assert(gy_message);
        gtp_xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);

        switch(gy_message->cmd_code) {
        case OGS_DIAM_GY_CMD_CODE_CREDIT_CONTROL:
            switch(gy_message->cc_request_type) {
            case OGS_DIAM_GY_CC_REQUEST_TYPE_TERMINATION_REQUEST:
                diam_err = smf_gy_handle_cca_termination_request(sess,
                                gy_message, gtp_xact);
                sess->sm_data.gy_ccr_term_in_flight = false;
                sess->sm_data.gy_cca_term_err = diam_err;
                goto test_can_proceed;
            }
            break;
        }
        break;

    case SMF_EVT_S6B_MESSAGE:
        s6b_message = e->s6b_message;
        ogs_assert(s6b_message);

        switch(s6b_message->cmd_code) {
        case OGS_DIAM_S6B_CMD_SESSION_TERMINATION:
            sess->sm_data.s6b_str_in_flight = false;
            /* TODO: validate error code from message below: */
            sess->sm_data.s6b_sta_err = ER_DIAMETER_SUCCESS;
            goto test_can_proceed;
        }
        break;
    }
    return;

test_can_proceed:
    /* First wait for both Gx and Gy requests to be done: */
    if (!sess->sm_data.gx_ccr_term_in_flight &&
        !sess->sm_data.gy_ccr_term_in_flight &&
        !sess->sm_data.s6b_str_in_flight) {
        diam_err = ER_DIAMETER_SUCCESS;
        if (sess->sm_data.gx_cca_term_err != ER_DIAMETER_SUCCESS)
            diam_err = sess->sm_data.gx_cca_term_err;
        if (sess->sm_data.gy_cca_term_err != ER_DIAMETER_SUCCESS)
            diam_err = sess->sm_data.gy_cca_term_err;
        if (sess->sm_data.s6b_sta_err != ER_DIAMETER_SUCCESS)
            diam_err = sess->sm_data.s6b_sta_err;

        /* Initiated by peer request, let's answer: */
        if (gtp_xact) {
            if (diam_err == ER_DIAMETER_SUCCESS) {
                /*
                 * 1. MME sends Delete Session Request to SGW/SMF.
                 * 2. SMF sends Delete Session Response to SGW/MME.
                 */
                switch (gtp_xact->gtp_version) {
                case 1:
                    smf_gtp1_send_delete_pdp_context_response(
                                sess, gtp_xact);
                    break;
                case 2:
                    smf_gtp2_send_delete_session_response(
                                sess, gtp_xact);
                    break;
                }
            } else {
                uint8_t gtp_cause = gtp_cause_from_diameter(
                                    gtp_xact->gtp_version, diam_err, NULL);
                send_gtp_delete_err_msg(sess, gtp_xact, gtp_cause);
            }
        }
        OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
    }
}

void smf_gsm_state_wait_5gc_n1_n2_release(ogs_fsm_t *s, smf_event_t *e)
{
    int ngap_state;
    char *strerror = NULL;

    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *sbi_message = NULL;

    int r;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_SERVER:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXTS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                    smf_nsmf_handle_update_sm_context(
                            sess, stream, sbi_message);
                    break;
    /*
     * Network-requested PDU Session Release
     *
     * 1.  V*: smf_nsmf_handle_release_sm_context
     * 2.  V*: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *         OGS_PFCP_MODIFY_DEACTIVATE
     * 3.  V*: OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 4.  V: smf_nsmf_pdusession_build_release_data
     * 5.  H: smf_nsmf_handle_release_data_in_hsmf
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 7.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 8.  H*: ogs_sbi_send_http_status_no_content
     * 9.  H*: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                 SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H*: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H*: SMF_SESS_CLEAR(sess)
     * 12. V: smf_nsmf_handle_release_data_in_hsmf
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V*: ogs_sbi_send_http_status_no_content
     * 16. V*: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                    smf_nsmf_handle_release_sm_context(
                            sess, stream, sbi_message);

                    if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                        ogs_assert(OGS_OK ==
                            smf_5gc_pfcp_send_all_pdr_modification_request(
                                sess, stream,
                                OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                                OGS_PFCP_MODIFY_UL_ONLY|
                                OGS_PFCP_MODIFY_DEACTIVATE,
                                OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT,
                                0));
                    } else {
                        ogs_warn("[%s:%d] Session Release "
                                "[PFCP-Delete-Trigger:%d]",
                            smf_ue->supi, sess->psi, e->h.sbi.state);

                        r = smf_sbi_cleanup_session(
                                sess, stream,
                                SMF_UECM_STATE_DEREG_BY_AMF,
                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        OGS_FSM_TRAN(s,
                                smf_gsm_state_5gc_session_will_deregister);
                    }
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                END
                break;

            CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                DEFAULT
    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V: smf_nsmf_handle_update_data_in_vsmf
     * 12. V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ngap_build_pdu_session_resource_release_command_transfer+
     *        gsm_build_pdu_session_release_command
     * 16  V: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V: ogs_sbi_send_http_status_no_content(stream)
     * 18. V: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
     *        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H: case OGS_EVENT_SBI_CLIENT:
     * 22. H: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V*: case OGS_EVENT_SBI_SERVER:
     * 26. V*: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V*: ogs_sbi_send_http_status_no_content+
     *         smf_sbi_send_sm_context_status_notify
     * 28. V*: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                    ogs_assert(true ==
                            ogs_sbi_send_http_status_no_content(stream));

                    sess->resource_status = OpenAPI_resource_status_RELEASED;
                    if (sess->n1_released == true &&
                        sess->n2_released == true &&
                        sess->resource_status ==
                            OpenAPI_resource_status_RELEASED) {
                        ogs_assert(true ==
                                smf_sbi_send_sm_context_status_notify(sess));
                        OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
                    }
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                            sbi_message->h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                        "Invalid resource name [%s]",
                        sbi_message->h.resource.component[0], NULL));
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                    "Invalid API name", sbi_message->h.service.name,
                    NULL));
            OGS_FSM_TRAN(s, smf_gsm_state_exception);
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        if (stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID)
            stream = ogs_sbi_stream_find_by_id(stream_id);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V: smf_nsmf_handle_update_data_in_vsmf
     * 12. V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ngap_build_pdu_session_resource_release_command_transfer+
     *        gsm_build_pdu_session_release_command
     * 16  V: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V: ogs_sbi_send_http_status_no_content(stream)
     * 18. V: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
     *        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H*: case OGS_EVENT_SBI_CLIENT:
     * 22. H*: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H*: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                 SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V: case OGS_EVENT_SBI_SERVER:
     * 26. V: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V: ogs_sbi_send_http_status_no_content+
     *        smf_sbi_send_sm_context_status_notify
     * 28. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                        r = smf_sbi_cleanup_session(
                                sess, NULL,
                                SMF_UECM_STATE_DEREG_BY_N1N2_HR,
                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        OGS_FSM_TRAN(s,
                                smf_gsm_state_5gc_session_will_deregister);
                        break;
                    DEFAULT
                        ogs_error("[%s:%d] Invalid resource name [%s]",
                                smf_ue->supi, sess->psi,
                                sbi_message->h.resource.component[2]);
                        ogs_assert_if_reached();
                    END
                    break;

                DEFAULT
                    ogs_error("[%s:%d] Invalid HTTP method [%s]",
                            smf_ue->supi, sess->psi, sbi_message->h.method);
                    ogs_assert_if_reached();
                END
                break;
            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                ogs_info("[%s:%d] state [%d] res_status [%d], stream [%p:%d]",
                    smf_ue->supi, sess->psi,
                    e->h.sbi.state, sbi_message->res_status,
                    stream, stream_id);
                smf_namf_comm_handle_n1_n2_message_transfer(
                        sess, stream, e->h.sbi.state, sbi_message);
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            ogs_pkbuf_t *n1smbuf = NULL;

            ogs_error("[%s:%d] state [%d] res_status [%d]",
                smf_ue->supi, sess->psi,
                e->h.sbi.state, sbi_message->res_status);

            n1smbuf = gsm_build_pdu_session_release_reject(sess,
                OGS_5GSM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE);
            ogs_assert(n1smbuf);

            smf_sbi_send_sm_context_update_error_n1_n2_message(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    n1smbuf, OpenAPI_n2_sm_info_type_NULL, NULL);
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_DELETE)

                    UDM_SDM_CLEAR(sess);

                    r = smf_sbi_cleanup_session(
                            sess, stream, e->h.sbi.state,
                            SMF_SBI_CLEANUP_MODE_CONTEXT_ONLY);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    break;

                DEFAULT
                    ogs_error("[%s] Ignore invalid HTTP method [%s]",
                        smf_ue->supi, sbi_message->h.method);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[1]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                    smf_ue->supi, sess->psi, sbi_message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    case SMF_EVT_NGAP_MESSAGE:
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        ogs_assert(e->ngap.type);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        switch (e->ngap.type) {
        case OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP:
            /*
             * In case of PDU Session Anchor change, when removing the session,
             * it cannot be activated because there is no UPF.
             *
             * 1. InitialContextSetupResponse
             * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
             * 3. PFCP Session Modifcation Request (Apply: FORWARD)
             * 4. No UPF, so we cannot do PFCP Session Modifcation Response
             *
             * At this point, we just reply HTTP_STATUS_NO_CONTENT to the AMF.
             */
            ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
            break;

        case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
            ngap_state = sess->ngap_state.pdu_session_resource_release;

            if (ngap_state == SMF_NGAP_STATE_NONE) {
                strerror = ogs_msprintf(
                        "[%s:%d] No PDUSessionResourceReleaseRequest",
                        smf_ue->supi, sess->psi);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror,
                        NULL, NULL));
                ogs_free(strerror);

                OGS_FSM_TRAN(s, smf_gsm_state_exception);

            } else if (
                ngap_state == SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED ||
                ngap_state == SMF_NGAP_STATE_DELETE_TRIGGER_PCF_INITIATED ||
                ngap_state == SMF_NGAP_STATE_DELETE_TRIGGER_SMF_INITIATED) {

                ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));

    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V: smf_nsmf_handle_update_data_in_vsmf
     * 12. V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ngap_build_pdu_session_resource_release_command_transfer+
     *        gsm_build_pdu_session_release_command
     * 16  V: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V*: ogs_sbi_send_http_status_no_content(stream)
     * 18. V*: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
     *         case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V*: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V*: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H: case OGS_EVENT_SBI_CLIENT:
     * 22. H: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V: case OGS_EVENT_SBI_SERVER:
     * 26. V: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V: ogs_sbi_send_http_status_no_content+
     *        smf_sbi_send_sm_context_status_notify
     * 28. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                sess->n2_released = true;
                if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                    ogs_sbi_stream_t *vsmf_to_hsmf_release_stream = NULL;
                    if (sess->vsmf_to_hsmf_release_stream_id >=
                            OGS_MIN_POOL_ID &&
                        sess->vsmf_to_hsmf_release_stream_id <= OGS_MAX_POOL_ID)
                        vsmf_to_hsmf_release_stream =
                            ogs_sbi_stream_find_by_id(
                                    sess->vsmf_to_hsmf_release_stream_id);

                    if (vsmf_to_hsmf_release_stream) {
                        ogs_assert(true ==
                                ogs_sbi_send_http_status_no_content(
                                    vsmf_to_hsmf_release_stream));
                        sess->vsmf_to_hsmf_release_stream_id =
                            OGS_INVALID_POOL_ID;
                    }

                    if (sess->n1_released == true &&
                        sess->n2_released == true &&
                        sess->resource_status ==
                            OpenAPI_resource_status_RELEASED) {
                        ogs_assert(true ==
                                smf_sbi_send_sm_context_status_notify(sess));
                        OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
                    }
                } else if (sess->n1_released == true &&
                           sess->n2_released == true) {
                    r = smf_sbi_cleanup_session(
                            sess, NULL,
                            SMF_UECM_STATE_DEREG_BY_N1N2,
                            SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    OGS_FSM_TRAN(s,
                            smf_gsm_state_5gc_session_will_deregister);
                }
            } else {
                ogs_fatal("Invalid state [%d]", ngap_state);
                ogs_assert_if_reached();
            }
            break;
        default:
            ogs_error("Unknown message[%d]", e->ngap.type);
        }
        break;

    case SMF_EVT_5GSM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        switch (nas_message->gsm.h.message_type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
            ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));

    /*
     * UE-requested PDU Session Release
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V: OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 3.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_wait_5gc_n1_n2_release);
     * 11. V: smf_nsmf_handle_update_data_in_vsmf
     * 12. V: OpenAPI_request_indication_UE_REQ_PDU_SES_REL
     * 13. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED
     * 14. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 15. V: ngap_build_pdu_session_resource_release_command_transfer+
     *        gsm_build_pdu_session_release_command
     * 16  V: OGS_FSM_TRAN(&sess->sm, smf_gsm_state_wait_5gc_n1_n2_release)
     * 17. V*: ogs_sbi_send_http_status_no_content(stream)
     * 18. V*: case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
     *         case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
     * 19. V*: ogs_sbi_send_http_status_no_content(vsmf_to_hsmf_release_stream)
     * 20. V*: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 21. H: case OGS_EVENT_SBI_CLIENT:
     * 22. H: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 23. H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_N1N2_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 24. H: smf_sbi_send_status_notify+SMF_SESS_CLEAR(sess)
     * 25. V: case OGS_EVENT_SBI_SERVER:
     * 26. V: CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 27. V: ogs_sbi_send_http_status_no_content+
     *        smf_sbi_send_sm_context_status_notify
     * 28. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
            sess->n1_released = true;
            if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                ogs_sbi_stream_t *vsmf_to_hsmf_release_stream = NULL;
                if (sess->vsmf_to_hsmf_release_stream_id >= OGS_MIN_POOL_ID &&
                    sess->vsmf_to_hsmf_release_stream_id <= OGS_MAX_POOL_ID)
                    vsmf_to_hsmf_release_stream =
                        ogs_sbi_stream_find_by_id(
                                sess->vsmf_to_hsmf_release_stream_id);

                if (vsmf_to_hsmf_release_stream) {
                    ogs_assert(true ==
                            ogs_sbi_send_http_status_no_content(
                                vsmf_to_hsmf_release_stream));
                    sess->vsmf_to_hsmf_release_stream_id = OGS_INVALID_POOL_ID;
                }

                if (sess->n1_released == true &&
                    sess->n2_released == true &&
                    sess->resource_status == OpenAPI_resource_status_RELEASED) {
                    ogs_assert(true ==
                            smf_sbi_send_sm_context_status_notify(sess));
                    OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
                }
            } else if (sess->n1_released == true && sess->n2_released == true) {
                r = smf_sbi_cleanup_session(
                        sess, NULL,
                        SMF_UECM_STATE_DEREG_BY_N1N2,
                        SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
            }

            break;
        default:
            strerror = ogs_msprintf("Unknown message [%d]",
                    nas_message->gsm.h.message_type);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror,
                    NULL, NULL));
            ogs_free(strerror);
        }
        break;

    default:
        ogs_error("Unknown event [%s]", smf_event_get_name(e));
    }
}

void smf_gsm_state_5gc_n1_n2_reject(ogs_fsm_t *s, smf_event_t *e)
{
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        if (PCF_SM_POLICY_ASSOCIATED(sess)) {
            int r = 0;

            memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

            sess->nsmf_param.ue_location = true;
            sess->nsmf_param.ue_timezone = true;

            r = smf_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NPCF_SMPOLICYCONTROL, NULL,
                    smf_npcf_smpolicycontrol_build_delete,
                    sess, NULL,
                    OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT, NULL);
            ogs_expect(r == OGS_OK);
        } else {
            smf_namf_comm_send_n1_n2_pdu_establishment_reject(sess, NULL);
        }
        break;
    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICIES)
                if (!sbi_message->h.resource.component[1]) {
                    ogs_error("[%s:%d] HTTP response error [%d]",
                            smf_ue->supi, sess->psi,
                            sbi_message->res_status);
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                } else {
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                        PCF_SM_POLICY_CLEAR(sess);

                        if (sbi_message->res_status !=
                                OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                            ogs_error("[%s:%d] HTTP response error [%d]",
                                    smf_ue->supi, sess->psi,
                                    sbi_message->res_status);
                            /* In spite of error from PCF, continue with
                               session teardown, so as to not leave stale
                               sessions. */
                        }

                        smf_namf_comm_send_n1_n2_pdu_establishment_reject(
                                sess, NULL);
                        break;
                    DEFAULT
                        ogs_error("[%s:%d] Unknown resource name [%s]",
                                smf_ue->supi, sess->psi,
                                sbi_message->h.resource.component[2]);
                        OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    END
                }
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                    smf_ue->supi, sess->psi, sbi_message->h.service.name);
            OGS_FSM_TRAN(s, smf_gsm_state_exception);
        END
        break;

    default:
        ogs_error("Unknown event [%s]", smf_event_get_name(e));
        OGS_FSM_TRAN(s, smf_gsm_state_exception);
    }
}

void smf_gsm_state_5gc_session_will_deregister(ogs_fsm_t *s, smf_event_t *e)
{
    char *strerror = NULL;

    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *sbi_message = NULL;

    int r;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_SERVER:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXTS)
                SWITCH(sbi_message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                    ogs_assert(true == ogs_sbi_send_response(
                                stream, OGS_SBI_HTTP_STATUS_TOO_MANY_REQUESTS));
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                                sbi_message->h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                            "Invalid resource name [%s]",
                            sbi_message->h.resource.component[2], NULL));
                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                            sbi_message->h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                        "Invalid resource name [%s]",
                        sbi_message->h.resource.component[0], NULL));
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                    "Invalid API name", sbi_message->h.service.name,
                    NULL));
            OGS_FSM_TRAN(s, smf_gsm_state_exception);
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        sbi_message = e->h.sbi.message;
        ogs_assert(sbi_message);

        smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
        ogs_assert(smf_ue);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        if (stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID)
            stream = ogs_sbi_stream_find_by_id(stream_id);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICIES)
                if (!sbi_message->h.resource.component[1]) {
                    ogs_assert(stream);
                    strerror = ogs_msprintf(
                            "[%s:%d] HTTP response error [%d]",
                            smf_ue->supi, sess->psi,
                            sbi_message->res_status);
                    ogs_assert(strerror);

                    ogs_error("%s", strerror);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(
                            stream, sbi_message->res_status,
                            sbi_message, strerror, NULL,
                            (sbi_message->ProblemDetails) ?
                                    sbi_message->ProblemDetails->cause : NULL));
                    ogs_free(strerror);

                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                } else {
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                        PCF_SM_POLICY_CLEAR(sess);

                        if (sbi_message->res_status !=
                                OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                            ogs_error("[%s:%d] HTTP response error [%d]",
                                    smf_ue->supi, sess->psi,
                                    sbi_message->res_status);
                            /* In spite of error from PCF, continue with
                               session teardown, so as to not leave stale
                               sessions. */
                        }

                        r = smf_sbi_cleanup_session(
                                sess, stream, e->h.sbi.state,
                                SMF_SBI_CLEANUP_MODE_SUBSCRIPTION_FIRST);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        break;

                    DEFAULT
                        strerror = ogs_msprintf("[%s:%d] "
                                "Unknown resource name [%s]",
                                smf_ue->supi, sess->psi,
                                sbi_message->h.resource.component[2]);
                        ogs_assert(strerror);

                        ogs_error("%s", strerror);
                        if (stream)
                            ogs_assert(true ==
                                ogs_sbi_server_send_error(stream,
                                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                    sbi_message, strerror, NULL, NULL));
                        ogs_free(strerror);
                        OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    END
                }
                break;

            DEFAULT
                strerror = ogs_msprintf("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        sbi_message, strerror, NULL, NULL));
                ogs_free(strerror);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                SWITCH(sbi_message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_DELETE)

                    UDM_SDM_CLEAR(sess);

                    r = smf_sbi_cleanup_session(
                            sess, stream, e->h.sbi.state,
                            SMF_SBI_CLEANUP_MODE_CONTEXT_ONLY);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    break;

                DEFAULT
                    ogs_error("Ignore invalid HTTP method [%s]",
                        sbi_message->h.method);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message->h.resource.component[1]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message->h.service.name);
        END
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_gsm_state_session_will_release(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sess_t *sess = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        SMF_SESS_CLEAR(sess);
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_gsm_state_exception(ogs_fsm_t *s, smf_event_t *e)
{
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = smf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_error("[%s:%d] State machine exception", smf_ue->supi, sess->psi);
        SMF_SESS_CLEAR(sess);
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_gsm_state_final(ogs_fsm_t *s, smf_event_t *e)
{
}
