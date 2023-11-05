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

static int initialized = 0;

int ogs_app_config_init(void)
{
    ogs_assert(initialized == 0);

    memset(&global_conf, 0, sizeof(ogs_app_global_conf_t));
    memset(&local_conf, 0, sizeof(ogs_app_local_conf_t));

    initialized = 1;

    return OGS_OK;
}

void ogs_app_config_final(void)
{
    ogs_assert(initialized == 1);

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
