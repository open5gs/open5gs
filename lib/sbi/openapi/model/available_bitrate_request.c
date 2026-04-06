
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "available_bitrate_request.h"

char* OpenAPI_available_bitrate_request_ToString(OpenAPI_available_bitrate_request_e available_bitrate_request)
{
    const char *available_bitrate_requestArray[] =  { "NULL", "UL", "DL", "BOTH", "STOP" };
    size_t sizeofArray = sizeof(available_bitrate_requestArray) / sizeof(available_bitrate_requestArray[0]);
    if (available_bitrate_request < sizeofArray)
        return (char *)available_bitrate_requestArray[available_bitrate_request];
    else
        return (char *)"Unknown";
}

OpenAPI_available_bitrate_request_e OpenAPI_available_bitrate_request_FromString(char* available_bitrate_request)
{
    int stringToReturn = 0;
    const char *available_bitrate_requestArray[] =  { "NULL", "UL", "DL", "BOTH", "STOP" };
    size_t sizeofArray = sizeof(available_bitrate_requestArray) / sizeof(available_bitrate_requestArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(available_bitrate_request, available_bitrate_requestArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

