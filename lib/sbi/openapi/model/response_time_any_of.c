
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "response_time_any_of.h"

char* OpenAPI_response_time_any_of_ToString(OpenAPI_response_time_any_of_e response_time_any_of)
{
    const char *response_time_any_ofArray[] =  { "NULL", "LOW_DELAY", "DELAY_TOLERANT", "NO_DELAY" };
    size_t sizeofArray = sizeof(response_time_any_ofArray) / sizeof(response_time_any_ofArray[0]);
    if (response_time_any_of < sizeofArray)
        return (char *)response_time_any_ofArray[response_time_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_response_time_any_of_e OpenAPI_response_time_any_of_FromString(char* response_time_any_of)
{
    int stringToReturn = 0;
    const char *response_time_any_ofArray[] =  { "NULL", "LOW_DELAY", "DELAY_TOLERANT", "NO_DELAY" };
    size_t sizeofArray = sizeof(response_time_any_ofArray) / sizeof(response_time_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(response_time_any_of, response_time_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

