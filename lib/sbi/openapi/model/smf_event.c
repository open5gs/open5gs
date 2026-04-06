
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_event.h"

char* OpenAPI_smf_event_ToString(OpenAPI_smf_event_e smf_event)
{
    const char *smf_eventArray[] =  { "NULL", "AC_TY_CH", "UP_PATH_CH", "PDU_SES_REL", "PLMN_CH", "UE_IP_CH", "RAT_TY_CH", "DDDS", "COMM_FAIL", "PDU_SES_EST", "QFI_ALLOC", "QOS_MON", "SMCC_EXP", "DISPERSION", "RED_TRANS_EXP", "WLAN_INFO", "UPF_INFO", "UP_STATUS_INFO", "UPF_EVENT", "SATB_CH", "TRAFFIC_CORRELATION", "TRAFF_ROUTE_REQ_OUTCOME", "SIM_CONN_FAIL", "QFI_DEALLOCATION", "QOS_FLOW_CHANGE", "ENERGY_USAGE_DATA", "SIGNALLING_INFO" };
    size_t sizeofArray = sizeof(smf_eventArray) / sizeof(smf_eventArray[0]);
    if (smf_event < sizeofArray)
        return (char *)smf_eventArray[smf_event];
    else
        return (char *)"Unknown";
}

OpenAPI_smf_event_e OpenAPI_smf_event_FromString(char* smf_event)
{
    int stringToReturn = 0;
    const char *smf_eventArray[] =  { "NULL", "AC_TY_CH", "UP_PATH_CH", "PDU_SES_REL", "PLMN_CH", "UE_IP_CH", "RAT_TY_CH", "DDDS", "COMM_FAIL", "PDU_SES_EST", "QFI_ALLOC", "QOS_MON", "SMCC_EXP", "DISPERSION", "RED_TRANS_EXP", "WLAN_INFO", "UPF_INFO", "UP_STATUS_INFO", "UPF_EVENT", "SATB_CH", "TRAFFIC_CORRELATION", "TRAFF_ROUTE_REQ_OUTCOME", "SIM_CONN_FAIL", "QFI_DEALLOCATION", "QOS_FLOW_CHANGE", "ENERGY_USAGE_DATA", "SIGNALLING_INFO" };
    size_t sizeofArray = sizeof(smf_eventArray) / sizeof(smf_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(smf_event, smf_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

