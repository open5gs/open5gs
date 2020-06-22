
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "max_integrity_protected_data_rate.h"

char* OpenAPI_max_integrity_protected_data_rate_ToString(OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate)
{
    const char *max_integrity_protected_data_rateArray[] =  { "NULL", "_64_KBPS", "MAX_UE_RATE" };
    size_t sizeofArray = sizeof(max_integrity_protected_data_rateArray) / sizeof(max_integrity_protected_data_rateArray[0]);
    if (max_integrity_protected_data_rate < sizeofArray)
        return (char *)max_integrity_protected_data_rateArray[max_integrity_protected_data_rate];
    else
        return (char *)"Unknown";
}

OpenAPI_max_integrity_protected_data_rate_e OpenAPI_max_integrity_protected_data_rate_FromString(char* max_integrity_protected_data_rate)
{
    int stringToReturn = 0;
    const char *max_integrity_protected_data_rateArray[] =  { "NULL", "_64_KBPS", "MAX_UE_RATE" };
    size_t sizeofArray = sizeof(max_integrity_protected_data_rateArray) / sizeof(max_integrity_protected_data_rateArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(max_integrity_protected_data_rate, max_integrity_protected_data_rateArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

