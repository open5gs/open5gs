
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sac_event_trigger.h"

char* OpenAPI_sac_event_trigger_ToString(OpenAPI_sac_event_trigger_e sac_event_trigger)
{
    const char *sac_event_triggerArray[] =  { "NULL", "THRESHOLD", "PERIODIC" };
    size_t sizeofArray = sizeof(sac_event_triggerArray) / sizeof(sac_event_triggerArray[0]);
    if (sac_event_trigger < sizeofArray)
        return (char *)sac_event_triggerArray[sac_event_trigger];
    else
        return (char *)"Unknown";
}

OpenAPI_sac_event_trigger_e OpenAPI_sac_event_trigger_FromString(char* sac_event_trigger)
{
    int stringToReturn = 0;
    const char *sac_event_triggerArray[] =  { "NULL", "THRESHOLD", "PERIODIC" };
    size_t sizeofArray = sizeof(sac_event_triggerArray) / sizeof(sac_event_triggerArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sac_event_trigger, sac_event_triggerArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

