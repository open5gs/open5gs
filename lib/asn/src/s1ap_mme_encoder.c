/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */


/*! \file s1ap_mme_encoder.c
   \brief s1ap encode procedures for MME
   \author Sebastien ROUX <sebastien.roux@eurecom.fr>
   \date 2012
   \version 0.1
*/

#if 0 /* modified by acetcom */
#include "intertask_interface.h"
#endif

#include "s1ap_common.h"
#include "s1ap_ies_defs.h"
#include "s1ap_mme_encoder.h"
#if 0 /* modified by acetcom */
#include "s1ap_mme.h"
#include "assertions.h"
#include "log.h"
#else
#include "core_debug.h"
#endif

static inline int                       s1ap_mme_encode_initial_context_setup_request (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       s1ap_mme_encode_s1setupresponse (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       s1ap_mme_encode_s1setupfailure (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       s1ap_mme_encode_ue_context_release_command (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       s1ap_mme_encode_downlink_nas_transport (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);

static inline int                       s1ap_mme_encode_initiating (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       s1ap_mme_encode_successfull_outcome (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * len);
static inline int                       s1ap_mme_encode_unsuccessfull_outcome (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * len);

static inline int
s1ap_mme_encode_initial_context_setup_request (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  S1ap_InitialContextSetupRequest_t       initialContextSetupRequest;
  S1ap_InitialContextSetupRequest_t      *initialContextSetupRequest_p = &initialContextSetupRequest;

  memset (initialContextSetupRequest_p, 0, sizeof (S1ap_InitialContextSetupRequest_t));

  if (s1ap_encode_s1ap_initialcontextsetuprequesties (initialContextSetupRequest_p, &message_p->msg.s1ap_InitialContextSetupRequestIEs) < 0) {
    return -1;
  }

  return s1ap_generate_initiating_message (buffer, length, S1ap_ProcedureCode_id_InitialContextSetup, S1ap_Criticality_reject, &asn_DEF_S1ap_InitialContextSetupRequest, initialContextSetupRequest_p);
}

int
s1ap_mme_encode_pdu (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  d_assert (message_p != NULL, return -1, "Null param");
  d_assert (buffer != NULL, return -1, "Null param");
  d_assert (length != NULL, return -1, "Null param");

  switch (message_p->direction) {
  case S1AP_PDU_PR_initiatingMessage:
    return s1ap_mme_encode_initiating (message_p, buffer, length);

  case S1AP_PDU_PR_successfulOutcome:
    return s1ap_mme_encode_successfull_outcome (message_p, buffer, length);

  case S1AP_PDU_PR_unsuccessfulOutcome:
    return s1ap_mme_encode_unsuccessfull_outcome (message_p, buffer, length);

  default:
    d_warn("Unknown message outcome (%d) or not implemented", (int)message_p->direction);
    break;
  }

  return -1;
}

static inline int
s1ap_mme_encode_initiating (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  switch (message_p->procedureCode) {
  case S1ap_ProcedureCode_id_downlinkNASTransport:
    return s1ap_mme_encode_downlink_nas_transport (message_p, buffer, length);

  case S1ap_ProcedureCode_id_InitialContextSetup:
    return s1ap_mme_encode_initial_context_setup_request (message_p, buffer, length);

  case S1ap_ProcedureCode_id_UEContextRelease:
    return s1ap_mme_encode_ue_context_release_command (message_p, buffer, length);

  default:
    d_warn("Unknown procedure ID (%d) for initiating message_p\n", (int)message_p->procedureCode);
    break;
  }

  return -1;
}

static inline int
s1ap_mme_encode_successfull_outcome (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  switch (message_p->procedureCode) {
  case S1ap_ProcedureCode_id_S1Setup:
    return s1ap_mme_encode_s1setupresponse (message_p, buffer, length);

  default:
    d_warn("Unknown procedure ID (%d) for successfull outcome message\n", (int)message_p->procedureCode);
    break;
  }

  return -1;
}

static inline int
s1ap_mme_encode_unsuccessfull_outcome (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  switch (message_p->procedureCode) {
  case S1ap_ProcedureCode_id_S1Setup:
    return s1ap_mme_encode_s1setupfailure (message_p, buffer, length);

  default:
    d_warn("Unknown procedure ID (%d) for unsuccessfull outcome message\n", (int)message_p->procedureCode);
    break;
  }

  return -1;
}

static inline int
s1ap_mme_encode_s1setupresponse (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  S1ap_S1SetupResponse_t                  s1SetupResponse;
  S1ap_S1SetupResponse_t                 *s1SetupResponse_p = &s1SetupResponse;

  memset (s1SetupResponse_p, 0, sizeof (S1ap_S1SetupResponse_t));

  if (s1ap_encode_s1ap_s1setupresponseies (s1SetupResponse_p, &message_p->msg.s1ap_S1SetupResponseIEs) < 0) {
    return -1;
  }

  return s1ap_generate_successfull_outcome (buffer, length, S1ap_ProcedureCode_id_S1Setup, message_p->criticality, &asn_DEF_S1ap_S1SetupResponse, s1SetupResponse_p);
}

static inline int
s1ap_mme_encode_s1setupfailure (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  S1ap_S1SetupFailure_t                   s1SetupFailure;
  S1ap_S1SetupFailure_t                  *s1SetupFailure_p = &s1SetupFailure;

  memset (s1SetupFailure_p, 0, sizeof (S1ap_S1SetupFailure_t));

  if (s1ap_encode_s1ap_s1setupfailureies (s1SetupFailure_p, &message_p->msg.s1ap_S1SetupFailureIEs) < 0) {
    return -1;
  }

  return s1ap_generate_unsuccessfull_outcome (buffer, length, S1ap_ProcedureCode_id_S1Setup, message_p->criticality, &asn_DEF_S1ap_S1SetupFailure, s1SetupFailure_p);
}

static inline int
s1ap_mme_encode_downlink_nas_transport (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  S1ap_DownlinkNASTransport_t             downlinkNasTransport;
  S1ap_DownlinkNASTransport_t            *downlinkNasTransport_p = &downlinkNasTransport;

  memset (downlinkNasTransport_p, 0, sizeof (S1ap_DownlinkNASTransport_t));

  /*
   * Convert IE structure into asn1 message_p
   */
  if (s1ap_encode_s1ap_downlinknastransport_ies (downlinkNasTransport_p, &message_p->msg.s1ap_DownlinkNASTransport_IEs) < 0) {
    return -1;
  }

  /*
   * Generate Initiating message_p for the list of IEs
   */
  return s1ap_generate_initiating_message (buffer, length, S1ap_ProcedureCode_id_downlinkNASTransport, message_p->criticality, &asn_DEF_S1ap_DownlinkNASTransport, downlinkNasTransport_p);
}

static inline int
s1ap_mme_encode_ue_context_release_command (
  s1ap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  S1ap_UEContextReleaseCommand_t          ueContextReleaseCommand;
  S1ap_UEContextReleaseCommand_t         *ueContextReleaseCommand_p = &ueContextReleaseCommand;

  memset (ueContextReleaseCommand_p, 0, sizeof (S1ap_UEContextReleaseCommand_t));

  /*
   * Convert IE structure into asn1 message_p
   */
  if (s1ap_encode_s1ap_uecontextreleasecommand_ies (ueContextReleaseCommand_p, &message_p->msg.s1ap_UEContextReleaseCommand_IEs) < 0) {
    return -1;
  }

  return s1ap_generate_initiating_message (buffer, length, S1ap_ProcedureCode_id_UEContextRelease, message_p->criticality, &asn_DEF_S1ap_UEContextReleaseCommand, ueContextReleaseCommand_p);
}
