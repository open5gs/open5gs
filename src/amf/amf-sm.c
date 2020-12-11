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

#include "sbi-path.h"
#include "ngap-path.h"
#include "nas-path.h"
#include "ngap-handler.h"
#include "nnrf-handler.h"
#include "namf-handler.h"
#include "nsmf-handler.h"
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
    int rv;
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
    ogs_sbi_xact_t *sbi_xact = NULL;
    int state = AMF_UPDATE_SM_CONTEXT_NO_STATE;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *sbi_request = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_t *subscription = NULL;
    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t sbi_message;

    amf_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        rv = amf_sbi_open();
        if (rv != OGS_OK) {
            ogs_fatal("Can't establish SBI path");
        }

        rv = ngap_open();
        if (rv != OGS_OK) {
            ogs_error("Can't establish NGAP path");
            break;
        }

        break;

    case OGS_FSM_EXIT_SIG:
        ngap_close();
        amf_sbi_close();
        break;

    case AMF_EVT_SBI_SERVER:
        sbi_request = e->sbi.request;
        ogs_assert(sbi_request);
        stream = e->sbi.data;
        ogs_assert(stream);

        rv = ogs_sbi_parse_request(&sbi_message, sbi_request);
        if (rv != OGS_OK) {
            /* 'sbi_message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP sbi_message");
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP sbi_message", NULL);
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
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &sbi_message, "Not supported version", NULL);
            ogs_sbi_message_free(&sbi_message);
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    amf_nnrf_handle_nf_status_notify(stream, &sbi_message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, &sbi_message,
                            "Invalid HTTP method", sbi_message.h.method);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[0]);
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
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                                "No N1N2MessageTransferReqData", NULL);
                        }
                        break;

                    DEFAULT
                        ogs_error("Invalid HTTP method [%s]",
                                sbi_message.h.method);
                        ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_FORBIDDEN, &sbi_message,
                                "Invalid HTTP method", sbi_message.h.method);
                    END
                    break;

                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                            sbi_message.h.resource.component[2]);
                    ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                            "Invalid resource name",
                            sbi_message.h.resource.component[2]);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_CALLBACK)
            SWITCH(sbi_message.h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS)
                amf_namf_callback_handle_sm_context_status(
                        stream, &sbi_message);
                break;

            /* TODO */
#if 0
            CASE(OGS_SBI_RESOURCE_NAME_DEREG_NOTIFY)
                break;
            CASE(OGS_SBI_RESOURCE_NAME_AM_POLICY_NOTIFY)
                break;
#endif

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[1]);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[1]);
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message.h.service.name);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                    "Invalid API name", sbi_message.h.resource.component[0]);
        END

        /* In lib/sbi/server.c, notify_completed() releases 'request' buffer. */
        ogs_sbi_message_free(&sbi_message);
        break;

    case AMF_EVT_SBI_CLIENT:
        ogs_assert(e);

        sbi_response = e->sbi.response;
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
                nf_instance = e->sbi.data;
                ogs_assert(nf_instance);
                ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

                e->sbi.message = &sbi_message;
                ogs_fsm_dispatch(&nf_instance->sm, e);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                subscription = e->sbi.data;
                ogs_assert(subscription);

                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_CREATED ||
                        sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        amf_nnrf_handle_nf_status_subscribe(
                                subscription, &sbi_message);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription->id, sbi_message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    if (sbi_message.res_status ==
                            OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_sbi_subscription_remove(subscription);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription->id, sbi_message.res_status);
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
                sbi_xact = e->sbi.data;
                ogs_assert(sbi_xact);

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
            sbi_xact = e->sbi.data;
            ogs_assert(sbi_xact);

            amf_ue = (amf_ue_t *)sbi_xact->sbi_object;
            ogs_assert(amf_ue);
            amf_ue = amf_ue_cycle(amf_ue);
            ogs_assert(amf_ue);

            ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

            e->amf_ue = amf_ue;
            e->sbi.message = &sbi_message;;

            ogs_sbi_xact_remove(sbi_xact);

            ogs_fsm_dispatch(&amf_ue->sm, e);
            break;

        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            sbi_xact = e->sbi.data;
            ogs_assert(sbi_xact);

            sess = (amf_sess_t *)sbi_xact->sbi_object;
            ogs_assert(sess);
            sess = amf_sess_cycle(sess);

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
             * IF THIS HAPPENS IN THE REAL WORLD,
             * I WILL MODIFY THE ASSERTS BELOW.
             */
            ogs_assert(sess);

            amf_ue = sess->amf_ue;
            ogs_assert(amf_ue);
            amf_ue = amf_ue_cycle(amf_ue);
            ogs_assert(amf_ue);

            ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

            e->amf_ue = amf_ue;
            e->sess = sess;
            e->sbi.message = &sbi_message;;

            state = sbi_xact->state;

            ogs_sbi_xact_remove(sbi_xact);

            SWITCH(sbi_message.h.resource.component[2])
            CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                amf_nsmf_pdu_session_handle_update_sm_context(
                        sess, state, &sbi_message);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT ||
                    sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                    ogs_info("[%s:%d] Release SM context [%d]",
                            amf_ue->supi, sess->psi, sbi_message.res_status);
                } else {
                    ogs_error("[%s] HTTP response error [%d]",
                            amf_ue->supi, sbi_message.res_status);
                }

                amf_nsmf_pdu_session_handle_release_sm_context(sess, state);
                break;

            DEFAULT
                amf_nsmf_pdu_session_handle_create_sm_context(
                        sess, &sbi_message);
            END
            break;

        DEFAULT
            ogs_error("Invalid service name [%s]", sbi_message.h.service.name);
            ogs_assert_if_reached();
        END

        ogs_sbi_message_free(&sbi_message);
        ogs_sbi_response_free(sbi_response);
        break;

    case AMF_EVT_SBI_TIMER:
        ogs_assert(e);

        switch(e->timer_id) {
        case AMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case AMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case AMF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case AMF_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, amf_nf_state_exception))
                ogs_error("[%s] State machine exception [%d]",
                        nf_instance->id, e->timer_id);
            break;

        case AMF_TIMER_SUBSCRIPTION_VALIDITY:
            subscription = e->sbi.data;
            ogs_assert(subscription);

            ogs_info("[%s] Subscription validity expired", subscription->id);
            ogs_sbi_subscription_remove(subscription);

            ogs_nnrf_nfm_send_nf_status_subscribe(subscription->client,
                    amf_self()->nf_type, subscription->nf_instance_id);
            break;

        case AMF_TIMER_SBI_CLIENT_WAIT:
            sbi_xact = e->sbi.data;
            ogs_assert(sbi_xact);

            sbi_object = sbi_xact->sbi_object;
            ogs_assert(sbi_object);

            ogs_assert(sbi_object->type > OGS_SBI_OBJ_BASE &&
                        sbi_object->type < OGS_SBI_OBJ_TOP);

            switch(sbi_object->type) {
            case OGS_SBI_OBJ_UE_TYPE:
                amf_ue = (amf_ue_t *)sbi_object;
                ogs_assert(amf_ue);
                ogs_error("[%s] Cannot receive SBI message", amf_ue->suci);
                nas_5gs_send_gmm_reject_from_sbi(amf_ue,
                        OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
                break;

            case OGS_SBI_OBJ_SESS_TYPE:
                sess = (amf_sess_t *)sbi_object;
                ogs_assert(sess);
                ogs_error("[%d:%d] Cannot receive SBI message",
                        sess->psi, sess->pti);
                if (sess->payload_container_type) {
                    nas_5gs_send_back_5gsm_message_from_sbi(sess,
                            OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
                } else {
                    ngap_send_error_indication2(amf_ue,
                            NGAP_Cause_PR_transport,
                            NGAP_CauseTransport_transport_resource_unavailable);
                }
                break;

            default:
                ogs_fatal("Not implemented [%s:%d]",
                    OpenAPI_nf_type_ToString(sbi_xact->target_nf_type),
                    sbi_object->type);
                ogs_assert_if_reached();
            }

            ogs_sbi_xact_remove(sbi_xact);
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    case AMF_EVT_NGAP_LO_ACCEPT:
        sock = e->ngap.sock;
        ogs_assert(sock);
        addr = e->ngap.addr;
        ogs_assert(addr);

        ogs_info("gNB-N1 accepted[%s] in master_sm module",
            OGS_ADDR(addr, buf));

        gnb = amf_gnb_find_by_addr(addr);
        if (!gnb) {
            gnb = amf_gnb_add(sock, addr);
            ogs_assert(gnb);
        } else {
            ogs_warn("gNB context duplicated with IP-address [%s]!!!",
                    OGS_ADDR(addr, buf));
            ogs_sock_destroy(sock);
            ogs_free(addr);
            ogs_warn("N1 Socket Closed");
        }

        break;

    case AMF_EVT_NGAP_LO_SCTP_COMM_UP:
        sock = e->ngap.sock;
        ogs_assert(sock);
        addr = e->ngap.addr;
        ogs_assert(addr);

        max_num_of_ostreams = e->ngap.max_num_of_ostreams;

        gnb = amf_gnb_find_by_addr(addr);
        if (!gnb) {
            gnb = amf_gnb_add(sock, addr);
            ogs_assert(gnb);
        } else {
            ogs_free(addr);
        }

        gnb->max_num_of_ostreams =
                ogs_min(max_num_of_ostreams, gnb->max_num_of_ostreams);

        ogs_debug("gNB-N1 SCTP_COMM_UP[%s] Max Num of Outbound Streams[%d]", 
            OGS_ADDR(gnb->sctp.addr, buf), gnb->max_num_of_ostreams);

        break;

    case AMF_EVT_NGAP_LO_CONNREFUSED:
        sock = e->ngap.sock;
        ogs_assert(sock);
        addr = e->ngap.addr;
        ogs_assert(addr);

        gnb = amf_gnb_find_by_addr(addr);
        if (gnb) {
            ogs_info("gNB-N1[%s] connection refused!!!", OGS_ADDR(addr, buf));
            amf_sbi_send_deactivate_all_ue_in_gnb(
                    gnb, AMF_UPDATE_SM_CONTEXT_NG_RESET);
            amf_gnb_remove(gnb);
        } else {
            ogs_warn("gNB-N1[%s] connection refused, Already Removed!",
                    OGS_ADDR(addr, buf));
        }
        ogs_free(addr);

        break;
    case AMF_EVT_NGAP_MESSAGE:
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
            e->gnb = gnb;
            e->ngap.message = &ngap_message;
            ogs_fsm_dispatch(&gnb->sm, e);
        } else {
            ogs_error("Cannot decode NGAP message");
            ngap_send_error_indication(
                    gnb, NULL, NULL, NGAP_Cause_PR_protocol, 
                    NGAP_CauseProtocol_abstract_syntax_error_falsely_constructed_message);
        }

        ogs_ngap_free(&ngap_message);
        ogs_pkbuf_free(pkbuf);
        break;

    case AMF_EVT_NGAP_TIMER:
        ran_ue = e->ran_ue;
        ogs_assert(ran_ue);

        switch (e->timer_id) {
        case AMF_TIMER_NG_DELAYED_SEND:
            gnb = e->gnb;
            ogs_assert(gnb);
            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);

            ogs_expect(OGS_OK == ngap_send_to_ran_ue(ran_ue, pkbuf));
            ogs_timer_delete(e->timer);
            break;
        case AMF_TIMER_NG_HOLDING:
            ogs_warn("Implicit NG release");
            ogs_warn("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
                  ran_ue->ran_ue_ngap_id,
                  (long long)ran_ue->amf_ue_ngap_id);
            ngap_handle_ue_context_release_action(ran_ue);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;

    case AMF_EVT_5GMM_MESSAGE:
        ran_ue = e->ran_ue;
        ogs_assert(ran_ue);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_nas_5gmm_decode(&nas_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_nas_5gmm_decode() failed");
            ogs_pkbuf_free(pkbuf);
            return;
        }

        amf_ue = ran_ue->amf_ue;
        if (!amf_ue) {
            amf_ue = amf_ue_find_by_message(&nas_message);
            if (!amf_ue) {
                amf_ue = amf_ue_add(ran_ue);
                ogs_assert(amf_ue);
            } else {
                /* Here, if the AMF_UE Context is found,
                 * the integrity check is not performed
                 * For example, REGISTRATION_REQUEST,
                 * TRACKING_AREA_UPDATE_REQUEST message
                 *
                 * Now, We will check the MAC in the NAS message*/
                ogs_nas_security_header_type_t h;
                h.type = e->nas.type;
                if (h.integrity_protected) {
                    /* Decryption was performed in NGAP handler.
                     * So, we disabled 'ciphered'
                     * not to decrypt NAS message */
                    h.ciphered = 0;
                    if (nas_5gs_security_decode(amf_ue, h, pkbuf) != OGS_OK) {
                        ogs_error("[%s] nas_security_decode() failed",
                                amf_ue->suci);
                        ogs_pkbuf_free(pkbuf);
                        return;
                    }
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
             * NAS signalling connection via a NG-RAN while it has maintained
             * an old NAS signalling connection for UE still via NG-RAN.
             * In this case, AMF shall trigger the AN release procedure toward
             * the old NG-RAN to release the old NAS signalling connection
             * as defined in clause 4.2.6 with following logic: */

            /* If NAS(amf_ue_t) has already been associated with
             * older NG(ran_ue_t) context */
            if (CM_CONNECTED(amf_ue)) {
                /* Previous NG(ran_ue_t) context the holding timer(30secs)
                 * is started.
                 * Newly associated NG(ran_ue_t) context holding timer
                 * is stopped. */
                ogs_debug("[%s] Start NG Holding Timer", amf_ue->suci);
                ogs_debug("[%s]    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
                        amf_ue->suci, amf_ue->ran_ue->ran_ue_ngap_id,
                        (long long)amf_ue->ran_ue->amf_ue_ngap_id);

                /* De-associate NG with NAS/EMM */
                ran_ue_deassociate(amf_ue->ran_ue);

                ngap_send_ran_ue_context_release_command(amf_ue->ran_ue,
                        NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                        NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
            }
            amf_ue_associate_ran_ue(amf_ue, ran_ue);
        }

        ogs_assert(amf_ue);
        ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

        e->amf_ue = amf_ue;
        e->nas.message = &nas_message;

        ogs_fsm_dispatch(&amf_ue->sm, e);

        ogs_pkbuf_free(pkbuf);
        break;
    case AMF_EVT_5GMM_TIMER:
        amf_ue = e->amf_ue;
        ogs_assert(amf_ue);
        ogs_assert(OGS_FSM_STATE(&amf_ue->sm));

        ogs_fsm_dispatch(&amf_ue->sm, e);
        break;

    default:
        ogs_error("No handler for event %s", amf_event_get_name(e));
        break;
    }
}
