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

static ogs_app_context_t self;

static int initialized = 0;

static int app_context_prepare(void);

int ogs_app_context_init(void)
{
    ogs_assert(initialized == 0);

    memset(&self, 0, sizeof(ogs_app_context_t));

    initialized = 1;

    return OGS_OK;
}

void ogs_app_context_final(void)
{
    ogs_assert(initialized == 1);

    if (self.document) {
        yaml_document_delete(self.document);
        free(self.document);
    }

    if (self.pollset)
        ogs_pollset_destroy(self.pollset);
    if (self.timer_mgr)
        ogs_timer_mgr_destroy(self.timer_mgr);
    if (self.queue)
        ogs_queue_destroy(self.queue);

    initialized = 0;
}

ogs_app_context_t *ogs_app(void)
{
    return &self;
}

static void regenerate_all_timer_duration(void)
{
    ogs_assert(self.time.message.duration);

    self.time.message.sbi.client_wait_duration = self.time.message.duration;
    self.time.message.sbi.connection_deadline =
        self.time.message.sbi.client_wait_duration + ogs_time_from_sec(1);
    self.time.message.sbi.reconnect_interval =
        ogs_max(ogs_time_from_sec(3),
            self.time.message.sbi.client_wait_duration + ogs_time_from_sec(1));
    self.time.message.sbi.reconnect_interval_in_exception =
                ogs_time_from_sec(2);

#define PFCP_N1_RESPONSE_RETRY_COUNT  3
    self.time.message.pfcp.n1_response_rcount = PFCP_N1_RESPONSE_RETRY_COUNT;
    self.time.message.pfcp.t1_response_duration =
        (self.time.message.duration /
         (self.time.message.pfcp.n1_response_rcount + 1));
    ogs_assert(self.time.message.pfcp.t1_response_duration);

#define PFCP_N1_HOLDING_RETRY_COUNT 1
    self.time.message.pfcp.n1_holding_rcount = PFCP_N1_HOLDING_RETRY_COUNT;
    self.time.message.pfcp.t1_holding_duration =
        self.time.message.pfcp.n1_response_rcount *
        self.time.message.pfcp.t1_response_duration;
    ogs_assert(self.time.message.pfcp.t1_holding_duration);

    self.time.message.pfcp.association_interval =
        ogs_max(ogs_time_from_sec(3),
            self.time.message.sbi.client_wait_duration + ogs_time_from_sec(1));

    self.time.message.pfcp.no_heartbeat_duration =
        ogs_max(ogs_time_from_sec(10),
            self.time.message.sbi.client_wait_duration + ogs_time_from_sec(1));

#define GTP_N3_RESPONSE_RETRY_COUNT  3
    self.time.message.gtp.n3_response_rcount = GTP_N3_RESPONSE_RETRY_COUNT;
    self.time.message.gtp.t3_response_duration =
        (self.time.message.duration /
         (self.time.message.gtp.n3_response_rcount + 1));
    ogs_assert(self.time.message.gtp.t3_response_duration);

#define GTP_N3_HOLDING_RETRY_COUNT 1
    self.time.message.gtp.n3_holding_rcount = GTP_N3_HOLDING_RETRY_COUNT;
    self.time.message.gtp.t3_holding_duration =
        self.time.message.gtp.n3_response_rcount *
        self.time.message.gtp.t3_response_duration;
    ogs_assert(self.time.message.gtp.t3_holding_duration);

#if 0
    ogs_trace("%lld, %lld, %lld, %d, %lld, %d %lld, %d, %lld, %d, %lld",
        (long long)self.time.message.duration,
        (long long)self.time.message.sbi.client_wait_duration,
        (long long)self.time.message.sbi.connection_deadline,
        self.time.message.pfcp.n1_response_rcount,
        (long long)self.time.message.pfcp.t1_response_duration,
        self.time.message.pfcp.n1_holding_rcount,
        (long long)self.time.message.pfcp.t1_holding_duration,
        self.time.message.gtp.n3_response_rcount,
        (long long)self.time.message.gtp.t3_response_duration,
        self.time.message.gtp.n3_holding_rcount,
        (long long)self.time.message.gtp.t3_holding_duration);
    ogs_trace("%lld, %lld, %lld",
        (long long)self.time.message.sbi.reconnect_interval,
        (long long)self.time.message.pfcp.association_interval,
        (long long)self.time.message.pfcp.no_heartbeat_duration);
#endif
}

static int app_context_prepare(void)
{
    /* <Heartbeat Checking Interval>
     *  Heartbeat Interval(e.g: 10 seconds) + No Heartbeat Margin(1 second) */
    self.time.nf_instance.no_heartbeat_margin = 1;

    /* 3600 seconds = 1 hour */
    self.time.nf_instance.validity_duration = 3600;

    /* 86400 seconds = 1 day */
    self.time.subscription.validity_duration = 86400;

    /*
     * Message Wait Duration : 10 seconds (Default)
     *
     * The paging retry timer is 2 seconds and the retry count is 3.
     *
     * It is recomended to set at least 9 seconds to reflect
     * the paging failure result to GTPv2-C or HTTP2(SBI).
     */
    self.time.message.duration = ogs_time_from_sec(10);

    /*
     * Handover Wait Duration : 300 ms (Default)
     *
     * Time to wait for AMF/MME to send UEContextReleaseCommand
     * to the source gNB/eNB after receiving HandoverNotify
     */
    self.time.handover.duration = ogs_time_from_msec(300);

    /* Size of internal metrics pool (amount of ogs_metrics_spec_t) */
    self.metrics.max_specs = 512;

    regenerate_all_timer_duration();

    return OGS_OK;
}

static int app_context_validation(void)
{
    if (self.parameter.no_ipv4 == 1 &&
        self.parameter.no_ipv6 == 1) {
        ogs_error("Both `no_ipv4` and `no_ipv6` set to `true` in `%s`",
                self.file);
        return OGS_ERROR;
    }

    if (self.time.nf_instance.validity_duration == 0) {
        ogs_error("NF Instance validity-time should not 0");
        ogs_error("time:");
        ogs_error("  nf_instance:");
        ogs_error("    validity: 0");

        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_app_context_parse_config(const char *local)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = self.document;
    ogs_assert(document);

    rv = app_context_prepare();
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
                        ogs_assert(self.num_of_serving_plmn_id <
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
                                            &self.serving_plmn_id[
                                                self.num_of_serving_plmn_id],
                                            atoi(mcc), atoi(mnc), strlen(mnc));
                                    self.num_of_serving_plmn_id++;
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
                                        self.time.nf_instance.
                                            heartbeat_interval = atoi(v);
                                } else if (!strcmp(sbi_key, "validity")) {
                                    const char *v =
                                        ogs_yaml_iter_value(&sbi_iter);
                                    if (v)
                                        self.time.nf_instance.
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
                                        self.time.subscription.
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
                                        self.time.message.duration =
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
                                        self.time.handover.duration =
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

    rv = app_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}
