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

#include "ogs-app.h"

static ogs_app_global_conf_t global_conf;
static ogs_app_local_conf_t local_conf;

static OGS_POOL(policy_conf_pool, ogs_app_policy_conf_t);
static OGS_POOL(sess_conf_pool, ogs_app_sess_conf_t);

static int initialized = 0;

int ogs_app_config_init(void)
{
    ogs_assert(initialized == 0);

    memset(&global_conf, 0, sizeof(ogs_app_global_conf_t));
    memset(&local_conf, 0, sizeof(ogs_app_local_conf_t));

    ogs_pool_init(&policy_conf_pool, OGS_MAX_NUM_OF_SLICE);
    ogs_pool_init(&sess_conf_pool,
            OGS_MAX_NUM_OF_SLICE*OGS_MAX_NUM_OF_SESS);

    initialized = 1;

    return OGS_OK;
}

void ogs_app_config_final(void)
{
    ogs_assert(initialized == 1);

    ogs_app_policy_conf_remove_all();

    ogs_pool_final(&policy_conf_pool);
    ogs_pool_final(&sess_conf_pool);

    initialized = 0;
}

static void recalculate_pool_size(void)
{
    ogs_app()->pool.packet =
        global_conf.max.ue * OGS_MAX_NUM_OF_PACKET_BUFFER;

#define MAX_NUM_OF_TUNNEL       3   /* Num of Tunnel per Bearer */
    ogs_app()->pool.sess = global_conf.max.ue * OGS_MAX_NUM_OF_SESS;
    ogs_app()->pool.bearer = ogs_app()->pool.sess * OGS_MAX_NUM_OF_BEARER;
    ogs_app()->pool.tunnel = ogs_app()->pool.bearer * MAX_NUM_OF_TUNNEL;

#define POOL_NUM_PER_UE 16
    ogs_app()->pool.timer = global_conf.max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.message = global_conf.max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.event = global_conf.max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.socket = global_conf.max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.xact = global_conf.max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.stream = global_conf.max.ue * POOL_NUM_PER_UE;

    ogs_app()->pool.nf = global_conf.max.peer;
#define NF_SERVICE_PER_NF_INSTANCE 16
    ogs_app()->pool.nf_service =
        ogs_app()->pool.nf * NF_SERVICE_PER_NF_INSTANCE;
    ogs_app()->pool.subscription =
        ogs_app()->pool.nf * NF_SERVICE_PER_NF_INSTANCE;

    ogs_app()->pool.gtp_node = ogs_app()->pool.nf;
    if (global_conf.max.gtp_peer)
        ogs_app()->pool.gtp_node = global_conf.max.gtp_peer;

    /* Num of TAI-LAI Mapping Table */
    ogs_app()->pool.csmap = ogs_app()->pool.nf;

#define MAX_NUM_OF_IMPU         8
    ogs_app()->pool.impi = global_conf.max.ue;
    ogs_app()->pool.impu = ogs_app()->pool.impi * MAX_NUM_OF_IMPU;
}

ogs_app_global_conf_t *ogs_global_conf(void)
{
    return &global_conf;
}

ogs_app_local_conf_t *ogs_local_conf(void)
{
    return &local_conf;
}

static int global_conf_prepare(void)
{
    global_conf.sockopt.no_delay = true;

#define MAX_NUM_OF_UE               1024    /* Num of UEs */
#define MAX_NUM_OF_PEER             64      /* Num of Peer */

    global_conf.max.ue = MAX_NUM_OF_UE;
    global_conf.max.peer = MAX_NUM_OF_PEER;

    ogs_pkbuf_default_init(&global_conf.pkbuf_config);

    recalculate_pool_size();

    return OGS_OK;
}

static int global_conf_validation(void)
{
    if (global_conf.parameter.no_ipv4 == 1 &&
        global_conf.parameter.no_ipv6 == 1) {
        ogs_error("Both `no_ipv4` and `no_ipv6` set to `true` in `%s`",
                ogs_app()->file);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_app_parse_global_conf(ogs_yaml_iter_t *parent)
{
    int rv;
    ogs_yaml_iter_t global_iter;

    ogs_assert(parent);

    rv = global_conf_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_recurse(parent, &global_iter);
    while (ogs_yaml_iter_next(&global_iter)) {
        const char *global_key = ogs_yaml_iter_key(&global_iter);
        ogs_assert(global_key);
        if (!strcmp(global_key, "parameter")) {
            ogs_yaml_iter_t parameter_iter;
            ogs_yaml_iter_recurse(&global_iter, &parameter_iter);
            while (ogs_yaml_iter_next(&parameter_iter)) {
                const char *parameter_key = ogs_yaml_iter_key(&parameter_iter);
                ogs_assert(parameter_key);
                if (!strcmp(parameter_key, "no_hss")) {
                    global_conf.parameter.no_hss =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_mme")) {
                    global_conf.parameter.no_mme =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_sgwu")) {
                    global_conf.parameter.no_sgwu =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_sgwc")) {
                    global_conf.parameter.no_sgwc =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_sgw")) {
                    global_conf.parameter.no_sgw =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_pgw")) {
                    global_conf.parameter.no_pgw =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_pcrf")) {
                    global_conf.parameter.no_pcrf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_nrf")) {
                    global_conf.parameter.no_nrf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_scp")) {
                    global_conf.parameter.no_scp =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_sepp")) {
                    global_conf.parameter.no_sepp =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_amf")) {
                    global_conf.parameter.no_amf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_smf")) {
                    global_conf.parameter.no_smf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_upf")) {
                    global_conf.parameter.no_upf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_ausf")) {
                    global_conf.parameter.no_ausf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_udm")) {
                    global_conf.parameter.no_udm =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_pcf")) {
                    global_conf.parameter.no_pcf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_nssf")) {
                    global_conf.parameter.no_nssf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_bsf")) {
                    global_conf.parameter.no_bsf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_udr")) {
                    global_conf.parameter.no_udr =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_ipv4")) {
                    global_conf.parameter.no_ipv4 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_ipv6")) {
                    global_conf.parameter.no_ipv6 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "prefer_ipv4")) {
                    global_conf.parameter.prefer_ipv4 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "multicast")) {
                    global_conf.parameter.multicast =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "use_openair")) {
                    global_conf.parameter.use_openair =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key,
                            "no_ipv4v6_local_addr_in_packet_filter")) {
                    global_conf.parameter.
                        no_ipv4v6_local_addr_in_packet_filter =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key,
                            "no_pfcp_rr_select")) {
                    global_conf.parameter.no_pfcp_rr_select =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else
                    ogs_warn("unknown key `%s`", parameter_key);
            }
        } else if (!strcmp(global_key, "sockopt")) {
            ogs_yaml_iter_t sockopt_iter;
            ogs_yaml_iter_recurse(&global_iter, &sockopt_iter);
            while (ogs_yaml_iter_next(&sockopt_iter)) {
                const char *sockopt_key =
                    ogs_yaml_iter_key(&sockopt_iter);
                ogs_assert(sockopt_key);
                if (!strcmp(sockopt_key, "no_delay")) {
                    global_conf.sockopt.no_delay =
                        ogs_yaml_iter_bool(&sockopt_iter);
                } else if (!strcmp(sockopt_key, "linger")) {
                    const char *v = ogs_yaml_iter_value(&sockopt_iter);
                    if (v)
                        global_conf.sockopt.l_linger = atoi(v);
                    global_conf.sockopt.l_onoff = true;
                } else
                    ogs_warn("unknown key `%s`", sockopt_key);
            }
        } else if (!strcmp(global_key, "max")) {
            ogs_yaml_iter_t max_iter;
            ogs_yaml_iter_recurse(&global_iter, &max_iter);
            while (ogs_yaml_iter_next(&max_iter)) {
                const char *max_key = ogs_yaml_iter_key(&max_iter);
                ogs_assert(max_key);
                if (!strcmp(max_key, "ue")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) global_conf.max.ue = atoi(v);
                } else if (!strcmp(max_key, "peer") ||
                            !strcmp(max_key, "enb")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) global_conf.max.peer = atoi(v);
                } else if (!strcmp(max_key, "gtp_peer") ||
                            !strcmp(max_key, "enb")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) global_conf.max.gtp_peer = atoi(v);
                } else
                    ogs_warn("unknown key `%s`", max_key);
            }

            recalculate_pool_size();

        } else if (!strcmp(global_key, "pool")) {
            ogs_yaml_iter_t pool_iter;
            ogs_yaml_iter_recurse(&global_iter, &pool_iter);
            while (ogs_yaml_iter_next(&pool_iter)) {
                const char *pool_key = ogs_yaml_iter_key(&pool_iter);
                ogs_assert(pool_key);
                if (!strcmp(pool_key, "128")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_128_pool = atoi(v);
                } else if (!strcmp(pool_key, "256")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_256_pool = atoi(v);
                } else if (!strcmp(pool_key, "512")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_512_pool = atoi(v);
                } else if (!strcmp(pool_key, "1024")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_1024_pool = atoi(v);
                } else if (!strcmp(pool_key, "2048")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_2048_pool = atoi(v);
                } else if (!strcmp(pool_key, "8192")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_8192_pool = atoi(v);
                } else if (!strcmp(pool_key, "32768")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_32768_pool =
                        atoi(v);
                } else if (!strcmp(pool_key, "big")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v) global_conf.pkbuf_config.cluster_big_pool = atoi(v);
                } else
                    ogs_warn("unknown key `%s`", pool_key);
            }
        }
    }

    rv = global_conf_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

static void regenerate_all_timer_duration(void)
{
    ogs_assert(local_conf.time.message.duration);

    local_conf.time.message.sbi.client_wait_duration =
        local_conf.time.message.duration;
    local_conf.time.message.sbi.connection_deadline =
        local_conf.time.message.sbi.client_wait_duration + ogs_time_from_sec(1);
    local_conf.time.message.sbi.reconnect_interval =
        ogs_max(ogs_time_from_sec(3),
            local_conf.time.message.sbi.client_wait_duration +
            ogs_time_from_sec(1));
    local_conf.time.message.sbi.reconnect_interval_in_exception =
                ogs_time_from_sec(2);

#define PFCP_N1_RESPONSE_RETRY_COUNT  3
    local_conf.time.message.pfcp.n1_response_rcount =
        PFCP_N1_RESPONSE_RETRY_COUNT;
    local_conf.time.message.pfcp.t1_response_duration =
        (local_conf.time.message.duration /
         (local_conf.time.message.pfcp.n1_response_rcount + 1));
    ogs_assert(local_conf.time.message.pfcp.t1_response_duration);

#define PFCP_N1_HOLDING_RETRY_COUNT 1
    local_conf.time.message.pfcp.n1_holding_rcount =
        PFCP_N1_HOLDING_RETRY_COUNT;
    local_conf.time.message.pfcp.t1_holding_duration =
        local_conf.time.message.pfcp.n1_response_rcount *
        local_conf.time.message.pfcp.t1_response_duration;
    ogs_assert(local_conf.time.message.pfcp.t1_holding_duration);

    local_conf.time.message.pfcp.association_interval =
        ogs_max(ogs_time_from_sec(3),
            local_conf.time.message.sbi.client_wait_duration +
            ogs_time_from_sec(1));

    local_conf.time.message.pfcp.no_heartbeat_duration =
        ogs_max(ogs_time_from_sec(10),
            local_conf.time.message.sbi.client_wait_duration +
            ogs_time_from_sec(1));

#define GTP_N3_RESPONSE_RETRY_COUNT  3
    local_conf.time.message.gtp.n3_response_rcount =
        GTP_N3_RESPONSE_RETRY_COUNT;
    local_conf.time.message.gtp.t3_response_duration =
        (local_conf.time.message.duration /
         (local_conf.time.message.gtp.n3_response_rcount + 1));
    ogs_assert(local_conf.time.message.gtp.t3_response_duration);

#define GTP_N3_HOLDING_RETRY_COUNT 1
    local_conf.time.message.gtp.n3_holding_rcount = GTP_N3_HOLDING_RETRY_COUNT;
    local_conf.time.message.gtp.t3_holding_duration =
        local_conf.time.message.gtp.n3_response_rcount *
        local_conf.time.message.gtp.t3_response_duration;
    ogs_assert(local_conf.time.message.gtp.t3_holding_duration);

#if 0
    ogs_trace("%lld, %lld, %lld, %d, %lld, %d %lld, %d, %lld, %d, %lld",
        (long long)local_conf.time.message.duration,
        (long long)local_conf.time.message.sbi.client_wait_duration,
        (long long)local_conf.time.message.sbi.connection_deadline,
        local_conf.time.message.pfcp.n1_response_rcount,
        (long long)local_conf.time.message.pfcp.t1_response_duration,
        local_conf.time.message.pfcp.n1_holding_rcount,
        (long long)local_conf.time.message.pfcp.t1_holding_duration,
        local_conf.time.message.gtp.n3_response_rcount,
        (long long)local_conf.time.message.gtp.t3_response_duration,
        local_conf.time.message.gtp.n3_holding_rcount,
        (long long)local_conf.time.message.gtp.t3_holding_duration);
    ogs_trace("%lld, %lld, %lld",
        (long long)local_conf.time.message.sbi.reconnect_interval,
        (long long)local_conf.time.message.pfcp.association_interval,
        (long long)local_conf.time.message.pfcp.no_heartbeat_duration);
#endif
}

static int local_conf_prepare(void)
{
    /* <Heartbeat Checking Interval>
     *  Heartbeat Interval(e.g: 10 seconds) + No Heartbeat Margin(1 second) */
    local_conf.time.nf_instance.no_heartbeat_margin = 1;

    /* 3600 seconds = 1 hour */
    local_conf.time.nf_instance.validity_duration = 3600;

    /* 86400 seconds = 1 day */
    local_conf.time.subscription.validity_duration = 86400;

    /*
     * Message Wait Duration : 10 seconds (Default)
     *
     * The paging retry timer is 2 seconds and the retry count is 3.
     *
     * It is recomended to set at least 9 seconds to reflect
     * the paging failure result to GTPv2-C or HTTP2(SBI).
     */
    local_conf.time.message.duration = ogs_time_from_sec(10);

    /*
     * Handover Wait Duration : 300 ms (Default)
     *
     * Time to wait for AMF/MME to send UEContextReleaseCommand
     * to the source gNB/eNB after receiving HandoverNotify
     */
    local_conf.time.handover.duration = ogs_time_from_msec(300);

    /* Size of internal metrics pool (amount of ogs_metrics_spec_t) */
    ogs_app()->metrics.max_specs = 512;

    regenerate_all_timer_duration();

    return OGS_OK;
}

static int local_conf_validation(void)
{
    if (local_conf.time.nf_instance.validity_duration == 0) {
        ogs_error("NF Instance validity-time should not 0");
        ogs_error("time:");
        ogs_error("  nf_instance:");
        ogs_error("    validity: 0");

        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_app_parse_local_conf(const char *local)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = local_conf_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, local)) {
            ogs_yaml_iter_t local_iter;
            ogs_yaml_iter_recurse(&root_iter, &local_iter);
            while (ogs_yaml_iter_next(&local_iter)) {
                const char *local_key = ogs_yaml_iter_key(&local_iter);
                ogs_assert(local_key);
                if (!strcmp(local_key, "serving")) {
                    ogs_yaml_iter_t serving_array, serving_iter;
                    ogs_yaml_iter_recurse(&local_iter, &serving_array);
                    do {
                        const char *mnc = NULL, *mcc = NULL;
                        ogs_assert(local_conf.num_of_serving_plmn_id <
                                OGS_MAX_NUM_OF_PLMN);

                        if (ogs_yaml_iter_type(&serving_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&serving_iter, &serving_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&serving_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&serving_array))
                                break;
                            ogs_yaml_iter_recurse(
                                    &serving_array, &serving_iter);
                        } else if (ogs_yaml_iter_type(&serving_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&serving_iter)) {
                            const char *serving_key =
                                ogs_yaml_iter_key(&serving_iter);
                            ogs_assert(serving_key);
                            if (!strcmp(serving_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&serving_iter,
                                        &plmn_id_iter);
                                while (ogs_yaml_iter_next(&plmn_id_iter)) {
                                    const char *id_key =
                                        ogs_yaml_iter_key(&plmn_id_iter);
                                    ogs_assert(id_key);
                                    if (!strcmp(id_key, "mcc")) {
                                        mcc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    } else if (!strcmp(id_key, "mnc")) {
                                        mnc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    }
                                }

                                if (mcc && mnc) {
                                    ogs_plmn_id_build(
                                            &local_conf.serving_plmn_id[
                                                local_conf.
                                                num_of_serving_plmn_id],
                                            atoi(mcc), atoi(mnc), strlen(mnc));
                                    local_conf.num_of_serving_plmn_id++;
                                } else {
                                    ogs_error("Invalid [MCC:%s, MNC:%s]",
                                            mcc, mnc);
                                }
                            } else
                                ogs_warn("unknown key `%s`", serving_key);
                        }
                    } while (ogs_yaml_iter_type(&serving_array) ==
                            YAML_SEQUENCE_NODE);
                } else if (!strcmp(local_key, "time")) {
                    ogs_yaml_iter_t time_iter;
                    ogs_yaml_iter_recurse(&local_iter, &time_iter);
                    while (ogs_yaml_iter_next(&time_iter)) {
                        const char *time_key = ogs_yaml_iter_key(&time_iter);
                        ogs_assert(time_key);
                        if (!strcmp(time_key, "nf_instance")) {
                            ogs_yaml_iter_t sbi_iter;
                            ogs_yaml_iter_recurse(&time_iter, &sbi_iter);

                            while (ogs_yaml_iter_next(&sbi_iter)) {
                                const char *sbi_key =
                                    ogs_yaml_iter_key(&sbi_iter);
                                ogs_assert(sbi_key);

                                if (!strcmp(sbi_key, "heartbeat")) {
                                    const char *v = ogs_yaml_iter_value(
                                            &sbi_iter);
                                    if (v)
                                        local_conf.time.nf_instance.
                                            heartbeat_interval = atoi(v);
                                } else if (!strcmp(sbi_key, "validity")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&sbi_iter);
                                    if (v)
                                        local_conf.time.nf_instance.
                                            validity_duration = atoi(v);
                                } else
                                    ogs_warn("unknown key `%s`", sbi_key);
                            }
                        } else if (!strcmp(time_key, "subscription")) {
                            ogs_yaml_iter_t sbi_iter;
                            ogs_yaml_iter_recurse(&time_iter, &sbi_iter);

                            while (ogs_yaml_iter_next(&sbi_iter)) {
                                const char *sbi_key =
                                    ogs_yaml_iter_key(&sbi_iter);
                                ogs_assert(sbi_key);

                                if (!strcmp(sbi_key, "validity")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&sbi_iter);
                                    if (v)
                                        local_conf.time.subscription.
                                            validity_duration = atoi(v);
                                } else
                                    ogs_warn("unknown key `%s`", sbi_key);
                            }
                        } else if (!strcmp(time_key, "message")) {
                            ogs_yaml_iter_t msg_iter;
                            ogs_yaml_iter_recurse(&time_iter, &msg_iter);

                            while (ogs_yaml_iter_next(&msg_iter)) {
                                const char *msg_key =
                                    ogs_yaml_iter_key(&msg_iter);
                                ogs_assert(msg_key);

                                if (!strcmp(msg_key, "duration")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&msg_iter);
                                    if (v) {
                                        local_conf.time.message.duration =
                                            ogs_time_from_msec(atoll(v));
                                        regenerate_all_timer_duration();
                                    }
                                } else
                                    ogs_warn("unknown key `%s`", msg_key);
                            }
                        } else if (!strcmp(time_key, "handover")) {
                            ogs_yaml_iter_t msg_iter;
                            ogs_yaml_iter_recurse(&time_iter, &msg_iter);

                            while (ogs_yaml_iter_next(&msg_iter)) {
                                const char *msg_key =
                                    ogs_yaml_iter_key(&msg_iter);
                                ogs_assert(msg_key);

                                if (!strcmp(msg_key, "duration")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&msg_iter);
                                    if (v) {
                                        local_conf.time.handover.duration =
                                            ogs_time_from_msec(atoll(v));
                                    }
                                } else
                                    ogs_warn("unknown key `%s`", msg_key);
                            }
                        } else if (!strcmp(time_key, "t3502")) {
                            /* handle config in amf */
                        } else if (!strcmp(time_key, "t3512")) {
                            /* handle config in amf */
                        } else if (!strcmp(time_key, "t3402")) {
                            /* handle config in mme */
                        } else if (!strcmp(time_key, "t3412")) {
                            /* handle config in mme */
                        } else if (!strcmp(time_key, "t3423")) {
                            /* handle config in mme */
                        } else
                            ogs_warn("unknown key `%s`", time_key);
                    }
                } else if (!strcmp(local_key, OGS_POLICY_STRING)) {
                    rv = ogs_app_parse_policy_conf(&local_iter);
                    if (rv != OGS_OK) {
                        ogs_error("ogs_policy_conf_parse_config() failed");
                        return rv;
                    }
                }
            }
        }
    }

    rv = local_conf_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

int ogs_app_parse_sockopt_config(
        ogs_yaml_iter_t *parent, ogs_sockopt_t *option)
{
    ogs_yaml_iter_t sockopt_iter;

    ogs_assert(parent);
    ogs_assert(option);

    ogs_sockopt_init(option);

    ogs_yaml_iter_recurse(parent, &sockopt_iter);
    while (ogs_yaml_iter_next(&sockopt_iter)) {
        const char *sockopt_key = ogs_yaml_iter_key(&sockopt_iter);
        ogs_assert(sockopt_key);

        if (!strcmp(sockopt_key, "sctp")) {
            ogs_yaml_iter_t sctp_iter;
            ogs_yaml_iter_recurse(&sockopt_iter, &sctp_iter);

            while (ogs_yaml_iter_next(&sctp_iter)) {
                const char *sctp_key = ogs_yaml_iter_key(&sctp_iter);
                ogs_assert(sctp_key);
                if (!strcmp(sctp_key, "spp_hbinterval")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.spp_hbinterval = atoi(v);
                } else if (!strcmp(sctp_key, "spp_sackdelay")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.spp_sackdelay = atoi(v);
                } else if (!strcmp(sctp_key, "srto_initial")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.srto_initial = atoi(v);
                } else if (!strcmp(sctp_key, "srto_min")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.srto_min = atoi(v);
                } else if (!strcmp(sctp_key, "srto_max")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.srto_max = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_num_ostreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_num_ostreams = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_max_instreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_max_instreams = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_max_attempts")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_max_attempts = atoi(v);
                } else if (!strcmp(sctp_key, "sinit_max_init_timeo")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) option->sctp.sinit_max_init_timeo = atoi(v);
                } else {
                    ogs_error("unknown key `%s`", sctp_key);
                    return OGS_ERROR;
                }
            }
        } else if (!strcmp(sockopt_key, "sctp_nodelay")) {
            option->sctp_nodelay = ogs_yaml_iter_bool(&sockopt_iter);
        } else if (!strcmp(sockopt_key, "tcp_nodelay")) {
            option->tcp_nodelay = ogs_yaml_iter_bool(&sockopt_iter);

        } else if (!strcmp(sockopt_key, "so_linger")) {
            ogs_yaml_iter_t so_linger_iter;
            ogs_yaml_iter_recurse(&sockopt_iter, &so_linger_iter);

            while (ogs_yaml_iter_next(&so_linger_iter)) {
                const char *so_linger_key = ogs_yaml_iter_key(&so_linger_iter);
                ogs_assert(so_linger_key);
                if (!strcmp(so_linger_key, "l_onoff")) {
                    option->so_linger.l_onoff =
                        ogs_yaml_iter_bool(&so_linger_iter);
                } else if (!strcmp(so_linger_key, "l_linger")) {
                    const char *v = ogs_yaml_iter_value(&so_linger_iter);
                    if (v) option->so_linger.l_linger = atoi(v);
                } else {
                    ogs_error("unknown key `%s`", so_linger_key);
                    return OGS_ERROR;
                }
            }

        } else if (!strcmp(sockopt_key, "so_bindtodevice")) {
            option->so_bindtodevice = ogs_yaml_iter_value(&sockopt_iter);

        } else {
            ogs_error("unknown key `%s`", sockopt_key);
            return OGS_ERROR;
        }
    }

    return OGS_OK;
}

static int policy_conf_prepare(void)
{
    return OGS_OK;
}

static int policy_conf_validation(void)
{
    ogs_app_policy_conf_t *policy_conf = NULL;
    bool default_indicator = false;

    ogs_list_for_each(&local_conf.policy_list, policy_conf) {
        if (policy_conf->data.default_indicator == true)
            default_indicator = true;

        if (ogs_list_count(&policy_conf->sess_list) == 0) {
            ogs_error("At least 1 Session is required");
            return OGS_ERROR;
        }
    }

    if (default_indicator == false) {
        ogs_error("At least 1 Default S-NSSAI is required");
        return OGS_ERROR;
    }

    if (ogs_list_count(&local_conf.policy_list) == 0 &&
        ogs_app()->db_uri == NULL) {
        ogs_error("Both db_uri and policy configuration are not set");
        return OGS_ERROR;
    }

    ogs_list_for_each(&local_conf.policy_list, policy_conf) {
        ogs_app_sess_conf_t *sess_conf = NULL;
        int j, k;
        ogs_list_for_each(&policy_conf->sess_list, sess_conf) {
            ogs_info("NAME[%s]", sess_conf->data.session.name);
            ogs_info("QCI[%d]", sess_conf->data.session.qos.index);
            ogs_info("ARP[%d:%d:%d]",
                    sess_conf->data.session.qos.arp.priority_level,
                    sess_conf->data.session.qos.arp.pre_emption_capability,
                    sess_conf->data.session.qos.arp.pre_emption_vulnerability);
            ogs_info("AMBR[Downlink:%lld:Uplink:%lld]",
                    (long long)sess_conf->data.session.ambr.downlink,
                    (long long)sess_conf->data.session.ambr.uplink);
            for (j = 0; j < sess_conf->data.num_of_pcc_rule; j++) {
                ogs_info("PCC_RULE[%d]", j+1);
                ogs_info("  QCI[%d]", sess_conf->data.pcc_rule[j].qos.index);
                ogs_info("  ARP[%d:%d:%d]",
                        sess_conf->data.pcc_rule[j].qos.arp.priority_level,
                        sess_conf->data.pcc_rule[j].qos.arp.
                        pre_emption_capability,
                        sess_conf->data.pcc_rule[j].qos.arp.
                        pre_emption_vulnerability);
                ogs_info("  MBR[Downlink:%lld:Uplink:%lld]",
                        (long long)sess_conf->data.pcc_rule[j].qos.mbr.downlink,
                        (long long)sess_conf->data.pcc_rule[j].qos.mbr.uplink);
                ogs_info("  GBR[Downlink:%lld:Uplink:%lld]",
                        (long long)sess_conf->data.pcc_rule[j].qos.gbr.downlink,
                        (long long)sess_conf->data.pcc_rule[j].qos.gbr.uplink);
                ogs_info("  NUM_OF_FLOW [%d]",
                    sess_conf->data.pcc_rule[j].num_of_flow);

                for (k = 0; k < sess_conf->data.pcc_rule[j].num_of_flow; k++) {
                    ogs_info("    DIRECTION[%d]",
                            sess_conf->data.pcc_rule[j].flow[k].direction);
                    ogs_info("    DESCRIPTION[%s]",
                            sess_conf->data.pcc_rule[j].flow[k].description);
                }
            }
        }
    }

    return OGS_OK;
}

static int parse_br_conf(ogs_yaml_iter_t *parent, ogs_bitrate_t *br)
{
    ogs_yaml_iter_t br_iter;
    ogs_yaml_iter_recurse(parent, &br_iter);

    while (ogs_yaml_iter_next(&br_iter)) {
        const char *br_key = ogs_yaml_iter_key(&br_iter);
        ogs_assert(br_key);
        if (!strcmp(br_key, OGS_DOWNLINK_STRING)) {
            uint8_t unit = 0;
            int n;

            ogs_yaml_iter_t downlink_iter;
            ogs_yaml_iter_recurse(&br_iter, &downlink_iter);

            while (ogs_yaml_iter_next(&downlink_iter)) {
                const char *downlink_key =
                    ogs_yaml_iter_key(&downlink_iter);
                ogs_assert(downlink_key);
                if (!strcmp(downlink_key, OGS_VALUE_STRING)) {
                    const char *v = ogs_yaml_iter_value(&downlink_iter);
                    if (v) br->downlink = atoi(v);
                } else if (!strcmp(downlink_key, OGS_UNIT_STRING)) {
                    const char *v = ogs_yaml_iter_value(&downlink_iter);
                    if (v) {
                        unit = atoi(v);
                        if (unit == 0 || unit == 1 || unit == 2 ||
                            unit == 3 || unit == 4) {
                        } else {
                            ogs_error("Unknown Unit [%d]", unit);
                            return OGS_ERROR;
                        }
                    }
                } else
                    ogs_warn("unknown key `%s`", downlink_key);
            }

            for (n = 0; n < unit; n++)
                br->downlink *= 1000;
        } else if (!strcmp(br_key, OGS_UPLINK_STRING)) {
            uint8_t unit = 0;
            int n;

            ogs_yaml_iter_t uplink_iter;
            ogs_yaml_iter_recurse(&br_iter, &uplink_iter);

            while (ogs_yaml_iter_next(&uplink_iter)) {
                const char *uplink_key =
                    ogs_yaml_iter_key(&uplink_iter);
                ogs_assert(uplink_key);
                if (!strcmp(uplink_key, OGS_VALUE_STRING)) {
                    const char *v = ogs_yaml_iter_value(&uplink_iter);
                    if (v) br->uplink = atoi(v);
                } else if (!strcmp(uplink_key, OGS_UNIT_STRING)) {
                    const char *v = ogs_yaml_iter_value(&uplink_iter);
                    if (v) {
                        unit = atoi(v);
                        if (unit == 0 || unit == 1 || unit == 2 ||
                            unit == 3 || unit == 4) {
                        } else {
                            ogs_error("Unknown Unit [%d]", unit);
                            return OGS_ERROR;
                        }
                    }
                } else
                    ogs_warn("unknown key `%s`", uplink_key);
            }

            for (n = 0; n < unit; n++)
                br->uplink *= 1000;
        } else
            ogs_warn("unknown key `%s`", br_key);
    }

    return OGS_OK;
}

static int parse_qos_conf(ogs_yaml_iter_t *parent, ogs_qos_t *qos)
{
    int rv;
    ogs_yaml_iter_t qos_iter;

    ogs_assert(parent);
    ogs_assert(qos);

    ogs_yaml_iter_recurse(parent, &qos_iter);
    while (ogs_yaml_iter_next(&qos_iter)) {
        const char *qos_key = ogs_yaml_iter_key(&qos_iter);
        ogs_assert(qos_key);
        if (!strcmp(qos_key, OGS_INDEX_STRING)) {
            const char *v = ogs_yaml_iter_value(&qos_iter);
            if (v) {
                uint8_t index = atoi(v);
                if (index == 1 || index == 2 || index == 3 || index == 4 ||
                    index == 65 || index == 66 || index == 67 || index == 75 ||
                    index == 71 || index == 72 || index == 73 || index == 74 ||
                    index == 76 || index == 5 || index == 6 || index == 7 ||
                    index == 8 || index == 9 || index == 69 || index == 70 ||
                    index == 79 || index == 80 || index == 82 || index == 83 ||
                    index == 84 || index == 85 || index == 86)
                    qos->index = index;
                else {
                    ogs_error("Unknown QCI [%d]", index);
                    return OGS_ERROR;
                }
            }
        } else if (!strcmp(qos_key, OGS_ARP_STRING)) {
            ogs_yaml_iter_t arp_iter;
            ogs_yaml_iter_recurse(&qos_iter, &arp_iter);
            while (ogs_yaml_iter_next(&arp_iter)) {
                const char *arp_key = ogs_yaml_iter_key(&arp_iter);
                ogs_assert(arp_key);
                if (!strcmp(arp_key, OGS_PRIORITY_LEVEL_STRING)) {
                    const char *v = ogs_yaml_iter_value(&arp_iter);
                    if (v) {
                        uint8_t priority_level = atoi(v);
                        if (priority_level >= 1 && priority_level <= 15)
                            qos->arp.priority_level = priority_level;
                        else {
                            ogs_error("Unknown Priority Level [%d]",
                                    priority_level);
                            return OGS_ERROR;
                        }
                    }
                } else if (!strcmp(arp_key, OGS_PRE_EMPTION_CAPABILITY_STRING)) {
                    const char *v = ogs_yaml_iter_value(&arp_iter);
                    if (v) {
                        uint8_t pre_emption_capability = atoi(v);
                        if (pre_emption_capability ==
                                OGS_5GC_PRE_EMPTION_DISABLED ||
                            pre_emption_capability ==
                            OGS_5GC_PRE_EMPTION_ENABLED)
                            qos->arp.pre_emption_capability =
                                pre_emption_capability;
                        else {
                            ogs_error("Unknown Preemption Capability [%d]",
                                    pre_emption_capability);
                            return OGS_ERROR;
                        }
                    }
                } else if (!strcmp(arp_key,
                            OGS_PRE_EMPTION_VULNERABILITY_STRING)) {
                    const char *v = ogs_yaml_iter_value(&arp_iter);
                    if (v) {
                        uint8_t pre_emption_vulnerability = atoi(v);
                        if (pre_emption_vulnerability ==
                                OGS_5GC_PRE_EMPTION_DISABLED ||
                            pre_emption_vulnerability ==
                            OGS_5GC_PRE_EMPTION_ENABLED)
                            qos->arp.pre_emption_vulnerability =
                                pre_emption_vulnerability;
                        else {
                            ogs_error("Unknown Preemption Vulnerablility [%d]",
                                    pre_emption_vulnerability);
                            return OGS_ERROR;
                        }
                    }
                } else
                    ogs_warn("unknown key `%s`", arp_key);
            }
        } else if (!strcmp(qos_key, OGS_MBR_STRING)) {
            rv = parse_br_conf(&qos_iter, &qos->mbr);
            if (rv != OGS_OK) {
                ogs_error("parse_br_conf() failed");
                return rv;
            }
        } else if (!strcmp(qos_key, OGS_GBR_STRING)) {
            rv = parse_br_conf(&qos_iter, &qos->gbr);
            if (rv != OGS_OK) {
                ogs_error("parse_br_conf() failed");
                return rv;
            }
        }
    }

    return OGS_OK;
}

int ogs_app_parse_policy_conf(ogs_yaml_iter_t *parent)
{
    int rv, i, j;
    ogs_yaml_iter_t policy_array, policy_iter;

    ogs_assert(parent);

    rv = policy_conf_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_recurse(parent, &policy_array);
    do {
        uint8_t sst;
        ogs_uint24_t sd;
        bool default_indicator = false;

        ogs_session_data_t session_data_array[OGS_MAX_NUM_OF_SESS];
        int num_of_session_data = 0;

        sst = 0;
        sd.v = OGS_S_NSSAI_NO_SD_VALUE;
        memset(session_data_array, 0, sizeof(session_data_array));

        if (ogs_yaml_iter_type(&policy_array) == YAML_MAPPING_NODE) {
            memcpy(&policy_iter, &policy_array, sizeof(ogs_yaml_iter_t));
        } else if (ogs_yaml_iter_type(&policy_array) == YAML_SEQUENCE_NODE) {
            if (!ogs_yaml_iter_next(&policy_array))
                break;
            ogs_yaml_iter_recurse(&policy_array, &policy_iter);
        } else if (ogs_yaml_iter_type(&policy_array) == YAML_SCALAR_NODE) {
            break;
        } else
            ogs_assert_if_reached();

        while (ogs_yaml_iter_next(&policy_iter)) {
            const char *policy_key = ogs_yaml_iter_key(&policy_iter);
            ogs_assert(policy_key);
            if (!strcmp(policy_key, OGS_SST_STRING)) {
                const char *v = ogs_yaml_iter_value(&policy_iter);
                if (v) {
                    sst = atoi(v);
                    if (sst == 1 || sst == 2 || sst == 3 || sst == 4) {
                    } else {
                        ogs_error("Unknown SST [%d]", sst);
                        return OGS_ERROR;
                    }
                }
            } else if (!strcmp(policy_key, OGS_SD_STRING)) {
                const char *v = ogs_yaml_iter_value(&policy_iter);
                if (v) sd = ogs_s_nssai_sd_from_string(v);
            } else if (!strcmp(policy_key, OGS_DEFAULT_INDICATOR_STRING)) {
                default_indicator = ogs_yaml_iter_bool(&policy_iter);
            } else if (!strcmp(policy_key, OGS_SESSION_STRING)) {
                ogs_yaml_iter_t session_array, session_iter;
                ogs_yaml_iter_recurse(&policy_iter, &session_array);
                do {
                    ogs_session_data_t *session_data =
                        &session_data_array[num_of_session_data];

                    if (ogs_yaml_iter_type(&session_array) ==
                            YAML_MAPPING_NODE) {
                        memcpy(&session_iter, &session_array,
                                sizeof(ogs_yaml_iter_t));
                    } else if (ogs_yaml_iter_type(&session_array) ==
                            YAML_SEQUENCE_NODE) {
                        if (!ogs_yaml_iter_next(&session_array))
                            break;
                        ogs_yaml_iter_recurse(&session_array, &session_iter);
                    } else if (ogs_yaml_iter_type(&session_array) ==
                            YAML_SCALAR_NODE) {
                        break;
                    } else
                        ogs_assert_if_reached();

                    while (ogs_yaml_iter_next(&session_iter)) {
                        const char *session_key =
                            ogs_yaml_iter_key(&session_iter);
                        ogs_assert(session_key);
                        if (!strcmp(session_key, OGS_NAME_STRING)) {
                            session_data->session.name =
                                (char *)ogs_yaml_iter_value(&session_iter);
                        } else if (!strcmp(session_key, OGS_TYPE_STRING)) {
                            const char *v = ogs_yaml_iter_value(&session_iter);
                            if (v) {
                                uint8_t session_type = atoi(v);
                                if (session_type == OGS_PDU_SESSION_TYPE_IPV4 ||
                                    session_type == OGS_PDU_SESSION_TYPE_IPV6 ||
                                    session_type ==
                                    OGS_PDU_SESSION_TYPE_IPV4V6 ||
                                    session_type ==
                                    OGS_PDU_SESSION_TYPE_UNSTRUCTURED ||
                                    session_type ==
                                    OGS_PDU_SESSION_TYPE_ETHERNET)
                                    session_data->session.session_type =
                                        session_type;
                                else {
                                    ogs_error("Unknown Session Type [%d]",
                                            session_type);
                                    return OGS_ERROR;
                                }
                            }
                        } else if (!strcmp(session_key, OGS_AMBR_STRING)) {
                            rv = parse_br_conf(&session_iter,
                                    &session_data->session.ambr);
                            if (rv != OGS_OK) {
                                ogs_error("parse_qos_conf() failed");
                                return rv;
                            }
                        } else if (!strcmp(session_key, OGS_QOS_STRING)) {
                            rv = parse_qos_conf(&session_iter,
                                    &session_data->session.qos);
                            if (rv != OGS_OK) {
                                ogs_error("parse_qos_conf() failed");
                                return rv;
                            }
                        } else if (!strcmp(session_key, OGS_PCC_RULE_STRING)) {
                            ogs_yaml_iter_t pcc_rule_array, pcc_rule_iter;
                            ogs_yaml_iter_recurse(&session_iter,
                                    &pcc_rule_array);
                            do {
                                ogs_pcc_rule_t *pcc_rule = NULL;

                                ogs_assert(session_data->num_of_pcc_rule <
                                        OGS_MAX_NUM_OF_PCC_RULE);
                                pcc_rule = &session_data->
                                    pcc_rule[session_data->num_of_pcc_rule];

                                if (ogs_yaml_iter_type(&pcc_rule_array) ==
                                        YAML_MAPPING_NODE) {
                                    memcpy(&pcc_rule_iter, &pcc_rule_array,
                                            sizeof(ogs_yaml_iter_t));
                                } else if (ogs_yaml_iter_type(
                                            &pcc_rule_array) ==
                                        YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(&pcc_rule_array))
                                        break;
                                    ogs_yaml_iter_recurse(
                                            &pcc_rule_array, &pcc_rule_iter);
                                } else if (ogs_yaml_iter_type(
                                            &pcc_rule_array) ==
                                        YAML_SCALAR_NODE) {
                                    break;
                                } else
                                    ogs_assert_if_reached();

                                while (ogs_yaml_iter_next(&pcc_rule_iter)) {
                                    const char *pcc_rule_key =
                                        ogs_yaml_iter_key(&pcc_rule_iter);
                                    ogs_assert(pcc_rule_key);
                                    if (!strcmp(pcc_rule_key,
                                                OGS_QOS_STRING)) {
                                        rv = parse_qos_conf(&pcc_rule_iter,
                                                &pcc_rule->qos);
                                        if (rv != OGS_OK) {
                                            ogs_error(
                                                    "parse_qos_conf() failed");
                                            return rv;
                                        }
                                    } else if (!strcmp(pcc_rule_key,
                                                OGS_FLOW_STRING)) {
                                        ogs_yaml_iter_t flow_array, flow_iter;
                                        ogs_yaml_iter_recurse(
                                                &pcc_rule_iter, &flow_array);
                                        do {
                                            ogs_flow_t *flow = NULL;

                                            ogs_assert(pcc_rule->num_of_flow <
                                            OGS_MAX_NUM_OF_FLOW_IN_PCC_RULE);
                                            flow = &pcc_rule->flow
                                                [pcc_rule->num_of_flow];

                                            if (ogs_yaml_iter_type(
                                                        &flow_array) ==
                                                    YAML_MAPPING_NODE) {
                                                memcpy(&flow_iter, &flow_array,
                                                    sizeof(ogs_yaml_iter_t));
                                            } else if (ogs_yaml_iter_type(
                                                        &flow_array) ==
                                                    YAML_SEQUENCE_NODE) {
                                                if (!ogs_yaml_iter_next(
                                                            &flow_array))
                                                    break;
                                                ogs_yaml_iter_recurse(
                                                    &flow_array, &flow_iter);
                                            } else if (ogs_yaml_iter_type(
                                                        &flow_array) ==
                                                    YAML_SCALAR_NODE) {
                                                break;
                                            } else
                                                ogs_assert_if_reached();

                                            while (ogs_yaml_iter_next(
                                                        &flow_iter)) {
                                                const char *flow_key =
                                                    ogs_yaml_iter_key(
                                                            &flow_iter);
                                                ogs_assert(flow_key);
                                                if (!strcmp(flow_key,
                                                    OGS_DIRECTION_STRING)) {
                                                    const char *v =
                                                        ogs_yaml_iter_value(
                                                                &flow_iter);
                                                    if (v) {
                                                        uint8_t direction =
                                                            atoi(v);
                                                        if (direction == OGS_FLOW_DOWNLINK_ONLY ||
                                                            direction == OGS_FLOW_UPLINK_ONLY)
                                                            flow->direction = direction;
                                                        else {
                                                            ogs_error("Unknown "
                                                            "Direction [%d]",
                                                            direction);
                                                            return OGS_ERROR;
                                                        }
                                                    }
                                                } else if (!strcmp(flow_key,
                                                    OGS_DESCRIPTION_STRING)) {
                                                    flow->description =
                                                        (char *)
                                                        ogs_yaml_iter_value(
                                                                &flow_iter);
                                                }
                                            }

                                            if (flow->direction &&
                                                flow->description)
                                                pcc_rule->num_of_flow++;

                                        } while (ogs_yaml_iter_type(
                                                    &flow_array) ==
                                                YAML_SEQUENCE_NODE);

                                    } else
                                        ogs_warn("unknown key `%s`",
                                                pcc_rule_key);
                                }

                                if (pcc_rule->qos.index &&
                                    pcc_rule->qos.arp.priority_level &&
                                    pcc_rule->qos.arp.pre_emption_capability &&
                                    pcc_rule->qos.arp.
                                    pre_emption_vulnerability)
                                    session_data->num_of_pcc_rule++;
                                else
                                    ogs_warn("Mandatory is MISSING - "
                                            "QCI[%d], ARP[%d:%d:%d]",
                                        pcc_rule->qos.index,
                                        pcc_rule->qos.arp.priority_level,
                                        pcc_rule->qos.arp.
                                        pre_emption_capability,
                                        pcc_rule->qos.arp.
                                        pre_emption_vulnerability);

                            } while (ogs_yaml_iter_type(&pcc_rule_array) ==
                                    YAML_SEQUENCE_NODE);

                        } else
                            ogs_warn("unknown key `%s`", session_key);
                    }

                    if (session_data->session.name &&
                            session_data->session.session_type)
                        num_of_session_data++;
                    else
                        ogs_warn("Mandatory is MISSING - "
                                "APN/DNN:[%s], SessionType:[%d]",
                                session_data->session.name ?
                                session_data->session.name : "NULL",
                                session_data->session.session_type);

                } while (ogs_yaml_iter_type(&session_array) ==
                        YAML_SEQUENCE_NODE);
            } else
                ogs_warn("unknown key `%s`", policy_key);
        }

        if (sst) {
            ogs_app_policy_conf_t *policy_conf =
                ogs_app_policy_conf_add(sst, sd);
            if (!policy_conf) {
                ogs_error("ogs_app_policy_conf_add() failed");
                return OGS_ERROR;
            }
            policy_conf->data.default_indicator = default_indicator;

            for (i = 0; i < num_of_session_data; i++) {
                ogs_session_data_t *session_data = NULL;
                ogs_app_sess_conf_t *sess_conf = NULL;

                session_data = &session_data_array[i];
                ogs_assert(session_data->session.name);
                ogs_assert(session_data->session.session_type);

                rv = ogs_check_br_conf(&session_data->session.ambr);
                if (rv != OGS_OK) {
                    ogs_error("check_br_conf(AMBR) failed");
                    return OGS_ERROR;
                }
                rv = ogs_check_qos_conf(&session_data->session.qos);
                if (rv != OGS_OK) {
                    ogs_error("check_qos_conf(SESS) failed");
                    return OGS_ERROR;
                }

                sess_conf = ogs_app_sess_conf_add(
                        policy_conf, session_data->session.name);
                if (!sess_conf) {
                    ogs_error("ogs_app_sess_conf_add() failed");
                    return OGS_ERROR;
                }

                sess_conf->data.session.session_type =
                    session_data->session.session_type;

                memcpy(&sess_conf->data.session.ambr,
                        &session_data->session.ambr,
                        sizeof(sess_conf->data.session.ambr));
                memcpy(&sess_conf->data.session.qos,
                        &session_data->session.qos,
                        sizeof(sess_conf->data.session.qos));

                sess_conf->data.num_of_pcc_rule =
                    session_data->num_of_pcc_rule;
                for (j = 0; j < sess_conf->data.num_of_pcc_rule; j++) {
                    rv = ogs_check_qos_conf(&session_data->pcc_rule[j].qos);
                    if (rv != OGS_OK) {
                        ogs_error("ogs_check_qos_conf() failed");
                        ogs_app_sess_conf_remove(sess_conf);
                        return OGS_ERROR;
                    }

                    OGS_STORE_PCC_RULE(&sess_conf->data.pcc_rule[j],
                            &session_data->pcc_rule[j]);
                }
            }
        } else {
            ogs_error("No SST");
            return OGS_ERROR;
        }

    } while (ogs_yaml_iter_type(&policy_array) == YAML_SEQUENCE_NODE);

    rv = policy_conf_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

ogs_app_policy_conf_t *ogs_app_policy_conf_add(uint8_t sst, ogs_uint24_t sd)
{
    ogs_app_policy_conf_t *policy_conf = NULL;

    ogs_assert(sst);

    ogs_pool_alloc(&policy_conf_pool, &policy_conf);
    if (!policy_conf) {
        ogs_error("Maximum number of policy_conf[%d] reached",
                OGS_MAX_NUM_OF_SLICE);
        return NULL;
    }
    memset(policy_conf, 0, sizeof *policy_conf);

    policy_conf->data.s_nssai.sst = sst;
    policy_conf->data.s_nssai.sd.v = sd.v;

    ogs_list_init(&policy_conf->sess_list);

    ogs_list_add(&local_conf.policy_list, policy_conf);

    ogs_info("POLICY config added [%d]",
            ogs_list_count(&local_conf.policy_list));
    return policy_conf;
}
ogs_app_policy_conf_t *ogs_app_policy_conf_find_by_s_nssai(
        uint8_t sst, ogs_uint24_t sd)
{
    ogs_app_policy_conf_t *policy_conf = NULL;

    ogs_assert(sst);

    ogs_list_for_each(&local_conf.policy_list, policy_conf) {
        if (policy_conf->data.s_nssai.sst == sst &&
                policy_conf->data.s_nssai.sd.v == sd.v)
            break;
    }

    return policy_conf;
}
void ogs_app_policy_conf_remove(ogs_app_policy_conf_t *policy_conf)
{
    ogs_assert(policy_conf);

    ogs_list_remove(&local_conf.policy_list, policy_conf);

    ogs_app_sess_conf_remove_all(policy_conf);

    ogs_pool_free(&policy_conf_pool, policy_conf);

    ogs_info("POLICY config removed [%d]",
            ogs_list_count(&local_conf.policy_list));
}
void ogs_app_policy_conf_remove_all(void)
{
    ogs_app_policy_conf_t *policy_conf = NULL, *next_conf = NULL;;

    ogs_list_for_each_safe(&local_conf.policy_list, next_conf, policy_conf)
        ogs_app_policy_conf_remove(policy_conf);
}

ogs_app_sess_conf_t *ogs_app_sess_conf_add(
        ogs_app_policy_conf_t *policy_conf, char *name)
{
    ogs_app_sess_conf_t *sess_conf = NULL;

    ogs_assert(policy_conf);
    ogs_assert(name);

    ogs_pool_alloc(&sess_conf_pool, &sess_conf);
    if (!sess_conf) {
        ogs_error("Maximum number of sess_conf[%d] reached",
                OGS_MAX_NUM_OF_SLICE*OGS_MAX_NUM_OF_SESS);
        return NULL;
    }
    memset(sess_conf, 0, sizeof *sess_conf);

    sess_conf->data.session.name = ogs_strdup(name);
    if (!sess_conf->data.session.name) {
        ogs_error("Not enough memroy for %s", name);
        ogs_pool_free(&sess_conf_pool, sess_conf);
        return NULL;
    }

    ogs_list_add(&policy_conf->sess_list, sess_conf);

    sess_conf->policy_conf = policy_conf;

    ogs_info("SESSION config added [%d]",
            ogs_list_count(&policy_conf->sess_list));
    return sess_conf;
}
ogs_app_sess_conf_t *ogs_app_sess_conf_find_by_dnn(
        ogs_app_policy_conf_t *policy_conf, char *name)
{
    ogs_app_sess_conf_t *sess_conf = NULL;

    ogs_assert(policy_conf);
    ogs_assert(name);

    ogs_list_for_each(&policy_conf->sess_list, sess_conf) {
        ogs_assert(sess_conf->data.session.name);
        if (strcmp(sess_conf->data.session.name, name) == 0)
            break;
    }

    return sess_conf;
}
void ogs_app_sess_conf_remove(ogs_app_sess_conf_t *sess_conf)
{
    ogs_app_policy_conf_t *policy_conf = NULL;

    ogs_assert(sess_conf);
    policy_conf = sess_conf->policy_conf;
    ogs_assert(policy_conf);

    ogs_list_remove(&policy_conf->sess_list, sess_conf);

    ogs_session_data_free(&sess_conf->data);

    ogs_pool_free(&sess_conf_pool, sess_conf);

    ogs_info("SESSION config removed [%d]",
            ogs_list_count(&policy_conf->sess_list));
}
void ogs_app_sess_conf_remove_all(
        ogs_app_policy_conf_t *policy_conf)
{
    ogs_app_sess_conf_t *sess_conf = NULL, *next_conf = NULL;;

    ogs_list_for_each_safe(&policy_conf->sess_list, next_conf, sess_conf)
        ogs_app_sess_conf_remove(sess_conf);
}
