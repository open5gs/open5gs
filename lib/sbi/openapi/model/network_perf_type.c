
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_perf_type.h"

char* OpenAPI_network_perf_type_ToString(OpenAPI_network_perf_type_e network_perf_type)
{
    const char *network_perf_typeArray[] =  { "NULL", "GNB_ACTIVE_RATIO", "GNB_COMPUTING_USAGE", "GNB_MEMORY_USAGE", "GNB_DISK_USAGE", "GNB_RSC_USAGE_OVERALL_TRAFFIC", "GNB_RSC_USAGE_GBR_TRAFFIC", "GNB_RSC_USAGE_DELAY_CRIT_GBR_TRAFFIC", "NUM_OF_UE", "SESS_SUCC_RATIO", "HO_SUCC_RATIO" };
    size_t sizeofArray = sizeof(network_perf_typeArray) / sizeof(network_perf_typeArray[0]);
    if (network_perf_type < sizeofArray)
        return (char *)network_perf_typeArray[network_perf_type];
    else
        return (char *)"Unknown";
}

OpenAPI_network_perf_type_e OpenAPI_network_perf_type_FromString(char* network_perf_type)
{
    int stringToReturn = 0;
    const char *network_perf_typeArray[] =  { "NULL", "GNB_ACTIVE_RATIO", "GNB_COMPUTING_USAGE", "GNB_MEMORY_USAGE", "GNB_DISK_USAGE", "GNB_RSC_USAGE_OVERALL_TRAFFIC", "GNB_RSC_USAGE_GBR_TRAFFIC", "GNB_RSC_USAGE_DELAY_CRIT_GBR_TRAFFIC", "NUM_OF_UE", "SESS_SUCC_RATIO", "HO_SUCC_RATIO" };
    size_t sizeofArray = sizeof(network_perf_typeArray) / sizeof(network_perf_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(network_perf_type, network_perf_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

