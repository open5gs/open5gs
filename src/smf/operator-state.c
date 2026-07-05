/*
 * Copyright (C) 2026 by Open5GS contributors
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "operator-state.h"

#include "context.h"
#include "migration.h"
#include "pdu-info.h"

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define SMF_OPERATOR_STATE_SCHEMA "5g3e.smf.operator-state/v1"
#define SMF_OPERATOR_STATE_DEFAULT_DIR "/var/lib/open5gs/smf-state"
#define SMF_OPERATOR_STATE_ADMIN_MAX 64
#define SMF_OPERATOR_STATE_RECOVERY_MAX 128

typedef struct smf_operator_admin_record_s {
    bool used;
    char addr[OGS_ADDRSTRLEN];
    bool draining;
    ogs_time_t updated_at;
} smf_operator_admin_record_t;

typedef struct smf_operator_recovery_record_s {
    bool used;
    char supi[OGS_MAX_IMSI_BCD_LEN + sizeof("imsi-")];
    uint8_t psi;
    uint64_t migration_id;
    char migration_state[48];
    char source_upf[OGS_ADDRSTRLEN];
    char target_upf[OGS_ADDRSTRLEN];
    char ue_ip[OGS_ADDRSTRLEN];
    char n3_endpoint[128];
    char source_upf_n4_seid[32];
    char target_upf_n4_seid[32];
    char recovery_action[80];
    ogs_time_t updated_at;
} smf_operator_recovery_record_t;

static struct {
    bool initialized;
    bool enabled;
    char *configured_dir;
    char dir[PATH_MAX];
    char journal_path[PATH_MAX];
    char snapshot_path[PATH_MAX];
    smf_operator_admin_record_t admins[SMF_OPERATOR_STATE_ADMIN_MAX];
    smf_operator_recovery_record_t recovery[SMF_OPERATOR_STATE_RECOVERY_MAX];
} state;

static int mkdir_recursive(const char *path)
{
    char tmp[PATH_MAX];
    size_t len;
    char *p = NULL;

    if (!path || !path[0])
        return OGS_ERROR;

    ogs_cpystrn(tmp, path, sizeof(tmp));
    len = strlen(tmp);
    if (len == 0)
        return OGS_ERROR;
    if (tmp[len - 1] == '/')
        tmp[len - 1] = '\0';

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0750) != 0 && errno != EEXIST)
                return OGS_ERROR;
            *p = '/';
        }
    }

    if (mkdir(tmp, 0750) != 0 && errno != EEXIST)
        return OGS_ERROR;

    return OGS_OK;
}

static void admin_upsert(const char *addr, bool draining, bool restore)
{
    int i, free_slot = -1;

    if (!addr || !addr[0])
        return;

    for (i = 0; i < SMF_OPERATOR_STATE_ADMIN_MAX; i++) {
        if (!state.admins[i].used) {
            if (free_slot < 0)
                free_slot = i;
            continue;
        }
        if (strcmp(state.admins[i].addr, addr) == 0) {
            state.admins[i].draining = draining;
            state.admins[i].updated_at = ogs_time_now();
            if (restore)
                smf_upf_admin_restore(addr, draining ? 1 : 0);
            return;
        }
    }

    if (!draining && !restore)
        return;

    if (free_slot >= 0) {
        state.admins[free_slot].used = true;
        ogs_cpystrn(state.admins[free_slot].addr, addr,
                sizeof(state.admins[free_slot].addr));
        state.admins[free_slot].draining = draining;
        state.admins[free_slot].updated_at = ogs_time_now();
        if (restore)
            smf_upf_admin_restore(addr, draining ? 1 : 0);
    } else {
        ogs_warn("[SMF-STATE] admin state table full; dropping %s", addr);
    }
}

static int recovery_find(
        const char *supi, uint8_t psi, uint64_t migration_id)
{
    int i;

    for (i = 0; i < SMF_OPERATOR_STATE_RECOVERY_MAX; i++) {
        if (!state.recovery[i].used)
            continue;
        if (migration_id && state.recovery[i].migration_id == migration_id)
            return i;
        if (supi && supi[0] &&
            strcmp(state.recovery[i].supi, supi) == 0 &&
            state.recovery[i].psi == psi)
            return i;
    }

    return -1;
}

static int recovery_slot(
        const char *supi, uint8_t psi, uint64_t migration_id)
{
    int i, free_slot = -1;

    i = recovery_find(supi, psi, migration_id);
    if (i >= 0)
        return i;

    for (i = 0; i < SMF_OPERATOR_STATE_RECOVERY_MAX; i++) {
        if (!state.recovery[i].used) {
            free_slot = i;
            break;
        }
    }

    return free_slot >= 0 ? free_slot : 0;
}

static void recovery_remove(
        const char *supi, uint8_t psi, uint64_t migration_id)
{
    int i = recovery_find(supi, psi, migration_id);

    if (i >= 0)
        memset(&state.recovery[i], 0, sizeof(state.recovery[i]));
}

static const char *recovery_action_for_state(const char *migration_state)
{
    if (!migration_state)
        return "recovery_required";

    if (!strcmp(migration_state, "completed") ||
        !strcmp(migration_state, "rolled_back") ||
        !strcmp(migration_state, "failed") ||
        !strcmp(migration_state, "idle"))
        return "terminal";

    if (!strcmp(migration_state, "switch_confirmed") ||
        !strcmp(migration_state, "source_releasing") ||
        !strcmp(migration_state, "source_draining"))
        return "complete_post_switch_source_cleanup";

    if (!strcmp(migration_state, "path_switching"))
        return "verify_target_then_cleanup_or_manual_reconcile";

    return "abort_pre_switch_target_and_keep_source";
}

static void node_addr_copy(ogs_pfcp_node_t *node, char *dst, size_t dstlen)
{
    const char *addr = "";

    if (!dst || dstlen == 0)
        return;
    if (node && node->addr_list)
        addr = ogs_sockaddr_to_string_static(node->addr_list);
    ogs_cpystrn(dst, addr ? addr : "", dstlen);
}

static void ue_ip_copy(smf_sess_t *sess, char *dst, size_t dstlen)
{
    if (!dst || dstlen == 0)
        return;

    dst[0] = '\0';
    if (!sess)
        return;

    if (sess->ipv4)
        OGS_INET_NTOP(&sess->ipv4->addr, dst);
    else if (sess->ipv6)
        OGS_INET6_NTOP(&sess->ipv6->addr, dst);
}

static void n3_endpoint_copy(smf_sess_t *sess, char *dst, size_t dstlen)
{
    char addr[OGS_ADDRSTRLEN] = "";

    if (!dst || dstlen == 0)
        return;

    dst[0] = '\0';
    if (!sess || !sess->remote_dl_teid)
        return;

    if (sess->remote_dl_ip.ipv4)
        OGS_INET_NTOP(&sess->remote_dl_ip.addr, addr);
    else if (sess->remote_dl_ip.ipv6)
        OGS_INET6_NTOP(&sess->remote_dl_ip.addr6, addr);

    if (addr[0])
        ogs_snprintf(dst, dstlen, "%s/%u", addr, sess->remote_dl_teid);
}

static void recovery_upsert_from_session(smf_sess_t *sess)
{
    smf_ue_t *smf_ue = NULL;
    const char *supi = "";
    const char *state_name = NULL;
    int slot;

    if (!sess || !sess->migration.id)
        return;

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    if (smf_ue && smf_ue->supi)
        supi = smf_ue->supi;

    state_name = smf_migration_state_name(sess->migration.state);
    if (!strcmp(recovery_action_for_state(state_name), "terminal")) {
        recovery_remove(supi, sess->psi, sess->migration.id);
        return;
    }

    slot = recovery_slot(supi, sess->psi, sess->migration.id);
    if (slot < 0)
        return;

    memset(&state.recovery[slot], 0, sizeof(state.recovery[slot]));
    state.recovery[slot].used = true;
    ogs_cpystrn(state.recovery[slot].supi, supi,
            sizeof(state.recovery[slot].supi));
    state.recovery[slot].psi = sess->psi;
    state.recovery[slot].migration_id = sess->migration.id;
    ogs_cpystrn(state.recovery[slot].migration_state, state_name,
            sizeof(state.recovery[slot].migration_state));
    node_addr_copy(sess->migration.source_node, state.recovery[slot].source_upf,
            sizeof(state.recovery[slot].source_upf));
    node_addr_copy(sess->migration.target_node, state.recovery[slot].target_upf,
            sizeof(state.recovery[slot].target_upf));
    ue_ip_copy(sess, state.recovery[slot].ue_ip,
            sizeof(state.recovery[slot].ue_ip));
    n3_endpoint_copy(sess, state.recovery[slot].n3_endpoint,
            sizeof(state.recovery[slot].n3_endpoint));
    ogs_snprintf(state.recovery[slot].source_upf_n4_seid,
            sizeof(state.recovery[slot].source_upf_n4_seid),
            "%llu", (unsigned long long)sess->migration.source_upf_n4_seid);
    ogs_snprintf(state.recovery[slot].target_upf_n4_seid,
            sizeof(state.recovery[slot].target_upf_n4_seid),
            "%llu", (unsigned long long)sess->migration.target_upf_n4_seid);
    ogs_cpystrn(state.recovery[slot].recovery_action,
            recovery_action_for_state(state_name),
            sizeof(state.recovery[slot].recovery_action));
    state.recovery[slot].updated_at = ogs_time_now();
}

static cJSON *admin_record_to_json(const smf_operator_admin_record_t *rec)
{
    cJSON *o = cJSON_CreateObject();

    if (!o)
        return NULL;

    cJSON_AddStringToObject(o, "type", "upf_admin");
    cJSON_AddStringToObject(o, "upf", rec->addr);
    cJSON_AddStringToObject(o, "admin_state",
            rec->draining ? "draining" : "active");
    cJSON_AddNumberToObject(o, "updated_at_us", (double)rec->updated_at);

    return o;
}

static cJSON *recovery_record_to_json(
        const smf_operator_recovery_record_t *rec)
{
    cJSON *o = cJSON_CreateObject();
    char id[32];

    if (!o)
        return NULL;

    ogs_snprintf(id, sizeof(id), "%llu",
            (unsigned long long)rec->migration_id);

    cJSON_AddStringToObject(o, "type", "migration");
    cJSON_AddStringToObject(o, "supi", rec->supi);
    cJSON_AddNumberToObject(o, "psi", (double)rec->psi);
    cJSON_AddNumberToObject(o, "migration_id", (double)rec->migration_id);
    cJSON_AddStringToObject(o, "migration_id_str", id);
    cJSON_AddStringToObject(o, "migration_state", rec->migration_state);
    cJSON_AddStringToObject(o, "source_upf", rec->source_upf);
    cJSON_AddStringToObject(o, "target_upf", rec->target_upf);
    cJSON_AddStringToObject(o, "ue_ip", rec->ue_ip);
    cJSON_AddStringToObject(o, "n3_endpoint", rec->n3_endpoint);
    cJSON_AddStringToObject(o, "source_upf_n4_seid_str",
            rec->source_upf_n4_seid);
    cJSON_AddStringToObject(o, "target_upf_n4_seid_str",
            rec->target_upf_n4_seid);
    cJSON_AddStringToObject(o, "recovery_action", rec->recovery_action);
    cJSON_AddBoolToObject(o, "recovery_required", true);
    cJSON_AddNumberToObject(o, "updated_at_us", (double)rec->updated_at);

    return o;
}

static int write_json_file(const char *path, cJSON *root)
{
    char *tmp = NULL;
    FILE *fp = NULL;
    int rv = OGS_ERROR;

    tmp = cJSON_PrintUnformatted(root);
    if (!tmp)
        return OGS_ERROR;

    fp = fopen(path, "wb");
    if (!fp) {
        ogs_warn("[SMF-STATE] cannot open %s: %s", path, strerror(errno));
        cJSON_free(tmp);
        return OGS_ERROR;
    }

    if (fprintf(fp, "%s\n", tmp) > 0)
        rv = OGS_OK;
    fclose(fp);
    cJSON_free(tmp);

    return rv;
}

static void write_snapshot(void)
{
    cJSON *root = NULL;
    cJSON *admins = NULL;
    cJSON *recovery = NULL;
    int i;

    if (!state.enabled)
        return;

    root = cJSON_CreateObject();
    admins = cJSON_CreateArray();
    recovery = cJSON_CreateArray();
    if (!root || !admins || !recovery) {
        if (root) cJSON_Delete(root);
        if (admins) cJSON_Delete(admins);
        if (recovery) cJSON_Delete(recovery);
        return;
    }

    cJSON_AddStringToObject(root, "schema", SMF_OPERATOR_STATE_SCHEMA);
    cJSON_AddNumberToObject(root, "updated_at_us", (double)ogs_time_now());
    cJSON_AddStringToObject(root, "state_dir", state.dir);

    for (i = 0; i < SMF_OPERATOR_STATE_ADMIN_MAX; i++) {
        cJSON *o;
        if (!state.admins[i].used)
            continue;
        o = admin_record_to_json(&state.admins[i]);
        if (o)
            cJSON_AddItemToArray(admins, o);
    }

    for (i = 0; i < SMF_OPERATOR_STATE_RECOVERY_MAX; i++) {
        cJSON *o;
        if (!state.recovery[i].used)
            continue;
        o = recovery_record_to_json(&state.recovery[i]);
        if (o)
            cJSON_AddItemToArray(recovery, o);
    }

    cJSON_AddItemToObjectCS(root, "admin", admins);
    cJSON_AddItemToObjectCS(root, "recovery", recovery);
    write_json_file(state.snapshot_path, root);
    cJSON_Delete(root);
}

static void append_record(cJSON *record)
{
    char *line = NULL;
    FILE *fp = NULL;

    if (!state.enabled || !record)
        return;

    line = cJSON_PrintUnformatted(record);
    if (!line)
        return;

    fp = fopen(state.journal_path, "ab");
    if (!fp) {
        ogs_warn("[SMF-STATE] cannot append %s: %s",
                state.journal_path, strerror(errno));
        cJSON_free(line);
        return;
    }

    fprintf(fp, "%s\n", line);
    fclose(fp);
    cJSON_free(line);
}

static void replay_record(cJSON *record)
{
    cJSON *type = NULL;

    if (!record)
        return;

    type = cJSON_GetObjectItemCaseSensitive(record, "type");
    if (!cJSON_IsString(type) || !type->valuestring)
        return;

    if (!strcmp(type->valuestring, "upf_admin")) {
        cJSON *upf = cJSON_GetObjectItemCaseSensitive(record, "upf");
        cJSON *admin_state =
            cJSON_GetObjectItemCaseSensitive(record, "admin_state");
        if (cJSON_IsString(upf) && upf->valuestring &&
            cJSON_IsString(admin_state) && admin_state->valuestring)
            admin_upsert(upf->valuestring,
                    strcmp(admin_state->valuestring, "draining") == 0, true);
    } else if (!strcmp(type->valuestring, "migration")) {
        cJSON *supi = cJSON_GetObjectItemCaseSensitive(record, "supi");
        cJSON *psi = cJSON_GetObjectItemCaseSensitive(record, "psi");
        cJSON *migration_id =
            cJSON_GetObjectItemCaseSensitive(record, "migration_id_str");
        cJSON *migration_state =
            cJSON_GetObjectItemCaseSensitive(record, "migration_state");
        cJSON *source_upf =
            cJSON_GetObjectItemCaseSensitive(record, "source_upf");
        cJSON *target_upf =
            cJSON_GetObjectItemCaseSensitive(record, "target_upf");
        cJSON *ue_ip = cJSON_GetObjectItemCaseSensitive(record, "ue_ip");
        cJSON *n3_endpoint =
            cJSON_GetObjectItemCaseSensitive(record, "n3_endpoint");
        cJSON *source_seid =
            cJSON_GetObjectItemCaseSensitive(record, "source_upf_n4_seid_str");
        cJSON *target_seid =
            cJSON_GetObjectItemCaseSensitive(record, "target_upf_n4_seid_str");
        unsigned long long id = 0;
        int slot;

        if (!cJSON_IsString(migration_id) || !migration_id->valuestring)
            migration_id =
                cJSON_GetObjectItemCaseSensitive(record, "migration_id");
        if (cJSON_IsString(migration_id) && migration_id->valuestring)
            id = strtoull(migration_id->valuestring, NULL, 10);
        else if (cJSON_IsNumber(migration_id))
            id = (unsigned long long)migration_id->valuedouble;

        if (!cJSON_IsString(supi) || !supi->valuestring ||
            !cJSON_IsNumber(psi) ||
            !cJSON_IsString(migration_state) ||
            !migration_state->valuestring)
            return;

        if (!strcmp(recovery_action_for_state(
                        migration_state->valuestring), "terminal")) {
            recovery_remove(supi->valuestring, (uint8_t)psi->valueint,
                    (uint64_t)id);
            return;
        }

        slot = recovery_slot(supi->valuestring, (uint8_t)psi->valueint,
                (uint64_t)id);
        if (slot < 0)
            return;

        memset(&state.recovery[slot], 0, sizeof(state.recovery[slot]));
        state.recovery[slot].used = true;
        ogs_cpystrn(state.recovery[slot].supi, supi->valuestring,
                sizeof(state.recovery[slot].supi));
        state.recovery[slot].psi = (uint8_t)psi->valueint;
        state.recovery[slot].migration_id = (uint64_t)id;
        ogs_cpystrn(state.recovery[slot].migration_state,
                migration_state->valuestring,
                sizeof(state.recovery[slot].migration_state));
        if (cJSON_IsString(source_upf) && source_upf->valuestring)
            ogs_cpystrn(state.recovery[slot].source_upf,
                    source_upf->valuestring,
                    sizeof(state.recovery[slot].source_upf));
        if (cJSON_IsString(target_upf) && target_upf->valuestring)
            ogs_cpystrn(state.recovery[slot].target_upf,
                    target_upf->valuestring,
                    sizeof(state.recovery[slot].target_upf));
        if (cJSON_IsString(ue_ip) && ue_ip->valuestring)
            ogs_cpystrn(state.recovery[slot].ue_ip, ue_ip->valuestring,
                    sizeof(state.recovery[slot].ue_ip));
        if (cJSON_IsString(n3_endpoint) && n3_endpoint->valuestring)
            ogs_cpystrn(state.recovery[slot].n3_endpoint,
                    n3_endpoint->valuestring,
                    sizeof(state.recovery[slot].n3_endpoint));
        if (cJSON_IsString(source_seid) && source_seid->valuestring)
            ogs_cpystrn(state.recovery[slot].source_upf_n4_seid,
                    source_seid->valuestring,
                    sizeof(state.recovery[slot].source_upf_n4_seid));
        if (cJSON_IsString(target_seid) && target_seid->valuestring)
            ogs_cpystrn(state.recovery[slot].target_upf_n4_seid,
                    target_seid->valuestring,
                    sizeof(state.recovery[slot].target_upf_n4_seid));
        ogs_cpystrn(state.recovery[slot].recovery_action,
                recovery_action_for_state(migration_state->valuestring),
                sizeof(state.recovery[slot].recovery_action));
        state.recovery[slot].updated_at = ogs_time_now();
    } else if (!strcmp(type->valuestring, "migration_terminal")) {
        cJSON *supi = cJSON_GetObjectItemCaseSensitive(record, "supi");
        cJSON *psi = cJSON_GetObjectItemCaseSensitive(record, "psi");
        cJSON *migration_id =
            cJSON_GetObjectItemCaseSensitive(record, "migration_id_str");
        unsigned long long id = 0;

        if (cJSON_IsString(migration_id) && migration_id->valuestring)
            id = strtoull(migration_id->valuestring, NULL, 10);
        if (cJSON_IsString(supi) && supi->valuestring && cJSON_IsNumber(psi))
            recovery_remove(supi->valuestring, (uint8_t)psi->valueint,
                    (uint64_t)id);
    }
}

static void replay_json_file(const char *path, bool snapshot)
{
    FILE *fp = NULL;
    char line[8192];

    fp = fopen(path, "rb");
    if (!fp)
        return;

    while (fgets(line, sizeof(line), fp)) {
        cJSON *root = cJSON_Parse(line);
        if (!root)
            continue;
        if (snapshot) {
            cJSON *admins = cJSON_GetObjectItemCaseSensitive(root, "admin");
            cJSON *recovery =
                cJSON_GetObjectItemCaseSensitive(root, "recovery");
            cJSON *item = NULL;

            cJSON_ArrayForEach(item, admins)
                replay_record(item);
            cJSON_ArrayForEach(item, recovery)
                replay_record(item);
        } else {
            replay_record(root);
        }
        cJSON_Delete(root);
    }

    fclose(fp);
}

void smf_operator_state_set_path(const char *path)
{
    if (state.configured_dir) {
        ogs_free(state.configured_dir);
        state.configured_dir = NULL;
    }

    if (path && path[0])
        state.configured_dir = ogs_strdup(path);
}

void smf_operator_state_init(void)
{
    const char *dir = state.configured_dir ?
        state.configured_dir : SMF_OPERATOR_STATE_DEFAULT_DIR;

    if (state.initialized)
        return;

    ogs_cpystrn(state.dir, dir, sizeof(state.dir));
    ogs_snprintf(state.journal_path, sizeof(state.journal_path),
            "%s/journal.jsonl", state.dir);
    ogs_snprintf(state.snapshot_path, sizeof(state.snapshot_path),
            "%s/snapshot.json", state.dir);

    if (mkdir_recursive(state.dir) != OGS_OK) {
        ogs_warn("[SMF-STATE] disabling operator persistence; "
                "cannot create %s: %s", state.dir, strerror(errno));
        state.enabled = false;
        state.initialized = true;
        return;
    }

    state.enabled = true;
    replay_json_file(state.snapshot_path, true);
    replay_json_file(state.journal_path, false);
    write_snapshot();

    if (smf_operator_state_recovery_required())
        ogs_warn("[SMF-STATE] recovery_required after replay; "
                "operator scale-in must fail closed until reconciled");

    state.initialized = true;
}

void smf_operator_state_final(void)
{
    write_snapshot();

    if (state.configured_dir) {
        ogs_free(state.configured_dir);
        state.configured_dir = NULL;
    }

    memset(&state, 0, sizeof(state));
}

void smf_operator_state_record_upf_admin(const char *addr, int draining)
{
    cJSON *record = NULL;

    if (!state.enabled || !addr || !addr[0])
        return;

    admin_upsert(addr, draining ? true : false, false);

    record = cJSON_CreateObject();
    if (!record)
        return;
    cJSON_AddStringToObject(record, "type", "upf_admin");
    cJSON_AddStringToObject(record, "upf", addr);
    cJSON_AddStringToObject(record, "admin_state",
            draining ? "draining" : "active");
    cJSON_AddNumberToObject(record, "updated_at_us", (double)ogs_time_now());
    append_record(record);
    cJSON_Delete(record);
    write_snapshot();
}

void smf_operator_state_record_migration(smf_sess_t *sess)
{
    cJSON *record = NULL;
    int slot;

    if (!state.enabled || !sess || !sess->migration.id)
        return;

    recovery_upsert_from_session(sess);
    slot = recovery_find(NULL, 0, sess->migration.id);
    if (slot < 0)
        return;

    record = recovery_record_to_json(&state.recovery[slot]);
    if (!record)
        return;

    append_record(record);
    cJSON_Delete(record);
    write_snapshot();
}

void smf_operator_state_record_migration_terminal(
        smf_sess_t *sess, const char *outcome)
{
    smf_ue_t *smf_ue = NULL;
    const char *supi = "";
    cJSON *record = NULL;
    char id[32];

    if (!state.enabled || !sess || !sess->migration.id)
        return;

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    if (smf_ue && smf_ue->supi)
        supi = smf_ue->supi;

    recovery_remove(supi, sess->psi, sess->migration.id);

    record = cJSON_CreateObject();
    if (!record)
        return;

    ogs_snprintf(id, sizeof(id), "%llu",
            (unsigned long long)sess->migration.id);
    cJSON_AddStringToObject(record, "type", "migration_terminal");
    cJSON_AddStringToObject(record, "supi", supi);
    cJSON_AddNumberToObject(record, "psi", (double)sess->psi);
    cJSON_AddStringToObject(record, "migration_id_str", id);
    cJSON_AddStringToObject(record, "outcome", outcome ? outcome : "unknown");
    cJSON_AddNumberToObject(record, "updated_at_us", (double)ogs_time_now());
    append_record(record);
    cJSON_Delete(record);
    write_snapshot();
}

bool smf_operator_state_recovery_required(void)
{
    int i;

    for (i = 0; i < SMF_OPERATOR_STATE_RECOVERY_MAX; i++)
        if (state.recovery[i].used)
            return true;

    return false;
}

cJSON *smf_operator_state_recovery_to_json(void)
{
    cJSON *root = NULL;
    cJSON *entries = NULL;
    int i, count = 0;

    root = cJSON_CreateObject();
    entries = cJSON_CreateArray();
    if (!root || !entries) {
        if (root) cJSON_Delete(root);
        if (entries) cJSON_Delete(entries);
        return NULL;
    }

    cJSON_AddStringToObject(root, "schema", SMF_OPERATOR_STATE_SCHEMA);
    cJSON_AddBoolToObject(root, "enabled", state.enabled);
    cJSON_AddBoolToObject(root, "recovery_required",
            smf_operator_state_recovery_required());
    cJSON_AddStringToObject(root, "state_dir", state.dir);
    cJSON_AddStringToObject(root, "journal", state.journal_path);
    cJSON_AddStringToObject(root, "snapshot", state.snapshot_path);

    for (i = 0; i < SMF_OPERATOR_STATE_RECOVERY_MAX; i++) {
        cJSON *o;
        if (!state.recovery[i].used)
            continue;
        o = recovery_record_to_json(&state.recovery[i]);
        if (o) {
            cJSON_AddItemToArray(entries, o);
            count++;
        }
    }

    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddItemToObjectCS(root, "entries", entries);

    return root;
}
