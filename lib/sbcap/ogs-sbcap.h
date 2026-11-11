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

#ifndef OGS_SBCAP_H
#define OGS_SBCAP_H

#include "core/ogs-core.h"

#include "SBCAP_asn_constant.h"
#include "SBCAP_Broadcast-Cancelled-Area-List-5GS.h"
#include "SBCAP_Broadcast-Cancelled-Area-List.h"
#include "SBCAP_Broadcast-Empty-Area-List-5GS.h"
#include "SBCAP_Broadcast-Empty-Area-List.h"
#include "SBCAP_Broadcast-Scheduled-Area-List-5GS.h"
#include "SBCAP_Broadcast-Scheduled-Area-List.h"
#include "SBCAP_CancelledCellinEAI.h"
#include "SBCAP_CancelledCellinEAI-Item.h"
#include "SBCAP_CancelledCellinTAI-5GS.h"
#include "SBCAP_CancelledCellinTAI.h"
#include "SBCAP_CancelledCellinTAI-Item.h"
#include "SBCAP_Cause.h"
#include "SBCAP_CellId-Broadcast-List-5GS.h"
#include "SBCAP_CellId-Broadcast-List.h"
#include "SBCAP_CellId-Broadcast-List-Item.h"
#include "SBCAP_CellID-Cancelled-Item.h"
#include "SBCAP_CellID-Cancelled-List-5GS.h"
#include "SBCAP_CellID-Cancelled-List.h"
#include "SBCAP_CellIdentity.h"
#include "SBCAP_Concurrent-Warning-Message-Indicator.h"
#include "SBCAP_Criticality-Diagnostics.h"
#include "SBCAP_CriticalityDiagnostics-IE-List.h"
#include "SBCAP_Criticality.h"
#include "SBCAP_Data-Coding-Scheme.h"
#include "SBCAP_ECGIList.h"
#include "SBCAP_EmergencyAreaID-Broadcast-List.h"
#include "SBCAP_EmergencyAreaID-Broadcast-List-Item.h"
#include "SBCAP_EmergencyAreaID-Cancelled-Item.h"
#include "SBCAP_EmergencyAreaID-Cancelled-List.h"
#include "SBCAP_Emergency-Area-ID.h"
#include "SBCAP_Emergency-Area-ID-List.h"
#include "SBCAP_ENB-ID.h"
#include "SBCAP_Error-Indication.h"
#include "SBCAP_EUTRAN-CGI.h"
#include "SBCAP_Extended-Repetition-Period.h"
#include "SBCAP_Failed-Cell-List.h"
#include "SBCAP_Failed-Cell-List-NR.h"
#include "SBCAP_Global-ENB-ID.h"
#include "SBCAP_Global-GNB-ID.h"
#include "SBCAP_Global-NgENB-ID.h"
#include "SBCAP_Global-RAN-Node-ID.h"
#include "SBCAP_GNB-ID.h"
#include "SBCAP_InitiatingMessage.h"
#include "SBCAP_List-of-5GS-Cells-for-Failure.h"
#include "SBCAP_List-of-5GS-TAI-for-Restart.h"
#include "SBCAP_List-of-5GS-TAIs.h"
#include "SBCAP_List-of-EAIs-Restart.h"
#include "SBCAP_List-of-TAIs.h"
#include "SBCAP_List-of-TAIs-Restart.h"
#include "SBCAP_Message-Identifier.h"
#include "SBCAP_NgENB-ID.h"
#include "SBCAP_NRCellIdentity.h"
#include "SBCAP_NR-CGI.h"
#include "SBCAP_NR-CGIList.h"
#include "SBCAP_NumberOfBroadcasts.h"
#include "SBCAP_Number-of-Broadcasts-Requested.h"
#include "SBCAP_Omc-Id.h"
#include "SBCAP_PLMNidentity.h"
#include "SBCAP_Presence.h"
#include "SBCAP_ProcedureCode.h"
#include "SBCAP_ProtocolExtensionContainer.h"
#include "SBCAP_ProtocolExtensionField.h"
#include "SBCAP_ProtocolExtensionID.h"
#include "SBCAP_ProtocolIE-Container.h"
#include "SBCAP_ProtocolIE-ContainerList.h"
#include "SBCAP_ProtocolIE-Field.h"
#include "SBCAP_ProtocolIE-ID.h"
#include "SBCAP_PWS-Failure-Indication.h"
#include "SBCAP_PWS-Restart-Indication.h"
#include "SBCAP_RAT-Selector-5GS.h"
#include "SBCAP_Repetition-Period.h"
#include "SBCAP_Restarted-Cell-List.h"
#include "SBCAP_Restarted-Cell-List-NR.h"
#include "SBCAP_SBC-AP-PDU.h"
#include "SBCAP_ScheduledCellinEAI.h"
#include "SBCAP_ScheduledCellinEAI-Item.h"
#include "SBCAP_ScheduledCellinTAI-5GS.h"
#include "SBCAP_ScheduledCellinTAI.h"
#include "SBCAP_ScheduledCellinTAI-Item.h"
#include "SBCAP_Send-Stop-Warning-Indication.h"
#include "SBCAP_Send-Write-Replace-Warning-Indication.h"
#include "SBCAP_Serial-Number.h"
#include "SBCAP_Stop-All-Indicator.h"
#include "SBCAP_Stop-Warning-Indication.h"
#include "SBCAP_Stop-Warning-Request.h"
#include "SBCAP_Stop-Warning-Response.h"
#include "SBCAP_SuccessfulOutcome.h"
#include "SBCAP_TAC-5GS.h"
#include "SBCAP_TAC.h"
#include "SBCAP_TAI-5GS.h"
#include "SBCAP_TAI-Broadcast-List-5GS.h"
#include "SBCAP_TAI-Broadcast-List.h"
#include "SBCAP_TAI-Broadcast-List-Item.h"
#include "SBCAP_TAI-Cancelled-List-5GS.h"
#include "SBCAP_TAI-Cancelled-List.h"
#include "SBCAP_TAI-Cancelled-List-Item.h"
#include "SBCAP_TAI.h"
#include "SBCAP_TAI-List-for-Warning.h"
#include "SBCAP_TBCD-STRING.h"
#include "SBCAP_TriggeringMessage.h"
#include "SBCAP_TypeOfError.h"
#include "SBCAP_Unknown-5GS-Tracking-Area-List.h"
#include "SBCAP_Unknown-Tracking-Area-List.h"
#include "SBCAP_UnsuccessfulOutcome.h"
#include "SBCAP_Warning-Area-Coordinates.h"
#include "SBCAP_Warning-Area-List-5GS.h"
#include "SBCAP_Warning-Area-List.h"
#include "SBCAP_Warning-Message-Content.h"
#include "SBCAP_Warning-Security-Information.h"
#include "SBCAP_Warning-Type.h"
#include "SBCAP_Write-Replace-Warning-Indication.h"
#include "SBCAP_Write-Replace-Warning-Request.h"
#include "SBCAP_Write-Replace-Warning-Response.h"

//#include "asn1c/util/conv.h"
#include "asn1c/util/message.h"

#define OGS_SBCAP_INSIDE

//#include "s1ap/conv.h"
#include "sbcap/message.h"
//#include "s1ap/build.h"

#undef OGS_SBCAP_INSIDE

#ifdef __cplusplus
extern "C" {
#endif

extern int __ogs_sbcap_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sbcap_domain

#ifdef __cplusplus
}
#endif

#endif
