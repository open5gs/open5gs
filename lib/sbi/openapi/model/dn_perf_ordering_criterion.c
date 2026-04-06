
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dn_perf_ordering_criterion.h"

char* OpenAPI_dn_perf_ordering_criterion_ToString(OpenAPI_dn_perf_ordering_criterion_e dn_perf_ordering_criterion)
{
    const char *dn_perf_ordering_criterionArray[] =  { "NULL", "AVERAGE_TRAFFIC_RATE", "MAXIMUM_TRAFFIC_RATE", "AVERAGE_PACKET_DELAY", "MAXIMUM_PACKET_DELAY", "AVERAGE_PACKET_LOSS_RATE" };
    size_t sizeofArray = sizeof(dn_perf_ordering_criterionArray) / sizeof(dn_perf_ordering_criterionArray[0]);
    if (dn_perf_ordering_criterion < sizeofArray)
        return (char *)dn_perf_ordering_criterionArray[dn_perf_ordering_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_dn_perf_ordering_criterion_e OpenAPI_dn_perf_ordering_criterion_FromString(char* dn_perf_ordering_criterion)
{
    int stringToReturn = 0;
    const char *dn_perf_ordering_criterionArray[] =  { "NULL", "AVERAGE_TRAFFIC_RATE", "MAXIMUM_TRAFFIC_RATE", "AVERAGE_PACKET_DELAY", "MAXIMUM_PACKET_DELAY", "AVERAGE_PACKET_LOSS_RATE" };
    size_t sizeofArray = sizeof(dn_perf_ordering_criterionArray) / sizeof(dn_perf_ordering_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dn_perf_ordering_criterion, dn_perf_ordering_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

