
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uli_change_granularity.h"

char* OpenAPI_uli_change_granularity_ToString(OpenAPI_uli_change_granularity_e uli_change_granularity)
{
    const char *uli_change_granularityArray[] =  { "NULL", "GNB", "TAI" };
    size_t sizeofArray = sizeof(uli_change_granularityArray) / sizeof(uli_change_granularityArray[0]);
    if (uli_change_granularity < sizeofArray)
        return (char *)uli_change_granularityArray[uli_change_granularity];
    else
        return (char *)"Unknown";
}

OpenAPI_uli_change_granularity_e OpenAPI_uli_change_granularity_FromString(char* uli_change_granularity)
{
    int stringToReturn = 0;
    const char *uli_change_granularityArray[] =  { "NULL", "GNB", "TAI" };
    size_t sizeofArray = sizeof(uli_change_granularityArray) / sizeof(uli_change_granularityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(uli_change_granularity, uli_change_granularityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

