
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dn_perf_ordering_criterion_any_of.h"

char* OpenAPI_dn_perf_ordering_criterion_any_of_ToString(OpenAPI_dn_perf_ordering_criterion_any_of_e dn_perf_ordering_criterion_any_of)
{
    const char *dn_perf_ordering_criterion_any_ofArray[] =  { "NULL", "AVERAGE_TRAFFIC_RATE", "MAXIMUM_TRAFFIC_RATE", "AVERAGE_PACKET_DELAY", "MAXIMUM_PACKET_DELAY", "AVERAGE_PACKET_LOSS_RATE" };
    size_t sizeofArray = sizeof(dn_perf_ordering_criterion_any_ofArray) / sizeof(dn_perf_ordering_criterion_any_ofArray[0]);
    if (dn_perf_ordering_criterion_any_of < sizeofArray)
        return (char *)dn_perf_ordering_criterion_any_ofArray[dn_perf_ordering_criterion_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_dn_perf_ordering_criterion_any_of_e OpenAPI_dn_perf_ordering_criterion_any_of_FromString(char* dn_perf_ordering_criterion_any_of)
{
    int stringToReturn = 0;
    const char *dn_perf_ordering_criterion_any_ofArray[] =  { "NULL", "AVERAGE_TRAFFIC_RATE", "MAXIMUM_TRAFFIC_RATE", "AVERAGE_PACKET_DELAY", "MAXIMUM_PACKET_DELAY", "AVERAGE_PACKET_LOSS_RATE" };
    size_t sizeofArray = sizeof(dn_perf_ordering_criterion_any_ofArray) / sizeof(dn_perf_ordering_criterion_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dn_perf_ordering_criterion_any_of, dn_perf_ordering_criterion_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

