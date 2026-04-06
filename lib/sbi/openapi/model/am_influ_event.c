
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_influ_event.h"

char* OpenAPI_am_influ_event_ToString(OpenAPI_am_influ_event_e am_influ_event)
{
    const char *am_influ_eventArray[] =  { "NULL", "SERVICE_AREA_COVRG_OUTCOME", "SLICE_REPLACE_OUTCOME" };
    size_t sizeofArray = sizeof(am_influ_eventArray) / sizeof(am_influ_eventArray[0]);
    if (am_influ_event < sizeofArray)
        return (char *)am_influ_eventArray[am_influ_event];
    else
        return (char *)"Unknown";
}

OpenAPI_am_influ_event_e OpenAPI_am_influ_event_FromString(char* am_influ_event)
{
    int stringToReturn = 0;
    const char *am_influ_eventArray[] =  { "NULL", "SERVICE_AREA_COVRG_OUTCOME", "SLICE_REPLACE_OUTCOME" };
    size_t sizeofArray = sizeof(am_influ_eventArray) / sizeof(am_influ_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(am_influ_event, am_influ_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

