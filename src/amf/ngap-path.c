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

#include "ogs-sctp.h"

#include "context.h"

#include "ngap-build.h"
#include "ngap-path.h"
#include "nas-security.h"
#include "nas-path.h"
#include "sbi-path.h"

int ngap_open(void)
{
    ogs_socknode_t *node = NULL;

    ogs_list_for_each(&amf_self()->ngap_list, node)
        if (ngap_server(node) == NULL) return OGS_ERROR;

    ogs_list_for_each(&amf_self()->ngap_list6, node)
        if (ngap_server(node) == NULL) return OGS_ERROR;

    return OGS_OK;
}

void ngap_close()
{
    ogs_socknode_remove_all(&amf_self()->ngap_list);
    ogs_socknode_remove_all(&amf_self()->ngap_list6);
}

int ngap_send_to_gnb(amf_gnb_t *gnb, ogs_pkbuf_t *pkbuf, uint16_t stream_no)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(gnb);
    ogs_assert(pkbuf);
    ogs_assert(gnb->sctp.sock);
    if (gnb->sctp.sock->fd == INVALID_SOCKET) {
        ogs_fatal("gNB SCTP socket has already been destroyed");
        ogs_log_hexdump(OGS_LOG_FATAL, pkbuf->data, pkbuf->len);
        ogs_assert_if_reached();
        return OGS_ERROR;
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    ogs_sctp_ppid_in_pkbuf(pkbuf) = OGS_SCTP_NGAP_PPID;
    ogs_sctp_stream_no_in_pkbuf(pkbuf) = stream_no;

    if (gnb->sctp.type == SOCK_STREAM) {
        ogs_sctp_write_to_buffer(&gnb->sctp, pkbuf);
        return OGS_OK;
    } else {
        return ogs_sctp_senddata(gnb->sctp.sock, pkbuf, gnb->sctp.addr);
    }
}

int ngap_send_to_ran_ue(ran_ue_t *ran_ue, ogs_pkbuf_t *pkbuf)
{
    amf_gnb_t *gnb = NULL;

    ogs_assert(ran_ue);
    gnb = ran_ue->gnb;
    ogs_assert(gnb);

    return ngap_send_to_gnb(gnb, pkbuf, ran_ue->gnb_ostream_id);
}

int ngap_delayed_send_to_ran_ue(
        ran_ue_t *ran_ue, ogs_pkbuf_t *pkbuf, ogs_time_t duration)
{
    ogs_assert(ran_ue);
    ogs_assert(pkbuf);
        
    if (duration) {
        amf_event_t *e = NULL;

        e = amf_event_new(AMF_EVT_NGAP_TIMER);
        ogs_assert(e);
        e->timer = ogs_timer_add(
                ogs_app()->timer_mgr, amf_timer_ng_delayed_send, e);
        ogs_assert(e->timer);
        e->pkbuf = pkbuf;
        e->ran_ue = ran_ue;
        e->gnb = ran_ue->gnb;

        ogs_timer_start(e->timer, duration);

        return OGS_OK;
    } else {
        amf_gnb_t *gnb = NULL;
        gnb = ran_ue->gnb;
        ogs_assert(gnb);
        return ngap_send_to_ran_ue(ran_ue, pkbuf);
    }
}

int ngap_send_to_5gsm(amf_ue_t *amf_ue, ogs_pkbuf_t *esmbuf)
{
    int rv;
    amf_event_t *e = NULL;

    ogs_assert(amf_ue);
    ogs_assert(esmbuf);

    e = amf_event_new(AMF_EVT_5GSM_MESSAGE);
    ogs_assert(e);
    e->amf_ue = amf_ue;
    e->pkbuf = esmbuf;
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        amf_event_free(e);
    }

    return rv;
}
 
int ngap_send_to_nas(ran_ue_t *ran_ue,
        NGAP_ProcedureCode_t procedureCode, NGAP_NAS_PDU_t *nasPdu)
{
    ogs_nas_5gs_security_header_t *sh = NULL;
    ogs_nas_security_header_type_t security_header_type;

    ogs_nas_5gmm_header_t *h = NULL;
    ogs_pkbuf_t *nasbuf = NULL;
    amf_event_t *e = NULL;

    ogs_assert(ran_ue);
    ogs_assert(nasPdu);

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    nasbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+nasPdu->size);
    ogs_assert(nasbuf);
    ogs_pkbuf_reserve(nasbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(nasbuf, nasPdu->buf, nasPdu->size);

    sh = (ogs_nas_5gs_security_header_t *)nasbuf->data;
    ogs_assert(sh);

    memset(&security_header_type, 0, sizeof(ogs_nas_security_header_type_t));
    switch(sh->security_header_type) {
    case OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
        security_header_type.integrity_protected = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED:
        security_header_type.integrity_protected = 1;
        security_header_type.ciphered = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
        security_header_type.integrity_protected = 1;
        security_header_type.new_security_context = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT:
        security_header_type.integrity_protected = 1;
        security_header_type.ciphered = 1;
        security_header_type.new_security_context = 1;
        ogs_pkbuf_pull(nasbuf, 7);
        break;
    default:
        ogs_error("Not implemented(security header type:0x%x)",
                sh->security_header_type);
        return OGS_ERROR;
    }

    if (ran_ue->amf_ue) {
        if (nas_5gs_security_decode(ran_ue->amf_ue,
                security_header_type, nasbuf) != OGS_OK) {
            ogs_error("nas_eps_security_decode failed()");
	        return OGS_ERROR;
        }
    }

    h = (ogs_nas_5gmm_header_t *)nasbuf->data;
    ogs_assert(h);
    if (h->extended_protocol_discriminator ==
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM) {
        int rv;
        e = amf_event_new(AMF_EVT_5GMM_MESSAGE);
        if (!e) {
            ogs_error("ngap_send_to_nas() failed");
            ogs_pkbuf_free(nasbuf);
            return OGS_ERROR;
        }
        e->ran_ue = ran_ue;
        e->ngap.code = procedureCode;
        e->nas.type = security_header_type.type;
        e->pkbuf = nasbuf;
        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_warn("ngap_send_to_nas() failed:%d", (int)rv);
            ogs_pkbuf_free(e->pkbuf);
            amf_event_free(e);
        }
        return rv;
    } else if (h->extended_protocol_discriminator ==
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM) {
        amf_ue_t *amf_ue = ran_ue->amf_ue;
        if (!amf_ue) {
            ogs_error("No UE Context");
            ogs_pkbuf_free(nasbuf);
            return OGS_ERROR;
        }
        return ngap_send_to_5gsm(amf_ue, nasbuf);
    } else {
        ogs_error("Unknown NAS Protocol discriminator 0x%02x",
                  h->extended_protocol_discriminator);
        ogs_pkbuf_free(nasbuf);
        return OGS_ERROR;
    }
}

int ngap_send_ng_setup_response(amf_gnb_t *gnb)
{
    int rv;
    ogs_pkbuf_t *ngap_buffer;

    ogs_debug("NG-Setup response");
    ngap_buffer = ngap_build_ng_setup_response();
    ogs_expect_or_return_val(ngap_buffer, OGS_ERROR);

    rv = ngap_send_to_gnb(gnb, ngap_buffer, NGAP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_ng_setup_failure(
        amf_gnb_t *gnb, NGAP_Cause_PR group, long cause)
{
    int rv;
    ogs_pkbuf_t *ngap_buffer;

    ogs_debug("NG-Setup failure");
    ngap_buffer = ngap_build_ng_setup_failure(
            group, cause, NGAP_TimeToWait_v10s);
    ogs_expect_or_return_val(ngap_buffer, OGS_ERROR);

    rv = ngap_send_to_gnb(gnb, ngap_buffer, NGAP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_ran_configuration_update_ack(amf_gnb_t *gnb)
{
    int rv;
    ogs_pkbuf_t *ngap_buffer;

    ogs_debug("RANConfigurationUpdateAcknowledge");
    ngap_buffer = ngap_build_ran_configuration_update_ack();
    ogs_expect_or_return_val(ngap_buffer, OGS_ERROR);

    rv = ngap_send_to_gnb(gnb, ngap_buffer, NGAP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_ran_configuration_update_failure(
        amf_gnb_t *gnb, NGAP_Cause_PR group, long cause)
{
    int rv;
    ogs_pkbuf_t *ngap_buffer;

    ogs_debug("RANConfigurationUpdateFailure");
    ngap_buffer = ngap_build_ran_configuration_update_failure(
            group, cause, NGAP_TimeToWait_v10s);
    ogs_expect_or_return_val(ngap_buffer, OGS_ERROR);

    rv = ngap_send_to_gnb(gnb, ngap_buffer, NGAP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_ran_ue_context_release_command(
    ran_ue_t *ran_ue, NGAP_Cause_PR group, long cause,
    uint8_t action, ogs_time_t duration)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(ran_ue);

    ogs_debug("UEContextReleaseCommand");
    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    ogs_assert(action != NGAP_UE_CTX_REL_INVALID_ACTION);
    ran_ue->ue_ctx_rel_action = action;

    ogs_debug("    Group[%d] Cause[%d] Action[%d] Duration[%d]",
            group, (int)cause, action, (int)duration);

    ngapbuf = ngap_build_ue_context_release_command(ran_ue, group, cause);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_delayed_send_to_ran_ue(ran_ue, ngapbuf, duration);
    ogs_expect(rv == OGS_OK);

    ogs_timer_start(ran_ue->t_ng_holding,
            amf_timer_cfg(AMF_TIMER_NG_HOLDING)->duration);

    return rv;
}

int ngap_send_amf_ue_context_release_command(
    amf_ue_t *amf_ue, NGAP_Cause_PR group, long cause,
    uint8_t action, ogs_time_t duration)
{
    ogs_assert(amf_ue);

    ran_ue_t *ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    if (ran_ue) {
        ogs_assert(OGS_OK ==
            ngap_send_ran_ue_context_release_command(ran_ue,
                group, cause, action, duration));
        ogs_debug("    SUPI[%s]", amf_ue->supi);
    } else {
        ogs_error("[%s] No NG Context - "
                "Group[%d] Cause[%d] Action[%d] Duration[%d]",
                amf_ue->supi, group, (int)cause, action, (int)duration);
    }

    return OGS_OK;
}

int ngap_send_paging(amf_ue_t *amf_ue)
{
    ogs_pkbuf_t *ngapbuf = NULL;
    amf_gnb_t *gnb = NULL;
    int i, j;
    int rv;

    ogs_list_for_each(&amf_self()->gnb_list, gnb) {
        for (i = 0; i < gnb->num_of_supported_ta_list; i++) {
            for (j = 0; j < gnb->supported_ta_list[i].num_of_bplmn_list; j++) {
                if (memcmp(&gnb->supported_ta_list[i].bplmn_list[j].plmn_id,
                            &amf_ue->nr_tai.plmn_id, OGS_PLMN_ID_LEN) == 0 &&
                    gnb->supported_ta_list[i].tac.v == amf_ue->nr_tai.tac.v) {

                    if (amf_ue->t3513.pkbuf) {
                        ngapbuf = amf_ue->t3513.pkbuf;
                    } else {
                        ngapbuf = ngap_build_paging(amf_ue);
                        ogs_expect_or_return_val(ngapbuf, OGS_ERROR);
                    }

                    amf_ue->t3513.pkbuf = ogs_pkbuf_copy(ngapbuf);
                    ogs_expect_or_return_val(amf_ue->t3513.pkbuf, OGS_ERROR);

                    rv = ngap_send_to_gnb(gnb, ngapbuf, NGAP_NON_UE_SIGNALLING);
                    ogs_expect_or_return_val(rv == OGS_OK, rv);
                }
            }
        }
    }

    /* Start T3513 */
    ogs_timer_start(amf_ue->t3513.timer, 
            amf_timer_cfg(AMF_TIMER_T3513)->duration);

    return OGS_OK;
}

int ngap_send_pdu_resource_setup_request(
        amf_sess_t *sess, ogs_pkbuf_t *n2smbuf)
{
    int rv;
    ran_ue_t *ran_ue = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    if (ran_ue->ue_context_requested == true &&
        ran_ue->initial_context_setup_request_sent == false) {
        ngapbuf = ngap_sess_build_initial_context_setup_request(
                sess, NULL, n2smbuf);
        ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

        ran_ue->initial_context_setup_request_sent = true;
    } else {
        ngapbuf = ngap_sess_build_pdu_session_resource_setup_request(
                sess, NULL, n2smbuf);
        ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

        rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
        ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);
    }

    return OGS_OK;
}

int ngap_send_downlink_ran_configuration_transfer(
        amf_gnb_t *target_gnb, NGAP_SONConfigurationTransfer_t *transfer)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(target_gnb);
    ogs_assert(transfer);

    ngapbuf = ngap_build_downlink_ran_configuration_transfer(transfer);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_gnb(target_gnb, ngapbuf, NGAP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_path_switch_ack(amf_sess_t *sess)
{
    int rv;

    amf_ue_t *amf_ue = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    ngapbuf = ngap_build_path_switch_ack(amf_ue);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = nas_5gs_send_to_gnb(amf_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_handover_request(amf_ue_t *amf_ue)
{
    int rv;

    ran_ue_t *source_ue = NULL, *target_ue = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(amf_ue);
    source_ue = amf_ue->ran_ue;
    ogs_assert(source_ue);

    target_ue = source_ue->target_ue;
    ogs_assert(target_ue);

    ngapbuf = ngap_build_handover_request(target_ue);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_ran_ue(target_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_handover_preparation_failure(
        ran_ue_t *source_ue, NGAP_Cause_t *cause)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(source_ue);
    ogs_assert(cause);

    ngapbuf = ngap_build_handover_preparation_failure(source_ue, cause);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_ran_ue(source_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_handover_command(amf_ue_t *amf_ue)
{
    int rv;

    ran_ue_t *source_ue = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(amf_ue);
    source_ue = amf_ue->ran_ue;
    ogs_assert(source_ue);

    ngapbuf = ngap_build_handover_command(source_ue);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_ran_ue(source_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_handover_cancel_ack(ran_ue_t *source_ue)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(source_ue);

    ngapbuf = ngap_build_handover_cancel_ack(source_ue);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_ran_ue(source_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_downlink_ran_status_transfer(
        ran_ue_t *target_ue,
        NGAP_RANStatusTransfer_TransparentContainer_t *transfer)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(target_ue);
    ogs_assert(transfer);

    ngapbuf = ngap_build_uplink_ran_status_transfer(target_ue, transfer);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_ran_ue(target_ue, ngapbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_error_indication(
        amf_gnb_t *gnb,
        uint32_t *ran_ue_ngap_id,
        uint64_t *amf_ue_ngap_id,
        NGAP_Cause_PR group, long cause)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(gnb);

    ngapbuf = ogs_ngap_build_error_indication(
            ran_ue_ngap_id, amf_ue_ngap_id, group, cause);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_gnb(gnb, ngapbuf, NGAP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_error_indication2(
        amf_ue_t *amf_ue, NGAP_Cause_PR group, long cause)
{
    int rv;
    amf_gnb_t *gnb;
    ran_ue_t *ran_ue;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_expect_or_return_val(ran_ue, OGS_ERROR);
    gnb = ran_ue->gnb;
    ogs_expect_or_return_val(gnb, OGS_ERROR);

    rv = ngap_send_error_indication(
        gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id, group, cause);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int ngap_send_ng_reset_ack(
        amf_gnb_t *gnb,
        NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface)
{
    int rv;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(gnb);

    ngapbuf = ogs_ngap_build_ng_reset_ack(partOfNG_Interface);
    ogs_expect_or_return_val(ngapbuf, OGS_ERROR);

    rv = ngap_send_to_gnb(gnb, ngapbuf, NGAP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}
