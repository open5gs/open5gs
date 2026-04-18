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
#include "s1ap-path.h"

#if !HAVE_USRSCTP
static void s1ap_handle_send_failed(mme_enb_t *enb,
        const uint8_t *data, size_t data_len, uint32_t error);
#endif

#if HAVE_USRSCTP
static void usrsctp_recv_handler(struct socket *socket, void *data, int flags);
#else
static void lksctp_accept_handler(short when, ogs_socket_t fd, void *data);
#endif

void s1ap_accept_handler(ogs_sock_t *sock);
void s1ap_recv_handler(ogs_sock_t *sock);

ogs_sock_t *s1ap_server(ogs_socknode_t *node)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;
#if !HAVE_USRSCTP
    ogs_poll_t *poll = NULL;
#endif

    ogs_assert(node);

#if HAVE_USRSCTP
    sock = ogs_sctp_server(SOCK_SEQPACKET, node->addr, node->option);
    if (!sock) return NULL;
    usrsctp_set_non_blocking((struct socket *)sock, 1);
    usrsctp_set_upcall((struct socket *)sock, usrsctp_recv_handler, NULL);
#else
    sock = ogs_sctp_server(SOCK_STREAM, node->addr, node->option);
    if (!sock) return NULL;
    poll = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, sock->fd, lksctp_accept_handler, sock);
    ogs_assert(poll);

    node->poll = poll;
#endif

    node->sock = sock;
    node->cleanup = ogs_sctp_destroy;

    ogs_info("s1ap_server() [%s]:%d",
            OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));

    return sock;
}

void s1ap_recv_upcall(short when, ogs_socket_t fd, void *data)
{
    ogs_sock_t *sock = NULL;

    ogs_assert(fd != INVALID_SOCKET);
    sock = data;
    ogs_assert(sock);

    s1ap_recv_handler(sock);
}

#if HAVE_USRSCTP
static void usrsctp_recv_handler(struct socket *socket, void *data, int flags)
{
    int events;

    while ((events = usrsctp_get_events(socket)) &&
           (events & SCTP_EVENT_READ)) {
        s1ap_recv_handler((ogs_sock_t *)socket);
    }
}
#else
static void lksctp_accept_handler(short when, ogs_socket_t fd, void *data)
{
    ogs_assert(data);
    ogs_assert(fd != INVALID_SOCKET);

    s1ap_accept_handler(data);
}
#endif

void s1ap_accept_handler(ogs_sock_t *sock)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *new = NULL;

    ogs_assert(sock);

    new = ogs_sock_accept(sock);
    if (new) {
        ogs_sockaddr_t *addr = NULL;

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &new->remote_addr, sizeof(ogs_sockaddr_t));

        ogs_info("eNB-S1 accepted[%s]:%d in s1_path module", 
            OGS_ADDR(addr, buf), OGS_PORT(addr));

        s1ap_event_push(MME_EVENT_S1AP_LO_ACCEPT, new, addr, NULL, 0, 0);
    } else {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "accept() failed");
    }
}

void s1ap_recv_handler(ogs_sock_t *sock)
{
    ogs_pkbuf_t *pkbuf;
    int size;
    ogs_sockaddr_t *addr = NULL;
    ogs_sockaddr_t from;
    ogs_sctp_info_t sinfo;
    int flags = 0;

    ogs_assert(sock);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);
    size = ogs_sctp_recvmsg(
            sock, pkbuf->data, pkbuf->len, &from, &sinfo, &flags);
    if (size < 0 || size >= OGS_MAX_SDU_LEN) {
        ogs_error("ogs_sctp_recvmsg(%d) failed(%d:%s)",
                size, errno, strerror(errno));
        ogs_pkbuf_free(pkbuf);
        return;
    }

    if (flags & MSG_NOTIFICATION) {
        union sctp_notification *not =
            (union sctp_notification *)pkbuf->data;

        switch(not->sn_header.sn_type) {
        case SCTP_ASSOC_CHANGE :
            ogs_debug("SCTP_ASSOC_CHANGE:"
                    "[T:%d, F:0x%x, S:%d, I/O:%d/%d]", 
                    not->sn_assoc_change.sac_type,
                    not->sn_assoc_change.sac_flags,
                    not->sn_assoc_change.sac_state,
                    not->sn_assoc_change.sac_inbound_streams,
                    not->sn_assoc_change.sac_outbound_streams);

            if (not->sn_assoc_change.sac_state == SCTP_COMM_UP) {
                ogs_debug("SCTP_COMM_UP");

                if ((not->sn_assoc_change.sac_outbound_streams-1) >= 1) {
                    /* NEXT_ID(MAX >= MIN) */
                    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                    ogs_assert(addr);
                    memcpy(addr, &from, sizeof(ogs_sockaddr_t));

                    s1ap_event_push(MME_EVENT_S1AP_LO_SCTP_COMM_UP,
                            sock, addr, NULL,
                            not->sn_assoc_change.sac_inbound_streams,
                            not->sn_assoc_change.sac_outbound_streams);
                } else
                    ogs_error("Invalid sn_assoc_change.sac_outbound_streams %d",
                            not->sn_assoc_change.sac_outbound_streams);
            } else if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP ||
                    not->sn_assoc_change.sac_state == SCTP_COMM_LOST) {

                if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP)
                    ogs_debug("SCTP_SHUTDOWN_COMP");
                if (not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                    ogs_debug("SCTP_COMM_LOST");

                addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                ogs_assert(addr);
                memcpy(addr, &from, sizeof(ogs_sockaddr_t));

                s1ap_event_push(MME_EVENT_S1AP_LO_CONNREFUSED,
                        sock, addr, NULL, 0, 0);
            }
            break;

        case SCTP_SHUTDOWN_EVENT :
            ogs_debug("SCTP_SHUTDOWN_EVENT:[T:%d, F:0x%x, L:%d]",
                    not->sn_shutdown_event.sse_type,
                    not->sn_shutdown_event.sse_flags,
                    not->sn_shutdown_event.sse_length);

            addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
            ogs_assert(addr);
            memcpy(addr, &from, sizeof(ogs_sockaddr_t));

            s1ap_event_push(MME_EVENT_S1AP_LO_CONNREFUSED,
                    sock, addr, NULL, 0, 0);
            break;

        case SCTP_SEND_FAILED :
#if HAVE_USRSCTP
            ogs_error("SCTP_SEND_FAILED:[T:%d, F:0x%x, S:%d]",
                    not->sn_send_failed_event.ssfe_type,
                    not->sn_send_failed_event.ssfe_flags,
                    not->sn_send_failed_event.ssfe_error);
#else
        {
            mme_enb_t *enb;
            size_t hdr_len = sizeof(struct sctp_send_failed);
            const uint8_t *failed_payload =
                (const uint8_t *)not + hdr_len;
            ssize_t failed_len = (ssize_t)size - (ssize_t)hdr_len;

            ogs_error("SCTP_SEND_FAILED:[T:%d, F:0x%x, E:%d, L:%zd]",
                    not->sn_send_failed.ssf_type,
                    not->sn_send_failed.ssf_flags,
                    not->sn_send_failed.ssf_error,
                    failed_len);

            enb = mme_enb_find_by_addr(&from);
            if (enb && failed_len > 0)
                s1ap_handle_send_failed(enb, failed_payload,
                        (size_t)failed_len,
                        not->sn_send_failed.ssf_error);
        }
#endif
            break;

        case SCTP_PEER_ADDR_CHANGE:
            ogs_warn("SCTP_PEER_ADDR_CHANGE:[T:%d, F:0x%x, S:%d]", 
                    not->sn_paddr_change.spc_type,
                    not->sn_paddr_change.spc_flags,
                    not->sn_paddr_change.spc_error);
            break;
        case SCTP_REMOTE_ERROR:
            ogs_warn("SCTP_REMOTE_ERROR:[T:%d, F:0x%x, S:%d]", 
                    not->sn_remote_error.sre_type,
                    not->sn_remote_error.sre_flags,
                    not->sn_remote_error.sre_error);
            break;
        default :
            ogs_error("Discarding event with unknown flags:0x%x type:0x%x",
                    flags, not->sn_header.sn_type);
            break;
        }
    } else if (flags & MSG_EOR) {
        ogs_pkbuf_trim(pkbuf, size);

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &from, sizeof(ogs_sockaddr_t));

        s1ap_event_push(MME_EVENT_S1AP_MESSAGE, sock, addr, pkbuf, 0, 0);
        return;
    } else {
        ogs_error("ogs_sctp_recvmsg(%d) failed(%d:%s-0x%x)",
                size, errno, strerror(errno), flags);
    }

    ogs_pkbuf_free(pkbuf);
}

#if !HAVE_USRSCTP
/*
 * Extract MME-UE-S1AP-ID / eNB-UE-S1AP-ID from a decoded S1AP PDU by
 * dispatching on procedure code. Covers MME-originated UE-associated
 * procedures; non-UE-associated procedures (S1 Setup, Paging, MME
 * Configuration Transfer) return false.
 *
 * A macro reduces the per-procedure boilerplate: each procedure carries
 * its IEs in a distinct container/element type, but the IE iteration
 * pattern is identical — id dispatch on MME_UE_S1AP_ID / eNB_UE_S1AP_ID.
 */
#define S1AP_SCAN_UE_IDS(_ies_container, _ies_type) do { \
    int _i; \
    for (_i = 0; _i < (_ies_container).protocolIEs.list.count; _i++) { \
        _ies_type *_ie = \
            (_ies_container).protocolIEs.list.array[_i]; \
        if (!_ie) continue; \
        if (_ie->id == S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID) { \
            *mme_ue_s1ap_id = _ie->value.choice.MME_UE_S1AP_ID; \
            *have_mme = true; \
        } else if (_ie->id == S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID) { \
            *enb_ue_s1ap_id = _ie->value.choice.ENB_UE_S1AP_ID; \
            *have_enb = true; \
        } \
    } \
} while (0)

static bool s1ap_extract_ue_ids(const ogs_s1ap_message_t *message,
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id, bool *have_mme,
        S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id, bool *have_enb)
{
    *have_mme = false;
    *have_enb = false;

    switch (message->present) {
    case S1AP_S1AP_PDU_PR_initiatingMessage:
    {
        S1AP_InitiatingMessage_t *im = message->choice.initiatingMessage;
        if (!im) return false;
        switch (im->procedureCode) {
        case S1AP_ProcedureCode_id_downlinkNASTransport:
            S1AP_SCAN_UE_IDS(im->value.choice.DownlinkNASTransport,
                    S1AP_DownlinkNASTransport_IEs_t);
            return true;
        case S1AP_ProcedureCode_id_InitialContextSetup:
            S1AP_SCAN_UE_IDS(im->value.choice.InitialContextSetupRequest,
                    S1AP_InitialContextSetupRequestIEs_t);
            return true;
        case S1AP_ProcedureCode_id_UEContextModification:
            S1AP_SCAN_UE_IDS(im->value.choice.UEContextModificationRequest,
                    S1AP_UEContextModificationRequestIEs_t);
            return true;
        case S1AP_ProcedureCode_id_E_RABSetup:
            S1AP_SCAN_UE_IDS(im->value.choice.E_RABSetupRequest,
                    S1AP_E_RABSetupRequestIEs_t);
            return true;
        case S1AP_ProcedureCode_id_E_RABModify:
            S1AP_SCAN_UE_IDS(im->value.choice.E_RABModifyRequest,
                    S1AP_E_RABModifyRequestIEs_t);
            return true;
        case S1AP_ProcedureCode_id_E_RABRelease:
            S1AP_SCAN_UE_IDS(im->value.choice.E_RABReleaseCommand,
                    S1AP_E_RABReleaseCommandIEs_t);
            return true;
        case S1AP_ProcedureCode_id_MMEStatusTransfer:
            S1AP_SCAN_UE_IDS(im->value.choice.MMEStatusTransfer,
                    S1AP_MMEStatusTransferIEs_t);
            return true;
        case S1AP_ProcedureCode_id_HandoverResourceAllocation:
        {
            /* HandoverRequest carries only MME_UE_S1AP_ID (target-eNB
             * assigns its own eNB-UE-S1AP-ID in the acknowledge). */
            S1AP_HandoverRequest_t *msg = &im->value.choice.HandoverRequest;
            int i;
            for (i = 0; i < msg->protocolIEs.list.count; i++) {
                S1AP_HandoverRequestIEs_t *ie =
                    msg->protocolIEs.list.array[i];
                if (!ie) continue;
                if (ie->id == S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID) {
                    *mme_ue_s1ap_id = ie->value.choice.MME_UE_S1AP_ID;
                    *have_mme = true;
                }
            }
            return true;
        }
        case S1AP_ProcedureCode_id_UEContextRelease:
        {
            /* UEContextReleaseCommand uses UE_S1AP_IDs choice (pair or MME only). */
            S1AP_UEContextReleaseCommand_t *msg =
                &im->value.choice.UEContextReleaseCommand;
            int i;
            for (i = 0; i < msg->protocolIEs.list.count; i++) {
                S1AP_UEContextReleaseCommand_IEs_t *ie =
                    msg->protocolIEs.list.array[i];
                if (!ie) continue;
                if (ie->id != S1AP_ProtocolIE_ID_id_UE_S1AP_IDs)
                    continue;
                if (ie->value.choice.UE_S1AP_IDs.present ==
                        S1AP_UE_S1AP_IDs_PR_uE_S1AP_ID_pair) {
                    S1AP_UE_S1AP_ID_pair_t *pair =
                        ie->value.choice.UE_S1AP_IDs.choice.uE_S1AP_ID_pair;
                    if (pair) {
                        *mme_ue_s1ap_id = pair->mME_UE_S1AP_ID;
                        *have_mme = true;
                        *enb_ue_s1ap_id = pair->eNB_UE_S1AP_ID;
                        *have_enb = true;
                    }
                } else if (ie->value.choice.UE_S1AP_IDs.present ==
                        S1AP_UE_S1AP_IDs_PR_mME_UE_S1AP_ID) {
                    *mme_ue_s1ap_id =
                        ie->value.choice.UE_S1AP_IDs.choice.mME_UE_S1AP_ID;
                    *have_mme = true;
                }
            }
            return true;
        }
        default:
            return false;
        }
    }
    case S1AP_S1AP_PDU_PR_successfulOutcome:
    {
        S1AP_SuccessfulOutcome_t *so = message->choice.successfulOutcome;
        if (!so) return false;
        switch (so->procedureCode) {
        case S1AP_ProcedureCode_id_PathSwitchRequest:
            S1AP_SCAN_UE_IDS(so->value.choice.PathSwitchRequestAcknowledge,
                    S1AP_PathSwitchRequestAcknowledgeIEs_t);
            return true;
        case S1AP_ProcedureCode_id_HandoverPreparation:
            S1AP_SCAN_UE_IDS(so->value.choice.HandoverCommand,
                    S1AP_HandoverCommandIEs_t);
            return true;
        default:
            return false;
        }
    }
    case S1AP_S1AP_PDU_PR_unsuccessfulOutcome:
    {
        S1AP_UnsuccessfulOutcome_t *uo = message->choice.unsuccessfulOutcome;
        if (!uo) return false;
        switch (uo->procedureCode) {
        case S1AP_ProcedureCode_id_HandoverPreparation:
            S1AP_SCAN_UE_IDS(uo->value.choice.HandoverPreparationFailure,
                    S1AP_HandoverPreparationFailureIEs_t);
            return true;
        case S1AP_ProcedureCode_id_PathSwitchRequest:
            S1AP_SCAN_UE_IDS(uo->value.choice.PathSwitchRequestFailure,
                    S1AP_PathSwitchRequestFailureIEs_t);
            return true;
        default:
            return false;
        }
    }
    default:
        return false;
    }
}

#undef S1AP_SCAN_UE_IDS

/*
 * Handle the SCTP_SEND_FAILED notification payload.
 *
 * The kernel returns the full unsent S1AP PDU bytes in ssf_data. Decode it,
 * extract the UE identifiers, and release the MME's local S1 context for
 * that UE. This matches commercial MME behavior per the reviewer: the
 * procedure the MME was attempting has failed at the transport level, so
 * any timer/context the MME holds for the affected UE must be cleared
 * locally (the message never reached the eNB, and retransmission is futile
 * while the association is degraded).
 *
 * A per-eNB "path degraded" counter is incremented regardless of whether
 * the UE IDs could be extracted — operators can use it to detect a
 * misbehaving peer even when the failed procedure is non-UE-associated
 * (Paging, S1 Setup, MME Configuration Transfer, etc.).
 */
static void s1ap_handle_send_failed(mme_enb_t *enb,
        const uint8_t *data, size_t data_len, uint32_t error)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_s1ap_message_t message;
    S1AP_MME_UE_S1AP_ID_t mme_ue_s1ap_id = 0;
    S1AP_ENB_UE_S1AP_ID_t enb_ue_s1ap_id = 0;
    bool have_mme = false, have_enb = false;
    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(enb);
    ogs_assert(data);

    enb->sctp_send_failed_count++;
    ogs_warn("    IP[%s] ENB_ID[%d] path degraded: "
            "SCTP_SEND_FAILED count=%u error=%u",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id,
            enb->sctp_send_failed_count, error);

    pkbuf = ogs_pkbuf_alloc(NULL, data_len);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return;
    }
    ogs_pkbuf_put_data(pkbuf, data, data_len);

    memset(&message, 0, sizeof(message));
    if (ogs_s1ap_decode(&message, pkbuf) != OGS_OK) {
        ogs_warn("    Failed to decode unsent S1AP PDU (len=%zu)", data_len);
        ogs_pkbuf_free(pkbuf);
        return;
    }

    if (!s1ap_extract_ue_ids(&message,
                &mme_ue_s1ap_id, &have_mme,
                &enb_ue_s1ap_id, &have_enb)) {
        ogs_info("    SCTP_SEND_FAILED on non-UE-associated or "
                "unhandled procedure — no local release needed");
        goto out;
    }

    if (!have_mme) {
        ogs_warn("    SCTP_SEND_FAILED: no MME_UE_S1AP_ID in failed PDU");
        goto out;
    }

    if (mme_ue_s1ap_id > 0xffffffffULL) {
        ogs_warn("    SCTP_SEND_FAILED: MME_UE_S1AP_ID out of range [%lld]",
                (long long)mme_ue_s1ap_id);
        goto out;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id((uint32_t)mme_ue_s1ap_id);
    if (!enb_ue) {
        ogs_info("    SCTP_SEND_FAILED: S1 context already released "
                "(MME_UE_S1AP_ID[%lld])", (long long)mme_ue_s1ap_id);
        goto out;
    }

    ogs_warn("    Local UE Context Release: "
            "ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    /* Local release: drop the S1 context without sending any S1AP
     * message toward the eNB. The MME-UE context (NAS, bearers) is
     * preserved so the UE can re-attach cleanly on a healthy association.
     * Matches S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK semantics. */
    mme_ue = mme_ue_find_by_id(enb_ue->mme_ue_id);
    if (mme_ue)
        enb_ue_deassociate_mme_ue(enb_ue, mme_ue);
    enb_ue_remove(enb_ue);

out:
    ogs_s1ap_free(&message);
    ogs_pkbuf_free(pkbuf);
}
#endif /* !HAVE_USRSCTP */
