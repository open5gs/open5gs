/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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
#include "ngap-path.h"
#include "nas-path.h"
#include "ngap-handler.h"
#include "nnrf-handler.h"
#include "namf-handler.h"
#include "nsmf-handler.h"
#include "nnssf-handler.h"
#include "nas-security.h"

void amf_state_initial(ogs_fsm_t *s, amf_event_t *e)
{
    amf_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &amf_state_operational);
}

void amf_state_final(ogs_fsm_t *s, amf_event_t *e)
{
    amf_sm_debug(e);

    ogs_assert(s);
}

void amf_state_operational(ogs_fsm_t *s, amf_event_t *e)
{
    int r, rv;
    char buf[OGS_ADDRSTRLEN];
    const char *api_version = NULL;

    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;
    amf_gnb_t *gnb = NULL;
    uint16_t max_num_of_ostreams = 0;

    ogs_ngap_message_t ngap_message;
    ogs_pkbuf_t *pkbuf = NULL;
    int rc;

    ogs_nas_5gs_message_t nas_message;
    ran_ue_t *ran_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    ogs_sbi_object_t *sbi_object = NULL;
    ogs_pool_id_t sbi_object_id = OGS_INVALID_POOL_ID;
    ogs_sbi_xact_t *sbi_xact = NULL;
    ogs_pool_id_t sbi_xact_id = OGS_INVALID_POOL_ID;
    int state = AMF_CREATE_SM_CONTEXT_NO_STATE;
    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_request_t *sbi_request = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t sbi_message;

    amf_sm_debug(e);

    ogs_assert(s);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_SERVER:
        sbi_request = e->h.sbi.request;
        ogs_assert(sbi_request);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        rv = ogs_sbi_parse_request(&sbi_message, sbi_request);
        if (rv != OGS_OK) {
            /* 'sbi_message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP sbi_message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP sbi_message", NULL, NULL));
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            api_version = OGS_SBI_API_V2;
            break;
        DEFAULT
            api_version = OGS_SBI_API_V1;
        END

        ogs_assert(api_version);
        if (strcmp(sbi_message.h.api.version, api_version) != 0) {
            ogs_error("Not supported version [%s]", sbi_message.h.api.version);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &sbi_message, "Not supported version", NULL, NULL));
            ogs_sbi_message_free(&sbi_message);
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    ogs_nnrf_nfm_handle_nf_status_notify(stream, &sbi_message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, &sbi_message,
                            "Invalid HTTP method", sbi_message.h.method, NULL));
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[0], NULL));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                SWITCH(sbi_message.h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES)
                    SWITCH(sbi_message.h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        rv = amf_namf_comm_handle_n1_n2_message_transfer(
                                stream, &sbi_message);
                        if (rv != OGS_OK) {
                            ogs_assert(true ==
                                ogs_sbi_server_send_error(stream,
                                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                    &sbi_message,
                                    "No N1N2MessageTransferReqData", NULL, NULL));
                        }
                        break;

                    DEFAULT
                        ogs_error("Invalid HTTP method [%s]",
                                sbi_message.h.method);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_FORBIDDEN, &sbi_message,
                                "Invalid HTTP method", sbi_message.h.method,
                                NULL));
                    END
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_TRANSFER)
                    SWITCH(sbi_message.h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        amf_namf_comm_handle_ue_context_transfer_request(
                                stream, &sbi_message);
                        break;
                    DEFAULT
                        ogs_error("Invalid HTTP method [%s]",
                                sbi_message.h.method);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_FORBIDDEN, &sbi_message,
                                "Invalid HTTP method", sbi_message.h.method,
                                NULL));
                    END
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_TRANSFER_UPDATE)
                    SWITCH(sbi_message.h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        amf_namf_comm_handle_registration_status_update_request(
                                stream, &sbi_message);
                        break;
                    DEFAULT
                        ogs_error("Invalid HTTP method [%s]",
                                sbi_message.h.method);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_FORBIDDEN, &sbi_message,
                                "Invalid HTTP method", sbi_message.h.method,
                                NULL));
                    END
                    break;

                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                            sbi_message.h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                            "Invalid resource name",
                            sbi_message.h.resource.component[2], NULL));
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[0], NULL));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_CALLBACK)
            SWITCH(sbi_message.h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS)
                amf_namf_callback_handle_sm_context_status(
                        stream, &sbi_message);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_DEREG_NOTIFY)
                amf_namf_callback_handle_dereg_notify(stream, &sbi_message);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SDMSUBSCRIPTION_NOTIFY)
                amf_namf_callback_handle_sdm_data_change_notify(
                        stream, &sbi_message);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_AM_POLICY_NOTIFY)
                ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[1]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[1], NULL));
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message.h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                    "Invalid API name", sbi_message.h.resource.component[0],
                    NULL));
        END

        /* In lib/sbi/server.c, notify_completed() releases 'request' buffer. */
        ogs_sbi_message_free(&sbi_message);
        break;

    case OGS_EVENT_SBI_CLIENT:
        ogs_assert(e);

        sbi_response = e->h.sbi.response;
        ogs_assert(sbi_response);
        rv = ogs_sbi_parse_response(&sbi_message, sbi_response);
        if (rv != OGS_OK) {
            ogs_error("cannot parse HTTP response");
            ogs_sbi_message_free(&sbi_message);
            ogs_sbi_response_free(sbi_response);
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
        CASE(OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION)
            api_version = OGS_SBI_API_V2;
            break;
        DEFAULT
            api_version = OGS_SBI_API_V1;
        END

        ogs_assert(api_version);
        if (strcmp(sbi_message.h.api.version, api_version) != 0) {
            ogs_error("Not supported version [%s]", sbi_message.h.api.version);
            ogs_sbi_message_free(&sbi_message);
            ogs_sbi_response_free(sbi_response);
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                nf_instance = e->h.sbi.data;
                ogs_assert(nf_instance);
                ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

                e->h.sbi.message = &sbi_message;
                ogs_fsm_dispatch(&nf_instance->sm, e);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                subscription_data = e->h.sbi.data;
                ogs_assert(subscription_data);

                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_CREATED ||
                        sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        ogs_nnrf_nfm_handle_nf_status_subscribe(
                                subscription_data, &sbi_message);
                    } else {
                        ogs_error("HTTP response error : %d",
                                sbi_message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_PATCH)
                    if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK ||
                        sbi_message.res_status ==
                            OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_nnrf_nfm_handle_nf_status_update(
                                subscription_data, &sbi_message);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown",
                                sbi_message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    if (sbi_message.res_status ==
                            OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_sbi_subscription_data_remove(subscription_data);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown",
                                sbi_message.res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_assert_if_reached();
                END
                break;
            
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NNRF_DISC)
            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
                ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                        sbi_xact_id <= OGS_MAX_POOL_ID);

                sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
                if (!sbi_xact) {
                    /* CLIENT_WAIT timer could remove SBI transaction
                     * before receiving SBI message */
                    ogs_error("SBI transaction has already been removed [%d]",
                            sbi_xact_id);
                    break;
                }

                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_GET)
                    if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK)
                        amf_nnrf_handle_nf_discover(sbi_xact, &sbi_message);
                    else
                        ogs_error("HTTP response error [%d]",
                                sbi_message.res_status);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAUSF_AUTH)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
        CASE(OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)
        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                    sbi_xact_id <= OGS_MAX_POOL_ID);

            sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
            if (!sbi_xact) {
                /* CLIENT_WAIT timer could remove SBI transaction
                 * before receiving SBI message */
                ogs_error("SBI transaction has already been removed [%d]",
                        sbi_xact_id);
                break;
            }

            state = sbi_xact->state;

            sbi_object_id = sbi_xact->sbi_object_id;
            ogs_assert(sbi_object_id >= OGS_MIN_POOL_ID &&
                    sbi_object_id <= OGS_MAX_POOL_ID);

            ogs_sbi_xact_remove(sbi_xact);

            amf_ue = amf_ue_find_by_id(sbi_object_id);
            if (!amf_ue) {
                ogs_error("UE(amf_ue) Context has already been removed");
                break;
            }

            ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

            e->amf_ue_id = amf_ue->id;
            e->h.sbi.message = &sbi_message;;
            e->h.sbi.state = state;

            ogs_fsm_dispatch(&amf_ue->sm, e);
            break;

        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                    sbi_xact_id <= OGS_MAX_POOL_ID);

            sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
            if (!sbi_xact) {
                /* CLIENT_WAIT timer could remove SBI transaction
                 * before receiving SBI message */
                ogs_error("SBI transaction has already been removed [%d]",
                        sbi_xact_id);
                break;
            }

            state = sbi_xact->state;

            sbi_object_id = sbi_xact->sbi_object_id;
            ogs_assert(sbi_object_id >= OGS_MIN_POOL_ID &&
                    sbi_object_id <= OGS_MAX_POOL_ID);

            ogs_sbi_xact_remove(sbi_xact);

            sess = amf_sess_find_by_id(sbi_object_id);
            if (!sess) {
            /*
             * 1. If AMF-UE context is duplicated in Identity-Response,
             *    OLD AMF-UE's all session contexts are removed.
             * 2. If there is an unfinished transaction with SMF,
             *    Transaction's session context is NULL.
             *
             * For example,
             *
             * 1. gNB->AMF : PDUSessionResourceSetupResponse
             * 2. AMF->SMF : [POST] /nsmf-pdusession/v1/sm-contexts/1/modify
             * 3. UE ->AMF : Registration request with Unknwon GUTI
             * 4. AMF->UE  : Identity request
             * 5. UE ->AMF : Identity response
             *               AMF UE context duplicated.
             *               All session contexts are removed
             * 6. SMF->AMF : RESPONSE /nsmf-pdusession/v1/sm-contexts/1/modify
             *               No Session Context
             *               Assertion
             *
             * OR
             *
             * In ./tests/vonr/af-test/test3_func()
             * 1. Send PDU session establishment request
             * 2. Receive PDU session establishment accept
             * 3. Send PDUSessionResourceSetupResponse
             * 4. Send De-registration request
             * 5. SMF->AMF : RESPONSE /nsmf-pdusession/v1/sm-contexts/3/release
             * 6. SMF->AMF : RESPONSE /nsmf-pdusession/v1/sm-contexts/1/modify
             *
             * IF THIS HAPPENS IN THE REAL WORLD,
             * I WILL MODIFY THE ASSERTS BELOW.
             */
                ogs_error("Session has already been removed");
                break;
            }

            amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
            if (!amf_ue) {
                ogs_error("UE(amf-ue) context has already been removed");
                break;
            }

            ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

            e->amf_ue_id = amf_ue->id;
            e->sess_id = sess->id;
            e->h.sbi.message = &sbi_message;;

            SWITCH(sbi_message.h.resource.component[2])
            CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                amf_nsmf_pdusession_handle_update_sm_context(
                        sess, state, &sbi_message);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT ||
                    sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                    ogs_info("[%s:%d] Release SM context [%d]",
                            amf_ue->supi, sess->psi, sbi_message.res_status);
                } else {
                    ogs_error("[%s:%d] HTTP response error [%d]",
                            amf_ue->supi, sess->psi, sbi_message.res_status);
                }
                amf_nsmf_pdusession_handle_release_sm_context(sess, state);
                break;

            DEFAULT
                rv = amf_nsmf_pdusession_handle_create_sm_context(
                        sess, &sbi_message);
                if (rv != OGS_OK) {
                    /*
                     * 1. First PDU session establishment request
                     *    (PSI:5, internet)
                     * 2. First session created
                     * 3. Seconds PDU session establishment request
                     *    (PSI:5, ims)
                     * 4. AMF sends DUPLICATED_PDU_SESSION_ID to the SMF
                     * 5. AMF try to create second PDU session.
                     * 6. But, Second session rejected due to Subscription Info.
                     *
                     * In above situation, AMF need to clear SM_CONTEXT_REF.
                     * Otherwise, AMF have redundant PDU session.
                     *
                     * Moreover, AMF could send UEContextReleaseRequest
                     * with deactivating this redundant session.
                     *
                     * So, if CreateSMContext is failed,
                     * we'll clear SM_CONTEXT_REF.
                     */
                    ogs_error("[%s:%d] create_sm_context failed() [%d]",
                            amf_ue->supi, sess->psi, sbi_message.res_status);
                    AMF_SESS_CLEAR(sess);
                }
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION)
            sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                    sbi_xact_id <= OGS_MAX_POOL_ID);

            sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
            if (!sbi_xact) {
                /* CLIENT_WAIT timer could remove SBI transaction
                 * before receiving SBI message */
                ogs_error("SBI transaction has already been removed [%d]",
                        sbi_xact_id);
                break;
            }

            sbi_object_id = sbi_xact->sbi_object_id;
            ogs_assert(sbi_object_id >= OGS_MIN_POOL_ID &&
                    sbi_object_id <= OGS_MAX_POOL_ID);

            state = sbi_xact->state;

            ogs_sbi_xact_remove(sbi_xact);

            sess = amf_sess_find_by_id(sbi_object_id);
            if (!sess) {
                ogs_error("Session has already been removed");
                break;
            }

            amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
            if (!amf_ue) {
                ogs_error("UE(amf-ue) context has already been removed");
                break;
            }

            ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

            e->amf_ue_id = amf_ue->id;
            e->sess_id = sess->id;
            e->h.sbi.message = &sbi_message;;
            e->h.sbi.state = state;

            amf_nnssf_nsselection_handle_get(sess, &sbi_message);
            break;

        DEFAULT
            ogs_error("Invalid service name [%s]", sbi_message.h.service.name);
            ogs_assert_if_reached();
        END

        ogs_sbi_message_free(&sbi_message);
        ogs_sbi_response_free(sbi_response);
        break;

    case OGS_EVENT_SBI_TIMER:
        ogs_assert(e);

        switch(e->h.timer_id) {
        case OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case OGS_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->h.sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            ogs_sbi_self()->nf_instance->load = amf_instance_get_load();

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_exception))
                ogs_error("[%s:%s] State machine exception [%d]",
                        OpenAPI_nf_type_ToString(nf_instance->nf_type),
                        nf_instance->id, e->h.timer_id);
            break;

        case OGS_TIMER_SUBSCRIPTION_VALIDITY:
            subscription_data = e->h.sbi.data;
            ogs_assert(subscription_data);

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_subscribe(
                    ogs_sbi_self()->nf_instance->nf_type,
                    subscription_data->req_nf_instance_id,
                    subscription_data->subscr_cond.nf_type,
                    subscription_data->subscr_cond.service_name));

            ogs_error("[%s] Subscription validity expired",
                subscription_data->id);
            ogs_sbi_subscription_data_remove(subscription_data);
            break;

        case OGS_TIMER_SUBSCRIPTION_PATCH:
            subscription_data = e->h.sbi.data;
            ogs_assert(subscription_data);

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_update(subscription_data));

            ogs_info("[%s] Need to update Subscription",
                    subscription_data->id);
            break;

        case OGS_TIMER_SBI_CLIENT_WAIT:
            /*
             * ogs_pollset_poll() receives the time of the expiration
             * of next timer as an argument. If this timeout is
             * in very near future (1 millisecond), and if there are
             * multiple events that need to be processed by ogs_pollset_poll(),
             * these could take more than 1 millisecond for processing,
             * resulting in the timer already passed the expiration.
             *
             * In case that another NF is under heavy load and responds
             * to an SBI request with some delay of a few seconds,
             * it can happen that ogs_pollset_poll() adds SBI responses
             * to the event list for further processing,
             * then ogs_timer_mgr_expire() is called which will add
             * an additional event for timer expiration. When all events are
             * processed one-by-one, the SBI xact would get deleted twice
             * in a row, resulting in a crash.
             *
             * 1. ogs_pollset_poll()
             *    message was received and put into an event list,
             * 2. ogs_timer_mgr_expire()
             *    add an additional event for timer expiration
             * 3. message event is processed. (free SBI xact)
             * 4. timer expiration event is processed. (double-free SBI xact)
             *
             * To avoid double-free SBI xact,
             * we need to check ogs_sbi_xact_find_by_id()
             */
            sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                    sbi_xact_id <= OGS_MAX_POOL_ID);

            sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
            if (!sbi_xact) {
                ogs_error("SBI transaction has already been removed [%d]",
                        sbi_xact_id);
                break;
            }

            sbi_object = sbi_xact->sbi_object;
            ogs_assert(sbi_object);

            sbi_object_id = sbi_xact->sbi_object_id;
            ogs_assert(sbi_object_id >= OGS_MIN_POOL_ID &&
                    sbi_object_id <= OGS_MAX_POOL_ID);

            service_type = sbi_xact->service_type;

            ogs_sbi_xact_remove(sbi_xact);

            ogs_assert(sbi_object->type > OGS_SBI_OBJ_BASE &&
                        sbi_object->type < OGS_SBI_OBJ_TOP);

            switch(sbi_object->type) {
            case OGS_SBI_OBJ_UE_TYPE:
                amf_ue = amf_ue_find_by_id(sbi_object_id);
                if (!amf_ue) {
                    ogs_error("UE(amf_ue) Context has already been removed");
                    break;
                }

                ogs_error("[%s] Cannot receive SBI message", amf_ue->suci);
                r = nas_5gs_send_gmm_reject_from_sbi(amf_ue,
                        OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                break;

            case OGS_SBI_OBJ_SESS_TYPE:
                sess = amf_sess_find_by_id(sbi_object_id);
                if (!sess) {
                    ogs_error("Session has already been removed");
                    break;
                }

                amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
                if (!amf_ue) {
                    ogs_error("UE(amf_ue) Context has already been removed");
                    break;
                }

                ogs_error("[%d:%d] Cannot receive SBI message",
                        sess->psi, sess->pti);
                if (sess->payload_container_type) {
                    r = nas_5gs_send_back_gsm_message(
                            ran_ue_find_by_id(sess->ran_ue_id), sess,
                            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                            AMF_NAS_BACKOFF_TIME);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else {
                    r = ngap_send_error_indication2(
                            ran_ue_find_by_id(sess->ran_ue_id),
                            NGAP_Cause_PR_transport,
                            NGAP_CauseTransport_transport_resource_unavailable);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                }
                break;

            default:
                ogs_fatal("Not implemented [%s:%d]",
                    ogs_sbi_service_type_to_name(service_type),
                    sbi_object->type);
                ogs_assert_if_reached();
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    case AMF_EVENT_NGAP_LO_ACCEPT:
        sock = e->ngap.sock;
        ogs_assert(sock);
        addr = e->ngap.addr;
        ogs_assert(addr);

        ogs_info("gNB-N2 accepted[%s] in master_sm module",
            OGS_ADDR(addr, buf));

        gnb = amf_gnb_find_by_addr(addr);
        if (!gnb) {
            gnb = amf_gnb_add(sock, addr);
            if (!gnb) {
                ogs_error("amf_gnb_add() failed");
                ogs_sock_destroy(sock);
                ogs_free(addr);
            }
        } else {
            ogs_warn("gNB context duplicated with IP-address [%s]!!!",
                    OGS_ADDR(addr, buf));
            ogs_sock_destroy(sock);
            ogs_free(addr);
            ogs_warn("N2 Socket Closed");
        }

        break;

    case AMF_EVENT_NGAP_LO_SCTP_COMM_UP:
        sock = e->ngap.sock;
        ogs_assert(sock);
        addr = e->ngap.addr;
        ogs_assert(addr);

        max_num_of_ostreams = e->ngap.max_num_of_ostreams;

        gnb = amf_gnb_find_by_addr(addr);
        if (!gnb) {
            gnb = amf_gnb_add(sock, addr);
            if (!gnb) {
                ogs_error("amf_gnb_add() failed");
                ogs_free(addr);
                break;
            }
        } else {
            ogs_free(addr);
        }

        if (gnb->max_num_of_ostreams)
            gnb->max_num_of_ostreams =
                    ogs_min(max_num_of_ostreams, gnb->max_num_of_ostreams);
        else
            gnb->max_num_of_ostreams = max_num_of_ostreams;

        ogs_info("gNB-N2[%s] max_num_of_ostreams : %d",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->max_num_of_ostreams);

        break;

    case AMF_EVENT_NGAP_LO_CONNREFUSED:
        sock = e->ngap.sock;
        ogs_assert(sock);
        addr = e->ngap.addr;
        ogs_assert(addr);

        gnb = amf_gnb_find_by_addr(addr);
        if (gnb) {
            ogs_info("gNB-N2[%s] connection refused!!!", OGS_ADDR(addr, buf));
            amf_sbi_send_deactivate_all_ue_in_gnb(
                    gnb, AMF_REMOVE_S1_CONTEXT_BY_LO_CONNREFUSED);
            amf_gnb_remove(gnb);
        } else {
            ogs_warn("gNB-N2[%s] connection refused, Already Removed!",
                    OGS_ADDR(addr, buf));
        }
        ogs_free(addr);

        break;
    case AMF_EVENT_NGAP_MESSAGE:
        sock = e->ngap.sock;
        ogs_assert(sock);
        addr = e->ngap.addr;
        ogs_assert(addr);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        gnb = amf_gnb_find_by_addr(addr);
        ogs_free(addr);

        ogs_assert(gnb);
        ogs_assert(OGS_FSM_STATE(&gnb->sm));

        rc = ogs_ngap_decode(&ngap_message, pkbuf);
        if (rc == OGS_OK) {
            e->gnb_id = gnb->id;
            e->ngap.message = &ngap_message;
            ogs_fsm_dispatch(&gnb->sm, e);
        } else {
            ogs_error("Cannot decode NGAP message");
            r = ngap_send_error_indication(
                    gnb, NULL, NULL, NGAP_Cause_PR_protocol, 
                    NGAP_CauseProtocol_abstract_syntax_error_falsely_constructed_message);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

        ogs_ngap_free(&ngap_message);
        ogs_pkbuf_free(pkbuf);
        break;

    case AMF_EVENT_NGAP_TIMER:
        ran_ue = ran_ue_find_by_id(e->ran_ue_id);
        if (!ran_ue) {
            ogs_error("NG Context has already been removed");
            break;
        }

        switch (e->h.timer_id) {
        case AMF_TIMER_NG_DELAYED_SEND:
            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);

            r = ngap_send_to_ran_ue(ran_ue, pkbuf);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            ogs_timer_delete(e->timer);
            break;
        case AMF_TIMER_NG_HOLDING:
            ogs_warn("Implicit NG release");
            ogs_warn("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
                  (long long)ran_ue->ran_ue_ngap_id,
                  (long long)ran_ue->amf_ue_ngap_id);
            ngap_handle_ue_context_release_action(ran_ue);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->h.timer_id), e->h.timer_id);
            break;
        }
        break;

    case AMF_EVENT_5GMM_MESSAGE:
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        ran_ue = ran_ue_find_by_id(e->ran_ue_id);
        if (!ran_ue) {
            ogs_error("NG Context has already been removed");
            break;
        }

        if (ogs_nas_5gmm_decode(&nas_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_nas_5gmm_decode() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

        amf_ue = amf_ue_find_by_id(ran_ue->amf_ue_id);
        if (!amf_ue) {
            amf_ue = amf_ue_find_by_message(&nas_message);
            if (!amf_ue) {
                amf_ue = amf_ue_add(ran_ue);
                if (amf_ue == NULL) {
                    r = ngap_send_ran_ue_context_release_command(
                            ran_ue,
                            NGAP_Cause_PR_misc,
                            NGAP_CauseMisc_control_processing_overload,
                            NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    ogs_pkbuf_free(pkbuf);
                    break;
                }

                ogs_assert(CM_IDLE(amf_ue));
            } else {
                /* Here, if the AMF_UE Context is found,
                 * the integrity check is not performed
                 * For example, REGISTRATION_REQUEST, SERVICE_REQUEST message
                 *
                 * Now, We will check the MAC in the NAS message*/
                ogs_nas_security_header_type_t h;
                h.type = e->nas.type;
                if (h.integrity_protected) {
                    /* Decryption was performed in NGAP handler.
                     * So, we disabled 'ciphered' not to decrypt NAS message */
                    h.ciphered = 0;
                    if (nas_5gs_security_decode(amf_ue, h, pkbuf) != OGS_OK) {
                        ogs_error("[%s] nas_security_decode() failed",
                                amf_ue->suci);
                        ogs_pkbuf_free(pkbuf);
                        break;
                    }
                }

                /*
                 * TS23.502
                 * 4.2.3.2 UE Triggered Service Request
                 *
                 * 4. [Conditional]
                 * AMF to SMF: Nsmf_PDUSession_UpdateSMContext Request
                 *
                 * The AMF may receive a Service Request to establish another
                 * NAS signalling connection via a NG-RAN while it has
                 * maintained an old NAS signalling connection for UE still
                 * via NG-RAN. In this case, AMF shall trigger the AN release
                 * procedure toward the old NG-RAN to release the old NAS
                 * signalling connection as defined in clause 4.2.6
                 * with following logic:
                 */

                /* If NAS(amf_ue_t) has already been associated with
                 * older NG(ran_ue_t) context */
                if (CM_CONNECTED(amf_ue)) {
    /*
     * Issue #2786
     *
     * In cases where the UE sends an Integrity Un-Protected Registration
     * Request or Service Request, there is an issue of sending
     * a UEContextReleaseCommand for the OLD RAN Context.
     *
     * For example, if the UE switchs off and power-on after
     * the first connection, the 5G Core sends a UEContextReleaseCommand.
     *
     * However, since there is no RAN context for this on the gNB,
     * the gNB does not send a UEContextReleaseComplete,
     * so the deletion of the RAN Context does not function properly.
     *
     * To solve this problem, the 5G Core has been modified to implicitly
     * delete the RAN Context instead of sending a UEContextReleaseCommand.
     */
                    HOLDING_NG_CONTEXT(amf_ue);
                }
            }
            amf_ue_associate_ran_ue(amf_ue, ran_ue);

            /*
             * TS 24.501
             * 5.3.7 Handling of the periodic registration update timer
             *
             * The mobile reachable timer shall be stopped
             * when a NAS signalling connection is established for the UE.
             * The implicit de-registration timer shall be stopped
             * when a NAS signalling connection is established for the UE.
             */
            CLEAR_AMF_UE_TIMER(amf_ue->mobile_reachable);
            CLEAR_AMF_UE_TIMER(amf_ue->implicit_deregistration);
        }

        ogs_assert(amf_ue);
        ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

        e->amf_ue_id = amf_ue->id;
        e->nas.message = &nas_message;

        ogs_fsm_dispatch(&amf_ue->sm, e);

        ogs_pkbuf_free(pkbuf);
        break;

    case AMF_EVENT_5GMM_TIMER:
        amf_ue = amf_ue_find_by_id(e->amf_ue_id);
        if (!amf_ue) {
            ogs_error("UE(amf_ue) Context has already been removed");
            break;
        }

        ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

        ogs_fsm_dispatch(&amf_ue->sm, e);
        break;

    default:
        ogs_error("No handler for event %s", amf_event_get_name(e));
        break;
    }
}
