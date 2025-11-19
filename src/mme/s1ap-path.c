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

#include "ogs-sctp.h"

#include "mme-event.h"
#include "mme-timer.h"

#include "nas-security.h"
#include "nas-path.h"

#include "s1ap-build.h"
#include "s1ap-path.h"

int s1ap_open(void)
{
    ogs_socknode_t *node = NULL;

    ogs_list_for_each(&mme_self()->s1ap_list, node)
        if (s1ap_server(node) == NULL) return OGS_ERROR;

    ogs_list_for_each(&mme_self()->s1ap_list6, node)
        if (s1ap_server(node) == NULL) return OGS_ERROR;

    return OGS_OK;
}

void s1ap_close(void)
{
    ogs_socknode_remove_all(&mme_self()->s1ap_list);
    ogs_socknode_remove_all(&mme_self()->s1ap_list6);
}

int s1ap_send_to_enb(mme_enb_t *enb, ogs_pkbuf_t *pkbuf, uint16_t stream_no)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(pkbuf);
    ogs_assert(enb);

    ogs_assert(enb->sctp.sock);
    if (enb->sctp.sock->fd == INVALID_SOCKET) {
        ogs_fatal("eNB SCTP socket has already been destroyed");
        ogs_log_hexdump(OGS_LOG_FATAL, pkbuf->data, pkbuf->len);
        ogs_pkbuf_free(pkbuf);
        return OGS_ERROR;
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    ogs_sctp_ppid_in_pkbuf(pkbuf) = OGS_SCTP_S1AP_PPID;
    ogs_sctp_stream_no_in_pkbuf(pkbuf) = stream_no;

    if (enb->sctp.type == SOCK_STREAM) {
        ogs_sctp_write_to_buffer(&enb->sctp, pkbuf);
        return OGS_OK;
    } else {
        return ogs_sctp_senddata(enb->sctp.sock, pkbuf, enb->sctp.addr);
    }
}

int s1ap_send_to_enb_ue(enb_ue_t *enb_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    mme_enb_t *enb = NULL;

    ogs_assert(pkbuf);

    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        ogs_pkbuf_free(pkbuf);
        return OGS_NOTFOUND;
    }

    enb = mme_enb_find_by_id(enb_ue->enb_id);
    if (!enb) {
        ogs_error("[%d] eNB has already been removed", enb_ue->enb_id);
        ogs_pkbuf_free(pkbuf);
        return OGS_NOTFOUND;
    }

    rv = s1ap_send_to_enb(enb, pkbuf, enb_ue->enb_ostream_id);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_delayed_send_to_enb_ue(
        enb_ue_t *enb_ue, ogs_pkbuf_t *pkbuf, ogs_time_t duration)
{
    ogs_assert(enb_ue);
    ogs_assert(pkbuf);

    if (duration) {
        mme_event_t *e = NULL;

        e = mme_event_new(MME_EVENT_S1AP_TIMER);
        ogs_assert(e);
        e->timer = ogs_timer_add(
                ogs_app()->timer_mgr, mme_timer_s1_delayed_send, e);
        ogs_assert(e->timer);
        e->pkbuf = pkbuf;
        e->enb_ue_id = enb_ue->id;

        ogs_timer_start(e->timer, duration);

        return OGS_OK;
    } else {
        int rv = s1ap_send_to_enb_ue(enb_ue, pkbuf);
        ogs_expect(rv == OGS_OK);

        return rv;
    }
}

int s1ap_send_to_esm(
        mme_ue_t *mme_ue, ogs_pkbuf_t *esmbuf,
        uint8_t nas_type, int create_action)
{
    int rv;
    mme_event_t *e = NULL;

    ogs_assert(mme_ue);
    ogs_assert(esmbuf);

    e = mme_event_new(MME_EVENT_ESM_MESSAGE);
    ogs_assert(e);
    e->mme_ue_id = mme_ue->id;
    e->pkbuf = esmbuf;
    e->nas_type = nas_type;
    e->create_action = create_action;
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        mme_event_free(e);
    }

    return rv;
}

int s1ap_send_to_nas(enb_ue_t *enb_ue,
        S1AP_ProcedureCode_t procedureCode, S1AP_NAS_PDU_t *nasPdu)
{
    int rv;

    mme_ue_t *mme_ue = NULL;

    ogs_nas_eps_security_header_t *sh = NULL;
    ogs_nas_security_header_type_t security_header_type;

    ogs_nas_emm_header_t *h = NULL;
    ogs_pkbuf_t *nasbuf = NULL;
    mme_event_t *e = NULL;

    ogs_assert(enb_ue);
    ogs_assert(nasPdu);

    if (nasPdu->size == 0) {
        ogs_error("Empty NAS PDU");
        enb_ue_remove(enb_ue);
        return OGS_ERROR;
    }

    mme_ue = mme_ue_find_by_id(enb_ue->mme_ue_id);

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM.
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    nasbuf = ogs_pkbuf_alloc(NULL, OGS_NAS_HEADROOM+nasPdu->size);
    ogs_assert(nasbuf);
    ogs_pkbuf_reserve(nasbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(nasbuf, nasPdu->buf, nasPdu->size);

    sh = (ogs_nas_eps_security_header_t *)nasbuf->data;
    ogs_assert(sh);

    memset(&security_header_type, 0, sizeof(ogs_nas_security_header_type_t));
    switch(sh->security_header_type) {
    case OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
        break;
    case OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE:
        security_header_type.service_request = 1;
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
        security_header_type.integrity_protected = 1;
        ogs_pkbuf_pull(nasbuf, 6);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED:
        security_header_type.integrity_protected = 1;
        security_header_type.ciphered = 1;
        ogs_pkbuf_pull(nasbuf, 6);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
        security_header_type.integrity_protected = 1;
        security_header_type.new_security_context = 1;
        ogs_pkbuf_pull(nasbuf, 6);
        break;
    case OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT:
        security_header_type.integrity_protected = 1;
        security_header_type.ciphered = 1;
        security_header_type.new_security_context = 1;
        ogs_pkbuf_pull(nasbuf, 6);
        break;
    default:
        ogs_error("Not implemented(security header type:0x%x)",
                sh->security_header_type);
        enb_ue_remove(enb_ue);
        return OGS_ERROR;
    }

    if (mme_ue) {
        if (nas_eps_security_decode(mme_ue,
                security_header_type, nasbuf) != OGS_OK) {
            ogs_error("nas_eps_security_decode failed()");
            enb_ue_remove(enb_ue);
            return OGS_ERROR;
        }
    }

    h = (ogs_nas_emm_header_t *)nasbuf->data;
    ogs_assert(h);

    if (procedureCode == S1AP_ProcedureCode_id_initialUEMessage) {
        if (h->protocol_discriminator != OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM) {

            ogs_error("Invalid protocol_discriminator [%d]",
                    h->protocol_discriminator);

            ogs_pkbuf_free(nasbuf);
            enb_ue_remove(enb_ue);

            return OGS_ERROR;
        }

        if (h->security_header_type !=
                OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE &&
            h->message_type != OGS_NAS_EPS_ATTACH_REQUEST &&
            h->message_type != OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST &&
            h->message_type != OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST &&
            h->message_type != OGS_NAS_EPS_DETACH_REQUEST) {

            ogs_error("Invalid EMM message type [%d]", h->message_type);

            ogs_pkbuf_free(nasbuf);
            enb_ue_remove(enb_ue);

            return OGS_ERROR;
        }
    }

    if (h->protocol_discriminator == OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM) {
        int rv;
        e = mme_event_new(MME_EVENT_EMM_MESSAGE);
        if (!e) {
            ogs_error("s1ap_send_to_nas() failed");
            ogs_pkbuf_free(nasbuf);
            return OGS_ERROR;
        }
        e->enb_ue_id = enb_ue->id;
        e->s1ap_code = procedureCode;
        e->nas_type = security_header_type.type;
        e->pkbuf = nasbuf;
        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_error("s1ap_send_to_nas() failed:%d", (int)rv);
            ogs_pkbuf_free(e->pkbuf);
            mme_event_free(e);
        }
        return rv;
    } else if (h->protocol_discriminator ==
            OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM) {
        if (!mme_ue) {
            ogs_error("No UE Context");
            ogs_pkbuf_free(nasbuf);
            return OGS_ERROR;
        }
        rv = s1ap_send_to_esm(
                mme_ue, nasbuf, security_header_type.type,
                OGS_GTP_CREATE_IN_UPLINK_NAS_TRANSPORT);
        ogs_expect(rv == OGS_OK);
        return rv;
    } else {
        ogs_error("Unknown/Unimplemented NAS Protocol discriminator 0x%02x",
                  h->protocol_discriminator);

        ogs_pkbuf_free(nasbuf);
        enb_ue_remove(enb_ue);

        return OGS_ERROR;
    }
}

int s1ap_send_s1_setup_response(mme_enb_t *enb)
{
    int rv;
    ogs_pkbuf_t *s1ap_buffer;

    ogs_assert(enb);

    ogs_debug("S1-Setup response");

    s1ap_buffer = s1ap_build_setup_rsp();
    if (!s1ap_buffer) {
        ogs_error("s1ap_build_setup_rsp() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(enb, s1ap_buffer, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_s1_setup_failure(
        mme_enb_t *enb, S1AP_Cause_PR group, long cause)
{
    int rv;
    ogs_pkbuf_t *s1ap_buffer;

    ogs_assert(enb);

    ogs_debug("S1-Setup failure");

    s1ap_buffer = s1ap_build_setup_failure(group, cause, S1AP_TimeToWait_v10s);
    if (!s1ap_buffer) {
        ogs_error("s1ap_build_setup_failure() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(enb, s1ap_buffer, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_enb_configuration_update_ack(mme_enb_t *enb)
{
    int rv;
    ogs_pkbuf_t *s1ap_buffer;

    ogs_assert(enb);

    ogs_debug("ENBConfigurationUpdateAcknowledge");

    s1ap_buffer = s1ap_build_enb_configuration_update_ack();
    if (!s1ap_buffer) {
        ogs_error("s1ap_build_enb_configuration_update_ack() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(enb, s1ap_buffer, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_enb_configuration_update_failure(
        mme_enb_t *enb, S1AP_Cause_PR group, long cause)
{
    int rv;
    ogs_pkbuf_t *s1ap_buffer;

    ogs_assert(enb);

    ogs_debug("ENBConfigurationUpdateFailure");

    s1ap_buffer = s1ap_build_enb_configuration_update_failure(
            group, cause, S1AP_TimeToWait_v10s);
    if (!s1ap_buffer) {
        ogs_error("s1ap_build_enb_configuration_update_failure() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(enb, s1ap_buffer, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_initial_context_setup_request(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("InitialContextSetupRequest");

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!enb_ue_find_by_id(mme_ue->enb_ue_id)) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_initial_context_setup_request(mme_ue, NULL);
    if (!s1apbuf) {
        ogs_error("s1ap_build_initial_context_setup_request() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_ue_context_modification_request(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("UEContextModificationRequest");

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!enb_ue_find_by_id(mme_ue->enb_ue_id)) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_ue_context_modification_request(mme_ue);
    if (!s1apbuf) {
        ogs_error("s1ap_build_ue_context_modification_request() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_ue_context_release_command(
    enb_ue_t *enb_ue, S1AP_Cause_PR group, long cause,
    uint8_t action, ogs_time_t duration)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("UEContextReleaseCommand");

    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    ogs_assert(action != S1AP_UE_CTX_REL_INVALID_ACTION);
    enb_ue->ue_ctx_rel_action = action;

    ogs_debug("    Group[%d] Cause[%d] Action[%d] Duration[%d]",
            group, (int)cause, action, (int)duration);

    s1apbuf = s1ap_build_ue_context_release_command(enb_ue, group, cause);
    if (!s1apbuf) {
        ogs_error("s1ap_build_ue_context_release_command() failed");
        return OGS_ERROR;
    }

    rv = s1ap_delayed_send_to_enb_ue(enb_ue, s1apbuf, duration);
    ogs_expect(rv == OGS_OK);

    ogs_timer_start(enb_ue->t_s1_holding,
            mme_timer_cfg(MME_TIMER_S1_HOLDING)->duration);

    return rv;
}

int s1ap_send_paging(mme_ue_t *mme_ue, S1AP_CNDomain_t cn_domain)
{
    ogs_pkbuf_t *s1apbuf = NULL;
    mme_enb_t *enb = NULL;
    int i;
    int rv;

    ogs_debug("S1-Paging");

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    /* Find enB with matched TAI */
    ogs_list_for_each(&mme_self()->enb_list, enb) {
        for (i = 0; i < enb->num_of_supported_ta_list; i++) {

            if (memcmp(&enb->supported_ta_list[i], &mme_ue->tai,
                        sizeof(ogs_eps_tai_t)) == 0) {

                if (mme_ue->t3413.pkbuf) {
                    s1apbuf = mme_ue->t3413.pkbuf;
                } else {
                    s1apbuf = s1ap_build_paging(mme_ue, cn_domain);
                    if (!s1apbuf) {
                        ogs_error("s1ap_build_paging() failed");
                        return OGS_ERROR;
                    }
                }

                mme_ue->t3413.pkbuf = ogs_pkbuf_copy(s1apbuf);
                if (!mme_ue->t3413.pkbuf) {
                    ogs_error("ogs_pkbuf_copy() failed");
                    ogs_pkbuf_free(s1apbuf);
                    return OGS_ERROR;
                }

                rv = s1ap_send_to_enb(enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
                if (rv != OGS_OK) {
                    ogs_error("s1ap_send_to_enb() failed");
                    return rv;
                }
            }
        }
    }

    /* Start T3413 */
    ogs_timer_start(mme_ue->t3413.timer,
            mme_timer_cfg(MME_TIMER_T3413)->duration);

    return OGS_OK;
}

int s1ap_send_mme_configuration_transfer(
        mme_enb_t *target_enb,
        S1AP_SONConfigurationTransfer_t *SONConfigurationTransfer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_assert(target_enb);
    ogs_assert(SONConfigurationTransfer);

    ogs_debug("MMEConfigurationTransfer");

    s1apbuf = s1ap_build_mme_configuration_transfer(SONConfigurationTransfer);
    if (!s1apbuf) {
        ogs_error("s1ap_build_mme_configuration_transfer() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(target_enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_mme_direct_information_transfer(
        mme_enb_t *target_enb, const uint8_t *buf, size_t buf_len)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_assert(target_enb);

    ogs_debug("Tx MME Direct Information Transfer");

    s1apbuf = s1ap_build_direct_information_transfer(buf, buf_len);
    if (!s1apbuf) {
        ogs_error("s1ap_build_direct_information_transfer() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(target_enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_e_rab_modification_confirm(mme_ue_t *mme_ue)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("E-RABModificationConfirm");

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!enb_ue_find_by_id(mme_ue->enb_ue_id)) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_e_rab_modification_confirm(mme_ue);
    if (!s1apbuf) {
        ogs_error("s1ap_build_e_rab_modification_confirm() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_path_switch_ack(
        mme_ue_t *mme_ue, bool e_rab_to_switched_in_uplink_list)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("PathSwitchAcknowledge");

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!enb_ue_find_by_id(mme_ue->enb_ue_id)) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_path_switch_ack(
                mme_ue, e_rab_to_switched_in_uplink_list);
    if (!s1apbuf) {
        ogs_error("s1ap_build_path_switch_ack() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_handover_command(enb_ue_t *source_ue)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("HandoverCommand");

    if (!source_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!mme_ue_find_by_id(source_ue->mme_ue_id)) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_handover_command(source_ue);
    if (!s1apbuf) {
        ogs_error("s1ap_build_handover_command() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb_ue(source_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_handover_preparation_failure(
        enb_ue_t *source_ue, S1AP_Cause_PR group, long cause)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("HandoverPreparationFailure");

    if (!source_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_assert(group);

    s1apbuf = s1ap_build_handover_preparation_failure(source_ue, group, cause);
    if (!s1apbuf) {
        ogs_error("s1ap_build_handover_preparation_failure() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb_ue(source_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_handover_cancel_ack(enb_ue_t *source_ue)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_debug("HandoverCancelAcknowledge");

    if (!source_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_handover_cancel_ack(source_ue);
    if (!s1apbuf) {
        ogs_error("s1ap_build_handover_cancel_ack() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb_ue(source_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}


int s1ap_send_handover_request(
        enb_ue_t *source_ue, mme_enb_t *target_enb,
        S1AP_HandoverType_t *handovertype, S1AP_Cause_t *cause,
        S1AP_Source_ToTarget_TransparentContainer_t
            *source_totarget_transparentContainer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    enb_ue_t *target_ue = NULL;

    ogs_assert(target_enb);

    ogs_info("HandoverRequest");

    if (!source_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!mme_ue_find_by_id(source_ue->mme_ue_id)) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    target_ue = enb_ue_find_by_id(source_ue->target_ue_id);
    if (target_ue) {
    /*
     * Issue #3014
     *
     * 1. HandoverRequired
     * 2. HandoverRequest
     * 3. HandoverFailure
     * 4. UEContextReleaseCommand
     * 5. HandoverPreparationFailure
     *
     * If UEContextReleaseComplete is not received,
     * the Source-UE will have the Target-UE.
     *
     * 6. HandoverRequired
     *
     * There may be cases where the Source UE has a Target UE
     * from a previous HandoverRequired process. In this case,
     * it is recommended to force the deletion of the Target UE information
     * when receiving a new HandoverRequired.
     *
     * 7. HandoverRequest
     * 8. HandoverFailure
     * 9. UEContextReleaseCommand
     * 10. UEContextReleaseComplete
     * 11. HandoverPreparationFailure
     *
     * ... Crashed ...
     */
        ogs_warn("DELETE the previously used TARGET in SOURCE");
        ogs_warn("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
        ogs_warn("    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);
        enb_ue_source_deassociate_target(target_ue);
        enb_ue_remove(target_ue);
    }

    target_ue = enb_ue_add(target_enb, INVALID_UE_S1AP_ID);
    if (target_ue == NULL) {
        rv = s1ap_send_error_indication(target_enb, NULL, NULL,
                S1AP_Cause_PR_misc,
                S1AP_CauseMisc_control_processing_overload);
        ogs_expect(rv == OGS_OK);

        return rv;
    }

    ogs_info("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    ogs_info("    Target : ENB_UE_S1AP_ID[Unknown] MME_UE_S1AP_ID[%d]",
            target_ue->mme_ue_s1ap_id);

    enb_ue_source_associate_target(source_ue, target_ue);

    s1apbuf = s1ap_build_handover_request(
            target_ue, handovertype, cause,
            source_totarget_transparentContainer);
    if (!s1apbuf) {
        ogs_error("s1ap_build_handover_request() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb_ue(target_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_mme_status_transfer(
        enb_ue_t *target_ue,
        S1AP_ENB_StatusTransfer_TransparentContainer_t
            *enb_statustransfer_transparentContainer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_info("MMEStatusTransfer");

    if (!target_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_mme_status_transfer(target_ue,
            enb_statustransfer_transparentContainer);
    if (!s1apbuf) {
        ogs_error("s1ap_build_mme_status_transfer() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb_ue(target_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_error_indication(
        mme_enb_t *enb,
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id,
        S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id,
        S1AP_Cause_PR group, long cause)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_assert(enb);

    ogs_info("ErrorIndication");

    s1apbuf = ogs_s1ap_build_error_indication(
            mme_ue_s1ap_id, enb_ue_s1ap_id, group, cause);
    if (!s1apbuf) {
        ogs_error("ogs_s1ap_build_error_indication() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_error_indication2(
        mme_ue_t *mme_ue, S1AP_Cause_PR group, long cause)
{
    int rv;
    mme_enb_t *enb;
    enb_ue_t *enb_ue;

    S1AP_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1AP_ENB_UE_S1AP_ID_t enb_ue_s1ap_id;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    enb = mme_enb_find_by_id(enb_ue->enb_id);
    if (!enb) {
        ogs_error("eNB has already been removed");
        return OGS_NOTFOUND;
    }

    mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id,
    enb_ue_s1ap_id = enb_ue->enb_ue_s1ap_id,

    rv = s1ap_send_error_indication(
        enb, &mme_ue_s1ap_id, &enb_ue_s1ap_id, group, cause);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int s1ap_send_s1_reset_ack(
        mme_enb_t *enb,
        S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_assert(enb);

    ogs_info("S1-Reset Acknowledge");
    s1apbuf = ogs_s1ap_build_s1_reset_ack(partOfS1_Interface);
    if (!s1apbuf) {
        ogs_error("ogs_s1ap_build_s1_reset_ack() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb(enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
    ogs_expect(rv == OGS_OK);

    return rv;
}
