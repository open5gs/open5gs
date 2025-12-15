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

#include "mme-context.h"
#include "mme-sm.h"
#include "mme-timer.h"

#include "s1ap-handler.h"
#include "s1ap-path.h"
#include "sgsap-path.h"
#include "nas-security.h"
#include "nas-path.h"
#include "emm-handler.h"
#include "esm-handler.h"
#include "mme-gn-handler.h"
#include "mme-gtp-path.h"
#include "mme-s11-handler.h"
#include "mme-fd-path.h"
#include "mme-s6a-handler.h"
#include "mme-path.h"

void mme_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    mme_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &mme_state_operational);
}

void mme_state_final(ogs_fsm_t *s, mme_event_t *e)
{
    mme_sm_debug(e);

    ogs_assert(s);
}

void mme_state_operational(ogs_fsm_t *s, mme_event_t *e)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];

    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;
    mme_enb_t *enb = NULL;
    uint16_t max_num_of_ostreams = 0;

    ogs_s1ap_message_t s1ap_message;
    ogs_sbcap_message_t sbcap_message;
    ogs_pkbuf_t *pkbuf = NULL;
    int rc, r;

    ogs_nas_eps_message_t nas_message;
    enb_ue_t *enb_ue = NULL;
    sgw_ue_t *sgw_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    mme_bearer_t *bearer = NULL;
    mme_bearer_t *default_bearer = NULL;
    mme_sess_t *sess = NULL;

    ogs_diam_s6a_message_t *s6a_message = NULL;
    uint8_t emm_cause = 0;

    ogs_gtp_node_t *gnode = NULL;
    ogs_gtp_xact_t *xact = NULL;
    ogs_gtp2_message_t gtp_message;
    ogs_gtp1_message_t gtp1_message;

    mme_vlr_t *vlr = NULL;
    mme_sbcap_t *sbc = NULL;

    ogs_assert(e);
    mme_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case MME_EVENT_S1AP_LO_ACCEPT:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        ogs_info("eNB-S1 accepted[%s] in master_sm module",
            OGS_ADDR(addr, buf));

        enb = mme_enb_find_by_addr(addr);
        if (!enb) {
            enb = mme_enb_add(sock, addr);
            if (!enb) {
                ogs_error("mme_enb_add() failed");
                ogs_sock_destroy(sock);
                ogs_free(addr);
            }
        } else {
            ogs_warn("eNB context duplicated with IP-address [%s]!!!",
                    OGS_ADDR(addr, buf));
            ogs_sock_destroy(sock);
            ogs_free(addr);
            ogs_warn("S1 Socket Closed");
        }

        break;

    case MME_EVENT_S1AP_LO_SCTP_COMM_UP:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        max_num_of_ostreams = e->max_num_of_ostreams;

        enb = mme_enb_find_by_addr(addr);
        if (!enb) {
            enb = mme_enb_add(sock, addr);
            if (!enb) {
                ogs_error("amf_enb_add() failed");
                ogs_free(addr);
            }
        } else {
            ogs_free(addr);
        }

        if (enb->max_num_of_ostreams)
            enb->max_num_of_ostreams =
                    ogs_min(max_num_of_ostreams, enb->max_num_of_ostreams);
        else
            enb->max_num_of_ostreams = max_num_of_ostreams;

        ogs_info("eNB-S1[%s] max_num_of_ostreams : %d",
            OGS_ADDR(enb->sctp.addr, buf), enb->max_num_of_ostreams);

        break;

    case MME_EVENT_S1AP_LO_CONNREFUSED:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        enb = mme_enb_find_by_addr(addr);
        if (enb) {
            ogs_info("eNB-S1[%s] connection refused!!!", OGS_ADDR(addr, buf));
            mme_gtp_send_release_all_ue_in_enb(
                    enb, OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_LO_CONNREFUSED);
            mme_enb_remove(enb);
        } else {
            ogs_warn("eNB-S1[%s] connection refused, Already Removed!",
                    OGS_ADDR(addr, buf));
        }
        ogs_free(addr);

        break;
    case MME_EVENT_S1AP_MESSAGE:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        ogs_assert(addr->ogs_sa_family == AF_INET ||
                addr->ogs_sa_family == AF_INET6);

        enb = mme_enb_find_by_addr(addr);
        ogs_free(addr);

        ogs_assert(enb);
        ogs_assert(OGS_FSM_STATE(&enb->sm));

        rc = ogs_s1ap_decode(&s1ap_message, pkbuf);
        if (rc == OGS_OK) {
            e->enb_id = enb->id;
            e->s1ap_message = &s1ap_message;
            ogs_fsm_dispatch(&enb->sm, e);
        } else {
            ogs_warn("Cannot decode S1AP message");
            r = s1ap_send_error_indication(
                    enb, NULL, NULL, S1AP_Cause_PR_protocol,
                    S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

        ogs_s1ap_free(&s1ap_message);
        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVENT_S1AP_TIMER:
        enb_ue = enb_ue_find_by_id(e->enb_ue_id);
        if (!enb_ue) {
            ogs_error("S1 Context has already been removed");
            break;
        }

        switch (e->timer_id) {
        case MME_TIMER_S1_DELAYED_SEND:
            pkbuf = e->pkbuf;
            ogs_assert(pkbuf);

            r = s1ap_send_to_enb_ue(enb_ue, pkbuf);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            ogs_timer_delete(e->timer);
            break;
        case MME_TIMER_S1_HOLDING:
            ogs_warn("Implicit S1 release");
            ogs_warn("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                  enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
            s1ap_handle_ue_context_release_action(enb_ue);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;

    case MME_EVENT_EMM_MESSAGE:
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        enb_ue = enb_ue_find_by_id(e->enb_ue_id);
        if (!enb_ue) {
            ogs_error("S1 Context has already been removed");
            break;
        }

        if (ogs_nas_emm_decode(&nas_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_nas_emm_decode() failed");
            ogs_pkbuf_free(pkbuf);
            return;
        }

        mme_ue = mme_ue_find_by_id(enb_ue->mme_ue_id);
        if (!mme_ue) {
            mme_ue = mme_ue_find_by_message(&nas_message);
            if (!mme_ue) {
                mme_ue = mme_ue_add(enb_ue);
                if (mme_ue == NULL) {
                    r = s1ap_send_ue_context_release_command(enb_ue,
                            S1AP_Cause_PR_misc,
                            S1AP_CauseMisc_control_processing_overload,
                            S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE, 0);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    ogs_pkbuf_free(pkbuf);
                    return;
                }

                ogs_assert(ECM_IDLE(mme_ue));
            } else {
                /* Here, if the MME_UE Context is found,
                 * the integrity check is not performed
                 * For example, ATTACH_REQUEST,
                 * TRACKING_AREA_UPDATE_REQUEST message
                 *
                 * Now, We will check the MAC in the NAS message*/
                ogs_nas_security_header_type_t h;
                h.type = e->nas_type;
                if (h.integrity_protected) {
                    /* Decryption was performed in S1AP handler.
                     * So, we disabled 'ciphered'
                     * not to decrypt NAS message */
                    h.ciphered = 0;
                    if (nas_eps_security_decode(mme_ue, h, pkbuf) != OGS_OK) {
                        ogs_error("nas_security_decode() failed");
                        ogs_pkbuf_free(pkbuf);
                        return;
                    }
                }

                /* If NAS(mme_ue_t) has already been associated with
                 * older S1(enb_ue_t) context */
                if (ECM_CONNECTED(mme_ue)) {
    /*
     * Issue #2786
     *
     * In cases where the UE sends an Integrity Un-Protected Attach
     * Request or Service Request, there is an issue of sending
     * a UEContextReleaseCommand for the OLD ENB Context.
     *
     * For example, if the UE switchs off and power-on after
     * the first connection, the EPC sends a UEContextReleaseCommand.
     *
     * However, since there is no ENB context for this on the eNB,
     * the eNB does not send a UEContextReleaseComplete,
     * so the deletion of the ENB Context does not function properly.
     *
     * To solve this problem, the EPC has been modified to implicitly
     * delete the ENB Context instead of sending a UEContextReleaseCommand.
     */
                    HOLDING_S1_CONTEXT(mme_ue);
                }
            }

            enb_ue_associate_mme_ue(enb_ue, mme_ue);
            ogs_debug("Mobile Reachable timer stopped for IMSI[%s]",
                mme_ue->imsi_bcd);
            CLEAR_MME_UE_TIMER(mme_ue->t_mobile_reachable);
        }

        ogs_assert(mme_ue);
        if (!OGS_FSM_STATE(&mme_ue->sm)) {
            ogs_fatal("MESSAGE[%d]", nas_message.emm.h.message_type);
            ogs_fatal("ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                    enb_ue ? enb_ue->enb_ue_s1ap_id : 0,
                    enb_ue ? enb_ue->mme_ue_s1ap_id : 0);
            ogs_fatal("context [%p:%p]", enb_ue, mme_ue);
            ogs_fatal("IMSI [%s]", mme_ue ? mme_ue->imsi_bcd : "No MME_UE");
            ogs_assert_if_reached();
        }
        ogs_assert(OGS_FSM_STATE(&mme_ue->sm));

        e->mme_ue_id = mme_ue->id;
        e->nas_message = &nas_message;

        ogs_fsm_dispatch(&mme_ue->sm, e);
        if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_exception)) {
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        }

        ogs_pkbuf_free(pkbuf);
        break;
    case MME_EVENT_EMM_TIMER:
        mme_ue = mme_ue_find_by_id(e->mme_ue_id);
        ogs_assert(mme_ue);
        ogs_assert(OGS_FSM_STATE(&mme_ue->sm));

        ogs_fsm_dispatch(&mme_ue->sm, e);
        break;

    case MME_EVENT_ESM_MESSAGE:
        mme_ue = mme_ue_find_by_id(e->mme_ue_id);
        ogs_assert(mme_ue);

        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        if (ogs_nas_esm_decode(&nas_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_nas_esm_decode() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

#define ESM_MESSAGE_CHECK \
    do { \
        ogs_error("emm_state_exception"); \
        ogs_error("nas_type:%d, create_action:%d", \
                e->nas_type, e->create_action); \
        ogs_error("esm.message[EBI:%d,PTI:%d,TYPE:%d]", \
                nas_message.esm.h.eps_bearer_identity, \
                nas_message.esm.h.procedure_transaction_identity, \
                nas_message.esm.h.message_type); \
    } while(0)

    /*
     * Because a race condition can occur between S6A Diameter and S1AP message,
     * the following error handling code has been added.
     *
     * 1. InitialUEMessage + Attach Request + PDN Connectivity request
     * 2. Authentication-Information-Request/Authentication-Information-Answer
     * 3. Authentication Request/Response
     * 4. Security-mode command/complete
     * 5. Update-Location-Request/Update-Location-Answer
     * 6. Detach request/accept
     *
     * In the ULR/ULA process in step 6, the PDN Connectivity request is
     * pushed to the queue as an ESM_MESSAGE because the NAS-Type is still
     * an Attach Request.
     *
     * See the code below in 'mme-s6a-handler.c' for where the queue is pushed.
     *
     *   if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
     *       rv = nas_eps_send_emm_to_esm(mme_ue,
     *               &mme_ue->pdn_connectivity_request);
     *       if (rv != OGS_OK) {
     *           ogs_error("nas_eps_send_emm_to_esm() failed");
     *           return OGS_NAS_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED;
     *       }
     *   } else if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
     *       r = nas_eps_send_tau_accept(mme_ue,
     *               S1AP_ProcedureCode_id_InitialContextSetup);
     *       ogs_expect(r == OGS_OK);
     *       ogs_assert(r != OGS_ERROR);
     *   } else {
     *       ogs_error("Invalid Type[%d]", mme_ue->nas_eps.type);
     *       return OGS_NAS_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED;
     *   }
     *
     * If you perform step 7 Detach request/accept here,
     * the NAS-Type becomes Detach Request and the EMM state changes
     * to emm_state_de_registered().
     *
     * Since the PDN, which is an ESM message that was previously queued,
     * should not be processed in de_registered, the message is ignored
     * through error handling below.
     *
     * Otherwise, MME will crash because there is no active bearer
     * in the initial_context_setup_request build process.
     *
     * See the code below in 's1ap-build.c' for where the crash occurs.
     *   ogs_list_for_each(&mme_ue->sess_list, sess) {
     *       ogs_list_for_each(&sess->bearer_list, bearer) {
     *           ...
     *           if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
     *           } else if (OGS_FSM_CHECK(&bearer->sm, esm_state_inactive)) {
     *               ogs_warn("No active EPS bearer [%d]", bearer->ebi);
     *               ogs_warn("    IMSI[%s] NAS-EPS Type[%d] "
     *                       "ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
     *                       mme_ue->imsi_bcd, mme_ue->nas_eps.type,
     *                       enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
     *               continue;
     *           }
     *           ...
     *       }
     *   }
     */
        if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_de_registered)) {
            ESM_MESSAGE_CHECK;
            ogs_pkbuf_free(pkbuf);
            break;
        } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_authentication)) {
            ESM_MESSAGE_CHECK;
            ogs_pkbuf_free(pkbuf);
            break;
        } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_security_mode)) {
            ESM_MESSAGE_CHECK;
            ogs_pkbuf_free(pkbuf);
            break;
        } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup)) {
        } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered)) {
        } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_exception)) {
            ESM_MESSAGE_CHECK;
            ogs_pkbuf_free(pkbuf);
            break;
        }

        bearer = mme_bearer_find_or_add_by_message(
                    mme_ue, &nas_message, e->create_action);
        if (!bearer) {
            ogs_pkbuf_free(pkbuf);
            break;
        }

        sess = mme_sess_find_by_id(bearer->sess_id);
        ogs_assert(sess);
        default_bearer = mme_default_bearer_in_sess(sess);
        ogs_assert(default_bearer);

        e->bearer_id = bearer->id;
        e->nas_message = &nas_message;

        ogs_fsm_dispatch(&bearer->sm, e);
        if (OGS_FSM_CHECK(&bearer->sm, esm_state_bearer_deactivated)) {
            if (default_bearer->ebi == bearer->ebi) {
                /* if the bearer is a default bearer,
                 * remove all session context linked the default bearer */
                MME_SESS_CLEAR(sess);
            } else {
                /* if the bearer is not a default bearer,
                 * just remove the bearer context */
                mme_bearer_remove(bearer);
            }

        } else if (OGS_FSM_CHECK(&bearer->sm, esm_state_pdn_did_disconnect)) {
            ogs_assert(default_bearer->ebi == bearer->ebi);
            MME_SESS_CLEAR(sess);

        } else if (OGS_FSM_CHECK(&bearer->sm, esm_state_exception)) {

            /*
             * The UE requested the wrong APN.
             *
             * From the Issues #568, MME need to accept further service request.
             * To do this, we are not going to release UE context.
             *
             * Just we'll remove MME session context.
             */
            MME_SESS_CLEAR(sess);
        }

        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVENT_ESM_TIMER:
        bearer = mme_bearer_find_by_id(e->bearer_id);
        ogs_assert(bearer);
        ogs_assert(OGS_FSM_STATE(&bearer->sm));

        ogs_fsm_dispatch(&bearer->sm, e);
        break;

    case MME_EVENT_S6A_MESSAGE:
        s6a_message = e->s6a_message;
        ogs_assert(s6a_message);

        /*
         * A race condition can occur in the following situations.
         * In conclusion, we can use this situation to determine
         * whether or not the UE Context has been removed and avoiding a crash.
         *
         * For example, suppose a UE Context is removed in the followings.
         *
         * 1. Attach Request
         * 2. Authentication-Information-Request
         * 3. Authentication-Information-Answer
         * 4. Authentication Request
         * 5. Authentication Response(MAC Failed)
         * 6. Authentication Reject
         * 7. UEContextReleaseCommand
         * 8. UEContextReleaseComplete
         *
         * The MME then sends a Purge-UE-request to the HSS and deletes
         * the UE context as soon as it receives a Purge-UE-Answer.
         *
         * Suppose an Attach Request is received from the same UE
         * between Purge-UE-Request/Answer, then the MME and HSS start
         * the Authentication-Information-Request/Answer process.
         *
         * This can lead to the following situations.
         *
         * 1. Purge-UE-Request
         * 2. Attach Request
         * 3. Authentication-Information-Request
         * 4. Purge-UE-Answer
         * 5. [UE Context Removed]
         * 6. Authentication-Information-Answer
         *
         * Since the UE Context has already been deleted
         * when the Authentication-Information-Answer is received,
         * it cannot be processed properly.
         *
         * Therefore, mme_ue_cycle() is used to check
         * whether the UE Context has been deleted and
         * decide whether to process or
         * ignore the Authentication-Information-Answer as shown below.
         */
        mme_ue = mme_ue_find_by_id(e->mme_ue_id);
        if (!mme_ue) {
            ogs_error("UE(mme-ue) context has already been removed");
            goto cleanup;
        }

        enb_ue = enb_ue_find_by_id(e->enb_ue_id);
        /*
         * The 'enb_ue' context is not checked
         * because the status is checked in the sending routine.
         */

        switch (s6a_message->cmd_code) {
        case OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION:
            ogs_debug("OGS_DIAM_S6A_CMD_CODE_AUTHENTICATION_INFORMATION");
            if (e->gtp_xact_id != OGS_INVALID_POOL_ID)
                xact = ogs_gtp_xact_find_by_id(e->gtp_xact_id);
            else
                xact = NULL;
            emm_cause = mme_s6a_handle_aia(mme_ue, s6a_message);
            if (emm_cause != OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED) {
                /* If authentication was triggered due to subscriber coming from
                 * an SGSN, report to it that something went wrong: */
                if (xact) {
                    rv = mme_gtp1_send_sgsn_context_ack(mme_ue, OGS_GTP1_CAUSE_AUTHENTICATION_FAILURE, xact);
                    if (rv != OGS_OK)
                        ogs_warn("Failed to send SGSN Context Ack (rv %d)", rv);
                } else
                    ogs_warn("Originating SGSN Context xact no longer valid (%d)", e->gtp_xact_id);

                /* Finally reject the UE: */
                if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
                    ogs_info("[%s] Attach reject [OGS_NAS_EMM_CAUSE:%d]",
                            mme_ue->imsi_bcd, emm_cause);
                    r = nas_eps_send_attach_reject(
                            enb_ue, mme_ue, emm_cause,
                            OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
                    /* This is usually an UE coming from 2G (Cell reselection),
                     * which we decided to re-authenticate */
                    ogs_info("[%s] TAU reject [OGS_NAS_EMM_CAUSE:%d]",
                            mme_ue->imsi_bcd, emm_cause);
                    r = nas_eps_send_tau_reject(
                            enb_ue, mme_ue, emm_cause);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else
                    ogs_error("Invalid Type[%d]", mme_ue->nas_eps.type);

                r = s1ap_send_ue_context_release_command(enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                break;
            }

            if (xact) {
                /* Subscriber coming from SGSN, store info so we can SGSN
                 * Context Ack after authenticating the UE: */
                mme_ue->gn.gtp_xact_id = e->gtp_xact_id;
            }

            /* Auth-Info accepted from HSS, now authenticate the UE: */
            r = nas_eps_send_authentication_request(mme_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            break;
        case OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION:
            ogs_debug("OGS_DIAM_S6A_CMD_CODE_UPDATE_LOCATION");
            emm_cause = mme_s6a_handle_ula(mme_ue, s6a_message);
            if (emm_cause != OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED) {
                if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
                    ogs_info("[%s] Attach reject [OGS_NAS_EMM_CAUSE:%d]",
                            mme_ue->imsi_bcd, emm_cause);
                    r = nas_eps_send_attach_reject(
                            enb_ue, mme_ue, emm_cause,
                            OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else if (mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
                    ogs_info("[%s] TAU reject [OGS_NAS_EMM_CAUSE:%d]",
                            mme_ue->imsi_bcd, emm_cause);
                    r = nas_eps_send_tau_reject(
                            enb_ue, mme_ue, emm_cause);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else
                    ogs_error("Invalid Type[%d]", mme_ue->nas_eps.type);

                r = s1ap_send_ue_context_release_command(enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        mme_ue_find_by_id(enb_ue->mme_ue_id) ?
                            S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE :
                            S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            break;
        case OGS_DIAM_S6A_CMD_CODE_PURGE_UE:
            ogs_debug("OGS_DIAM_S6A_CMD_CODE_PURGE_UE");
            mme_s6a_handle_pua(mme_ue, s6a_message);
            break;
        case OGS_DIAM_S6A_CMD_CODE_CANCEL_LOCATION:
            ogs_debug("OGS_DIAM_S6A_CMD_CODE_CANCEL_LOCATION");
            mme_s6a_handle_clr(mme_ue, s6a_message);
            break;
        case OGS_DIAM_S6A_CMD_CODE_INSERT_SUBSCRIBER_DATA:
            ogs_debug("OGS_DIAM_S6A_CMD_CODE_INSERT_SUBSCRIBER_DATA");
            mme_s6a_handle_idr(mme_ue, s6a_message);
            break;
        default:
            ogs_error("Invalid Type[%d]", s6a_message->cmd_code);
            break;
        }

cleanup:
        ogs_subscription_data_free(&s6a_message->idr_message.subscription_data);
        ogs_subscription_data_free(&s6a_message->ula_message.subscription_data);
        ogs_free(s6a_message);
        break;

    case MME_EVENT_S11_MESSAGE:
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_gtp2_parse_msg(&gtp_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_gtp2_parse_msg() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

        gnode = e->gnode;
        ogs_assert(gnode);

        rv = ogs_gtp_xact_receive(gnode, &gtp_message.h, &xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(pkbuf);
            break;
        }

        /*
         * 5.5.2 in spec 29.274
         *
         * If a peer's TEID is not available, the TEID field still shall be
         * present in the header and its value shall be set to "0" in the
         * following messages:
         *
         * - Create Session Request message on S2a/S2b/S5/S8
         *
         * - Create Session Request message on S4/S11, if for a given UE,
         *   the SGSN/MME has not yet obtained the Control TEID of the SGW.
         *
         * - If a node receives a message and the TEID-C in the GTPv2 header of
         *   the received message is not known, it shall respond with
         *   "Context not found" Cause in the corresponding response message
         *   to the sender, the TEID used in the GTPv2-C header in the response
         *   message shall be then set to zero.
         *
         * - If a node receives a request message containing protocol error,
         *   e.g. Mandatory IE missing, which requires the receiver to reject
         *   the message as specified in clause 7.7, it shall reject
         *   the request message. For the response message, the node should
         *   look up the remote peer's TEID and accordingly set the GTPv2-C
         *   header TEID and the message cause code. As an implementation
         *   option, the node may not look up the remote peer's TEID and
         *   set the GTPv2-C header TEID to zero in the response message.
         *   However in this case, the cause code shall not be set to
         *   "Context not found".
         */
        if (gtp_message.h.teid_presence && gtp_message.h.teid != 0) {
            /* Cause is not "Context not found" */
            mme_ue = mme_ue_find_by_s11_local_teid(gtp_message.h.teid);
        } else if (xact->local_teid) { /* rx no TEID or TEID=0 */
            /* 3GPP TS 29.274 5.5.2: we receive TEID=0 under some
             * conditions, such as cause "Session context not found". In those
             * cases, we still want to identify the local session which
             * originated the message, so try harder by using the TEID we
             * locally stored in xact when sending the original request: */
            mme_ue = mme_ue_find_by_s11_local_teid(xact->local_teid);
        }

        switch (gtp_message.h.type) {
        case OGS_GTP2_ECHO_REQUEST_TYPE:
            mme_s11_handle_echo_request(xact, &gtp_message.echo_request);
            break;
        case OGS_GTP2_ECHO_RESPONSE_TYPE:
            mme_s11_handle_echo_response(xact, &gtp_message.echo_response);
            break;
        case OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE:
            if (!gtp_message.h.teid_presence) ogs_error("No TEID");
            mme_s11_handle_create_session_response(
                xact, mme_ue, &gtp_message.create_session_response);
            break;
        case OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE:
            if (!gtp_message.h.teid_presence) ogs_error("No TEID");
            mme_s11_handle_modify_bearer_response(
                xact, mme_ue, &gtp_message.modify_bearer_response);
            break;
        case OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE:
            if (!gtp_message.h.teid_presence) ogs_error("No TEID");
            mme_s11_handle_delete_session_response(
                xact, mme_ue, &gtp_message.delete_session_response);
            break;
        case OGS_GTP2_CREATE_BEARER_REQUEST_TYPE:
            mme_s11_handle_create_bearer_request(
                xact, mme_ue, &gtp_message.create_bearer_request);
            break;
        case OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE:
            mme_s11_handle_update_bearer_request(
                xact, mme_ue, &gtp_message.update_bearer_request);
            break;
        case OGS_GTP2_DELETE_BEARER_REQUEST_TYPE:
            mme_s11_handle_delete_bearer_request(
                xact, mme_ue, &gtp_message.delete_bearer_request);
            break;
        case OGS_GTP2_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
            if (!gtp_message.h.teid_presence) ogs_error("No TEID");
            mme_s11_handle_release_access_bearers_response(
                xact, mme_ue, &gtp_message.release_access_bearers_response);
            break;
        case OGS_GTP2_DOWNLINK_DATA_NOTIFICATION_TYPE:
            mme_s11_handle_downlink_data_notification(
                xact, mme_ue, &gtp_message.downlink_data_notification);
            break;
        case OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            if (!gtp_message.h.teid_presence) ogs_error("No TEID");
            mme_s11_handle_create_indirect_data_forwarding_tunnel_response(
                xact, mme_ue,
                &gtp_message.create_indirect_data_forwarding_tunnel_response);
            break;
        case OGS_GTP2_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            if (!gtp_message.h.teid_presence) ogs_error("No TEID");
            mme_s11_handle_delete_indirect_data_forwarding_tunnel_response(
                xact, mme_ue,
                &gtp_message.delete_indirect_data_forwarding_tunnel_response);
            break;
        case OGS_GTP2_BEARER_RESOURCE_FAILURE_INDICATION_TYPE:
            if (!gtp_message.h.teid_presence) ogs_error("No TEID");
            mme_s11_handle_bearer_resource_failure_indication(
                xact, mme_ue,
                &gtp_message.bearer_resource_failure_indication);
            break;
        default:
            ogs_warn("Not implemented(type:%d)", gtp_message.h.type);
            break;
        }
        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVENT_S11_TIMER:
        sgw_ue = sgw_ue_find_by_id(e->sgw_ue_id);
        if (!sgw_ue) {
            ogs_error("SGW-UE Context has already been removed");
            break;
        }
        mme_ue = mme_ue_find_by_id(sgw_ue->mme_ue_id);
        if (!mme_ue) {
            ogs_error("MME-UE Context has already been removed");
            break;
        }

        switch (e->timer_id) {
        case MME_TIMER_S11_HOLDING:

            GTP_COUNTER_CLEAR(mme_ue,
                    GTP_COUNTER_DELETE_SESSION_BY_PATH_SWITCH);

            ogs_list_for_each(&mme_ue->sess_list, sess) {

                GTP_COUNTER_INCREMENT(
                    mme_ue, GTP_COUNTER_DELETE_SESSION_BY_PATH_SWITCH);

                enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
                if (enb_ue) {
                    ogs_assert(OGS_OK ==
                        mme_gtp_send_delete_session_request(
                            enb_ue, sgw_ue, sess,
                            OGS_GTP_DELETE_IN_PATH_SWITCH_REQUEST));
                } else
                    ogs_error("ENB-S1 Context has already been removed");

            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    case MME_EVENT_GN_MESSAGE:
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_gtp1_parse_msg(&gtp1_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_gtp1_parse_msg() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

        gnode = e->gnode;
        ogs_assert(gnode);

        rv = ogs_gtp1_xact_receive(gnode, &gtp1_message.h, &xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(pkbuf);
            break;
        }

        if (gtp1_message.h.teid != 0) {
            /* Cause is not "Context not found" */
            mme_ue = mme_ue_find_by_gn_local_teid(gtp1_message.h.teid);
        } else if (xact->local_teid) { /* rx no TEID or TEID=0 */
            /* Try harder by using the TEID we locally stored in xact when
             *sending the original request: */
            mme_ue = mme_ue_find_by_gn_local_teid(xact->local_teid);
        }

        switch (gtp1_message.h.type) {
        case OGS_GTP1_ECHO_REQUEST_TYPE:
            mme_gn_handle_echo_request(xact, &gtp1_message.echo_request);
            break;
        case OGS_GTP1_ECHO_RESPONSE_TYPE:
            mme_gn_handle_echo_response(xact, &gtp1_message.echo_response);
            break;
        case OGS_GTP1_SGSN_CONTEXT_REQUEST_TYPE:
            mme_gn_handle_sgsn_context_request(xact, &gtp1_message.sgsn_context_request);
            break;
        case OGS_GTP1_SGSN_CONTEXT_RESPONSE_TYPE:
            /* Clang scan-build SA: NULL pointer dereference: mme_ue=NULL if both gtp1_message.h.teid=0 and
             * xact->local_teid=0. The following function mme_gn_handle_sgsn_context_response() handles the NULL
             * but the later calls to OGS_FSM_TRAN() to change state will be a NULL pointer dereference. */
            ogs_assert(mme_ue);
            enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
            if (!enb_ue) {
                ogs_error("ENB-S1 Context has already been removed");
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
                break;
            }

            /* 3GPP TS 23.401 Figure D.3.6-1 step 5 */
            rv = mme_gn_handle_sgsn_context_response(xact, mme_ue, &gtp1_message.sgsn_context_response);
            if (rv == OGS_GTP1_CAUSE_ACCEPT || rv == OGS_GTP1_CAUSE_REQUEST_IMEI) {
                mme_s6a_send_air_from_gn(enb_ue, mme_ue, xact);
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
            } else {
                OGS_FSM_TRAN(&mme_ue->sm, &emm_state_exception);
            }
            break;
        case OGS_GTP1_SGSN_CONTEXT_ACKNOWLEDGE_TYPE:
            mme_gn_handle_sgsn_context_acknowledge(xact, mme_ue, &gtp1_message.sgsn_context_acknowledge);
            break;
        case OGS_GTP1_RAN_INFORMATION_RELAY_TYPE:
            mme_gn_handle_ran_information_relay(xact, &gtp1_message.ran_information_relay);
            break;
        default:
            ogs_warn("Not implemented(type:%d)", gtp1_message.h.type);
            break;
        }
        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVENT_GN_TIMER:
        mme_ue = mme_ue_find_by_id(e->mme_ue_id);
        ogs_assert(mme_ue);
        sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
        ogs_assert(sgw_ue);

        switch (e->timer_id) {
        case MME_TIMER_GN_HOLDING:
            /* 3GPP TS 23.401 Annex D.3.5 "Routing Area Update":
            * Step 13. "When the timer started in step 2) (see mme_gn_handle_sgsn_context_request()) expires the old MME
            * releases any RAN and Serving GW resources. If the PLMN has configured Secondary RAT usage data reporting,
            * the MME first releases RAN resource before releasing Serving GW resources."
            */
            GTP_COUNTER_CLEAR(mme_ue,
                    GTP_COUNTER_DELETE_SESSION_BY_PATH_SWITCH);
            ogs_list_for_each(&mme_ue->sess_list, sess) {
                GTP_COUNTER_INCREMENT(
                    mme_ue, GTP_COUNTER_DELETE_SESSION_BY_PATH_SWITCH);

                enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
                if (enb_ue) {
                    ogs_assert(OGS_OK ==
                        mme_gtp_send_delete_session_request(
                            enb_ue, sgw_ue, sess,
                            OGS_GTP_DELETE_IN_PATH_SWITCH_REQUEST));
                } else
                    ogs_error("ENB-S1 Context has already been removed");
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    case MME_EVENT_SGSAP_LO_SCTP_COMM_UP:
        sock = e->sock;
        ogs_assert(sock);

        max_num_of_ostreams = e->max_num_of_ostreams;

        vlr = mme_vlr_find_by_sock(sock);
        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        vlr->max_num_of_ostreams =
                ogs_min(max_num_of_ostreams, vlr->max_num_of_ostreams);

        ogs_debug("VLR-SGs SCTP_COMM_UP %s Max Num of Outbound Streams[%d]",
                ogs_sockaddr_to_string_static(vlr->sa_list),
                vlr->max_num_of_ostreams);

        e->vlr = vlr;
        ogs_fsm_dispatch(&vlr->sm, e);
        break;

    case MME_EVENT_SGSAP_LO_CONNREFUSED:
        sock = e->sock;
        ogs_assert(sock);

        vlr = mme_vlr_find_by_sock(sock);
        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        if (OGS_FSM_CHECK(&vlr->sm, sgsap_state_connected)) {
            e->vlr = vlr;
            ogs_fsm_dispatch(&vlr->sm, e);

            ogs_info("VLR-SGs %s connection refused!!!",
                    ogs_sockaddr_to_string_static(vlr->sa_list));

        } else {
            ogs_warn("VLR-SGs %s connection refused, Already Removed!",
                    ogs_sockaddr_to_string_static(vlr->sa_list));
        }

        break;
    case MME_EVENT_SGSAP_MESSAGE:
        sock = e->sock;
        ogs_assert(sock);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        vlr = mme_vlr_find_by_sock(sock);
        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        e->vlr = vlr;
        ogs_fsm_dispatch(&vlr->sm, e);

        ogs_pkbuf_free(pkbuf);
        break;

    case MME_EVENT_SGSAP_TIMER:
        vlr = e->vlr;
        ogs_assert(vlr);
        ogs_assert(OGS_FSM_STATE(&vlr->sm));

        ogs_fsm_dispatch(&vlr->sm, e);
        break;

        case MME_EVENT_SBCAP_LO_ACCEPT:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET);

        ogs_info("SBC/CBC accepted[%s] in master_sm module",
            OGS_ADDR(addr, buf));

        sbc = mme_sbcap_find_by_addr(addr);
        if (!sbc) {
            sbc = mme_sbcap_add(sock, addr);
            ogs_assert(sbc);
        } else {
            ogs_warn("SBC.CBC context duplicated with IP-address [%s]!!!",
                    OGS_ADDR(addr, buf));
            ogs_sock_destroy(sock);
            ogs_free(addr);
            ogs_warn("SBcAP Socket Closed");
        }


        break;

    case MME_EVENT_SBCAP_LO_SCTP_COMM_UP:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET);

        max_num_of_ostreams = e->max_num_of_ostreams;


        ogs_info("SBC/CBC [%s] max_num_of_ostreams : %d",
            OGS_ADDR(addr, buf), max_num_of_ostreams);

        break;

    case MME_EVENT_SBCAP_LO_CONNREFUSED:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);

        ogs_assert(addr->ogs_sa_family == AF_INET);

        sbc = mme_sbcap_find_by_addr(addr);
        if (sbc) {
            ogs_info("CBC/SBC [%s] connection refused!!!", OGS_ADDR(addr, buf));
            //mme_gtp_send_release_all_ue_in_enb(
            //        enb, OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_LO_CONNREFUSED);
            mme_sbc_remove(sbc);
        } else {
            ogs_warn("CBC/SBC [%s] connection refused, Already Removed!",
                    OGS_ADDR(addr, buf));
        }
        ogs_free(addr);

        break;
    case MME_EVENT_SBCAP_MESSAGE:
        sock = e->sock;
        ogs_assert(sock);
        addr = e->addr;
        ogs_assert(addr);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        ogs_assert(addr->ogs_sa_family == AF_INET);

        sbc = mme_sbcap_find_by_addr(addr);
        ogs_free(addr);

        ogs_assert(sbc);
        ogs_assert(OGS_FSM_STATE(&sbc->sm));
        ///sbcap_message
        //ogs_warn("Before SBCAP message");
        ogs_sbcap_decode(&sbcap_message, pkbuf);
        rc = OGS_OK;
        //ogs_warn("After SBCAP message");

        if (rc == OGS_OK) {
            e->sbc = sbc;
            e->sbcap_message = &sbcap_message;
            ogs_fsm_dispatch(&sbc->sm, e);
        } else {
            ogs_warn("Cannot decode SBCAP message");
            r = OGS_OK;//s1ap_send_error_indication(
                    //enb, NULL, NULL, S1AP_Cause_PR_protocol,
                    //S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

        ogs_sbcap_free(&sbcap_message);
        ogs_pkbuf_free(pkbuf);
        break;

    default:
        ogs_error("No handler for event %s", mme_event_get_name(e));
        break;
    }
}
