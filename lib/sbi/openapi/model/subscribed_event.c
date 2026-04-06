
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscribed_event.h"

char* OpenAPI_subscribed_event_ToString(OpenAPI_subscribed_event_e subscribed_event)
{
    const char *subscribed_eventArray[] =  { "NULL", "UP_PATH_CHANGE", "TRAFF_ROUTE_REQ_OUTCOME", "SIM_CONN_FAIL" };
    size_t sizeofArray = sizeof(subscribed_eventArray) / sizeof(subscribed_eventArray[0]);
    if (subscribed_event < sizeofArray)
        return (char *)subscribed_eventArray[subscribed_event];
    else
        return (char *)"Unknown";
}

OpenAPI_subscribed_event_e OpenAPI_subscribed_event_FromString(char* subscribed_event)
{
    int stringToReturn = 0;
    const char *subscribed_eventArray[] =  { "NULL", "UP_PATH_CHANGE", "TRAFF_ROUTE_REQ_OUTCOME", "SIM_CONN_FAIL" };
    size_t sizeofArray = sizeof(subscribed_eventArray) / sizeof(subscribed_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(subscribed_event, subscribed_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

