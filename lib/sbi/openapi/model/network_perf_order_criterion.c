
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_perf_order_criterion.h"

char* OpenAPI_network_perf_order_criterion_ToString(OpenAPI_network_perf_order_criterion_e network_perf_order_criterion)
{
    const char *network_perf_order_criterionArray[] =  { "NULL", "NUMBER_OF_UES", "COMMUNICATION_PERF", "MOBILITY_PERF" };
    size_t sizeofArray = sizeof(network_perf_order_criterionArray) / sizeof(network_perf_order_criterionArray[0]);
    if (network_perf_order_criterion < sizeofArray)
        return (char *)network_perf_order_criterionArray[network_perf_order_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_network_perf_order_criterion_e OpenAPI_network_perf_order_criterion_FromString(char* network_perf_order_criterion)
{
    int stringToReturn = 0;
    const char *network_perf_order_criterionArray[] =  { "NULL", "NUMBER_OF_UES", "COMMUNICATION_PERF", "MOBILITY_PERF" };
    size_t sizeofArray = sizeof(network_perf_order_criterionArray) / sizeof(network_perf_order_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(network_perf_order_criterion, network_perf_order_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

