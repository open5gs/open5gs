/*
 * Copyright (C) 2026 by Open5GS contributors
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
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "migration.h"
#include "gsm-build.h"
#include "ngap-build.h"
#include "pfcp-path.h"
#include "sbi-path.h"
#include "sbi/openapi/external/cJSON.h"

static bool is_action(const cJSON *action, const char *name)
{
    ogs_assert(name);

    return action && cJSON_IsString(action) && action->valuestring &&
        strcmp(action->valuestring, name) == 0;
}

static char *migration_node_string(ogs_pfcp_node_t *node);
static void migration_metrics_finish(
        smf_sess_t *sess, smf_metric_type_global_t counter);

static size_t migration_json_finalize(cJSON *root, char *buf, size_t buflen)
{
    char *tmp = NULL;
    size_t len;

    ogs_assert(root);
    ogs_assert(buf);

    if (buflen == 0) {
        cJSON_Delete(root);
        return 0;
    }

    if (cJSON_PrintPreallocated(root, buf, (int)buflen, 0)) {
        len = strlen(buf);
        cJSON_Delete(root);
        return len;
    }

    tmp = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (!tmp) {
        if (buflen >= 3) {
            memcpy(buf, "{}", 3);
            return 2;
        }
        buf[0] = '\0';
        return 0;
    }

    len = strlen(tmp);
    if (len >= buflen)
        len = buflen - 1;

    memcpy(buf, tmp, len);
    buf[len] = '\0';
    cJSON_free(tmp);

    return len;
}

static void migration_json_add_string(
        cJSON *root, const char *name, const char *value)
{
    cJSON *item = NULL;

    ogs_assert(root);
    ogs_assert(name);

    item = cJSON_CreateString(value ? value : "");
    if (item) {
        if (!cJSON_ReplaceItemInObjectCaseSensitive(root, name, item))
            cJSON_AddItemToObjectCS(root, name, item);
    }
}

static void migration_json_add_number(
        cJSON *root, const char *name, double value)
{
    cJSON *item = NULL;

    ogs_assert(root);
    ogs_assert(name);

    item = cJSON_CreateNumber(value);
    if (item) {
        if (!cJSON_ReplaceItemInObjectCaseSensitive(root, name, item))
            cJSON_AddItemToObjectCS(root, name, item);
    }
}

static int migration_ue_ip_string(smf_sess_t *sess, char *buf, size_t len)
{
    ogs_assert(sess);
    ogs_assert(buf);
    ogs_assert(len);

    buf[0] = '\0';

    if (sess->ipv4) {
        OGS_INET_NTOP(&sess->ipv4->addr, buf);
        return OGS_OK;
    }

    if (sess->ipv6) {
        OGS_INET6_NTOP(&sess->ipv6->addr, buf);
        return OGS_OK;
    }

    return OGS_ERROR;
}

static int migration_run_route_hook(
        smf_sess_t *sess, const char *action, const char *owner)
{
    smf_ue_t *smf_ue = NULL;
    ogs_proc_t proc;
    int rv, exit_code = 0;
    char psi[8];
    char ue_ip[OGS_ADDRSTRLEN];
    char *source_upf = NULL;
    char *target_upf = NULL;
    const char *argv[9];

    ogs_assert(sess);
    ogs_assert(action);
    ogs_assert(owner);

    if (!smf_self()->migration.route_hook) {
        ogs_warn("[MIGRATE] no smf.migration.route_hook configured; "
                "skipping N6 route hook [%s:%s]", action, owner);
        return OGS_OK;
    }

    if (migration_ue_ip_string(sess, ue_ip, sizeof(ue_ip)) != OGS_OK) {
        ogs_error("[MIGRATE] no UE IP for route hook");
        return OGS_ERROR;
    }

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_snprintf(psi, sizeof(psi), "%u", sess->psi);

    source_upf = migration_node_string(sess->migration.source_node);
    target_upf = migration_node_string(sess->migration.target_node);
    ogs_assert(source_upf);
    ogs_assert(target_upf);

    argv[0] = smf_self()->migration.route_hook;
    argv[1] = action;
    argv[2] = smf_ue ? smf_ue->supi : "";
    argv[3] = psi;
    argv[4] = ue_ip;
    argv[5] = source_upf;
    argv[6] = target_upf;
    argv[7] = owner;
    argv[8] = NULL;

    memset(&proc, 0, sizeof(proc));
    rv = ogs_proc_create(argv,
            ogs_proc_option_inherit_environment |
            ogs_proc_option_combined_stdout_stderr,
            &proc);
    if (rv != OGS_OK) {
        ogs_error("[MIGRATE] route hook create failed [%s]",
                smf_self()->migration.route_hook);
        ogs_free(source_upf);
        ogs_free(target_upf);
        return OGS_ERROR;
    }

    rv = ogs_proc_join(&proc, &exit_code);
    if (rv == OGS_OK)
        ogs_proc_destroy(&proc);

    ogs_free(source_upf);
    ogs_free(target_upf);

    if (rv != OGS_OK || exit_code != 0) {
        ogs_error("[MIGRATE] route hook [%s:%s] failed rv[%d] exit[%d]",
                action, owner, rv, exit_code);
        return OGS_ERROR;
    }

    ogs_info("[MIGRATE] route hook [%s:%s] completed", action, owner);

    return OGS_OK;
}

static char *migration_node_string(ogs_pfcp_node_t *node)
{
    char *node_string = NULL;

    if (node && node->addr_list)
        node_string = ogs_sockaddr_to_string_static(node->addr_list);

    return ogs_strdup(node_string ? node_string : "");
}

const char *smf_migration_state_name(smf_migration_state_e state)
{
    switch (state) {
    case SMF_MIGRATION_STATE_IDLE:
        return "idle";
    case SMF_MIGRATION_STATE_PRECHECK:
        return "precheck";
    case SMF_MIGRATION_STATE_TARGET_PREPARING:
        return "target_preparing";
    case SMF_MIGRATION_STATE_TARGET_READY:
        return "target_ready";
    case SMF_MIGRATION_STATE_ROUTE_PROGRAMMING:
        return "route_programming";
    case SMF_MIGRATION_STATE_PATH_SWITCHING:
        return "path_switching";
    case SMF_MIGRATION_STATE_SWITCH_CONFIRMED:
        return "switch_confirmed";
    case SMF_MIGRATION_STATE_SOURCE_DRAINING:
        return "source_draining";
    case SMF_MIGRATION_STATE_COMPLETED:
        return "completed";
    case SMF_MIGRATION_STATE_ABORTING:
        return "aborting";
    case SMF_MIGRATION_STATE_ROLLED_BACK:
        return "rolled_back";
    case SMF_MIGRATION_STATE_FAILED:
        return "failed";
    default:
        return "unknown";
    }
}

bool smf_migration_active(const smf_sess_t *sess)
{
    if (!sess)
        return false;

    switch (sess->migration.state) {
    case SMF_MIGRATION_STATE_PRECHECK:
    case SMF_MIGRATION_STATE_TARGET_PREPARING:
    case SMF_MIGRATION_STATE_TARGET_READY:
    case SMF_MIGRATION_STATE_ROUTE_PROGRAMMING:
    case SMF_MIGRATION_STATE_PATH_SWITCHING:
    case SMF_MIGRATION_STATE_SWITCH_CONFIRMED:
    case SMF_MIGRATION_STATE_SOURCE_DRAINING:
    case SMF_MIGRATION_STATE_ABORTING:
        return true;
    default:
        return false;
    }
}

void smf_migration_clear(smf_sess_t *sess)
{
    ogs_assert(sess);

    if (sess->migration.metrics_active)
        smf_metrics_inst_global_dec(SMF_METR_GLOB_GAUGE_SMF_MIGRATIONS_ACTIVE);

    if (sess->migration.target_local_dl_addr)
        ogs_freeaddrinfo(sess->migration.target_local_dl_addr);
    if (sess->migration.target_local_dl_addr6)
        ogs_freeaddrinfo(sess->migration.target_local_dl_addr6);
    if (sess->migration.target_local_ul_addr)
        ogs_freeaddrinfo(sess->migration.target_local_ul_addr);
    if (sess->migration.target_local_ul_addr6)
        ogs_freeaddrinfo(sess->migration.target_local_ul_addr6);

    memset(&sess->migration, 0, sizeof(sess->migration));
}

static void migration_metrics_finish(
        smf_sess_t *sess, smf_metric_type_global_t counter)
{
    ogs_assert(sess);

    if (sess->migration.metrics_active) {
        smf_metrics_inst_global_dec(SMF_METR_GLOB_GAUGE_SMF_MIGRATIONS_ACTIVE);
        sess->migration.metrics_active = false;
    }

    smf_metrics_inst_global_inc(counter);
}

void smf_migration_mark_failed(smf_sess_t *sess)
{
    ogs_assert(sess);

    migration_metrics_finish(sess, SMF_METR_GLOB_CTR_SMF_MIGRATION_FAILED);
    sess->migration.state = SMF_MIGRATION_STATE_FAILED;
}

static void migration_replace_sockaddr(
        ogs_sockaddr_t **dst, ogs_sockaddr_t **src)
{
    ogs_assert(dst);
    ogs_assert(src);

    if (*dst)
        ogs_freeaddrinfo(*dst);
    *dst = *src;
    *src = NULL;
}

static void migration_update_pdr_f_teid(
        smf_sess_t *sess, ogs_pfcp_interface_t src_if,
        ogs_pfcp_interface_t dst_if,
        ogs_sockaddr_t *addr, ogs_sockaddr_t *addr6, uint32_t teid)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(sess);

    if (!teid || (!addr && !addr6))
        return;

    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        if (pdr->src_if != src_if)
            continue;

        far = pdr->far;
        if (!far || far->dst_if != dst_if)
            continue;

        ogs_assert(OGS_OK == ogs_pfcp_sockaddr_to_f_teid(
                    addr, addr6, &pdr->f_teid, &pdr->f_teid_len));
        pdr->f_teid.teid = teid;
    }
}

static int migration_commit_target(smf_sess_t *sess)
{
    ogs_assert(sess);

    if (sess->migration.state != SMF_MIGRATION_STATE_PATH_SWITCHING) {
        ogs_error("Migration commit invalid state [%s]",
                smf_migration_state_name(sess->migration.state));
        return OGS_ERROR;
    }

    if (!sess->migration.target_node ||
        !sess->migration.target_upf_n4_seid ||
        !sess->migration.target_local_ul_teid ||
        (!sess->migration.target_local_ul_addr &&
         !sess->migration.target_local_ul_addr6)) {
        ogs_error("Migration target state is incomplete");
        smf_migration_mark_failed(sess);
        return OGS_ERROR;
    }

    migration_update_pdr_f_teid(sess, OGS_PFCP_INTERFACE_ACCESS,
            OGS_PFCP_INTERFACE_CORE,
            sess->migration.target_local_ul_addr,
            sess->migration.target_local_ul_addr6,
            sess->migration.target_local_ul_teid);
    migration_update_pdr_f_teid(sess, OGS_PFCP_INTERFACE_CORE,
            OGS_PFCP_INTERFACE_ACCESS,
            sess->migration.target_local_dl_addr,
            sess->migration.target_local_dl_addr6,
            sess->migration.target_local_dl_teid);

    OGS_SETUP_PFCP_NODE(sess, sess->migration.target_node);
    sess->upf_n4_seid = sess->migration.target_upf_n4_seid;

    sess->local_ul_teid = sess->migration.target_local_ul_teid;
    migration_replace_sockaddr(
            &sess->local_ul_addr, &sess->migration.target_local_ul_addr);
    migration_replace_sockaddr(
            &sess->local_ul_addr6, &sess->migration.target_local_ul_addr6);

    if (sess->migration.target_local_dl_teid) {
        sess->local_dl_teid = sess->migration.target_local_dl_teid;
        migration_replace_sockaddr(
                &sess->local_dl_addr, &sess->migration.target_local_dl_addr);
        migration_replace_sockaddr(
                &sess->local_dl_addr6, &sess->migration.target_local_dl_addr6);
    }

    sess->migration.state = SMF_MIGRATION_STATE_SWITCH_CONFIRMED;

    ogs_info("[MIGRATE] path switch confirmed; session now bound to target UPF");

    return OGS_OK;
}

int smf_migration_send_path_switch_request(smf_sess_t *sess)
{
    smf_n1_n2_message_transfer_param_t param;

    ogs_assert(sess);

    if (sess->migration.state != SMF_MIGRATION_STATE_TARGET_READY) {
        ogs_error("Migration switch requires target_ready, current [%s]",
                smf_migration_state_name(sess->migration.state));
        return OGS_ERROR;
    }

    if (!sess->migration.target_local_ul_teid ||
        (!sess->migration.target_local_ul_addr &&
         !sess->migration.target_local_ul_addr6)) {
        ogs_error("Migration switch missing target UL NG-U endpoint");
        return OGS_ERROR;
    }

    sess->migration.state = SMF_MIGRATION_STATE_ROUTE_PROGRAMMING;
    if (migration_run_route_hook(sess, "program", "target") != OGS_OK) {
        sess->migration.state = SMF_MIGRATION_STATE_FAILED;
        smf_migration_send_target_deletion(sess);
        return OGS_ERROR;
    }

    memset(&param, 0, sizeof(param));
    param.state = SMF_NETWORK_REQUESTED_PDU_SESSION_MODIFICATION;
    param.n1smbuf = gsm_build_pdu_session_modification_command(sess, 0, 0);
    ogs_assert(param.n1smbuf);
    param.n2smbuf =
        ngap_build_pdu_session_resource_modify_request_transfer_for_migration(
                sess);
    ogs_assert(param.n2smbuf);
    param.n1n2_failure_txf_notif_uri = true;

    sess->migration.state = SMF_MIGRATION_STATE_PATH_SWITCHING;

    smf_namf_comm_send_n1_n2_message_transfer(sess, NULL, &param);

    return OGS_OK;
}

int smf_migration_handle_path_switch_response(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    int rv;

    ogs_assert(sess);
    ogs_assert(stream);

    rv = migration_commit_target(sess);
    if (rv != OGS_OK) {
        smf_sbi_send_sm_context_update_error_log(
                stream, OGS_SBI_HTTP_STATUS_CONFLICT,
                "Migration target state is incomplete", NULL);
        return rv;
    }

    ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));

    return OGS_OK;
}

int smf_migration_send_source_deletion(smf_sess_t *sess)
{
    int rv;

    ogs_assert(sess);

    if (sess->migration.state != SMF_MIGRATION_STATE_SWITCH_CONFIRMED) {
        ogs_error("Migration source drain requires switch_confirmed, "
                "current [%s]", smf_migration_state_name(sess->migration.state));
        return OGS_ERROR;
    }

    if (!sess->migration.source_node || !sess->migration.source_upf_n4_seid) {
        ogs_error("Migration source state is incomplete");
        smf_migration_mark_failed(sess);
        return OGS_ERROR;
    }

    sess->migration.state = SMF_MIGRATION_STATE_SOURCE_DRAINING;
    if (migration_run_route_hook(sess, "withdraw", "source") != OGS_OK) {
        smf_migration_mark_failed(sess);
        return OGS_ERROR;
    }

    rv = smf_5gc_pfcp_send_session_deletion_request_to_node(
            sess, sess->migration.source_node, NULL,
            OGS_PFCP_DELETE_TRIGGER_UPF_MIGRATION_SOURCE,
            sess->migration.source_upf_n4_seid);
    if (rv != OGS_OK) {
        smf_migration_mark_failed(sess);
        return rv;
    }

    return OGS_OK;
}

int smf_migration_send_target_deletion(smf_sess_t *sess)
{
    int rv;

    ogs_assert(sess);

    switch (sess->migration.state) {
    case SMF_MIGRATION_STATE_TARGET_PREPARING:
    case SMF_MIGRATION_STATE_TARGET_READY:
    case SMF_MIGRATION_STATE_ROUTE_PROGRAMMING:
    case SMF_MIGRATION_STATE_PATH_SWITCHING:
    case SMF_MIGRATION_STATE_FAILED:
        break;
    default:
        ogs_error("Migration target cleanup invalid state [%s]",
                smf_migration_state_name(sess->migration.state));
        return OGS_ERROR;
    }

    if (!sess->migration.target_node || !sess->migration.target_upf_n4_seid) {
        smf_migration_clear(sess);
        sess->migration.state = SMF_MIGRATION_STATE_ROLLED_BACK;
        return OGS_OK;
    }

    if (migration_run_route_hook(sess, "withdraw", "target") != OGS_OK)
        ogs_warn("[MIGRATE] route withdraw failed during target cleanup");

    sess->migration.state = SMF_MIGRATION_STATE_ABORTING;
    rv = smf_5gc_pfcp_send_session_deletion_request_to_node(
            sess, sess->migration.target_node, NULL,
            OGS_PFCP_DELETE_TRIGGER_UPF_MIGRATION_TARGET,
            sess->migration.target_upf_n4_seid);
    if (rv != OGS_OK) {
        smf_migration_mark_failed(sess);
        return rv;
    }

    return OGS_OK;
}

void smf_migration_handle_source_deletion_response(
        smf_sess_t *sess, bool success)
{
    ogs_assert(sess);

    if (!success) {
        smf_migration_mark_failed(sess);
        return;
    }

    migration_metrics_finish(sess, SMF_METR_GLOB_CTR_SMF_MIGRATION_COMPLETED);
    smf_migration_clear(sess);
    sess->migration.state = SMF_MIGRATION_STATE_COMPLETED;
    ogs_info("[MIGRATE] source UPF PFCP state deleted");
}

void smf_migration_handle_target_deletion_response(
        smf_sess_t *sess, bool success)
{
    ogs_assert(sess);

    if (!success) {
        smf_migration_mark_failed(sess);
        return;
    }

    migration_metrics_finish(
            sess, SMF_METR_GLOB_CTR_SMF_MIGRATION_ROLLED_BACK);
    smf_migration_clear(sess);
    sess->migration.state = SMF_MIGRATION_STATE_ROLLED_BACK;
    ogs_info("[MIGRATE] target UPF PFCP state deleted");
}

size_t smf_handle_pdu_migrate(char *buf, size_t buflen,
        const char *method, const char *body, size_t body_len,
        int *status_code)
{
    cJSON *root = NULL;
    cJSON *request = NULL;
    cJSON *supi = NULL;
    cJSON *psi = NULL;
    cJSON *target_upf = NULL;
    cJSON *action = NULL;
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_pfcp_node_t *target_node = NULL;
    int rv;

    ogs_assert(buf);
    ogs_assert(method);
    ogs_assert(status_code);

    root = cJSON_CreateObject();
    if (!root) {
        *status_code = 500;
        if (buflen) buf[0] = '\0';
        return 0;
    }

    migration_json_add_string(root, "endpoint", "/pdu-migrate");

    if (strcmp(method, "POST") != 0) {
        *status_code = 405;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "method_not_allowed");
        return migration_json_finalize(root, buf, buflen);
    }

    if (!body || body_len == 0) {
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "missing_json_body");
        return migration_json_finalize(root, buf, buflen);
    }

    request = cJSON_ParseWithLength(body, body_len);
    if (!request) {
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "invalid_json_body");
        return migration_json_finalize(root, buf, buflen);
    }

    supi = cJSON_GetObjectItemCaseSensitive(request, "supi");
    psi = cJSON_GetObjectItemCaseSensitive(request, "psi");
    target_upf = cJSON_GetObjectItemCaseSensitive(request, "target_upf");
    action = cJSON_GetObjectItemCaseSensitive(request, "action");

    if (!cJSON_IsString(supi) || !supi->valuestring ||
        !cJSON_IsNumber(psi) || psi->valuedouble < 1 || psi->valuedouble > 255) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason",
                "required_fields_are_supi_psi");
        return migration_json_finalize(root, buf, buflen);
    }

    if (action && !is_action(action, "prepare") &&
        !is_action(action, "switch") && !is_action(action, "drain") &&
        !is_action(action, "abort")) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason",
                "action_must_be_prepare_switch_drain_or_abort");
        return migration_json_finalize(root, buf, buflen);
    }

    migration_json_add_string(root, "supi", supi->valuestring);
    migration_json_add_number(root, "psi", psi->valuedouble);
    migration_json_add_string(root, "action",
            is_action(action, "switch") ? "switch" :
            is_action(action, "drain") ? "drain" :
            is_action(action, "abort") ? "abort" : "prepare");
    if (target_upf && target_upf->valuestring)
        migration_json_add_string(root, "target_upf", target_upf->valuestring);

    smf_ue = smf_ue_find_by_supi(supi->valuestring);
    if (!smf_ue) {
        cJSON_Delete(request);
        *status_code = 404;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "ue_not_found");
        return migration_json_finalize(root, buf, buflen);
    }

    sess = smf_sess_find_by_psi(smf_ue, (uint8_t)psi->valueint);
    if (!sess) {
        cJSON_Delete(request);
        *status_code = 404;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "pdu_session_not_found");
        return migration_json_finalize(root, buf, buflen);
    }

    migration_json_add_string(root, "migration_state",
            smf_migration_state_name(sess->migration.state));

    if (is_action(action, "switch")) {
        rv = smf_migration_send_path_switch_request(sess);
        if (rv != OGS_OK) {
            cJSON_Delete(request);
            *status_code = 409;
            migration_json_add_string(root, "result", "rejected");
            migration_json_add_string(root, "reason",
                    "path_switch_request_failed");
            migration_json_add_string(root, "migration_state",
                    smf_migration_state_name(sess->migration.state));
            return migration_json_finalize(root, buf, buflen);
        }

        cJSON_Delete(request);
        *status_code = 202;
        migration_json_add_string(root, "result", "accepted");
        migration_json_add_string(root, "migration_state",
                smf_migration_state_name(sess->migration.state));
        return migration_json_finalize(root, buf, buflen);
    }

    if (is_action(action, "abort")) {
        rv = smf_migration_send_target_deletion(sess);
        if (rv != OGS_OK) {
            cJSON_Delete(request);
            *status_code = 409;
            migration_json_add_string(root, "result", "rejected");
            migration_json_add_string(root, "reason",
                    "target_cleanup_request_failed");
            migration_json_add_string(root, "migration_state",
                    smf_migration_state_name(sess->migration.state));
            return migration_json_finalize(root, buf, buflen);
        }

        cJSON_Delete(request);
        *status_code = 202;
        migration_json_add_string(root, "result", "accepted");
        migration_json_add_string(root, "migration_state",
                smf_migration_state_name(sess->migration.state));
        return migration_json_finalize(root, buf, buflen);
    }

    if (is_action(action, "drain")) {
        rv = smf_migration_send_source_deletion(sess);
        if (rv != OGS_OK) {
            cJSON_Delete(request);
            *status_code = 409;
            migration_json_add_string(root, "result", "rejected");
            migration_json_add_string(root, "reason",
                    "source_drain_request_failed");
            migration_json_add_string(root, "migration_state",
                    smf_migration_state_name(sess->migration.state));
            return migration_json_finalize(root, buf, buflen);
        }

        cJSON_Delete(request);
        *status_code = 202;
        migration_json_add_string(root, "result", "accepted");
        migration_json_add_string(root, "migration_state",
                smf_migration_state_name(sess->migration.state));
        return migration_json_finalize(root, buf, buflen);
    }

    if (!cJSON_IsString(target_upf) || !target_upf->valuestring) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason",
                "prepare_requires_target_upf");
        return migration_json_finalize(root, buf, buflen);
    }

    if (smf_migration_active(sess)) {
        cJSON_Delete(request);
        *status_code = 409;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "migration_already_active");
        return migration_json_finalize(root, buf, buflen);
    }

    if (sess->epc) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "epc_session_not_supported");
        return migration_json_finalize(root, buf, buflen);
    }

    if (!sess->pfcp_node) {
        cJSON_Delete(request);
        *status_code = 409;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "source_upf_not_bound");
        return migration_json_finalize(root, buf, buflen);
    }

    if (sess->n1_released || sess->n2_released ||
        !sess->remote_dl_teid ||
        (!sess->remote_dl_ip.ipv4 && !sess->remote_dl_ip.ipv6)) {
        cJSON_Delete(request);
        *status_code = 409;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "session_not_active");
        return migration_json_finalize(root, buf, buflen);
    }

    target_node = smf_upf_node_find_by_addr(target_upf->valuestring);
    if (!target_node) {
        cJSON_Delete(request);
        *status_code = 404;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "target_upf_not_found");
        return migration_json_finalize(root, buf, buflen);
    }

    if (target_node == sess->pfcp_node) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "target_is_source");
        return migration_json_finalize(root, buf, buflen);
    }

    if (!OGS_FSM_CHECK(&target_node->sm, smf_pfcp_state_associated)) {
        cJSON_Delete(request);
        *status_code = 409;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "target_upf_not_associated");
        return migration_json_finalize(root, buf, buflen);
    }

    if (!smf_sess_upf_eligible(sess, target_node)) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "target_upf_not_eligible");
        return migration_json_finalize(root, buf, buflen);
    }

    if (!target_node->up_function_features.ftup) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "target_upf_ftup_required");
        return migration_json_finalize(root, buf, buflen);
    }

    if ((sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4 ||
         sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4V6) &&
        !ogs_pfcp_find_subnet_by_dnn(AF_INET, sess->session.name)) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "dnn_ipv4_subnet_missing");
        return migration_json_finalize(root, buf, buflen);
    }

    if ((sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV6 ||
         sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4V6) &&
        !ogs_pfcp_find_subnet_by_dnn(AF_INET6, sess->session.name)) {
        cJSON_Delete(request);
        *status_code = 400;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "dnn_ipv6_subnet_missing");
        return migration_json_finalize(root, buf, buflen);
    }

    smf_migration_clear(sess);

    sess->migration.state = SMF_MIGRATION_STATE_TARGET_PREPARING;
    sess->migration.source_node = sess->pfcp_node;
    sess->migration.target_node = target_node;
    sess->migration.source_upf_n4_seid = sess->upf_n4_seid;
    sess->migration.metrics_active = true;
    smf_metrics_inst_global_inc(SMF_METR_GLOB_CTR_SMF_MIGRATION_ATTEMPTS);
    smf_metrics_inst_global_inc(SMF_METR_GLOB_GAUGE_SMF_MIGRATIONS_ACTIVE);

    rv = smf_5gc_pfcp_send_session_establishment_request_to_node(
            sess, target_node, NULL, OGS_PFCP_CREATE_UPF_MIGRATION_TARGET);
    if (rv != OGS_OK) {
        smf_migration_clear(sess);
        cJSON_Delete(request);
        *status_code = 500;
        migration_json_add_string(root, "result", "rejected");
        migration_json_add_string(root, "reason", "target_prepare_send_failed");
        return migration_json_finalize(root, buf, buflen);
    }

    ogs_info("[MIGRATE supi:%s psi:%d] target prepare requested [%s]",
            supi->valuestring, psi->valueint, target_upf->valuestring);

    cJSON_Delete(request);

    *status_code = 202;
    migration_json_add_string(root, "result", "accepted");
    migration_json_add_string(root, "migration_state",
            smf_migration_state_name(sess->migration.state));

    return migration_json_finalize(root, buf, buflen);
}
