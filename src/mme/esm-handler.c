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

#include "mme-context.h"
#include "nas-path.h"
#include "sgsap-path.h"
#include "mme-gtp-path.h"

#include "esm-build.h"
#include "esm-handler.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __esm_log_domain

int esm_handle_pdn_connectivity_request(mme_bearer_t *bearer, 
        ogs_nas_eps_pdn_connectivity_request_t *req, int create_action)
{
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    uint8_t security_protected_required = 0;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_assert(req);

    ogs_assert(MME_UE_HAVE_IMSI(mme_ue));
    ogs_assert(SECURITY_CONTEXT_IS_VALID(mme_ue));

    memcpy(&sess->request_type, &req->request_type, sizeof(sess->request_type));

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

    if (req->presencemask &
            OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT) {
        sess->session = mme_session_find_by_apn(
                            mme_ue, req->access_point_name.apn);
        if (!sess->session) {
            /* Invalid APN */
            ogs_assert(OGS_OK ==
                nas_eps_send_pdn_connectivity_reject(
                    sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN, create_action));
            ogs_warn("Invalid APN[%s]", req->access_point_name.apn);
            return OGS_ERROR;
        }

        if (sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV4 ||
            sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV6 ||
            sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
            uint8_t derived_pdn_type =
                (sess->session->session_type & sess->request_type.type);
            if (derived_pdn_type == 0) {
                ogs_error("Cannot derived PDN Type [UE:%d,HSS:%d]",
                    sess->request_type.type, sess->session->session_type);
                ogs_assert(OGS_OK ==
                    nas_eps_send_pdn_connectivity_reject(
                        sess, ESM_CAUSE_UNKNOWN_PDN_TYPE, create_action));
                return OGS_ERROR;
            }
        } else {
            ogs_fatal("Invalid PDN_TYPE[%d]", sess->session->session_type);
            ogs_assert_if_reached();
        }
    }

    if (req->presencemask &
        OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        ogs_nas_protocol_configuration_options_t
            *protocol_configuration_options = 
            &req->protocol_configuration_options;

        OGS_NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
    }

    if (security_protected_required) {
        CLEAR_BEARER_TIMER(bearer->t3489);
        ogs_assert(OGS_OK == nas_eps_send_esm_information_request(bearer));

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
            mme_gtp_send_create_session_request(sess, create_action));
    } else {
        ogs_error("No APN");
        ogs_assert(OGS_OK ==
            nas_eps_send_pdn_connectivity_reject(
                sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN, create_action));
        return OGS_ERROR;
    }

    return OGS_OK;
}

int esm_handle_information_response(mme_sess_t *sess, 
        ogs_nas_eps_esm_information_response_t *rsp)
{
    mme_ue_t *mme_ue = NULL;

    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_assert(rsp);

    if (rsp->presencemask &
            OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT) {
        sess->session = mme_session_find_by_apn(
                            mme_ue, rsp->access_point_name.apn);
    }

    if (rsp->presencemask &
        OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        ogs_nas_protocol_configuration_options_t
            *protocol_configuration_options = 
                &rsp->protocol_configuration_options;
        OGS_NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
    }

    if (sess->session) {
        ogs_assert(sess->session->name);
        ogs_debug("    APN[%s]", sess->session->name);

        if (sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV4 ||
            sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV6 ||
            sess->session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
            uint8_t derived_pdn_type =
                (sess->session->session_type & sess->request_type.type);
            if (derived_pdn_type == 0) {
                ogs_error("Cannot derived PDN Type [UE:%d,HSS:%d]",
                    sess->request_type.type, sess->session->session_type);
                ogs_assert(OGS_OK ==
                    nas_eps_send_pdn_connectivity_reject(
                        sess, ESM_CAUSE_UNKNOWN_PDN_TYPE,
                        OGS_GTP_CREATE_IN_ATTACH_REQUEST));
                return OGS_ERROR;
            }
        } else {
            ogs_fatal("Invalid PDN_TYPE[%d]", sess->session->session_type);
            ogs_assert_if_reached();
        }

        if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue) &&
            OGS_PDU_SESSION_TYPE_IS_VALID(sess->session->paa.session_type)) {
            mme_csmap_t *csmap = mme_csmap_find_by_tai(&mme_ue->tai);
            mme_ue->csmap = csmap;

            if (csmap) {
                ogs_assert(OGS_OK ==
                    sgsap_send_location_update_request(mme_ue));
            } else {
                ogs_assert(OGS_OK ==
                    nas_eps_send_attach_accept(mme_ue));
            }
        } else {
            ogs_assert(OGS_OK ==
                mme_gtp_send_create_session_request(
                    sess, OGS_GTP_CREATE_IN_ATTACH_REQUEST));
        }
    } else {
        if (rsp->access_point_name.length)
            ogs_error("Invalid APN[%s]", rsp->access_point_name.apn);
        else
            ogs_error("No APN");

        ogs_assert(OGS_OK ==
            nas_eps_send_pdn_connectivity_reject(
                sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN,
                OGS_GTP_CREATE_IN_ATTACH_REQUEST));
        return OGS_ERROR;
    }

    return OGS_OK;
}

int esm_handle_bearer_resource_allocation_request(
        mme_bearer_t *bearer, ogs_nas_eps_message_t *message)
{
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_assert(OGS_OK ==
        nas_eps_send_bearer_resource_allocation_reject(
            mme_ue, sess->pti, ESM_CAUSE_NETWORK_FAILURE));

    return OGS_OK;
}

int esm_handle_bearer_resource_modification_request(
        mme_bearer_t *bearer, ogs_nas_eps_message_t *message)
{
    mme_ue_t *mme_ue = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    ogs_assert(OGS_OK ==
        mme_gtp_send_bearer_resource_command(bearer, message));

    return OGS_OK;
}
