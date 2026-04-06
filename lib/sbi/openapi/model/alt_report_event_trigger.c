
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alt_report_event_trigger.h"

char* OpenAPI_alt_report_event_trigger_ToString(OpenAPI_alt_report_event_trigger_e alt_report_event_trigger)
{
    const char *alt_report_event_triggerArray[] =  { "NULL", "TA_CHANGE", "RAN_NODE" };
    size_t sizeofArray = sizeof(alt_report_event_triggerArray) / sizeof(alt_report_event_triggerArray[0]);
    if (alt_report_event_trigger < sizeofArray)
        return (char *)alt_report_event_triggerArray[alt_report_event_trigger];
    else
        return (char *)"Unknown";
}

OpenAPI_alt_report_event_trigger_e OpenAPI_alt_report_event_trigger_FromString(char* alt_report_event_trigger)
{
    int stringToReturn = 0;
    const char *alt_report_event_triggerArray[] =  { "NULL", "TA_CHANGE", "RAN_NODE" };
    size_t sizeofArray = sizeof(alt_report_event_triggerArray) / sizeof(alt_report_event_triggerArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(alt_report_event_trigger, alt_report_event_triggerArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

