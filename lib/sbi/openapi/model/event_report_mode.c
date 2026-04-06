
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_report_mode.h"

char* OpenAPI_event_report_mode_ToString(OpenAPI_event_report_mode_e event_report_mode)
{
    const char *event_report_modeArray[] =  { "NULL", "PERIODIC", "ON_EVENT_DETECTION" };
    size_t sizeofArray = sizeof(event_report_modeArray) / sizeof(event_report_modeArray[0]);
    if (event_report_mode < sizeofArray)
        return (char *)event_report_modeArray[event_report_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_event_report_mode_e OpenAPI_event_report_mode_FromString(char* event_report_mode)
{
    int stringToReturn = 0;
    const char *event_report_modeArray[] =  { "NULL", "PERIODIC", "ON_EVENT_DETECTION" };
    size_t sizeofArray = sizeof(event_report_modeArray) / sizeof(event_report_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_report_mode, event_report_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

