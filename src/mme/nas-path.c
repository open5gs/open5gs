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

#include "s1ap-path.h"
#include "s1ap-build.h"
#include "esm-build.h"
#include "emm-build.h"
#include "nas-path.h"
#include "mme-event.h"
#include "mme-timer.h"
#include "mme-sm.h"

int nas_eps_send_to_enb(mme_ue_t *mme_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(pkbuf);

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        ogs_pkbuf_free(pkbuf);
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        ogs_pkbuf_free(pkbuf);
        return OGS_NOTFOUND;
    }

    rv = s1ap_send_to_enb_ue(enb_ue, pkbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_emm_to_esm(mme_ue_t *mme_ue,
        ogs_nas_esm_message_container_t *esm_message_container)
{
    int rv;
    ogs_pkbuf_t *esmbuf = NULL;

    ogs_assert(esm_message_container);
    if (!esm_message_container->length) {
        ogs_error("Invalid ESM Message Container");
        return OGS_ERROR;
    }

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    esmbuf = ogs_pkbuf_alloc(NULL,
            OGS_NAS_HEADROOM+esm_message_container->length);
    ogs_assert(esmbuf);
    ogs_pkbuf_reserve(esmbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(esmbuf,
            esm_message_container->buffer, esm_message_container->length);

    rv = s1ap_send_to_esm(mme_ue, esmbuf, 0, OGS_GTP_CREATE_IN_ATTACH_REQUEST);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_to_downlink_nas_transport(
        enb_ue_t *enb_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_assert(pkbuf);

    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        ogs_pkbuf_free(pkbuf);
        return OGS_NOTFOUND;
    }

    s1apbuf = s1ap_build_downlink_nas_transport(enb_ue, pkbuf);
    if (!s1apbuf) {
        ogs_error("s1ap_build_downlink_nas_transport() failed");
        return OGS_ERROR;
    }

    rv = s1ap_send_to_enb_ue(enb_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_attach_accept(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    mme_sess_t *sess = NULL;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL, *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    sess = mme_sess_first(mme_ue);
    ogs_assert(sess);
    if (mme_sess_next(sess)) {
        ogs_error("There should only be one SESSION");
        return OGS_ERROR;
    }

    ogs_debug("[%s] Attach accept", mme_ue->imsi_bcd);

    esmbuf = esm_build_activate_default_bearer_context_request(
                sess, OGS_GTP_CREATE_IN_ATTACH_REQUEST);
    if (!esmbuf) {
        ogs_error("esm_build_activate_default_bearer_context_request() failed");
        return OGS_ERROR;
    }

    emmbuf = emm_build_attach_accept(mme_ue, esmbuf);
    if (!emmbuf) {
        ogs_error("emm_build_attach_accept() failed");
        return OGS_ERROR;
    }

    CLEAR_MME_UE_TIMER(mme_ue->t3450);
    mme_ue->t3450.pkbuf = ogs_pkbuf_copy(emmbuf);
    if (!mme_ue->t3450.pkbuf) {
        ogs_error("ogs_pkbuf_copy(mme_ue->t3450.pkbuf) failed");
        ogs_pkbuf_free(emmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(mme_ue->t3450.timer,
            mme_timer_cfg(MME_TIMER_T3450)->duration);

    /*
     * Issue #2040
     *
     * TS24.301
     * 5.5.1 Attach procedure
     * 5.5.1.2 Attach procedure for EPS services
     * 5.5.1.2.4 Attach accepted by the network
     *
     * If the attach request is accepted by the network,
     * the MME shall delete the stored UE radio capability information
     * or the UE radio capability ID, if any.
     */
    OGS_ASN_CLEAR_DATA(&mme_ue->ueRadioCapability);

    s1apbuf = s1ap_build_initial_context_setup_request(mme_ue, emmbuf);
    if (!s1apbuf) {
        ogs_error("s1ap_build_initial_context_setup_request() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_attach_reject(enb_ue_t *enb_ue, mme_ue_t *mme_ue,
    ogs_nas_emm_cause_t emm_cause, ogs_nas_esm_cause_t esm_cause)
{
    int rv;
    mme_sess_t *sess = NULL;
    ogs_pkbuf_t *esmbuf = NULL, *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Attach reject", mme_ue->imsi_bcd);
    ogs_debug("    Cause[%d]", emm_cause);

    sess = mme_sess_first(mme_ue);
    if (sess) {
        esmbuf = esm_build_pdn_connectivity_reject(
                    sess, esm_cause, OGS_GTP_CREATE_IN_ATTACH_REQUEST);
        if (!esmbuf) {
            ogs_error("esm_build_pdn_connectivity_reject() failed");
            return OGS_ERROR;
        }
    }

    emmbuf = emm_build_attach_reject(emm_cause, esmbuf);
    if (!emmbuf) {
        ogs_error("emm_build_attach_reject() failed");
        return OGS_ERROR;
    }
    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_identity_request(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("Identity request");

    if (mme_ue->t3470.pkbuf) {
        emmbuf = mme_ue->t3470.pkbuf;
    } else {
        emmbuf = emm_build_identity_request(mme_ue);
        if (!emmbuf) {
            ogs_error("emm_build_identity_request() failed");
            return OGS_ERROR;
        }
    }

    mme_ue->t3470.pkbuf = ogs_pkbuf_copy(emmbuf);
    if (!mme_ue->t3470.pkbuf) {
        ogs_error("ogs_pkbuf_copy(mme_ue->t3470.pkbuf) failed");
        ogs_pkbuf_free(emmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(mme_ue->t3470.timer, 
            mme_timer_cfg(MME_TIMER_T3470)->duration);

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_authentication_request(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Authentication request", mme_ue->imsi_bcd);

    if (mme_ue->t3460.pkbuf) {
        emmbuf = mme_ue->t3460.pkbuf;
    } else {
        emmbuf = emm_build_authentication_request(mme_ue);
        if (!emmbuf) {
            ogs_error("emm_build_authentication_request() failed");
            return OGS_ERROR;
        }
    }

    mme_ue->t3460.pkbuf = ogs_pkbuf_copy(emmbuf);
    if (!mme_ue->t3460.pkbuf) {
        ogs_error("ogs_pkbuf_copy(mme_ue->t3460.pkbuf) failed");
        ogs_pkbuf_free(emmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(mme_ue->t3460.timer, 
            mme_timer_cfg(MME_TIMER_T3460)->duration);

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_security_mode_command(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Security mode command", mme_ue->imsi_bcd);

    if (mme_ue->t3460.pkbuf) {
        emmbuf = mme_ue->t3460.pkbuf;
    } else {
        emmbuf = emm_build_security_mode_command(mme_ue);
        if (!emmbuf) {
            ogs_error("emm_build_security_mode_command() failed");
            return OGS_ERROR;
        }
    }

    mme_ue->t3460.pkbuf = ogs_pkbuf_copy(emmbuf);
    if (!mme_ue->t3460.pkbuf) {
        ogs_error("ogs_pkbuf_copy(mme_ue->t3460.pkbuf) failed");
        ogs_pkbuf_free(emmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(mme_ue->t3460.timer, 
            mme_timer_cfg(MME_TIMER_T3460)->duration);

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_authentication_reject(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Authentication reject", mme_ue->imsi_bcd);

    emmbuf = emm_build_authentication_reject();
    if (!emmbuf) {
        ogs_error("emm_build_authentication_reject() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_detach_request(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Detach request to UE", mme_ue->imsi_bcd);

    if (mme_ue->t3422.pkbuf) {
        emmbuf = mme_ue->t3422.pkbuf;
    } else {
        emmbuf = emm_build_detach_request(mme_ue);
        if (!emmbuf) {
            ogs_error("emm_build_detach_request() failed");
            return OGS_ERROR;
        }
    }

    mme_ue->t3422.pkbuf = ogs_pkbuf_copy(emmbuf);
    if (!mme_ue->t3422.pkbuf) {
        ogs_error("ogs_pkbuf_copy(mme_ue->t3422.pkbuf) failed");
        ogs_pkbuf_free(emmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(mme_ue->t3422.timer, 
            mme_timer_cfg(MME_TIMER_T3422)->duration);    

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_detach_accept(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Detach accept", mme_ue->imsi_bcd);

    /* reply with detach accept */
    if (mme_ue->nas_eps.detach.switch_off == 0) {
        emmbuf = emm_build_detach_accept(mme_ue);
        if (!emmbuf) {
            ogs_error("emm_build_detach_accept() failed");
            return OGS_ERROR;
        }

        rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
        if (rv != OGS_OK) {
            ogs_error("nas_eps_send_to_downlink_nas_transport() failed");
            return rv;
        }
    }

    rv = s1ap_send_ue_context_release_command(enb_ue,
            S1AP_Cause_PR_nas, S1AP_CauseNas_detach,
            S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_pdn_connectivity_reject(
    mme_sess_t *sess, ogs_nas_esm_cause_t esm_cause, int create_action)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *esmbuf = NULL;

    ogs_assert(sess);

    mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    if (create_action == OGS_GTP_CREATE_IN_ATTACH_REQUEST) {
        /* During the UE-attach process, we'll send Attach-Reject
         * with pyggybacking PDN-connectivity-Reject */
        rv = nas_eps_send_attach_reject(enb_ue, mme_ue,
            OGS_NAS_EMM_CAUSE_ESM_FAILURE, esm_cause);
        if (rv != OGS_OK) {
            ogs_error("nas_eps_send_attach_reject() failed");
            return rv;
        }

        rv = s1ap_send_ue_context_release_command(enb_ue,
                S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
        ogs_expect(rv == OGS_OK);
    } else {
        esmbuf = esm_build_pdn_connectivity_reject(
                    sess, esm_cause, create_action);
        if (!esmbuf) {
            ogs_error("esm_build_pdn_connectivity_reject() failed");
            return OGS_ERROR;
        }

        rv = nas_eps_send_to_downlink_nas_transport(enb_ue, esmbuf);
        ogs_expect(rv == OGS_OK);
    }

    return rv;
}

int nas_eps_send_esm_information_request(mme_bearer_t *bearer)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *esmbuf = NULL;

    ogs_assert(bearer);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    if (bearer->t3489.pkbuf) {
        esmbuf = bearer->t3489.pkbuf;
    } else {
        esmbuf = esm_build_information_request(bearer);
        if (!esmbuf) {
            ogs_error("esm_build_information_request() failed");
            return OGS_ERROR;
        }
    }

    bearer->t3489.pkbuf = ogs_pkbuf_copy(esmbuf);
    if (!bearer->t3489.pkbuf) {
        ogs_error("ogs_pkbuf_copy(bearer->t3489) failed");
        ogs_pkbuf_free(esmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(bearer->t3489.timer, 
            mme_timer_cfg(MME_TIMER_T3489)->duration);

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, esmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_activate_default_bearer_context_request(
        mme_bearer_t *bearer, int create_action)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_sess_t *sess = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(bearer);
    sess = mme_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    esmbuf = esm_build_activate_default_bearer_context_request(
                sess, create_action);
    if (!esmbuf) {
        ogs_error("esm_build_activate_default_bearer_context_request() failed");
        return OGS_ERROR;
    }

    s1apbuf = s1ap_build_e_rab_setup_request(bearer, esmbuf);
    if (!s1apbuf) {
        ogs_error("s1ap_build_e_rab_setup_request() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_activate_dedicated_bearer_context_request(
        mme_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(bearer);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    esmbuf = esm_build_activate_dedicated_bearer_context_request(bearer);
    if (!esmbuf) {
        ogs_error(
            "esm_build_activate_dedicated_bearer_context_request() failed");
        return OGS_ERROR;
    }

    s1apbuf = s1ap_build_e_rab_setup_request(bearer, esmbuf);
    if (!s1apbuf) {
        ogs_error("s1ap_build_e_rab_setup_request() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

void nas_eps_send_activate_all_dedicated_bearers(mme_bearer_t *default_bearer)
{
    int r;
    ogs_assert(default_bearer);

    mme_bearer_t *dedicated_bearer = mme_bearer_next(default_bearer);
    while (dedicated_bearer) {
        r = nas_eps_send_activate_dedicated_bearer_context_request(
                dedicated_bearer);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        dedicated_bearer = mme_bearer_next(dedicated_bearer);
    }
}

int nas_eps_send_modify_bearer_context_request(
        mme_bearer_t *bearer, int qos_presence, int tft_presence)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(bearer);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    esmbuf = esm_build_modify_bearer_context_request(
            bearer, qos_presence, tft_presence);
    if (!esmbuf) {
        ogs_error("esm_build_modify_bearer_context_request() failed");
        return OGS_ERROR;
    }

    if (qos_presence == 1) {
        s1apbuf = s1ap_build_e_rab_modify_request(bearer, esmbuf);
        if (!s1apbuf) {
            ogs_error("s1ap_build_e_rab_modify_request() failed");
            return OGS_ERROR;
        }

        rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
        ogs_expect(rv == OGS_OK);
    } else {
        rv = nas_eps_send_to_downlink_nas_transport(enb_ue, esmbuf);
        ogs_expect(rv == OGS_OK);
    }

    return rv;
}

int nas_eps_send_deactivate_bearer_context_request(mme_bearer_t *bearer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *esmbuf = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(bearer);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    esmbuf = esm_build_deactivate_bearer_context_request(
            bearer, OGS_NAS_ESM_CAUSE_REGULAR_DEACTIVATION);
    if (!esmbuf) {
        ogs_error("esm_build_deactivate_bearer_context_request() failed");
        return OGS_ERROR;
    }

    s1apbuf = s1ap_build_e_rab_release_command(bearer, esmbuf,
            S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release);
    if (!s1apbuf) {
        ogs_error("s1ap_build_e_rab_release_command() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_bearer_resource_allocation_reject(
        mme_ue_t *mme_ue, uint8_t pti, ogs_nas_esm_cause_t esm_cause)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *esmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_assert(pti != OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED);

    esmbuf = esm_build_bearer_resource_allocation_reject(
            mme_ue, pti, esm_cause);
    if (!esmbuf) {
        ogs_error("esm_build_bearer_resource_allocation_reject() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, esmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_bearer_resource_modification_reject(
        mme_ue_t *mme_ue, uint8_t pti, ogs_nas_esm_cause_t esm_cause)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *esmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_assert(pti != OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED);

    esmbuf = esm_build_bearer_resource_modification_reject(
            mme_ue, pti, esm_cause);
    if (!esmbuf) {
        ogs_error("esm_build_bearer_resource_modification_reject() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, esmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_tau_accept(
        mme_ue_t *mme_ue, S1AP_ProcedureCode_t procedureCode)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Tracking area update accept", mme_ue->imsi_bcd);

    emmbuf = emm_build_tau_accept(mme_ue);
    if (!emmbuf) {
        ogs_error("emm_build_tau_accept() failed");
        return OGS_ERROR;
    }

    CLEAR_MME_UE_TIMER(mme_ue->t3450);
    mme_ue->t3450.pkbuf = ogs_pkbuf_copy(emmbuf);
    if (!mme_ue->t3450.pkbuf) {
        ogs_error("ogs_pkbuf_copy(mme_ue->t3450.pkbuf) failed");
        ogs_pkbuf_free(emmbuf);
        return OGS_ERROR;
    }
    ogs_timer_start(mme_ue->t3450.timer,
            mme_timer_cfg(MME_TIMER_T3450)->duration);

    if (procedureCode == S1AP_ProcedureCode_id_InitialContextSetup) {
        ogs_pkbuf_t *s1apbuf = NULL;
        s1apbuf = s1ap_build_initial_context_setup_request(mme_ue, emmbuf);
        if (!s1apbuf) {
            ogs_error("s1ap_build_initial_context_setup_request() failed");
            return OGS_ERROR;
        }

        rv = nas_eps_send_to_enb(mme_ue, s1apbuf);
        ogs_expect(rv == OGS_OK);
    } else if (procedureCode == S1AP_ProcedureCode_id_downlinkNASTransport) {
        rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
        ogs_expect(rv == OGS_OK);
    } else
        ogs_assert_if_reached();

    return rv;
}

int nas_eps_send_tau_reject(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue, ogs_nas_emm_cause_t emm_cause)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Tracking area update reject", mme_ue->imsi_bcd);

    /* Build TAU reject */
    emmbuf = emm_build_tau_reject(emm_cause, mme_ue);
    if (!emmbuf) {
        ogs_error("emm_build_tau_reject() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_service_reject(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue, ogs_nas_emm_cause_t emm_cause)
{
    int rv;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] Service reject", mme_ue->imsi_bcd);

    /* Build Service Reject */
    emmbuf = emm_build_service_reject(emm_cause, mme_ue);
    if (!emmbuf) {
        ogs_error("emm_build_service_reject() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_cs_service_notification(mme_ue_t *mme_ue)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_debug("[%s] CS Service Notification", mme_ue->imsi_bcd);

    emmbuf = emm_build_cs_service_notification(mme_ue);
    if (!emmbuf) {
        ogs_error("emm_build_cs_service_notification() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int nas_eps_send_downlink_nas_transport(
        mme_ue_t *mme_ue, uint8_t *buffer, uint8_t length)
{
    int rv;
    enb_ue_t *enb_ue = NULL;
    ogs_pkbuf_t *emmbuf = NULL;

    if (!mme_ue) {
        ogs_error("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    if (!enb_ue) {
        ogs_error("S1 context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_assert(buffer);
    ogs_assert(length);

    ogs_debug("[%s] Downlink NAS transport", mme_ue->imsi_bcd);

    emmbuf = emm_build_downlink_nas_transport(mme_ue, buffer, length);
    if (!emmbuf) {
        ogs_error("emm_build_downlink_nas_transport() failed");
        return OGS_ERROR;
    }

    rv = nas_eps_send_to_downlink_nas_transport(enb_ue, emmbuf);
    ogs_expect(rv == OGS_OK);

    return rv;
}
