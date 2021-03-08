
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_trigger.h"

char* OpenAPI_reporting_trigger_ToString(OpenAPI_reporting_trigger_e reporting_trigger)
{
    const char *reporting_triggerArray[] =  { "NULL", "PERIODICAL", "EVENT_A2", "EVENT_A2_PERIODIC", "ALL_RRM_EVENT_TRIGGERS" };
    size_t sizeofArray = sizeof(reporting_triggerArray) / sizeof(reporting_triggerArray[0]);
    if (reporting_trigger < sizeofArray)
        return (char *)reporting_triggerArray[reporting_trigger];
    else
        return (char *)"Unknown";
}

OpenAPI_reporting_trigger_e OpenAPI_reporting_trigger_FromString(char* reporting_trigger)
{
    int stringToReturn = 0;
    const char *reporting_triggerArray[] =  { "NULL", "PERIODICAL", "EVENT_A2", "EVENT_A2_PERIODIC", "ALL_RRM_EVENT_TRIGGERS" };
    size_t sizeofArray = sizeof(reporting_triggerArray) / sizeof(reporting_triggerArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reporting_trigger, reporting_triggerArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

