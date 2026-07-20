/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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
#include "nas-path.h"
#include "sgsap-path.h"
#include "mme-gtp-path.h"

#include "esm-build.h"
#include "esm-handler.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __esm_log_domain

/*
 * Check whether the PDN Type requested by the UE is compatible with
 * the PDN Type provided by the subscription (HSS).
 *
 * This check MUST be performed before building Create Session Request;
 * mme_s11_build_create_session_request() asserts on an incompatible
 * PDN Type, so an unchecked UE request would crash the MME.
 *
 * Returns 0 if compatible, otherwise the ESM cause to be used
 * in PDN CONNECTIVITY REJECT.
 */
static ogs_nas_esm_cause_t esm_check_pdn_type(mme_sess_t *sess)
{
    uint8_t derived_pdn_type;

    ogs_assert(sess);
    ogs_assert(sess->session);

    if (sess->session->session_type != OGS_PDU_SESSION_TYPE_IPV4 &&
        sess->session->session_type != OGS_PDU_SESSION_TYPE_IPV6 &&
        sess->session->session_type != OGS_PDU_SESSION_TYPE_IPV4V6) {
        /* Subscription data is broken - this is not UE input */
        ogs_fatal("Invalid PDN_TYPE[%d]", sess->session->session_type);
        ogs_assert_if_reached();
    }

    derived_pdn_type =
        (sess->session->session_type & sess->ue_request_type.type);
    if (derived_pdn_type == 0) {
        ogs_error("Incompatible PDN Type [UE:%d,HSS:%d]",
                sess->ue_request_type.type, sess->session->session_type);
        if (sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV4)
            return OGS_NAS_ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED;
        else if (sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV6)
            return OGS_NAS_ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED;
        else
            return OGS_NAS_ESM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED;
    }

    return 0;
}

int esm_handle_pdn_connectivity_request(
        enb_ue_t *enb_ue, mme_bearer_t *bearer,
        ogs_nas_eps_pdn_connectivity_request_t *req,
        int create_action)
{
    int r;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    ogs_nas_esm_cause_t esm_cause;
    uint8_t security_protected_required = 0;
    const char *emergency_dnn = mme_self()->emergency.dnn;
    bool emergency;

    ogs_assert(req);

    if (!bearer) {
        ogs_error("No bearer context");
        return OGS_NOTFOUND;
    }
    sess = mme_sess_find_by_id(bearer->sess_id);
    if (!sess) {
        ogs_warn("Session context has already been removed");
        return OGS_NOTFOUND;
    }
    mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    if (!mme_ue) {
        ogs_warn("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_assert(MME_UE_HAVE_IMSI(mme_ue));

    if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        ogs_error("No Security Context : IMSI[%s]", mme_ue->imsi_bcd);
        r = nas_eps_send_pdn_connectivity_reject(
                sess, OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                create_action);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (req->request_type.type == OGS_NAS_EPS_PDN_TYPE_IPV4 ||
        req->request_type.type == OGS_NAS_EPS_PDN_TYPE_IPV6 ||
        req->request_type.type == OGS_NAS_EPS_PDN_TYPE_IPV4V6) {
        /* OK */
    } else {
        /* NOT-allowed PDN Type */
        r = nas_eps_send_pdn_connectivity_reject(
                sess, OGS_NAS_ESM_CAUSE_UNKNOWN_PDN_TYPE,
                create_action);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    memcpy(&sess->ue_request_type,
            &req->request_type, sizeof(sess->ue_request_type));

    security_protected_required = 0;
    if (req->presencemask &
        OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT) {
        ogs_nas_esm_information_transfer_flag_t *esm_information_transfer_flag =
            &req->esm_information_transfer_flag;
        security_protected_required = 
                esm_information_transfer_flag->security_protected_required;
        ogs_debug("    EIT(ESM information transfer)[%d]",
                security_protected_required);
    }

    emergency = (req->request_type.value == OGS_NAS_EPS_REQUEST_TYPE_EMERGENCY);
    if (emergency && !emergency_dnn) {
        /* Emergency call, but no emergency APN defined */
        r = nas_eps_send_pdn_connectivity_reject(
                sess, OGS_NAS_ESM_CAUSE_REQUEST_REJECTED_UNSPECIFIED, create_action);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        ogs_warn("Emergency call, but no emergency APN defined");
        return OGS_ERROR;
    }
    if ((req->presencemask &
            OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT) ||
        emergency) {
        const char *apn;
        if (emergency) {
            apn = emergency_dnn;
            sess->ue_request_type.value = 1;
        } else {
            apn = req->access_point_name.apn;
        }
        sess->session = mme_session_find_by_apn(mme_ue, apn);
        if (!sess->session) {
            /* Invalid APN */
            r = nas_eps_send_pdn_connectivity_reject(
                    sess, OGS_NAS_ESM_CAUSE_MISSING_OR_UNKNOWN_APN,
                    create_action);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            ogs_warn("Invalid APN[%s]", apn);
            return OGS_ERROR;
        }
    }

    if (req->presencemask &
        OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        ogs_nas_extended_protocol_configuration_options_t
            *extended_protocol_configuration_options =
            &req->extended_protocol_configuration_options;

        OGS_NAS_STORE_DATA(&sess->ue_epco,
            extended_protocol_configuration_options);
    } else if (req->presencemask &
        OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        ogs_nas_protocol_configuration_options_t
            *protocol_configuration_options =
            &req->protocol_configuration_options;

        OGS_NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
    }

    if (security_protected_required) {
        if (sess->session) {
            esm_cause = esm_check_pdn_type(sess);
            if (esm_cause) {
                r = nas_eps_send_pdn_connectivity_reject(
                        sess, esm_cause, create_action);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return OGS_ERROR;
            }
        }

        CLEAR_BEARER_TIMER(bearer->t3489);
        r = nas_eps_send_esm_information_request(bearer);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        return OGS_OK;
    }

    if (!sess->session) {
        /* Default APN */
        sess->session = mme_default_session(mme_ue);
    }

    if (sess->session) {
        mme_bearer_t *default_bearer = NULL;
        mme_bearer_t *dedicated_bearer = NULL, *next_dedicated_bearer = NULL;

        ogs_assert(sess->session->name);
        ogs_debug("    APN[%s]", sess->session->name);

        /*
         * The PDN Type of the Default APN has not been validated
         * against the UE requested PDN Type. Check it here before
         * building Create Session Request; otherwise an incompatible
         * combination would hit the assert in
         * mme_s11_build_create_session_request() and crash the MME.
         */
        esm_cause = esm_check_pdn_type(sess);
        if (esm_cause) {
            r = nas_eps_send_pdn_connectivity_reject(
                    sess, esm_cause, create_action);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        default_bearer = mme_default_bearer_in_sess(sess);
        if (default_bearer) {
            dedicated_bearer = mme_bearer_next(default_bearer);
            while (dedicated_bearer) {
                next_dedicated_bearer = mme_bearer_next(dedicated_bearer);

                ogs_warn("Dedicated-Bearer[%d] removed forcely",
                        dedicated_bearer->ebi);
                mme_bearer_remove(dedicated_bearer);

                dedicated_bearer = next_dedicated_bearer;
            }
        }

        ogs_assert(OGS_OK ==
            mme_gtp_send_create_session_request(enb_ue, sess, create_action));
    } else {
        ogs_error("No APN");
        r = nas_eps_send_pdn_connectivity_reject(
                sess, OGS_NAS_ESM_CAUSE_MISSING_OR_UNKNOWN_APN, create_action);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int esm_handle_information_response(
        enb_ue_t *enb_ue, mme_sess_t *sess,
        ogs_nas_eps_esm_information_response_t *rsp)
{
    int r;
    mme_ue_t *mme_ue = NULL;
    ogs_nas_esm_cause_t esm_cause;

    ogs_assert(rsp);

    if (!sess) {
        ogs_warn("Session context has already been removed");
        return OGS_NOTFOUND;
    }
    mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    if (!mme_ue) {
        ogs_warn("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    if (rsp->presencemask &
            OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT) {
        sess->session = mme_session_find_by_apn(
                            mme_ue, rsp->access_point_name.apn);
    }

    if (rsp->presencemask &
        OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        ogs_nas_extended_protocol_configuration_options_t
            *extended_protocol_configuration_options =
            &rsp->extended_protocol_configuration_options;

        OGS_NAS_STORE_DATA(&sess->ue_epco,
            extended_protocol_configuration_options);
    } else if (rsp->presencemask &
        OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        ogs_nas_protocol_configuration_options_t
            *protocol_configuration_options =
                &rsp->protocol_configuration_options;
        OGS_NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
    }

    if (sess->session) {
        ogs_assert(sess->session->name);
        ogs_debug("    APN[%s]", sess->session->name);

        esm_cause = esm_check_pdn_type(sess);
        if (esm_cause) {
            r = nas_eps_send_pdn_connectivity_reject(
                    sess, esm_cause, OGS_GTP_CREATE_IN_ATTACH_REQUEST);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue)) {
            mme_csmap_t *csmap = mme_csmap_find_by_tai(&mme_ue->tai);
            mme_ue->csmap = csmap;

            if (!csmap ||
                mme_ue->network_access_mode ==
                    OGS_NETWORK_ACCESS_MODE_ONLY_PACKET ||
                mme_ue->nas_eps.attach.value ==
                    OGS_NAS_ATTACH_TYPE_EPS_ATTACH) {
                r = nas_eps_send_attach_accept(mme_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                ogs_assert(OGS_OK ==
                    sgsap_send_location_update_request(mme_ue));
            }
        } else {
            ogs_assert(OGS_OK ==
                mme_gtp_send_create_session_request(
                    enb_ue, sess, OGS_GTP_CREATE_IN_ATTACH_REQUEST));
        }
    } else {
        if (rsp->access_point_name.length)
            ogs_error("Invalid APN[%s]", rsp->access_point_name.apn);
        else
            ogs_error("No APN");

        r = nas_eps_send_pdn_connectivity_reject(
                sess, OGS_NAS_ESM_CAUSE_MISSING_OR_UNKNOWN_APN,
                OGS_GTP_CREATE_IN_ATTACH_REQUEST);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int esm_handle_bearer_resource_allocation_request(
        enb_ue_t *enb_ue, mme_bearer_t *bearer, ogs_nas_eps_message_t *message)
{
    int r;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;

    if (!bearer) {
        ogs_error("No bearer context");
        return OGS_NOTFOUND;
    }
    sess = mme_sess_find_by_id(bearer->sess_id);
    if (!sess) {
        ogs_warn("Session context has already been removed");
        return OGS_NOTFOUND;
    }
    mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    if (!mme_ue) {
        ogs_warn("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    r = nas_eps_send_bearer_resource_allocation_reject(
            mme_ue, sess->pti, OGS_NAS_ESM_CAUSE_NETWORK_FAILURE);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return OGS_OK;
}

int esm_handle_bearer_resource_modification_request(
        enb_ue_t *enb_ue, mme_bearer_t *bearer, ogs_nas_eps_message_t *message)
{
    mme_ue_t *mme_ue = NULL;

    if (!bearer) {
        ogs_error("No bearer context");
        return OGS_NOTFOUND;
    }
    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    if (!mme_ue) {
        ogs_warn("UE(mme-ue) context has already been removed");
        return OGS_NOTFOUND;
    }

    ogs_assert(OGS_OK ==
        mme_gtp_send_bearer_resource_command(bearer, message));

    return OGS_OK;
}
