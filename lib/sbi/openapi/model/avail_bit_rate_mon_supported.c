
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "avail_bit_rate_mon_supported.h"

char* OpenAPI_avail_bit_rate_mon_supported_ToString(OpenAPI_avail_bit_rate_mon_supported_e avail_bit_rate_mon_supported)
{
    const char *avail_bit_rate_mon_supportedArray[] =  { "NULL", "SUPPORTED", "NOT_SUPPORTED", "UNKNOWN" };
    size_t sizeofArray = sizeof(avail_bit_rate_mon_supportedArray) / sizeof(avail_bit_rate_mon_supportedArray[0]);
    if (avail_bit_rate_mon_supported < sizeofArray)
        return (char *)avail_bit_rate_mon_supportedArray[avail_bit_rate_mon_supported];
    else
        return (char *)"Unknown";
}

OpenAPI_avail_bit_rate_mon_supported_e OpenAPI_avail_bit_rate_mon_supported_FromString(char* avail_bit_rate_mon_supported)
{
    int stringToReturn = 0;
    const char *avail_bit_rate_mon_supportedArray[] =  { "NULL", "SUPPORTED", "NOT_SUPPORTED", "UNKNOWN" };
    size_t sizeofArray = sizeof(avail_bit_rate_mon_supportedArray) / sizeof(avail_bit_rate_mon_supportedArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(avail_bit_rate_mon_supported, avail_bit_rate_mon_supportedArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

