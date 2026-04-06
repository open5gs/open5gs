
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event.h"

char* OpenAPI_event_ToString(OpenAPI_event_e event)
{
    const char *eventArray[] =  { "NULL", "SUCCESS_UE_POL_DEL_SP", "UNSUCCESS_UE_POL_DEL_SP", "PARTLY_UNSUCC_UE_POL_DEL_SP", "UNSUCCESS_PCF_SERVICE_AUTHORIZATION" };
    size_t sizeofArray = sizeof(eventArray) / sizeof(eventArray[0]);
    if (event < sizeofArray)
        return (char *)eventArray[event];
    else
        return (char *)"Unknown";
}

OpenAPI_event_e OpenAPI_event_FromString(char* event)
{
    int stringToReturn = 0;
    const char *eventArray[] =  { "NULL", "SUCCESS_UE_POL_DEL_SP", "UNSUCCESS_UE_POL_DEL_SP", "PARTLY_UNSUCC_UE_POL_DEL_SP", "UNSUCCESS_PCF_SERVICE_AUTHORIZATION" };
    size_t sizeofArray = sizeof(eventArray) / sizeof(eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event, eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

