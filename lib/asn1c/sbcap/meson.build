# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

libasn1c_sbcap_sources = files('''
    SBCAP_Broadcast-Cancelled-Area-List-5GS.c
    SBCAP_Broadcast-Cancelled-Area-List.c
    SBCAP_Broadcast-Empty-Area-List-5GS.c
    SBCAP_Broadcast-Empty-Area-List.c
    SBCAP_Broadcast-Scheduled-Area-List-5GS.c
    SBCAP_Broadcast-Scheduled-Area-List.c
    SBCAP_CancelledCellinEAI.c
    SBCAP_CancelledCellinEAI-Item.c
    SBCAP_CancelledCellinTAI-5GS.c
    SBCAP_CancelledCellinTAI.c
    SBCAP_CancelledCellinTAI-Item.c
    SBCAP_Cause.c
    SBCAP_CellId-Broadcast-List-5GS.c
    SBCAP_CellId-Broadcast-List.c
    SBCAP_CellId-Broadcast-List-Item.c
    SBCAP_CellID-Cancelled-Item.c
    SBCAP_CellID-Cancelled-List-5GS.c
    SBCAP_CellID-Cancelled-List.c
    SBCAP_CellIdentity.c
    SBCAP_Concurrent-Warning-Message-Indicator.c
    SBCAP_Criticality.c
    SBCAP_Criticality-Diagnostics.c
    SBCAP_CriticalityDiagnostics-IE-List.c
    SBCAP_Data-Coding-Scheme.c
    SBCAP_ECGIList.c
    SBCAP_EmergencyAreaID-Broadcast-List.c
    SBCAP_EmergencyAreaID-Broadcast-List-Item.c
    SBCAP_Emergency-Area-ID.c
    SBCAP_EmergencyAreaID-Cancelled-Item.c
    SBCAP_EmergencyAreaID-Cancelled-List.c
    SBCAP_Emergency-Area-ID-List.c
    SBCAP_ENB-ID.c
    SBCAP_Error-Indication.c
    SBCAP_EUTRAN-CGI.c
    SBCAP_Extended-Repetition-Period.c
    SBCAP_Failed-Cell-List.c
    SBCAP_Failed-Cell-List-NR.c
    SBCAP_Global-ENB-ID.c
    SBCAP_Global-GNB-ID.c
    SBCAP_Global-NgENB-ID.c
    SBCAP_Global-RAN-Node-ID.c
    SBCAP_GNB-ID.c
    SBCAP_InitiatingMessage.c
    SBCAP_List-of-5GS-Cells-for-Failure.c
    SBCAP_List-of-5GS-TAI-for-Restart.c
    SBCAP_List-of-5GS-TAIs.c
    SBCAP_List-of-EAIs-Restart.c
    SBCAP_List-of-TAIs.c
    SBCAP_List-of-TAIs-Restart.c
    SBCAP_Message-Identifier.c
    SBCAP_NgENB-ID.c
    SBCAP_NRCellIdentity.c
    SBCAP_NR-CGI.c
    SBCAP_NR-CGIList.c
    SBCAP_NumberOfBroadcasts.c
    SBCAP_Number-of-Broadcasts-Requested.c
    SBCAP_Omc-Id.c
    SBCAP_PLMNidentity.c
    SBCAP_Presence.c
    SBCAP_ProcedureCode.c
    SBCAP_ProtocolExtensionContainer.c
    SBCAP_ProtocolExtensionField.c
    SBCAP_ProtocolExtensionID.c
    SBCAP_ProtocolIE-Container.c
    SBCAP_ProtocolIE-ContainerList.c
    SBCAP_ProtocolIE-Field.c
    SBCAP_ProtocolIE-ID.c
    SBCAP_PWS-Failure-Indication.c
    SBCAP_PWS-Restart-Indication.c
    SBCAP_RAT-Selector-5GS.c
    SBCAP_Repetition-Period.c
    SBCAP_Restarted-Cell-List.c
    SBCAP_Restarted-Cell-List-NR.c
    SBCAP_SBC-AP-PDU.c
    SBCAP_ScheduledCellinEAI.c
    SBCAP_ScheduledCellinEAI-Item.c
    SBCAP_ScheduledCellinTAI-5GS.c
    SBCAP_ScheduledCellinTAI.c
    SBCAP_ScheduledCellinTAI-Item.c
    SBCAP_Send-Stop-Warning-Indication.c
    SBCAP_Send-Write-Replace-Warning-Indication.c
    SBCAP_Serial-Number.c
    SBCAP_Stop-All-Indicator.c
    SBCAP_Stop-Warning-Indication.c
    SBCAP_Stop-Warning-Request.c
    SBCAP_Stop-Warning-Response.c
    SBCAP_SuccessfulOutcome.c
    SBCAP_TAC-5GS.c
    SBCAP_TAC.c
    SBCAP_TAI-5GS.c
    SBCAP_TAI-Broadcast-List-5GS.c
    SBCAP_TAI-Broadcast-List.c
    SBCAP_TAI-Broadcast-List-Item.c
    SBCAP_TAI.c
    SBCAP_TAI-Cancelled-List-5GS.c
    SBCAP_TAI-Cancelled-List.c
    SBCAP_TAI-Cancelled-List-Item.c
    SBCAP_TAI-List-for-Warning.c
    SBCAP_TBCD-STRING.c
    SBCAP_TriggeringMessage.c
    SBCAP_TypeOfError.c
    SBCAP_Unknown-5GS-Tracking-Area-List.c
    SBCAP_Unknown-Tracking-Area-List.c
    SBCAP_UnsuccessfulOutcome.c
    SBCAP_Warning-Area-Coordinates.c
    SBCAP_Warning-Area-List-5GS.c
    SBCAP_Warning-Area-List.c
    SBCAP_Warning-Message-Content.c
    SBCAP_Warning-Security-Information.c
    SBCAP_Warning-Type.c
    SBCAP_Write-Replace-Warning-Indication.c
    SBCAP_Write-Replace-Warning-Request.c
    SBCAP_Write-Replace-Warning-Response.c
'''.split())


libasn1c_sbcap_inc = include_directories('.')

libasn1c_sbcap = library('ogsasn1c-sbcap',
    sources : libasn1c_sbcap_sources,
    version : libogslib_version,
    c_args : libasn1c_common_cc_flags,
    include_directories : libasn1c_sbcap_inc,
    dependencies : [libasn1c_common_dep, libasn1c_util_dep],
    install : true)

libasn1c_sbcap_dep = declare_dependency(
    link_with : libasn1c_sbcap,
    include_directories : libasn1c_sbcap_inc,
    dependencies : [libasn1c_common_dep, libasn1c_util_dep])
