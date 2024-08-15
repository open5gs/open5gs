/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef NGAP_PATH_H
#define NGAP_PATH_H

#include "ngap-build.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NGAP_NON_UE_SIGNALLING   0

#define ngap_event_push  amf_sctp_event_push

int ngap_open(void);
void ngap_close(void);

ogs_sock_t *ngap_server(ogs_socknode_t *node);
void ngap_recv_upcall(short when, ogs_socket_t fd, void *data);

int ngap_send_to_gnb(
        amf_gnb_t *gnb, ogs_pkbuf_t *pkb, uint16_t stream_no);
int ngap_send_to_ran_ue(ran_ue_t *ran_ue, ogs_pkbuf_t *pkbuf);
int ngap_delayed_send_to_ran_ue(ran_ue_t *ran_ue,
        ogs_pkbuf_t *pkbuf, ogs_time_t duration);
int ngap_send_to_nas(ran_ue_t *ran_ue,
        NGAP_ProcedureCode_t procedureCode, NGAP_NAS_PDU_t *nasPdu);
int ngap_send_to_5gsm(amf_ue_t *amf_ue, ogs_pkbuf_t *esmbuf);

int ngap_send_ng_setup_response(amf_gnb_t *gnb);
int ngap_send_ng_setup_failure(
        amf_gnb_t *gnb, NGAP_Cause_PR group, long cause);
int ngap_send_ran_configuration_update_ack(amf_gnb_t *gnb);
int ngap_send_ran_configuration_update_failure(
        amf_gnb_t *gnb, NGAP_Cause_PR group, long cause);

int ngap_send_ran_ue_context_release_command(
    ran_ue_t *ran_ue, NGAP_Cause_PR group, long cause,
    uint8_t action, ogs_time_t duration);

int ngap_send_paging(amf_ue_t *amf_ue);

int ngap_send_downlink_ran_configuration_transfer(
        amf_gnb_t *target_gnb, NGAP_SONConfigurationTransfer_t *transfer);

int ngap_send_path_switch_ack(amf_sess_t *sess);

int ngap_send_handover_request(amf_ue_t *amf_ue);
int ngap_send_handover_preparation_failure(
        ran_ue_t *source_ue, NGAP_Cause_t *cause);
int ngap_send_handover_command(amf_ue_t *amf_ue);
int ngap_send_handover_cancel_ack(ran_ue_t *source_ue);

int ngap_send_downlink_ran_status_transfer(
        ran_ue_t *target_ue,
        NGAP_RANStatusTransfer_TransparentContainer_t *transfer);

int ngap_send_error_indication(
        amf_gnb_t *gnb,
        uint64_t *ran_ue_ngap_id,
        uint64_t *amf_ue_ngap_id,
        NGAP_Cause_PR group, long cause);
int ngap_send_error_indication2(
        ran_ue_t *ran_ue, NGAP_Cause_PR group, long cause);
int ngap_send_ng_reset_ack(
        amf_gnb_t *gnb,
        NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface);

#ifdef __cplusplus
}
#endif

#endif /* NGAP_PATH_H */
