/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

static void send_gtp_create_err_msg(const smf_sess_t *sess, ogs_gtp_xact_t *gtp_xact, uint8_t gtp_cause)
{
    if (gtp_xact->gtp_version == 1)
        ogs_gtp1_send_error_message(gtp_xact, sess->sgw_s5c_teid,
            OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE, gtp_cause);
    else
        ogs_gtp2_send_error_message(gtp_xact, sess->sgw_s5c_teid,
            OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, gtp_cause);
}

void smf_gsm_state_initial(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e && e->sess);
    smf_sess_t *sess = e->sess;
    uint8_t gtp1_cause, gtp2_cause;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (!sess->gtp_rat_type) /* 5gc */
            OGS_FSM_TRAN(s, &smf_gsm_state_operational);
        break;

    case SMF_EVT_GN_MESSAGE:
        switch(e->gtp1_message->h.type) {
        case OGS_GTP1_CREATE_PDP_CONTEXT_REQUEST_TYPE:
            gtp1_cause = smf_gn_handle_create_pdp_context_request(sess,
                            e->gtp_xact,
                            &e->gtp1_message->create_pdp_context_request);
            if (gtp1_cause != OGS_GTP1_CAUSE_REQUEST_ACCEPTED) {
                send_gtp_create_err_msg(sess, e->gtp_xact, gtp1_cause);
                return;
            }
            OGS_FSM_TRAN(s, &smf_gsm_state_initial_wait_auth);
            smf_gx_send_ccr(sess, e->gtp_xact,
                OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST);
        }
        break;

    case SMF_EVT_S5C_MESSAGE:
        switch(e->gtp2_message->h.type) {
        case OGS_GTP2_CREATE_SESSION_REQUEST_TYPE:
            gtp2_cause = smf_s5c_handle_create_session_request(sess,
                            e->gtp_xact,
                            &e->gtp2_message->create_session_request);
            if (gtp2_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                send_gtp_create_err_msg(sess, e->gtp_xact, gtp2_cause);
                return;
            }
            OGS_FSM_TRAN(s, &smf_gsm_state_initial_wait_auth);
            switch (sess->gtp_rat_type) {
            case OGS_GTP2_RAT_TYPE_EUTRAN:
                smf_gx_send_ccr(sess, e->gtp_xact,
                    OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST);
                break;
            case OGS_GTP2_RAT_TYPE_WLAN:
                smf_s6b_send_aar(sess, e->gtp_xact);
                break;
            default:
                ogs_error("Unknown RAT Type [%d]", sess->gtp_rat_type);
                ogs_assert_if_reached();
            }
            break;
        }
        break;
    }
}

void smf_gsm_state_initial_wait_auth(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_assert(e && e->sess);
    smf_sess_t *sess = e->sess;
    uint32_t diam_err;
    uint8_t gtp_cause;

    switch (e->id) {
    case SMF_EVT_GX_MESSAGE:
        switch(e->gx_message->cmd_code) {
        case OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL:
            switch(e->gx_message->cc_request_type) {
            case OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST:
                ogs_assert(e->gtp_xact);
                diam_err = smf_gx_handle_cca_initial_request(sess,
                                e->gx_message, e->gtp_xact);
                if (diam_err != ER_DIAMETER_SUCCESS) {
                    uint8_t gtp_cause = gtp_cause_from_diameter(
                                            e->gtp_xact->gtp_version,
                                            diam_err, e->gx_message->exp_err);
                    send_gtp_create_err_msg(sess, e->gtp_xact, gtp_cause);
                    return;
                }
                switch(smf_use_gy_iface()) {
                case 1:
                    /* Gy is available, set up session for the bearer before accepting it towards the UE */
                    /* OGS_FSM_TRAN: keep current state, we handle Gy here. */
                    smf_gy_send_ccr(sess, e->gtp_xact,
                        OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST);
                    return;
                case 0:
                    /* Not using Gy, jump directly to PFCP Session Establishment Request */
                    OGS_FSM_TRAN(s, &smf_gsm_state_initial_wait_pfcp_establishment);
                    ogs_assert(OGS_OK ==
                        smf_epc_pfcp_send_session_establishment_request(sess, e->gtp_xact));
                    return;
                case -1:
                    ogs_error("No Gy Diameter Peer");
                    /* TODO: drop Gx connection here, possibly move to another "releasing" state! */
                    gtp_cause = (e->gtp_xact->gtp_version == 1) ?
                                    OGS_GTP1_CAUSE_NO_RESOURCES_AVAILABLE :
                                    OGS_GTP2_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER;
                    send_gtp_create_err_msg(sess, e->gtp_xact, gtp_cause);
                    return;
                }
                break;
            }
            break;
        }
        break;

    case SMF_EVT_GY_MESSAGE:
        switch(e->gy_message->cmd_code) {
        case OGS_DIAM_GY_CMD_CODE_CREDIT_CONTROL:
            switch(e->gy_message->cc_request_type) {
            case OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST:
                ogs_assert(e->gtp_xact);
                diam_err = smf_gy_handle_cca_initial_request(sess,
                                e->gy_message, e->gtp_xact);
                if (diam_err != ER_DIAMETER_SUCCESS) {
                    /* FIXME: tear down Gx session */
                    gtp_cause = gtp_cause_from_diameter(e->gtp_xact->gtp_version,
                                    diam_err, e->gy_message->exp_err);
                    send_gtp_create_err_msg(sess, e->gtp_xact, gtp_cause);
                    return;
                }
                OGS_FSM_TRAN(s, &smf_gsm_state_initial_wait_pfcp_establishment);
                ogs_assert(OGS_OK ==
                    smf_epc_pfcp_send_session_establishment_request(sess, e->gtp_xact));
                return;
            }
            break;
        }
        break;
    }
}

void smf_gsm_state_initial_wait_pfcp_establishment(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_assert(e && e->sess);
    smf_sess_t *sess = e->sess;
    uint8_t pfcp_cause, gtp_cause;

    switch (e->id) {
    case SMF_EVT_N4_MESSAGE:
        switch (e->pfcp_message->h.type) {
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            if (e->pfcp_xact->epc) {
                ogs_gtp_xact_t *gtp_xact = e->pfcp_xact->assoc_xact;
                pfcp_cause = smf_epc_n4_handle_session_establishment_response(
                                sess, e->pfcp_xact,
                                &e->pfcp_message->pfcp_session_establishment_response);
                if (pfcp_cause != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
                    /* FIXME: tear down Gy and Gx */
                    gtp_cause = gtp_cause_from_pfcp(pfcp_cause, gtp_xact->gtp_version);
                    send_gtp_create_err_msg(sess, e->gtp_xact, gtp_cause);
                    return;
                }
                switch (gtp_xact->gtp_version) {
                case 1:
                    ogs_assert(OGS_OK == smf_gtp1_send_create_pdp_context_response(sess, gtp_xact));
                    break;
                case 2:
                    ogs_assert(OGS_OK == smf_gtp_send_create_session_response(sess, gtp_xact));
                    break;
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
                    ogs_assert(sess->smf_ue);
                    smf_sess_t *eutran_sess = smf_sess_find_by_apn(
                        sess->smf_ue, sess->session.name, OGS_GTP2_RAT_TYPE_EUTRAN);
                    if (eutran_sess) {
                        ogs_assert(OGS_OK ==
                            smf_epc_pfcp_send_session_modification_request(
                                eutran_sess, NULL,
                                OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE,
                                OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                                OGS_GTP2_CAUSE_RAT_CHANGED_FROM_3GPP_TO_NON_3GPP));
                    }
                }
                smf_bearer_binding(sess);
            } else {
#if 0
                /* This is currently not happening, since 5gc isn't yet properly integrated and moves directly to operational state */
                smf_n1_n2_message_transfer_param_t param;
                pfcp_cause = smf_5gc_n4_handle_session_establishment_response(
                                sess, e->pfcp_xact,
                                &e->pfcp_message->pfcp_session_establishment_response);
                if (pfcp_cause != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                    return;
                memset(&param, 0, sizeof(param));
                param.state = SMF_UE_REQUESTED_PDU_SESSION_ESTABLISHMENT;
                param.n1smbuf = gsm_build_pdu_session_establishment_accept(sess);
                ogs_assert(param.n1smbuf);
                param.n2smbuf = ngap_build_pdu_session_resource_setup_request_transfer(
                                    sess);
                ogs_assert(param.n2smbuf);
                smf_namf_comm_send_n1_n2_message_transfer(sess, &param);
#endif
            }
            OGS_FSM_TRAN(s, &smf_gsm_state_operational);
            break;
        }
        break;
    }
}

void smf_gsm_state_operational(ogs_fsm_t *s, smf_event_t *e)
{
    int rv, ngap_state;
    char *strerror = NULL;
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_message_t *sbi_message = NULL;

    smf_n1_n2_message_transfer_param_t param;
    uint8_t pfcp_cause;

    int state = 0;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = e->sess;
    ogs_assert(sess);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case SMF_EVT_N4_MESSAGE:
        switch (e->pfcp_message->h.type) {
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            /* This is left here for 5gc sessions to properly receive messages,
               since startup transitions are not implemented yet for 5gc
               sessions. See #if0 in smf_gsm_state_initial_wait_pfcp_establishment() */
            assert(!e->pfcp_xact->epc);
            pfcp_cause = smf_5gc_n4_handle_session_establishment_response(
                            sess, e->pfcp_xact,
                            &e->pfcp_message->pfcp_session_establishment_response);
            if (pfcp_cause != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                return;
            memset(&param, 0, sizeof(param));
            param.state = SMF_UE_REQUESTED_PDU_SESSION_ESTABLISHMENT;
            param.n1smbuf = gsm_build_pdu_session_establishment_accept(sess);
            ogs_assert(param.n1smbuf);
            param.n2smbuf = ngap_build_pdu_session_resource_setup_request_transfer(
                                sess);
            ogs_assert(param.n2smbuf);
            smf_namf_comm_send_n1_n2_message_transfer(sess, &param);
            break;
        }
        break;

    case SMF_EVT_SBI_SERVER:
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);
        stream = e->sbi.data;
        ogs_assert(stream);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message->h.resource.component[2])
            CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                smf_nsmf_handle_update_sm_context(sess, stream, sbi_message);
                break;
            CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                smf_nsmf_handle_release_sm_context(sess, stream, sbi_message);
                break;
            DEFAULT
                smf_nsmf_handle_create_sm_context(sess, stream, sbi_message);
                break;
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                    "Invalid API name", sbi_message->h.service.name));
        END
        break;

    case SMF_EVT_SBI_CLIENT:
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);

        sess = e->sess;
        ogs_assert(sess);
        smf_ue = sess->smf_ue;
        ogs_assert(smf_ue);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            stream = e->sbi.data;
            ogs_assert(stream);

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
                            sbi_message, strerror, NULL));
                    ogs_free(strerror);

                    OGS_FSM_TRAN(s, smf_gsm_state_exception);
                    break;
                }

                if (smf_nudm_sdm_handle_get(
                            sess, stream, sbi_message) != true) {
                    OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
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
                        sbi_message, strerror, NULL));
                ogs_free(strerror);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            stream = e->sbi.data;
            state = e->sbi.state;

            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICIES)
                if (!sbi_message->h.resource.component[1]) {
                    ogs_assert(stream);
                    if (sbi_message->res_status !=
                            OGS_SBI_HTTP_STATUS_CREATED) {
                        strerror = ogs_msprintf(
                                "[%s:%d] HTTP response error [%d]",
                                smf_ue->supi, sess->psi,
                                sbi_message->res_status);
                        ogs_assert(strerror);

                        ogs_error("%s", strerror);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                sbi_message->res_status,
                                sbi_message, strerror, NULL));
                        ogs_free(strerror);
                        break;
                    }

                    smf_npcf_smpolicycontrol_handle_create(
                            sess, stream, state, sbi_message);
                } else {
                    SWITCH(sbi_message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                        if (sbi_message->res_status !=
                                OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                            strerror = ogs_msprintf(
                                    "[%s:%d] HTTP response error [%d]",
                                    smf_ue->supi, sess->psi,
                                    sbi_message->res_status);
                            ogs_assert(strerror);
                            ogs_error("%s", strerror);
                            ogs_free(strerror);
                        }

                        smf_npcf_smpolicycontrol_handle_delete(
                                sess, stream, state, sbi_message);
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
                                    sbi_message, strerror, NULL));
                        ogs_free(strerror);
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
                        sbi_message, strerror, NULL));
                ogs_free(strerror);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                smf_namf_comm_handle_n1_n2_message_transfer(
                        sess, e->sbi.state, sbi_message);
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

    case SMF_EVT_5GSM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);
        sess = e->sess;
        ogs_assert(sess);
        stream = e->sbi.data;
        ogs_assert(stream);
        smf_ue = sess->smf_ue;
        ogs_assert(smf_ue);

        switch (nas_message->gsm.h.message_type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
            rv = gsm_handle_pdu_session_establishment_request(sess, stream,
                    &nas_message->gsm.pdu_session_establishment_request);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NAS message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
            rv = gsm_handle_pdu_session_modification_request(sess, stream,
                    &nas_message->gsm.pdu_session_modification_request);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NAS message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
            ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
            break;

        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
            if (sess->policy_association_id) {
                smf_npcf_smpolicycontrol_param_t param;

                memset(&param, 0, sizeof(param));

                param.ran_nas_release.gsm_cause =
                    OGS_5GSM_CAUSE_REGULAR_DEACTIVATION;
                param.ran_nas_release.ngap_cause.group = NGAP_Cause_PR_nas;
                param.ran_nas_release.ngap_cause.value =
                    NGAP_CauseNas_normal_release;

                ogs_assert(true ==
                    smf_sbi_discover_and_send(OpenAPI_nf_type_PCF, sess, stream,
                        OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED, &param,
                        smf_npcf_smpolicycontrol_build_delete));
            } else {
                ogs_error("[%s:%d] No PolicyAssociationId",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        default:
            strerror = ogs_msprintf("Unknown message [%d]",
                    nas_message->gsm.h.message_type);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror, NULL));
            ogs_free(strerror);
        }
        break;

    case SMF_EVT_NGAP_MESSAGE:
        sess = e->sess;
        ogs_assert(sess);
        stream = e->sbi.data;
        ogs_assert(stream);
        smf_ue = sess->smf_ue;
        ogs_assert(smf_ue);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        ogs_assert(e->ngap.type);

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
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror, NULL));
                ogs_free(strerror);

                OGS_FSM_TRAN(s, smf_gsm_state_exception);

            } else if (
                ngap_state == SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED ||
                ngap_state == SMF_NGAP_STATE_DELETE_TRIGGER_PCF_INITIATED) {

                ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));

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

                smf_namf_comm_send_n1_n2_message_transfer(sess, &param);
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

    default:
        ogs_error("Unknown event [%s]", smf_event_get_name(e));
    }
}

void smf_gsm_state_session_will_release(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sess_t *sess = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = e->sess;
    ogs_assert(sess);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
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
    smf_sess_t *sess = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = e->sess;
    ogs_assert(sess);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
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
