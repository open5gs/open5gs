
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_control_request_trigger.h"

char* OpenAPI_policy_control_request_trigger_ToString(OpenAPI_policy_control_request_trigger_e policy_control_request_trigger)
{
    const char *policy_control_request_triggerArray[] =  { "NULL", "PLMN_CH", "RES_MO_RE", "AC_TY_CH", "UE_IP_CH", "UE_MAC_CH", "AN_CH_COR", "US_RE", "APP_STA", "APP_STO", "AN_INFO", "CM_SES_FAIL", "PS_DA_OFF", "DEF_QOS_CH", "SE_AMBR_CH", "QOS_NOTIF", "NO_CREDIT", "REALLO_OF_CREDIT", "PRA_CH", "SAREA_CH", "SCNN_CH", "RE_TIMEOUT", "RES_RELEASE", "SUCC_RES_ALLO", "RAT_TY_CH", "REF_QOS_IND_CH", "NUM_OF_PACKET_FILTER", "UE_STATUS_RESUME", "UE_TZ_CH", "AUTH_PROF_CH", "QOS_MONITORING", "SCELL_CH", "EPS_FALLBACK", "MA_PDU", "TSN_BRIDGE_INFO", "_5G_RG_JOIN", "_5G_RG_LEAVE", "DDN_FAILURE", "DDN_DELIVERY_STATUS", "GROUP_ID_LIST_CHG", "DDN_FAILURE_CANCELLATION", "DDN_DELIVERY_STATUS_CANCELLATION", "VPLMN_QOS_CH" };
    size_t sizeofArray = sizeof(policy_control_request_triggerArray) / sizeof(policy_control_request_triggerArray[0]);
    if (policy_control_request_trigger < sizeofArray)
        return (char *)policy_control_request_triggerArray[policy_control_request_trigger];
    else
        return (char *)"Unknown";
}

OpenAPI_policy_control_request_trigger_e OpenAPI_policy_control_request_trigger_FromString(char* policy_control_request_trigger)
{
    int stringToReturn = 0;
    const char *policy_control_request_triggerArray[] =  { "NULL", "PLMN_CH", "RES_MO_RE", "AC_TY_CH", "UE_IP_CH", "UE_MAC_CH", "AN_CH_COR", "US_RE", "APP_STA", "APP_STO", "AN_INFO", "CM_SES_FAIL", "PS_DA_OFF", "DEF_QOS_CH", "SE_AMBR_CH", "QOS_NOTIF", "NO_CREDIT", "REALLO_OF_CREDIT", "PRA_CH", "SAREA_CH", "SCNN_CH", "RE_TIMEOUT", "RES_RELEASE", "SUCC_RES_ALLO", "RAT_TY_CH", "REF_QOS_IND_CH", "NUM_OF_PACKET_FILTER", "UE_STATUS_RESUME", "UE_TZ_CH", "AUTH_PROF_CH", "QOS_MONITORING", "SCELL_CH", "EPS_FALLBACK", "MA_PDU", "TSN_BRIDGE_INFO", "_5G_RG_JOIN", "_5G_RG_LEAVE", "DDN_FAILURE", "DDN_DELIVERY_STATUS", "GROUP_ID_LIST_CHG", "DDN_FAILURE_CANCELLATION", "DDN_DELIVERY_STATUS_CANCELLATION", "VPLMN_QOS_CH" };
    size_t sizeofArray = sizeof(policy_control_request_triggerArray) / sizeof(policy_control_request_triggerArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(policy_control_request_trigger, policy_control_request_triggerArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

