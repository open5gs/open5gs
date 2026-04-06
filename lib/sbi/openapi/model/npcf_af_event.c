
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "npcf_af_event.h"

char* OpenAPI_npcf_af_event_ToString(OpenAPI_npcf_af_event_e npcf_af_event)
{
    const char *npcf_af_eventArray[] =  { "NULL", "ACCESS_TYPE_CHANGE", "ANI_REPORT", "APP_DETECTION", "CHARGING_CORRELATION", "EPS_FALLBACK", "EXTRA_UE_ADDR", "FAILED_QOS_UPDATE", "FAILED_RESOURCES_ALLOCATION", "OUT_OF_CREDIT", "PDU_SESSION_STATUS", "PLMN_CHG", "QOS_MONITORING", "QOS_MON_CAP_REPO", "QOS_NOTIF", "RAN_NAS_CAUSE", "REALLOCATION_OF_CREDIT", "SAT_CATEGORY_CHG", "SUCCESSFUL_QOS_UPDATE", "SUCCESSFUL_RESOURCES_ALLOCATION", "TSN_BRIDGE_INFO", "UP_PATH_CHG_FAILURE", "USAGE_REPORT", "UE_REACH_STATUS_CH", "BAT_OFFSET_INFO", "URSP_ENF_INFO", "PACK_DEL_VAR", "L4S_SUPP", "RT_DELAY_TWO_QOS_FLOWS", "RATE_LIMIT_INFO_REPO", "UP_PATH_CHANGE", "CN_HEALTH_MONITOR", "UE_TEMPORARILY_UNAVAILABLE" };
    size_t sizeofArray = sizeof(npcf_af_eventArray) / sizeof(npcf_af_eventArray[0]);
    if (npcf_af_event < sizeofArray)
        return (char *)npcf_af_eventArray[npcf_af_event];
    else
        return (char *)"Unknown";
}

OpenAPI_npcf_af_event_e OpenAPI_npcf_af_event_FromString(char* npcf_af_event)
{
    int stringToReturn = 0;
    const char *npcf_af_eventArray[] =  { "NULL", "ACCESS_TYPE_CHANGE", "ANI_REPORT", "APP_DETECTION", "CHARGING_CORRELATION", "EPS_FALLBACK", "EXTRA_UE_ADDR", "FAILED_QOS_UPDATE", "FAILED_RESOURCES_ALLOCATION", "OUT_OF_CREDIT", "PDU_SESSION_STATUS", "PLMN_CHG", "QOS_MONITORING", "QOS_MON_CAP_REPO", "QOS_NOTIF", "RAN_NAS_CAUSE", "REALLOCATION_OF_CREDIT", "SAT_CATEGORY_CHG", "SUCCESSFUL_QOS_UPDATE", "SUCCESSFUL_RESOURCES_ALLOCATION", "TSN_BRIDGE_INFO", "UP_PATH_CHG_FAILURE", "USAGE_REPORT", "UE_REACH_STATUS_CH", "BAT_OFFSET_INFO", "URSP_ENF_INFO", "PACK_DEL_VAR", "L4S_SUPP", "RT_DELAY_TWO_QOS_FLOWS", "RATE_LIMIT_INFO_REPO", "UP_PATH_CHANGE", "CN_HEALTH_MONITOR", "UE_TEMPORARILY_UNAVAILABLE" };
    size_t sizeofArray = sizeof(npcf_af_eventArray) / sizeof(npcf_af_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(npcf_af_event, npcf_af_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

