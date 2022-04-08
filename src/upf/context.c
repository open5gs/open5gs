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

#include "context.h"
#include "pfcp-path.h"

static upf_context_t self;

int __upf_log_domain;

static OGS_POOL(upf_sess_pool, upf_sess_t);

static int context_initialized = 0;

static void upf_sess_urr_acc_remove_all(upf_sess_t *sess);

void upf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize UPF context */
    memset(&self, 0, sizeof(upf_context_t));

    ogs_log_install_domain(&__upf_log_domain, "upf", ogs_core()->log.level);

    /* Setup UP Function Features */
    ogs_pfcp_self()->up_function_features.ftup = 1;
    ogs_pfcp_self()->up_function_features.empu = 1;
    ogs_pfcp_self()->up_function_features.mnop = 1;
    ogs_pfcp_self()->up_function_features_len = 3;

    ogs_list_init(&self.sess_list);
    ogs_pool_init(&upf_sess_pool, ogs_app()->pool.sess);

    self.sess_hash = ogs_hash_make();
    ogs_assert(self.sess_hash);
    self.ipv4_hash = ogs_hash_make();
    ogs_assert(self.ipv4_hash);
    self.ipv6_hash = ogs_hash_make();
    ogs_assert(self.ipv6_hash);

    context_initialized = 1;
}

void upf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    upf_sess_remove_all();

    ogs_assert(self.sess_hash);
    ogs_hash_destroy(self.sess_hash);
    ogs_assert(self.ipv4_hash);
    ogs_hash_destroy(self.ipv4_hash);
    ogs_assert(self.ipv6_hash);
    ogs_hash_destroy(self.ipv6_hash);

    ogs_pool_final(&upf_sess_pool);

    context_initialized = 0;
}

upf_context_t *upf_self(void)
{
    return &self;
}

static int upf_context_prepare(void)
{
    return OGS_OK;
}

static int upf_context_validation(void)
{
    if (ogs_list_first(&ogs_gtp_self()->gtpu_list) == NULL) {
        ogs_error("No upf.gtpu in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    if (ogs_list_first(&ogs_pfcp_self()->subnet_list) == NULL) {
        ogs_error("No upf.subnet: in '%s'", ogs_app()->file);
        return OGS_ERROR;
    }
    return OGS_OK;
}

int upf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = upf_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "upf")) {
            ogs_yaml_iter_t upf_iter;
            ogs_yaml_iter_recurse(&root_iter, &upf_iter);
            while (ogs_yaml_iter_next(&upf_iter)) {
                const char *upf_key = ogs_yaml_iter_key(&upf_iter);
                ogs_assert(upf_key);
                if (!strcmp(upf_key, "gtpu")) {
                    /* handle config in gtp library */
                } else if (!strcmp(upf_key, "pfcp")) {
                    /* handle config in pfcp library */
                } else if (!strcmp(upf_key, "subnet")) {
                    /* handle config in pfcp library */
                } else
                    ogs_warn("unknown key `%s`", upf_key);
            }
        }
    }

    rv = upf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

upf_sess_t *upf_sess_add(ogs_pfcp_f_seid_t *cp_f_seid)
{
    upf_sess_t *sess = NULL;

    ogs_assert(cp_f_seid);

    ogs_pool_alloc(&upf_sess_pool, &sess);
    ogs_assert(sess);
    memset(sess, 0, sizeof *sess);

    ogs_pfcp_pool_init(&sess->pfcp);

    sess->index = ogs_pool_index(&upf_sess_pool, sess);
    ogs_assert(sess->index > 0 && sess->index <= ogs_app()->pool.sess);

    sess->upf_n4_seid = sess->index;
    sess->smf_n4_seid = cp_f_seid->seid;
    ogs_hash_set(self.sess_hash, &sess->smf_n4_seid,
            sizeof(sess->smf_n4_seid), sess);

    ogs_list_add(&self.sess_list, sess);

    ogs_info("[Added] Number of UPF-Sessions is now %d",
            ogs_list_count(&self.sess_list));

    return sess;
}

int upf_sess_remove(upf_sess_t *sess)
{
    ogs_assert(sess);

    upf_sess_urr_acc_remove_all(sess);

    ogs_list_remove(&self.sess_list, sess);
    ogs_pfcp_sess_clear(&sess->pfcp);

    ogs_hash_set(self.sess_hash, &sess->smf_n4_seid,
            sizeof(sess->smf_n4_seid), NULL);

    if (sess->ipv4) {
        ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(self.ipv6_hash,
                sess->ipv6->addr, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    ogs_pfcp_pool_final(&sess->pfcp);

    ogs_pool_free(&upf_sess_pool, sess);

    ogs_info("[Removed] Number of UPF-sessions is now %d",
            ogs_list_count(&self.sess_list));

    return OGS_OK;
}

void upf_sess_remove_all(void)
{
    upf_sess_t *sess = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.sess_list, next, sess) {
        upf_sess_remove(sess);
    }
}

upf_sess_t *upf_sess_find(uint32_t index)
{
    return ogs_pool_find(&upf_sess_pool, index);
}

upf_sess_t *upf_sess_find_by_cp_seid(uint64_t seid)
{
    return (upf_sess_t *)ogs_hash_get(self.sess_hash, &seid, sizeof(seid));
}

upf_sess_t *upf_sess_find_by_up_seid(uint64_t seid)
{
    return upf_sess_find(seid);
}

upf_sess_t *upf_sess_find_by_ipv4(uint32_t addr)
{
    ogs_assert(self.ipv4_hash);
    return (upf_sess_t *)ogs_hash_get(self.ipv4_hash, &addr, OGS_IPV4_LEN);
}

upf_sess_t *upf_sess_find_by_ipv6(uint32_t *addr6)
{
    ogs_assert(self.ipv6_hash);
    ogs_assert(addr6);
    return (upf_sess_t *)ogs_hash_get(
            self.ipv6_hash, addr6, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3);
}

upf_sess_t *upf_sess_add_by_message(ogs_pfcp_message_t *message)
{
    upf_sess_t *sess = NULL;
    ogs_pfcp_f_seid_t *f_seid = NULL;

    ogs_pfcp_session_establishment_request_t *req =
        &message->pfcp_session_establishment_request;;

    f_seid = req->cp_f_seid.data;
    if (req->cp_f_seid.presence == 0 || f_seid == NULL) {
        ogs_error("No CP F-SEID");
        return NULL;
    }
    f_seid->seid = be64toh(f_seid->seid);

    sess = upf_sess_find_by_cp_seid(f_seid->seid);
    if (!sess) {
        sess = upf_sess_add(f_seid);
        if (!sess) return NULL;
    }
    ogs_assert(sess);

    return sess;
}

uint8_t upf_sess_set_ue_ip(upf_sess_t *sess,
        uint8_t session_type, ogs_pfcp_pdr_t *pdr)
{
    ogs_pfcp_ue_ip_addr_t *ue_ip = NULL;
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    uint8_t cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    ogs_assert(sess);
    ogs_assert(session_type);
    ogs_assert(pdr);
    ogs_assert(pdr->ue_ip_addr_len);
    ue_ip = &pdr->ue_ip_addr;
    ogs_assert(ue_ip);

    if (sess->ipv4) {
        ogs_hash_set(self.ipv4_hash,
                sess->ipv4->addr, OGS_IPV4_LEN, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv4);
    }
    if (sess->ipv6) {
        ogs_hash_set(self.ipv6_hash,
                sess->ipv6->addr, OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, NULL);
        ogs_pfcp_ue_ip_free(sess->ipv6);
    }

    /* Set PDN-Type and UE IP Address */
    if (session_type == OGS_PDU_SESSION_TYPE_IPV4) {
        if (ue_ip->ipv4 || pdr->dnn) {
            sess->ipv4 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET,
                            pdr->dnn, (uint8_t *)&(ue_ip->addr));
            if (!sess->ipv4) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                return cause_value;
            }
            ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }
    } else if (session_type == OGS_PDU_SESSION_TYPE_IPV6) {
        if (ue_ip->ipv6 || pdr->dnn) {
            sess->ipv6 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET6,
                            pdr->dnn, ue_ip->addr6);
            if (!sess->ipv6) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                return cause_value;
            }
            ogs_hash_set(self.ipv6_hash, sess->ipv6->addr,
                    OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }
    } else if (session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        if (ue_ip->ipv4 || pdr->dnn) {
            sess->ipv4 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET,
                            pdr->dnn, (uint8_t *)&(ue_ip->both.addr));
            if (!sess->ipv4) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                return cause_value;
            }
            ogs_hash_set(self.ipv4_hash, sess->ipv4->addr, OGS_IPV4_LEN, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }

        if (ue_ip->ipv6 || pdr->dnn) {
            sess->ipv6 = ogs_pfcp_ue_ip_alloc(&cause_value, AF_INET6,
                            pdr->dnn, ue_ip->both.addr6);
            if (!sess->ipv6) {
                ogs_error("ogs_pfcp_ue_ip_alloc() failed[%d]", cause_value);
                ogs_assert(cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
                if (sess->ipv4) {
                    ogs_hash_set(self.ipv4_hash,
                            sess->ipv4->addr, OGS_IPV4_LEN, NULL);
                    ogs_pfcp_ue_ip_free(sess->ipv4);
                    sess->ipv4 = NULL;
                }
                return cause_value;
            }
            ogs_hash_set(self.ipv6_hash, sess->ipv6->addr,
                    OGS_IPV6_DEFAULT_PREFIX_LEN >> 3, sess);
        } else {
            ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
        }
    } else {
        ogs_warn("Cannot support PDN-Type[%d], [IPv4:%d IPv6:%d DNN:%s]",
                session_type, ue_ip->ipv4, ue_ip->ipv6,
                pdr->dnn ? pdr->dnn : "");
    }

    ogs_info("UE F-SEID[CP:0x%lx UP:0x%lx] "
             "APN[%s] PDN-Type[%d] IPv4[%s] IPv6[%s]",
        (long)sess->upf_n4_seid, (long)sess->smf_n4_seid,
        pdr->dnn, session_type,
        sess->ipv4 ? OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
        sess->ipv6 ? OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");

    return cause_value;
}

void upf_sess_urr_acc_add(upf_sess_t *sess, ogs_pfcp_urr_t *urr, size_t size, bool is_uplink)
{
    upf_sess_urr_acc_t *urr_acc = &sess->urr_acc[urr->id];
    /* Increment total & ul octets + pkts */
    urr_acc->total_octets += size;
    urr_acc->total_pkts++;
    if (is_uplink) {
        urr_acc->dl_octets += size;
        urr_acc->dl_pkts++;
    } else {
        urr_acc->ul_octets += size;
        urr_acc->ul_pkts++;
    }

    urr_acc->time_of_last_packet = ogs_time_now();
    if (urr_acc->time_of_first_packet == 0)
        urr_acc->time_of_first_packet = urr_acc->time_of_last_packet;

    /* TODO: generate report if volume threshold/quota is reached, eg sess->urr_acc[urr->id].total_octets - sess->urr_acc[urr->id].last_report.total_octets > threshold */
}

/* report struct must be memzeroed before first use of this function.
 * report->num_of_usage_report must be set by the caller */
void upf_sess_urr_acc_fill_usage_report(upf_sess_t *sess, const ogs_pfcp_urr_t *urr,
                                  ogs_pfcp_user_plane_report_t *report, unsigned int idx)
{
    upf_sess_urr_acc_t *urr_acc = &sess->urr_acc[urr->id];
    ogs_time_t last_report_timestamp;
    ogs_time_t now;

    now = ogs_time_now(); /* we need UTC for start_time and end_time */

    if (urr_acc->last_report.timestamp)
        last_report_timestamp = urr_acc->last_report.timestamp;
    else
        last_report_timestamp = ogs_time_from_ntp32(urr_acc->time_threshold_start);

    report->type.usage_report = 1;
    report->usage_report[idx].id = urr->id;
    report->usage_report[idx].seqn = urr_acc->report_seqn++;
    report->usage_report[idx].start_time = urr_acc->time_threshold_start;
    report->usage_report[idx].end_time = ogs_time_to_ntp32(now);
    report->usage_report[idx].vol_measurement = (ogs_pfcp_volume_measurement_t){
        .dlnop = 1,
        .ulnop = 1,
        .tonop = 1,
        .dlvol = 1,
        .ulvol = 1,
        .tovol = 1,
        .total_volume = urr_acc->total_octets - urr_acc->last_report.total_octets,
        .uplink_volume = urr_acc->ul_octets - urr_acc->last_report.ul_octets,
        .downlink_volume = urr_acc->dl_octets - urr_acc->last_report.dl_octets,
        .total_n_packets = urr_acc->total_pkts - urr_acc->last_report.total_pkts,
        .uplink_n_packets = urr_acc->ul_pkts - urr_acc->last_report.ul_pkts,
        .downlink_n_packets = urr_acc->dl_pkts - urr_acc->last_report.dl_pkts,
    };
    if (now >= last_report_timestamp)
        report->usage_report[idx].dur_measurement = ((now - last_report_timestamp) + (OGS_USEC_PER_SEC/2)) / OGS_USEC_PER_SEC; /* FIXME: should use MONOTONIC here */
    /* else memset sets it to 0 */
    report->usage_report[idx].time_of_first_packet = ogs_time_to_ntp32(urr_acc->time_of_first_packet); /* TODO: First since last report? */
    report->usage_report[idx].time_of_last_packet = ogs_time_to_ntp32(urr_acc->time_of_last_packet);

    if (urr->time_threshold > 0 &&
            report->usage_report[idx].dur_measurement >= urr->time_threshold)
        report->usage_report[idx].rep_trigger.time_threshold = 1;
}

void upf_sess_urr_acc_snapshot(upf_sess_t *sess, ogs_pfcp_urr_t *urr)
{
    upf_sess_urr_acc_t *urr_acc = &sess->urr_acc[urr->id];
    urr_acc->last_report.total_octets = urr_acc->total_octets;
    urr_acc->last_report.dl_octets = urr_acc->dl_octets;
    urr_acc->last_report.ul_octets = urr_acc->ul_octets;
    urr_acc->last_report.total_pkts = urr_acc->total_pkts;
    urr_acc->last_report.dl_pkts = urr_acc->dl_pkts;
    urr_acc->last_report.ul_pkts = urr_acc->ul_pkts;
    urr_acc->last_report.timestamp = ogs_time_now();
}

static void upf_sess_urr_acc_time_threshold_cb(void *data)
{
    ogs_pfcp_urr_t *urr = (ogs_pfcp_urr_t *)data;
    ogs_pfcp_user_plane_report_t report;
    ogs_pfcp_sess_t *pfcp_sess = urr->sess;
    upf_sess_t *sess = UPF_SESS(pfcp_sess);

    ogs_warn("upf_time_threshold_cb() triggered! urr=%p", urr);

    if (urr->rep_triggers.time_threshold) {
        memset(&report, 0, sizeof(report));
        upf_sess_urr_acc_fill_usage_report(sess, urr, &report, 0);
        report.num_of_usage_report = 1;
        upf_sess_urr_acc_snapshot(sess, urr);

        ogs_assert(OGS_OK ==
            upf_pfcp_send_session_report_request(sess, &report));
    }
    /* Start new report period/iteration: */
    upf_sess_urr_acc_time_threshold_setup(sess, urr);
}

void upf_sess_urr_acc_time_threshold_setup(upf_sess_t *sess, ogs_pfcp_urr_t *urr)
{
    upf_sess_urr_acc_t *urr_acc = &sess->urr_acc[urr->id];

    ogs_debug("Installing URR time threshold timer");
    urr_acc->reporting_enabled = true;
    if (!urr_acc->t_time_threshold)
        urr_acc->t_time_threshold = ogs_timer_add(ogs_app()->timer_mgr,
                                        upf_sess_urr_acc_time_threshold_cb, urr);
    urr_acc->time_threshold_start = ogs_time_ntp32_now();
    ogs_timer_start(urr_acc->t_time_threshold, urr->time_threshold * OGS_USEC_PER_SEC);
}

static void upf_sess_urr_acc_remove_all(upf_sess_t *sess)
{
    unsigned int i;
    for (i = 0; i < OGS_ARRAY_SIZE(sess->urr_acc); i++) {
        if (sess->urr_acc[i].t_time_threshold) {
            ogs_timer_delete(sess->urr_acc[i].t_time_threshold);
            sess->urr_acc[i].t_time_threshold = NULL;
        }
    }
}
