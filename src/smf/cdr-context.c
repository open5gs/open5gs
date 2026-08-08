/*
 * Copyright (C) 2026 by DNL
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

#include "cdr-context.h"
#include "pfcp-path.h"
#include "ogs-cdr.h"

#include <sys/stat.h>
#include <errno.h>

static struct {
    bool initialized;
    ogs_cdr_file_t *file;
    ogs_time_t file_opened_at;
    uint32_t file_seq;          /* next file sequence number */
    uint64_t local_seq;         /* node-wide record counter */
    char seq_path[OGS_MAX_FILEPATH_LEN];
    uint8_t node_ipv4[4];
} g_cdr;

static bool cdr_enabled(void)
{
    return g_cdr.initialized && smf_self()->cdr.enabled;
}

static void seq_load(void)
{
    FILE *fp = fopen(g_cdr.seq_path, "r");
    if (!fp)
        return;
    if (fscanf(fp, "%u %llu", &g_cdr.file_seq,
            (unsigned long long *)&g_cdr.local_seq) != 2) {
        g_cdr.file_seq = 0;
        g_cdr.local_seq = 0;
    }
    fclose(fp);
}

static void seq_save(void)
{
    FILE *fp = fopen(g_cdr.seq_path, "w");
    if (!fp) {
        ogs_error("CDR: cannot write %s", g_cdr.seq_path);
        return;
    }
    fprintf(fp, "%u %llu\n", g_cdr.file_seq,
            (unsigned long long)g_cdr.local_seq);
    fclose(fp);
}

static void node_ipv4_init(void)
{
    ogs_sockaddr_t *addr = NULL;

    memset(g_cdr.node_ipv4, 0, 4);
    for (addr = ogs_gtp_self()->gtpc_addr; addr; addr = addr->next) {
        if (addr->ogs_sa_family == AF_INET) {
            memcpy(g_cdr.node_ipv4, &addr->sin.sin_addr.s_addr, 4);
            break;
        }
    }
}

void smf_cdr_init(void)
{
    if (!smf_self()->cdr.enabled)
        return;

    memset(&g_cdr, 0, sizeof(g_cdr));

    if (mkdir(smf_self()->cdr.directory, 0755) != 0 && errno != EEXIST) {
        ogs_error("CDR: cannot create directory [%s]",
                smf_self()->cdr.directory);
        return;
    }

    ogs_snprintf(g_cdr.seq_path, sizeof(g_cdr.seq_path),
            "%s/.sequence", smf_self()->cdr.directory);
    seq_load();
    node_ipv4_init();

    g_cdr.initialized = true;
    ogs_info("CDR: enabled, dir[%s] node-id[%s] file-seq[%u]",
            smf_self()->cdr.directory, smf_self()->cdr.node_id,
            g_cdr.file_seq);
}

static void file_close(uint8_t reason)
{
    if (!g_cdr.file)
        return;
    if (ogs_cdr_file_close(g_cdr.file, reason) != 0)
        ogs_error("CDR: file close failed");
    g_cdr.file = NULL;
}

void smf_cdr_final(void)
{
    if (!g_cdr.initialized)
        return;
    file_close(OGS_CDR_FILE_CLOSE_MANUAL);
    seq_save();
    g_cdr.initialized = false;
}

static int write_record(const uint8_t *buf, size_t len)
{
    ogs_time_t now = ogs_time_now();

    /* time-based rotation before append */
    if (g_cdr.file && smf_self()->cdr.file_rotation_interval &&
        now - g_cdr.file_opened_at >=
            (ogs_time_t)ogs_time_from_sec(
                smf_self()->cdr.file_rotation_interval))
        file_close(OGS_CDR_FILE_CLOSE_TIME_LIMIT);

    if (!g_cdr.file) {
        char path[OGS_MAX_FILEPATH_LEN];
        ogs_snprintf(path, sizeof(path), "%s/pgw_%08u.cdr",
                smf_self()->cdr.directory, g_cdr.file_seq);
        g_cdr.file = ogs_cdr_file_open(path, g_cdr.file_seq,
                g_cdr.node_ipv4, ogs_time_sec(now), ogs_timezone() / 60);
        if (!g_cdr.file) {
            ogs_error("CDR: cannot open file [%s]", path);
            return OGS_ERROR;
        }
        g_cdr.file_seq++;
        g_cdr.file_opened_at = now;
        seq_save();
    }

    if (ogs_cdr_file_append(g_cdr.file, buf, len, ogs_time_sec(now)) != 0) {
        ogs_error("CDR: append failed");
        return OGS_ERROR;
    }

    g_cdr.local_seq++;
    seq_save();

    /* size-based rotation after append */
    if (smf_self()->cdr.file_max_size &&
        ogs_cdr_file_size(g_cdr.file) >= smf_self()->cdr.file_max_size)
        file_close(OGS_CDR_FILE_CLOSE_SIZE_LIMIT);

    return OGS_OK;
}

static uint8_t pdn_type_from_session(const smf_sess_t *sess)
{
    switch (sess->session.session_type) {
    case OGS_PDU_SESSION_TYPE_IPV6:
        return OGS_CDR_PDN_TYPE_IPV6;
    case OGS_PDU_SESSION_TYPE_IPV4V6:
        return OGS_CDR_PDN_TYPE_IPV4V6;
    case OGS_PDU_SESSION_TYPE_IPV4:
    default:
        return OGS_CDR_PDN_TYPE_IPV4;
    }
}

static void fill_record(ogs_cdr_pgw_record_t *rec,
        smf_sess_t *sess, int cause, bool final_record)
{
    smf_ue_t *smf_ue = NULL;
    ogs_time_t now = ogs_time_now();

    memset(rec, 0, sizeof(*rec));

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);

    if (smf_ue) {
        if (smf_ue->imsi_len > 0 &&
            smf_ue->imsi_len <= (int)sizeof(rec->imsi)) {
            memcpy(rec->imsi, smf_ue->imsi, smf_ue->imsi_len);
            rec->imsi_len = smf_ue->imsi_len;
        }
        if (smf_ue->msisdn_len > 0 &&
            smf_ue->msisdn_len <= (int)sizeof(rec->msisdn)) {
            memcpy(rec->msisdn, smf_ue->msisdn, smf_ue->msisdn_len);
            rec->msisdn_len = smf_ue->msisdn_len;
        }
        if (smf_ue->imeisv_len > 0 &&
            smf_ue->imeisv_len <= (int)sizeof(rec->imeisv)) {
            memcpy(rec->imeisv, smf_ue->imeisv, smf_ue->imeisv_len);
            rec->imeisv_len = smf_ue->imeisv_len;
        }
    }

    if (sess->session.name)
        ogs_snprintf(rec->apn_ni, sizeof(rec->apn_ni), "%s",
                sess->session.name);
    rec->charging_id = sess->charging.id;

    memcpy(rec->pgw_addr, g_cdr.node_ipv4, 4);
    if (sess->sgw_s5c_ip.ipv4)
        memcpy(rec->sgw_addr, &sess->sgw_s5c_ip.addr, 4);

    rec->pdn_type = pdn_type_from_session(sess);
    if (sess->ipv4) {
        memcpy(rec->pdn_addr, &sess->ipv4->addr, 4);
        rec->has_pdn_addr = 1;
    }
    rec->dynamic_address = 1;

    if (sess->gtp.charging_characteristics.presence &&
        sess->gtp.charging_characteristics.len >= 2)
        memcpy(rec->charging_char,
                sess->gtp.charging_characteristics.data, 2);

    if (ogs_plmn_id_mcc(&sess->serving_plmn_id)) {
        memcpy(rec->serving_plmn, &sess->serving_plmn_id, 3);
        rec->has_serving_plmn = 1;
        memcpy(rec->pgw_plmn, &sess->serving_plmn_id, 3);
        rec->has_pgw_plmn = 1;
    }

    rec->rat_type = sess->gtp_rat_type;

    if (sess->gtp.ue_timezone.presence && sess->gtp.ue_timezone.len >= 2) {
        memcpy(rec->ms_timezone, sess->gtp.ue_timezone.data, 2);
        rec->has_ms_timezone = 1;
    }
    if (sess->gtp.user_location_information.presence &&
        sess->gtp.user_location_information.len > 0) {
        rec->uli_len = ogs_min(sess->gtp.user_location_information.len,
                (int)sizeof(rec->uli));
        memcpy(rec->uli, sess->gtp.user_location_information.data,
                rec->uli_len);
    }

    if (smf_self()->cdr.node_id)
        ogs_snprintf(rec->node_id, sizeof(rec->node_id), "%s",
                smf_self()->cdr.node_id);

    rec->opening_time = ogs_time_sec(sess->cdr.opening_time);
    rec->start_time = ogs_time_sec(sess->cdr.start_time);
    if (final_record) {
        rec->has_stop_time = 1;
        rec->stop_time = ogs_time_sec(now);
    }
    rec->tz_offset_min = ogs_timezone() / 60;
    rec->duration =
        (uint32_t)ogs_time_sec(now - sess->cdr.opening_time);
    rec->cause = cause;

    /* number records only when the session produced partials */
    if (cause == OGS_CDR_CAUSE_VOLUME_LIMIT ||
        cause == OGS_CDR_CAUSE_TIME_LIMIT ||
        sess->cdr.sequence > 0)
        rec->record_sequence_number = ++sess->cdr.sequence;

    rec->local_sequence_number = (uint32_t)(g_cdr.local_seq + 1);

    /* single container, default rating group */
    rec->rating_group = smf_self()->cdr.default_rating_group;
    rec->vol_uplink = sess->cdr.vol_uplink;
    rec->vol_downlink = sess->cdr.vol_downlink;
    if (sess->cdr.time_of_first_usage) {
        rec->has_usage_times = 1;
        rec->time_of_first_usage =
            ogs_time_sec(sess->cdr.time_of_first_usage);
        rec->time_of_last_usage =
            ogs_time_sec(sess->cdr.time_of_last_usage ?
                    sess->cdr.time_of_last_usage :
                    sess->cdr.time_of_first_usage);
    }
    rec->time_of_report = ogs_time_sec(now);

    switch (cause) {
    case OGS_CDR_CAUSE_VOLUME_LIMIT:
        rec->service_condition = OGS_CDR_SCC_VOLUME_LIMIT;
        break;
    case OGS_CDR_CAUSE_TIME_LIMIT:
        rec->service_condition = OGS_CDR_SCC_TIME_LIMIT;
        break;
    default:
        rec->service_condition = OGS_CDR_SCC_RECORD_CLOSURE;
        break;
    }
}

static void time_limit_expired(void *data);

static void time_limit_start(smf_sess_t *sess)
{
    if (!smf_self()->cdr.record_time_limit)
        return;
    if (!sess->cdr.t_time_limit)
        sess->cdr.t_time_limit = ogs_timer_add(ogs_app()->timer_mgr,
                time_limit_expired, (void *)(uintptr_t)sess->id);
    if (sess->cdr.t_time_limit)
        ogs_timer_start(sess->cdr.t_time_limit,
                ogs_time_from_sec(smf_self()->cdr.record_time_limit));
}

static void close_record(smf_sess_t *sess, int cause, bool final_record)
{
    uint8_t buf[OGS_CDR_MAX_RECORD_LEN];
    int len;
    ogs_cdr_pgw_record_t rec;

    fill_record(&rec, sess, cause, final_record);
    len = ogs_cdr_encode_pgw_record(&rec, buf, sizeof(buf));
    if (len <= 0) {
        ogs_error("CDR: encode failed, record dropped");
    } else if (write_record(buf, (size_t)len) != OGS_OK) {
        ogs_error("CDR: write failed, record dropped");
    }

    if (final_record) {
        sess->cdr.active = false;
        if (sess->cdr.t_time_limit)
            ogs_timer_stop(sess->cdr.t_time_limit);
        return;
    }

    /* open the next partial record */
    sess->cdr.vol_uplink = 0;
    sess->cdr.vol_downlink = 0;
    sess->cdr.time_of_first_usage = 0;
    sess->cdr.time_of_last_usage = 0;
    sess->cdr.opening_time = ogs_time_now();
    time_limit_start(sess);
}

static void time_limit_expired(void *data)
{
    smf_bearer_t *bearer = NULL;
    smf_sess_t *sess = smf_sess_find_by_id((ogs_pool_id_t)(uintptr_t)data);

    if (!sess || !sess->cdr.active)
        return;

    bearer = smf_default_bearer_in_sess(sess);
    if (!bearer || !bearer->urr || !sess->pfcp_node) {
        /* no counters to query; close on local state */
        close_record(sess, OGS_CDR_CAUSE_TIME_LIMIT, false);
        return;
    }

    /* Query all URRs; usage arrives in Session Modification Response */
    sess->cdr.pending_time_limit = true;
    if (smf_epc_pfcp_send_all_pdr_modification_request(
            sess, OGS_INVALID_POOL_ID, NULL,
            OGS_PFCP_MODIFY_URR_QUERY,
            OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
            0) != OGS_OK) {
        ogs_error("CDR: QAURR request failed, closing on local counters");
        sess->cdr.pending_time_limit = false;
        close_record(sess, OGS_CDR_CAUSE_TIME_LIMIT, false);
    }
}

void smf_cdr_sess_start(smf_sess_t *sess)
{
    ogs_assert(sess);

    if (!cdr_enabled())
        return;
    if (!sess->epc)    /* 5G SMF-CDR is a later phase */
        return;
    if (sess->cdr.active)
        return;

    memset(&sess->cdr, 0, sizeof(sess->cdr));
    sess->cdr.active = true;
    sess->cdr.start_time = ogs_time_now();
    sess->cdr.opening_time = sess->cdr.start_time;
    time_limit_start(sess);
}

void smf_cdr_sess_usage(smf_sess_t *sess, uint64_t ul_octets,
        uint64_t dl_octets, ogs_time_t time_of_first_packet,
        ogs_time_t time_of_last_packet)
{
    uint64_t volume_limit;

    ogs_assert(sess);

    if (!cdr_enabled() || !sess->cdr.active)
        return;

    sess->cdr.vol_uplink += ul_octets;
    sess->cdr.vol_downlink += dl_octets;
    if (time_of_first_packet && !sess->cdr.time_of_first_usage)
        sess->cdr.time_of_first_usage = time_of_first_packet;
    if (time_of_last_packet)
        sess->cdr.time_of_last_usage = time_of_last_packet;

    if (sess->cdr.pending_time_limit) {
        sess->cdr.pending_time_limit = false;
        close_record(sess, OGS_CDR_CAUSE_TIME_LIMIT, false);
        return;
    }

    volume_limit = smf_self()->cdr.record_volume_limit;
    if (volume_limit &&
        sess->cdr.vol_uplink + sess->cdr.vol_downlink >= volume_limit)
        close_record(sess, OGS_CDR_CAUSE_VOLUME_LIMIT, false);
}

void smf_cdr_sess_stop(smf_sess_t *sess, bool normal)
{
    ogs_assert(sess);

    if (cdr_enabled() && sess->cdr.active)
        close_record(sess,
                normal ? OGS_CDR_CAUSE_NORMAL_RELEASE :
                         OGS_CDR_CAUSE_ABNORMAL_RELEASE, true);

    if (sess->cdr.t_time_limit) {
        ogs_timer_delete(sess->cdr.t_time_limit);
        sess->cdr.t_time_limit = NULL;
    }
}
