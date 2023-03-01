
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_influ_event_any_of.h"

char* OpenAPI_am_influ_event_any_of_ToString(OpenAPI_am_influ_event_any_of_e am_influ_event_any_of)
{
    const char *am_influ_event_any_ofArray[] =  { "NULL", "SERVICE_AREA_COVRG_OUTCOME" };
    size_t sizeofArray = sizeof(am_influ_event_any_ofArray) / sizeof(am_influ_event_any_ofArray[0]);
    if (am_influ_event_any_of < sizeofArray)
        return (char *)am_influ_event_any_ofArray[am_influ_event_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_am_influ_event_any_of_e OpenAPI_am_influ_event_any_of_FromString(char* am_influ_event_any_of)
{
    int stringToReturn = 0;
    const char *am_influ_event_any_ofArray[] =  { "NULL", "SERVICE_AREA_COVRG_OUTCOME" };
    size_t sizeofArray = sizeof(am_influ_event_any_ofArray) / sizeof(am_influ_event_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(am_influ_event_any_of, am_influ_event_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

