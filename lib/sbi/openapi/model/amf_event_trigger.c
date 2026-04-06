
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_trigger.h"

char* OpenAPI_amf_event_trigger_ToString(OpenAPI_amf_event_trigger_e amf_event_trigger)
{
    const char *amf_event_triggerArray[] =  { "NULL", "ONE_TIME", "CONTINUOUS", "PERIODIC" };
    size_t sizeofArray = sizeof(amf_event_triggerArray) / sizeof(amf_event_triggerArray[0]);
    if (amf_event_trigger < sizeofArray)
        return (char *)amf_event_triggerArray[amf_event_trigger];
    else
        return (char *)"Unknown";
}

OpenAPI_amf_event_trigger_e OpenAPI_amf_event_trigger_FromString(char* amf_event_trigger)
{
    int stringToReturn = 0;
    const char *amf_event_triggerArray[] =  { "NULL", "ONE_TIME", "CONTINUOUS", "PERIODIC" };
    size_t sizeofArray = sizeof(amf_event_triggerArray) / sizeof(amf_event_triggerArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(amf_event_trigger, amf_event_triggerArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

