
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_subset_any_of.h"

char* OpenAPI_analytics_subset_any_of_ToString(OpenAPI_analytics_subset_any_of_e analytics_subset_any_of)
{
    const char *analytics_subset_any_ofArray[] =  { "NULL", "NUM_OF_UE_REG", "NUM_OF_PDU_SESS_ESTBL", "RES_USAGE", "NUM_OF_EXCEED_RES_USAGE_LOAD_LEVEL_THR", "PERIOD_OF_EXCEED_RES_USAGE_LOAD_LEVEL_THR", "EXCEED_LOAD_LEVEL_THR_IND", "LIST_OF_TOP_APP_UL", "LIST_OF_TOP_APP_DL", "NF_STATUS", "NF_RESOURCE_USAGE", "NF_LOAD", "NF_PEAK_LOAD", "NF_LOAD_AVG_IN_AOI", "DISPER_AMOUNT", "DISPER_CLASS", "RANKING", "PERCENTILE_RANKING", "RSSI", "RTT", "TRAFFIC_INFO", "NUMBER_OF_UES", "APP_LIST_FOR_UE_COMM", "N4_SESS_INACT_TIMER_FOR_UE_COMM", "AVG_TRAFFIC_RATE", "MAX_TRAFFIC_RATE", "AVG_PACKET_DELAY", "MAX_PACKET_DELAY", "AVG_PACKET_LOSS_RATE", "UE_LOCATION", "LIST_OF_HIGH_EXP_UE", "LIST_OF_MEDIUM_EXP_UE", "LIST_OF_LOW_EXP_UE", "AVG_UL_PKT_DROP_RATE", "VAR_UL_PKT_DROP_RATE", "AVG_DL_PKT_DROP_RATE", "VAR_DL_PKT_DROP_RATE", "AVG_UL_PKT_DELAY", "VAR_UL_PKT_DELAY", "AVG_DL_PKT_DELAY", "VAR_DL_PKT_DELAY" };
    size_t sizeofArray = sizeof(analytics_subset_any_ofArray) / sizeof(analytics_subset_any_ofArray[0]);
    if (analytics_subset_any_of < sizeofArray)
        return (char *)analytics_subset_any_ofArray[analytics_subset_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_analytics_subset_any_of_e OpenAPI_analytics_subset_any_of_FromString(char* analytics_subset_any_of)
{
    int stringToReturn = 0;
    const char *analytics_subset_any_ofArray[] =  { "NULL", "NUM_OF_UE_REG", "NUM_OF_PDU_SESS_ESTBL", "RES_USAGE", "NUM_OF_EXCEED_RES_USAGE_LOAD_LEVEL_THR", "PERIOD_OF_EXCEED_RES_USAGE_LOAD_LEVEL_THR", "EXCEED_LOAD_LEVEL_THR_IND", "LIST_OF_TOP_APP_UL", "LIST_OF_TOP_APP_DL", "NF_STATUS", "NF_RESOURCE_USAGE", "NF_LOAD", "NF_PEAK_LOAD", "NF_LOAD_AVG_IN_AOI", "DISPER_AMOUNT", "DISPER_CLASS", "RANKING", "PERCENTILE_RANKING", "RSSI", "RTT", "TRAFFIC_INFO", "NUMBER_OF_UES", "APP_LIST_FOR_UE_COMM", "N4_SESS_INACT_TIMER_FOR_UE_COMM", "AVG_TRAFFIC_RATE", "MAX_TRAFFIC_RATE", "AVG_PACKET_DELAY", "MAX_PACKET_DELAY", "AVG_PACKET_LOSS_RATE", "UE_LOCATION", "LIST_OF_HIGH_EXP_UE", "LIST_OF_MEDIUM_EXP_UE", "LIST_OF_LOW_EXP_UE", "AVG_UL_PKT_DROP_RATE", "VAR_UL_PKT_DROP_RATE", "AVG_DL_PKT_DROP_RATE", "VAR_DL_PKT_DROP_RATE", "AVG_UL_PKT_DELAY", "VAR_UL_PKT_DELAY", "AVG_DL_PKT_DELAY", "VAR_DL_PKT_DELAY" };
    size_t sizeofArray = sizeof(analytics_subset_any_ofArray) / sizeof(analytics_subset_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(analytics_subset_any_of, analytics_subset_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

