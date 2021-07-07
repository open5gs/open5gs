
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_trigger_any_of.h"

char* OpenAPI_amf_event_trigger_any_of_ToString(OpenAPI_amf_event_trigger_any_of_e amf_event_trigger_any_of)
{
    const char *amf_event_trigger_any_ofArray[] =  { "NULL", "ONE_TIME", "CONTINUOUS", "PERIODIC" };
    size_t sizeofArray = sizeof(amf_event_trigger_any_ofArray) / sizeof(amf_event_trigger_any_ofArray[0]);
    if (amf_event_trigger_any_of < sizeofArray)
        return (char *)amf_event_trigger_any_ofArray[amf_event_trigger_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_amf_event_trigger_any_of_e OpenAPI_amf_event_trigger_any_of_FromString(char* amf_event_trigger_any_of)
{
    int stringToReturn = 0;
    const char *amf_event_trigger_any_ofArray[] =  { "NULL", "ONE_TIME", "CONTINUOUS", "PERIODIC" };
    size_t sizeofArray = sizeof(amf_event_trigger_any_ofArray) / sizeof(amf_event_trigger_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(amf_event_trigger_any_of, amf_event_trigger_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

