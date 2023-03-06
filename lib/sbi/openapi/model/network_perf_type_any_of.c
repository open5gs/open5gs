
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_perf_type_any_of.h"

char* OpenAPI_network_perf_type_any_of_ToString(OpenAPI_network_perf_type_any_of_e network_perf_type_any_of)
{
    const char *network_perf_type_any_ofArray[] =  { "NULL", "GNB_ACTIVE_RATIO", "GNB_COMPUTING_USAGE", "GNB_MEMORY_USAGE", "GNB_DISK_USAGE", "NUM_OF_UE", "SESS_SUCC_RATIO", "HO_SUCC_RATIO" };
    size_t sizeofArray = sizeof(network_perf_type_any_ofArray) / sizeof(network_perf_type_any_ofArray[0]);
    if (network_perf_type_any_of < sizeofArray)
        return (char *)network_perf_type_any_ofArray[network_perf_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_network_perf_type_any_of_e OpenAPI_network_perf_type_any_of_FromString(char* network_perf_type_any_of)
{
    int stringToReturn = 0;
    const char *network_perf_type_any_ofArray[] =  { "NULL", "GNB_ACTIVE_RATIO", "GNB_COMPUTING_USAGE", "GNB_MEMORY_USAGE", "GNB_DISK_USAGE", "NUM_OF_UE", "SESS_SUCC_RATIO", "HO_SUCC_RATIO" };
    size_t sizeofArray = sizeof(network_perf_type_any_ofArray) / sizeof(network_perf_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(network_perf_type_any_of, network_perf_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

