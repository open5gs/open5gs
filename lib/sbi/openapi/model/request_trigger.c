
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "request_trigger.h"

char* OpenAPI_request_trigger_ToString(OpenAPI_request_trigger_e request_trigger)
{
    const char *request_triggerArray[] =  { "NULL", "LOC_CH", "PRA_CH", "SERV_AREA_CH", "RFSP_CH", "ALLOWED_NSSAI_CH", "UE_AMBR_CH", "SMF_SELECT_CH", "ACCESS_TYPE_CH" };
    size_t sizeofArray = sizeof(request_triggerArray) / sizeof(request_triggerArray[0]);
    if (request_trigger < sizeofArray)
        return (char *)request_triggerArray[request_trigger];
    else
        return (char *)"Unknown";
}

OpenAPI_request_trigger_e OpenAPI_request_trigger_FromString(char* request_trigger)
{
    int stringToReturn = 0;
    const char *request_triggerArray[] =  { "NULL", "LOC_CH", "PRA_CH", "SERV_AREA_CH", "RFSP_CH", "ALLOWED_NSSAI_CH", "UE_AMBR_CH", "SMF_SELECT_CH", "ACCESS_TYPE_CH" };
    size_t sizeofArray = sizeof(request_triggerArray) / sizeof(request_triggerArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(request_trigger, request_triggerArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

