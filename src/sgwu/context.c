/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "context.h"
#include "pfcp-path.h"

static sgwu_context_t self;

int __sgwu_log_domain;

static OGS_POOL(sgwu_sess_pool, sgwu_sess_t);
static OGS_POOL(sgwu_sxa_seid_pool, ogs_pool_id_t);

static int context_initialized = 0;

void sgwu_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize SGWU context */
    memset(&self, 0, sizeof(sgwu_context_t));

    ogs_log_install_domain(&__sgwu_log_domain, "sgwu", ogs_core()->log.level);

    /* Setup UP Function Features */
    ogs_pfcp_self()->up_function_features.ftup = 1;
    ogs_pfcp_self()->up_function_features.empu = 1;
    ogs_pfcp_self()->up_function_features_len = 2;

    ogs_list_init(&self.sess_list);
    ogs_pool_init(&sgwu_sess_pool, ogs_app()->pool.sess);
    ogs_pool_init(&sgwu_sxa_seid_pool, ogs_app()->pool.sess);
    ogs_pool_random_id_generate(&sgwu_sxa_seid_pool);

    self.sgwu_sxa_seid_hash = ogs_hash_make();
    ogs_assert(self.sgwu_sxa_seid_hash);
    self.sgwc_sxa_seid_hash = ogs_hash_make();
    ogs_assert(self.sgwc_sxa_seid_hash);
    self.sgwc_sxa_f_seid_hash = ogs_hash_make();
    ogs_assert(self.sgwc_sxa_f_seid_hash);

    context_initialized = 1;
}

void sgwu_context_final(void)
{
    ogs_assert(context_initialized == 1);

    sgwu_sess_remove_all();

    ogs_assert(self.sgwu_sxa_seid_hash);
    ogs_hash_destroy(self.sgwu_sxa_seid_hash);
    ogs_assert(self.sgwc_sxa_seid_hash);
    ogs_hash_destroy(self.sgwc_sxa_seid_hash);
    ogs_assert(self.sgwc_sxa_f_seid_hash);
    ogs_hash_destroy(self.sgwc_sxa_f_seid_hash);

    ogs_pool_final(&sgwu_sess_pool);
    ogs_pool_final(&sgwu_sxa_seid_pool);

    context_initialized = 0;
}

sgwu_context_t *sgwu_self(void)
{
    return &self;
}

static int sgwu_context_prepare(void)
{
    return OGS_OK;
}

static int sgwu_context_validation(void)
{
    if (ogs_list_first(&ogs_gtp_self()->gtpu_list) == NULL) {
        ogs_error("No sgwu.gtpu in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int sgwu_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = sgwu_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "sgwu")) {
            ogs_yaml_iter_t sgwu_iter;
            ogs_yaml_iter_recurse(&root_iter, &sgwu_iter);
            while (ogs_yaml_iter_next(&sgwu_iter)) {
                const char *sgwu_key = ogs_yaml_iter_key(&sgwu_iter);
                ogs_assert(sgwu_key);
                if (!strcmp(sgwu_key, "gtpu")) {
                    /* handle config in gtp library */
                } else if (!strcmp(sgwu_key, "pfcp")) {
                    /* handle config in pfcp library */
                } else if (!strcmp(sgwu_key, "sgwc")) {
                    /* handle config in pfcp library */
                } else
                    ogs_warn("unknown key `%s`", sgwu_key);
            }
        }
    }

    rv = sgwu_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

sgwu_sess_t *sgwu_sess_add(ogs_pfcp_f_seid_t *cp_f_seid)
{
    sgwu_sess_t *sess = NULL;

    ogs_assert(cp_f_seid);

    ogs_pool_id_calloc(&sgwu_sess_pool, &sess);
    if (!sess) {
        ogs_error("ogs_pool_id_calloc() is failed");
        return NULL;
    }

    ogs_pfcp_pool_init(&sess->pfcp);

    /* Set SEID */
    ogs_pool_alloc(&sgwu_sxa_seid_pool, &sess->sgwu_sxa_seid_node);
    ogs_assert(sess->sgwu_sxa_seid_node);

    sess->sgwu_sxa_seid = *(sess->sgwu_sxa_seid_node);

    ogs_hash_set(self.sgwu_sxa_seid_hash, &sess->sgwu_sxa_seid,
            sizeof(sess->sgwu_sxa_seid), sess);

    /* Since F-SEID is composed of ogs_ip_t and uint64-seid,
     * all these values must be put into the structure-sgwc_sxa_f_eid
     * before creating hash */
    sess->sgwc_sxa_f_seid.seid = cp_f_seid->seid;
    ogs_assert(OGS_OK ==
            ogs_pfcp_f_seid_to_ip(cp_f_seid, &sess->sgwc_sxa_f_seid.ip));

    ogs_hash_set(self.sgwc_sxa_f_seid_hash, &sess->sgwc_sxa_f_seid,
            sizeof(sess->sgwc_sxa_f_seid), sess);
    ogs_hash_set(self.sgwc_sxa_seid_hash, &sess->sgwc_sxa_f_seid.seid,
            sizeof(sess->sgwc_sxa_f_seid.seid), sess);

    ogs_info("UE F-SEID[UP:0x%lx CP:0x%lx]",
        (long)sess->sgwu_sxa_seid, (long)sess->sgwc_sxa_f_seid.seid);

    ogs_list_add(&self.sess_list, sess);

    ogs_info("[Added] Number of SGWU-Sessions is now %d",
            ogs_list_count(&self.sess_list));

    return sess;
}

int sgwu_sess_remove(sgwu_sess_t *sess)
{
    ogs_assert(sess);

    ogs_list_remove(&self.sess_list, sess);
    ogs_pfcp_sess_clear(&sess->pfcp);

    ogs_hash_set(self.sgwu_sxa_seid_hash, &sess->sgwu_sxa_seid,
            sizeof(sess->sgwu_sxa_seid), NULL);

    ogs_hash_set(self.sgwc_sxa_seid_hash, &sess->sgwc_sxa_f_seid.seid,
            sizeof(sess->sgwc_sxa_f_seid.seid), NULL);
    ogs_hash_set(self.sgwc_sxa_f_seid_hash, &sess->sgwc_sxa_f_seid,
            sizeof(sess->sgwc_sxa_f_seid), NULL);

    ogs_pfcp_pool_final(&sess->pfcp);

    ogs_pool_free(&sgwu_sxa_seid_pool, sess->sgwu_sxa_seid_node);
    ogs_pool_id_free(&sgwu_sess_pool, sess);

    ogs_info("[Removed] Number of SGWU-sessions is now %d",
            ogs_list_count(&self.sess_list));

    return OGS_OK;
}

void sgwu_sess_remove_all(void)
{
    sgwu_sess_t *sess = NULL, *next = NULL;

    ogs_list_for_each_safe(&self.sess_list, next, sess) {
        sgwu_sess_remove(sess);
    }
}

sgwu_sess_t *sgwu_sess_find_by_sgwc_sxa_seid(uint64_t seid)
{
    return ogs_hash_get(self.sgwc_sxa_seid_hash, &seid, sizeof(seid));
}

sgwu_sess_t *sgwu_sess_find_by_sgwc_sxa_f_seid(ogs_pfcp_f_seid_t *f_seid)
{
    struct {
        uint64_t seid;
        ogs_ip_t ip;
    } key;

    ogs_assert(f_seid);
    ogs_assert(OGS_OK == ogs_pfcp_f_seid_to_ip(f_seid, &key.ip));
    key.seid = f_seid->seid;

    return ogs_hash_get(self.sgwc_sxa_f_seid_hash, &key, sizeof(key));
}

sgwu_sess_t *sgwu_sess_find_by_sgwu_sxa_seid(uint64_t seid)
{
    return ogs_hash_get(self.sgwu_sxa_seid_hash, &seid, sizeof(seid));
}

sgwu_sess_t *sgwu_sess_find_by_id(ogs_pool_id_t id)
{
    return ogs_pool_find_by_id(&sgwu_sess_pool, id);
}

sgwu_sess_t *sgwu_sess_add_by_message(ogs_pfcp_message_t *message)
{
    sgwu_sess_t *sess = NULL;

    ogs_pfcp_f_seid_t *f_seid = NULL;

    ogs_pfcp_session_establishment_request_t *req =
        &message->pfcp_session_establishment_request;;

    f_seid = req->cp_f_seid.data;
    if (req->cp_f_seid.presence == 0 || f_seid == NULL) {
        ogs_error("No CP F-SEID");
        return NULL;
    }
    if (f_seid->ipv4 == 0 && f_seid->ipv6 == 0) {
        ogs_error("No IPv4 or IPv6");
        return NULL;
    }
    f_seid->seid = be64toh(f_seid->seid);

    sess = sgwu_sess_find_by_sgwc_sxa_f_seid(f_seid);
    if (!sess) {
        sess = sgwu_sess_add(f_seid);
        if (!sess) {
            ogs_error("No Session Context");
            return NULL;
        }
    }
    ogs_assert(sess);

    return sess;
}

void sgwu_sess_urr_acc_add(sgwu_sess_t *sess,
        ogs_pfcp_urr_t *urr, size_t size, bool is_uplink)
{
    sgwu_sess_urr_acc_t *urr_acc = NULL;
    uint64_t vol;

    ogs_assert(urr->id > 0 && urr->id <= OGS_MAX_NUM_OF_URR);
    urr_acc = &sess->urr_acc[urr->id-1];

    if (urr_acc->time_start == 0)
        urr_acc->time_start = ogs_time_ntp32_now();

    urr_acc->total_octets += size;
    urr_acc->total_pkts++;
    if (is_uplink) {
        urr_acc->ul_octets += size;
        urr_acc->ul_pkts++;
    } else {
        urr_acc->dl_octets += size;
        urr_acc->dl_pkts++;
    }

    urr_acc->time_of_last_packet = ogs_time_now();
    if (urr_acc->time_of_first_packet == 0)
        urr_acc->time_of_first_packet = urr_acc->time_of_last_packet;

    /* generate report if volume threshold is reached */
    vol = urr_acc->total_octets - urr_acc->last_report.total_octets;
    if (urr->rep_triggers.volume_threshold && urr->vol_threshold.tovol &&
        vol >= urr->vol_threshold.total_volume) {
        ogs_pfcp_user_plane_report_t report;
        memset(&report, 0, sizeof(report));
        sgwu_sess_urr_acc_fill_usage_report(sess, urr, &report, 0);
        report.num_of_usage_report = 1;
        sgwu_sess_urr_acc_snapshot(sess, urr);

        ogs_assert(OGS_OK ==
            sgwu_pfcp_send_session_report_request(sess, &report));
    }
}

/* report struct must be memzeroed before first use of this function.
 * report->num_of_usage_report must be set by the caller */
void sgwu_sess_urr_acc_fill_usage_report(sgwu_sess_t *sess,
        const ogs_pfcp_urr_t *urr,
        ogs_pfcp_user_plane_report_t *report, unsigned int idx)
{
    sgwu_sess_urr_acc_t *urr_acc = NULL;
    ogs_time_t last_report_timestamp;
    ogs_time_t now;

    ogs_assert(urr->id > 0 && urr->id <= OGS_MAX_NUM_OF_URR);
    urr_acc = &sess->urr_acc[urr->id-1];

    now = ogs_time_now();

    if (urr_acc->last_report.timestamp)
        last_report_timestamp = urr_acc->last_report.timestamp;
    else
        last_report_timestamp = ogs_time_from_ntp32(urr_acc->time_start);

    report->type.usage_report = 1;
    report->usage_report[idx].id = urr->id;
    report->usage_report[idx].seqn = urr_acc->report_seqn++;
    report->usage_report[idx].start_time = urr_acc->time_start;
    report->usage_report[idx].end_time = ogs_time_to_ntp32(now);
    report->usage_report[idx].vol_measurement =
        (ogs_pfcp_volume_measurement_t){
        .dlnop = 1,
        .ulnop = 1,
        .tonop = 1,
        .dlvol = 1,
        .ulvol = 1,
        .tovol = 1,
        .total_volume =
            urr_acc->total_octets - urr_acc->last_report.total_octets,
        .uplink_volume =
            urr_acc->ul_octets - urr_acc->last_report.ul_octets,
        .downlink_volume =
            urr_acc->dl_octets - urr_acc->last_report.dl_octets,
        .total_n_packets =
            urr_acc->total_pkts - urr_acc->last_report.total_pkts,
        .uplink_n_packets =
            urr_acc->ul_pkts - urr_acc->last_report.ul_pkts,
        .downlink_n_packets =
            urr_acc->dl_pkts - urr_acc->last_report.dl_pkts,
    };
    if (now >= last_report_timestamp)
        report->usage_report[idx].dur_measurement =
            ((now - last_report_timestamp) + (OGS_USEC_PER_SEC/2)) /
                OGS_USEC_PER_SEC;
    report->usage_report[idx].time_of_first_packet =
        ogs_time_to_ntp32(urr_acc->time_of_first_packet);
    report->usage_report[idx].time_of_last_packet =
        ogs_time_to_ntp32(urr_acc->time_of_last_packet);

    if (urr->rep_triggers.volume_threshold && urr->vol_threshold.tovol &&
        report->usage_report[idx].vol_measurement.total_volume >=
            urr->vol_threshold.total_volume)
        report->usage_report[idx].rep_trigger.volume_threshold = 1;
}

void sgwu_sess_urr_acc_snapshot(sgwu_sess_t *sess, ogs_pfcp_urr_t *urr)
{
    sgwu_sess_urr_acc_t *urr_acc = NULL;

    ogs_assert(urr->id > 0 && urr->id <= OGS_MAX_NUM_OF_URR);
    urr_acc = &sess->urr_acc[urr->id-1];

    urr_acc->last_report.total_octets = urr_acc->total_octets;
    urr_acc->last_report.dl_octets = urr_acc->dl_octets;
    urr_acc->last_report.ul_octets = urr_acc->ul_octets;
    urr_acc->last_report.total_pkts = urr_acc->total_pkts;
    urr_acc->last_report.dl_pkts = urr_acc->dl_pkts;
    urr_acc->last_report.ul_pkts = urr_acc->ul_pkts;
    urr_acc->last_report.timestamp = ogs_time_now();
}
