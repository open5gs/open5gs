
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "response_time.h"

char* OpenAPI_response_time_ToString(OpenAPI_response_time_e response_time)
{
    const char *response_timeArray[] =  { "NULL", "LOW_DELAY", "DELAY_TOLERANT", "NO_DELAY" };
    size_t sizeofArray = sizeof(response_timeArray) / sizeof(response_timeArray[0]);
    if (response_time < sizeofArray)
        return (char *)response_timeArray[response_time];
    else
        return (char *)"Unknown";
}

OpenAPI_response_time_e OpenAPI_response_time_FromString(char* response_time)
{
    int stringToReturn = 0;
    const char *response_timeArray[] =  { "NULL", "LOW_DELAY", "DELAY_TOLERANT", "NO_DELAY" };
    size_t sizeofArray = sizeof(response_timeArray) / sizeof(response_timeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(response_time, response_timeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

