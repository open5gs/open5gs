/* smf/s8-handler.c */
#include "s8-handler.h"

void smf_s8_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF S8] Received Echo Request");
    /* Send Echo Response */
    ogs_gtp2_send_echo_response(xact, req->recovery.u8, 0);
}

uint8_t smf_s8_handle_create_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_create_session_request_t *req)
{
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];
    uint8_t cause_value = 0;
    ogs_gtp2_f_teid_t *sgw_s8c_teid = NULL;
    ogs_gtp2_f_teid_t *sgw_s8u_teid = NULL;
    smf_bearer_t *bearer = NULL;
    ogs_paa_t *paa = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF S8] Create Session Request");

    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    /* Validate mandatory IEs */
    if (req->imsi.presence == 0) {
        ogs_error("S8 No IMSI");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        goto out;
    }
    if (req->sender_f_teid_for_control_plane.presence == 0) {
        ogs_error("S8 No Sender F-TEID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        goto out;
    }

    /* Store SGW S8-C F-TEID */
    sgw_s8c_teid = req->sender_f_teid_for_control_plane.data;
    sess->sgw_s8_teid = be32toh(sgw_s8c_teid->teid);

    /* Create default bearer */
    bearer = smf_bearer_add(sess);
    if (!bearer) {
        ogs_error("S8 Cannot create bearer");
        cause_value = OGS_GTP2_CAUSE_SYSTEM_FAILURE;
        goto out;
    }

    /* Allocate PAA (IP address) */
    paa = &sess->paa;
    if (req->pdn_address_allocation.presence) {
        /* Handle PAA from request */
        memcpy(paa, req->pdn_address_allocation.data,
               req->pdn_address_allocation.len);
    } else {
        /* Allocate new IP address */
        if (smf_sess_allocate_ip(sess) != OGS_OK) {
            ogs_error("S8 IP allocation failed");
            cause_value = OGS_GTP2_CAUSE_ALL_DYNAMIC_ADDRESSES_OCCUPIED;
            goto out;
        }
    }

    /* Setup GTP-U endpoint */
    if (req->bearer_contexts_to_be_created[0].s5_s8_u_sgw_f_teid.presence) {
        sgw_s8u_teid = req->bearer_contexts_to_be_created[0].
                           s5_s8_u_sgw_f_teid.data;
        bearer->sgw_s8u_teid = be32toh(sgw_s8u_teid->teid);
    }

out:
    smf_s8_send_create_session_response(sess, xact, cause_value);
    return cause_value;
}

/* smf/s8-handler.c (continued) */

uint8_t smf_s8_handle_delete_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_delete_session_request_t *req)
{
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Delete Session Request");

    /* Check if there are active bearers */
    if (ogs_list_count(&sess->bearer_list)) {
        ogs_list_t *list = &sess->bearer_list;
        ogs_list_t remove_list;
        smf_bearer_t *bearer = NULL, *next_bearer = NULL;

        ogs_list_init(&remove_list);
        ogs_list_for_each_safe(list, next_bearer, bearer) {
            ogs_list_add(&remove_list, &bearer->node);
        }

        /* Remove all bearers */
        ogs_list_for_each_safe(&remove_list, next_bearer, bearer) {
            smf_bearer_remove(bearer);
        }
    }

    /* Release IP address */
    smf_sess_free_ip(sess);

    /* Send Delete Session Response */
    smf_s8_send_delete_session_response(sess, xact, cause_value);

    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        SMF_SESS_CLEAR(sess);
    }

    return cause_value;
}

void smf_s8_handle_modify_bearer_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_modify_bearer_request_t *req)
{
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    smf_bearer_t *bearer = NULL;
    ogs_gtp2_f_teid_t *sgw_s8u_teid = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SMF] Modify Bearer Request");

    if (req->bearer_contexts_to_be_modified[0].presence) {
        bearer = smf_bearer_find_by_ebi(sess,
            req->bearer_contexts_to_be_modified[0].eps_bearer_id.u8);
        if (!bearer) {
            ogs_error("No Bearer Context");
            cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
        }
    } else {
        ogs_error("No Bearer Context to be modified");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (bearer && req->bearer_contexts_to_be_modified[0].s5_s8_u_sgw_f_teid.presence) {
        sgw_s8u_teid = req->bearer_contexts_to_be_modified[0].s5_s8_u_sgw_f_teid.data;
        bearer->sgw_s8u_teid = be32toh(sgw_s8u_teid->teid);

        /* Update GTP-U path if needed */
        if (bearer->pgw_s8u_teid)
            smf_bearer_update_gtp_path(bearer);
    }

    smf_s8_send_modify_bearer_response(sess, xact, cause_value);
}
uint8_t smf_s8_handle_bearer_resource_command(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_bearer_resource_command_t *cmd)
{
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    smf_bearer_t *bearer = NULL;
    ogs_gtp2_tft_t *tft = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(cmd);

    ogs_debug("[SMF] Bearer Resource Command");

    if (cmd->traffic_aggregate_description.presence == 0) {
        ogs_error("No TAD");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        goto error;
    }

    if (cmd->procedure_transaction_id.presence == 0) {
        ogs_error("No PTI");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        goto error;
    }

    /* Create new dedicated bearer */
    bearer = smf_bearer_add(sess);
    if (!bearer) {
        ogs_error("No memory for bearer");
        cause_value = OGS_GTP2_CAUSE_SYSTEM_FAILURE;
        goto error;
    }

    /* Set QoS */
    if (cmd->flow_quality_of_service.presence) {
        memcpy(&bearer->qos, cmd->flow_quality_of_service.data,
               cmd->flow_quality_of_service.len);
    }

    /* Set TFT */
    tft = &cmd->traffic_aggregate_description;
    if (tft->presence) {
        bearer->tft = ogs_calloc(1, sizeof(ogs_gtp2_tft_t));
        memcpy(bearer->tft, tft->data, tft->len);
    }

    /* Send Create Bearer Request to SGW */
    smf_s8_send_create_bearer_request(
        bearer, cmd->procedure_transaction_id.u8, bearer->tft);

    return cause_value;

    error:
        smf_s8_send_bearer_resource_failure_indication(
            sess, xact, cmd->procedure_transaction_id.u8, cause_value);
    return cause_value;
}


void smf_s8_handle_create_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_create_bearer_response_t *rsp)
{
    smf_bearer_t *bearer = NULL;
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[SMF] Create Bearer Response");

    bearer = xact->data;
    ogs_assert(bearer);

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer Context");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        goto cleanup;
    }

    if (rsp->bearer_contexts.s5_s8_u_sgw_f_teid.presence) {
        /* Store SGW S8-U F-TEID */
        ogs_gtp2_f_teid_t *sgw_s8u_teid =
            rsp->bearer_contexts.s5_s8_u_sgw_f_teid.data;
        bearer->sgw_s8u_teid = be32toh(sgw_s8u_teid->teid);

        /* Update GTP-U path */
        smf_bearer_update_gtp_path(bearer);
    }

    /* Update PCC rule status */
    if (bearer->pcc_rule.name)
        smf_pcc_rule_update(sess, bearer);

    cleanup:
        ogs_gtp_xact_commit(xact);
}

/* Add to smf/s8-handler.c */

void smf_s8_handle_update_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_update_bearer_response_t *rsp)
{
    smf_bearer_t *bearer = NULL;
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[SMF] Update Bearer Response");

    bearer = xact->data;
    ogs_assert(bearer);

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer Context");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        goto cleanup;
    }

    if (rsp->bearer_contexts.cause.presence) {
        ogs_gtp2_cause_t *cause = rsp->bearer_contexts.cause.data;
        cause_value = cause->value;
        if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("Bearer update failed with cause %d", cause_value);
    }

    /* Update PCC rule status */
    if (bearer->pcc_rule.name)
        smf_pcc_rule_update(sess, bearer);

    cleanup:
        ogs_gtp_xact_commit(xact);
}

/* Add to smf/s8-handler.c */

void smf_s8_handle_delete_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_delete_bearer_response_t *rsp)
{
    smf_bearer_t *bearer = NULL;
    uint8_t cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[SMF] Delete Bearer Response");

    bearer = xact->data;
    ogs_assert(bearer);

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer Context");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        goto cleanup;
    }

    if (rsp->bearer_contexts.cause.presence) {
        ogs_gtp2_cause_t *cause = rsp->bearer_contexts.cause.data;
        cause_value = cause->value;
        if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("Bearer deletion failed with cause %d", cause_value);
    }

    /* Remove the bearer */
    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED)
        smf_bearer_remove(bearer);

    cleanup:
        ogs_gtp_xact_commit(xact);
}