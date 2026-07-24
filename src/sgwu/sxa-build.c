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

#include "sxa-build.h"

ogs_pkbuf_t *sgwu_sxa_build_session_establishment_response(uint8_t type,
    sgwu_sess_t *sess, ogs_pfcp_pdr_t *created_pdr[], int num_of_created_pdr)
{
    ogs_pfcp_message_t *pfcp_message = NULL;
    ogs_pfcp_session_establishment_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    int i = 0, j = 0, rv;

    ogs_pfcp_node_id_t node_id;
    ogs_pfcp_f_seid_t f_seid;
    int len = 0;

    ogs_debug("Session Establishment Response");

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    rsp = &pfcp_message->pfcp_session_establishment_response;

    /* Node ID */
    rv = ogs_pfcp_sockaddr_to_node_id(&node_id, &len);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_sockaddr_to_node_id() failed");
        ogs_free(pfcp_message);
        return NULL;
    }
    rsp->node_id.presence = 1;
    rsp->node_id.data = &node_id;
    rsp->node_id.len = len;

    /* Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    /* F-SEID */
    rv = ogs_pfcp_sockaddr_to_f_seid(&f_seid, &len);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_sockaddr_to_f_seid() failed");
        ogs_free(pfcp_message);
        return NULL;
    }
    f_seid.seid = htobe64(sess->sgwu_sxa_seid);
    rsp->up_f_seid.presence = 1;
    rsp->up_f_seid.data = &f_seid;
    rsp->up_f_seid.len = len;

    ogs_pfcp_pdrbuf_init();

    /* Created PDR */
    for (i = 0, j = 0; i < num_of_created_pdr; i++) {
        bool pdr_presence = ogs_pfcp_build_created_pdr(
                &rsp->created_pdr[j], i, created_pdr[i]);
        if (pdr_presence == true) j++;
    }

    pfcp_message->h.type = type;
    pkbuf = ogs_pfcp_build_msg(pfcp_message);
    ogs_expect(pkbuf);

    ogs_pfcp_pdrbuf_clear();
    ogs_free(pfcp_message);

    return pkbuf;
}

static struct {
    ogs_pfcp_volume_measurement_t vol_meas;
} usage_report_buf[OGS_ARRAY_SIZE(
        ((ogs_pfcp_session_modification_response_t *)0)->usage_report)];

ogs_pkbuf_t *sgwu_sxa_build_session_modification_response(uint8_t type,
    sgwu_sess_t *sess, ogs_pfcp_pdr_t *created_pdr[], int num_of_created_pdr,
    ogs_pfcp_user_plane_report_t *report)
{
    ogs_pfcp_message_t *pfcp_message = NULL;
    ogs_pfcp_session_modification_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    int i = 0, j = 0;

    ogs_debug("Session Modification Response");

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    rsp = &pfcp_message->pfcp_session_modification_response;

    /* Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    ogs_pfcp_pdrbuf_init();

    /* Created PDR */
    for (i = 0, j = 0; i < num_of_created_pdr; i++) {
        bool pdr_presence = ogs_pfcp_build_created_pdr(
                &rsp->created_pdr[i], i, created_pdr[i]);
        if (pdr_presence == true) j++;
    }

    /* Usage Report */
    if (report && report->type.usage_report) {
        unsigned int r;
        ogs_assert(report->num_of_usage_report <=
                OGS_ARRAY_SIZE(rsp->usage_report));
        for (r = 0; r < report->num_of_usage_report; r++) {
            rsp->usage_report[r].presence = 1;
            rsp->usage_report[r].urr_id.presence = 1;
            rsp->usage_report[r].urr_id.u32 = report->usage_report[r].id;
            rsp->usage_report[r].ur_seqn.presence = 1;
            rsp->usage_report[r].ur_seqn.u32 = report->usage_report[r].seqn;
            rsp->usage_report[r].usage_report_trigger.presence = 1;
            rsp->usage_report[r].usage_report_trigger.u24 =
                (report->usage_report[r].rep_trigger.reptri_5 << 16)
                | (report->usage_report[r].rep_trigger.reptri_6 << 8)
                | (report->usage_report[r].rep_trigger.reptri_7);

            if (report->usage_report[r].start_time) {
                rsp->usage_report[r].start_time.presence = 1;
                rsp->usage_report[r].start_time.u32 =
                    report->usage_report[r].start_time;
            }
            if (report->usage_report[r].end_time) {
                rsp->usage_report[r].end_time.presence = 1;
                rsp->usage_report[r].end_time.u32 =
                    report->usage_report[r].end_time;
            }
            if (report->usage_report[r].vol_measurement.flags) {
                rsp->usage_report[r].volume_measurement.presence = 1;
                ogs_pfcp_build_volume_measurement(
                        &rsp->usage_report[r].volume_measurement,
                        &report->usage_report[r].vol_measurement,
                        &usage_report_buf[r].vol_meas,
                        sizeof(usage_report_buf[r].vol_meas));
            }
            rsp->usage_report[r].duration_measurement.presence = 1;
            rsp->usage_report[r].duration_measurement.u32 =
                report->usage_report[r].dur_measurement;
            if (report->usage_report[r].time_of_first_packet) {
                rsp->usage_report[r].time_of_first_packet.presence = 1;
                rsp->usage_report[r].time_of_first_packet.u32 =
                    report->usage_report[r].time_of_first_packet;
            }
            if (report->usage_report[r].time_of_last_packet) {
                rsp->usage_report[r].time_of_last_packet.presence = 1;
                rsp->usage_report[r].time_of_last_packet.u32 =
                    report->usage_report[r].time_of_last_packet;
            }
        }
    }

    pfcp_message->h.type = type;
    pkbuf = ogs_pfcp_build_msg(pfcp_message);
    ogs_expect(pkbuf);

    ogs_pfcp_pdrbuf_clear();
    ogs_free(pfcp_message);

    return pkbuf;
}

ogs_pkbuf_t *sgwu_sxa_build_session_deletion_response(uint8_t type,
        sgwu_sess_t *sess)
{
    ogs_pfcp_message_t *pfcp_message = NULL;
    ogs_pfcp_session_deletion_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_user_plane_report_t report_data;
    ogs_pfcp_user_plane_report_t *report = &report_data;
    size_t num_of_reports = 0;

    ogs_debug("Session Deletion Response");

    /* final usage reports (termination) for offline SGW-CDR */
    memset(report, 0, sizeof(*report));
    ogs_list_for_each(&sess->pfcp.urr_list, urr) {
        ogs_assert(num_of_reports < OGS_ARRAY_SIZE(report->usage_report));
        sgwu_sess_urr_acc_fill_usage_report(
                sess, urr, report, num_of_reports);
        report->usage_report[num_of_reports].
            rep_trigger.termination_report = 1;
        num_of_reports++;
        sgwu_sess_urr_acc_snapshot(sess, urr);
    }
    report->num_of_usage_report = num_of_reports;

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    rsp = &pfcp_message->pfcp_session_deletion_response;

    /* Cause */
    rsp->cause.presence = 1;
    rsp->cause.u8 = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    /* Usage Report */
    if (report && report->type.usage_report) {
        unsigned int r;
        ogs_assert(report->num_of_usage_report <=
                OGS_ARRAY_SIZE(rsp->usage_report));
        for (r = 0; r < report->num_of_usage_report; r++) {
            rsp->usage_report[r].presence = 1;
            rsp->usage_report[r].urr_id.presence = 1;
            rsp->usage_report[r].urr_id.u32 = report->usage_report[r].id;
            rsp->usage_report[r].ur_seqn.presence = 1;
            rsp->usage_report[r].ur_seqn.u32 = report->usage_report[r].seqn;
            rsp->usage_report[r].usage_report_trigger.presence = 1;
            rsp->usage_report[r].usage_report_trigger.u24 =
                (report->usage_report[r].rep_trigger.reptri_5 << 16)
                | (report->usage_report[r].rep_trigger.reptri_6 << 8)
                | (report->usage_report[r].rep_trigger.reptri_7);

            if (report->usage_report[r].start_time) {
                rsp->usage_report[r].start_time.presence = 1;
                rsp->usage_report[r].start_time.u32 =
                    report->usage_report[r].start_time;
            }
            if (report->usage_report[r].end_time) {
                rsp->usage_report[r].end_time.presence = 1;
                rsp->usage_report[r].end_time.u32 =
                    report->usage_report[r].end_time;
            }
            if (report->usage_report[r].vol_measurement.flags) {
                rsp->usage_report[r].volume_measurement.presence = 1;
                ogs_pfcp_build_volume_measurement(
                        &rsp->usage_report[r].volume_measurement,
                        &report->usage_report[r].vol_measurement,
                        &usage_report_buf[r].vol_meas,
                        sizeof(usage_report_buf[r].vol_meas));
            }
            rsp->usage_report[r].duration_measurement.presence = 1;
            rsp->usage_report[r].duration_measurement.u32 =
                report->usage_report[r].dur_measurement;
            if (report->usage_report[r].time_of_first_packet) {
                rsp->usage_report[r].time_of_first_packet.presence = 1;
                rsp->usage_report[r].time_of_first_packet.u32 =
                    report->usage_report[r].time_of_first_packet;
            }
            if (report->usage_report[r].time_of_last_packet) {
                rsp->usage_report[r].time_of_last_packet.presence = 1;
                rsp->usage_report[r].time_of_last_packet.u32 =
                    report->usage_report[r].time_of_last_packet;
            }
        }
    }

    pfcp_message->h.type = type;
    pkbuf = ogs_pfcp_build_msg(pfcp_message);
    ogs_expect(pkbuf);

    ogs_free(pfcp_message);

    return pkbuf;
}
