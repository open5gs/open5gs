
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reported_event_type_any_of.h"

char* OpenAPI_reported_event_type_any_of_ToString(OpenAPI_reported_event_type_any_of_e reported_event_type_any_of)
{
    const char *reported_event_type_any_ofArray[] =  { "NULL", "PERIODIC_EVENT", "ENTERING_AREA_EVENT", "LEAVING_AREA_EVENT", "BEING_INSIDE_AREA_EVENT", "MOTION_EVENT", "MAXIMUM_INTERVAL_EXPIRATION_EVENT", "LOCATION_CANCELLATION_EVENT" };
    size_t sizeofArray = sizeof(reported_event_type_any_ofArray) / sizeof(reported_event_type_any_ofArray[0]);
    if (reported_event_type_any_of < sizeofArray)
        return (char *)reported_event_type_any_ofArray[reported_event_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_reported_event_type_any_of_e OpenAPI_reported_event_type_any_of_FromString(char* reported_event_type_any_of)
{
    int stringToReturn = 0;
    const char *reported_event_type_any_ofArray[] =  { "NULL", "PERIODIC_EVENT", "ENTERING_AREA_EVENT", "LEAVING_AREA_EVENT", "BEING_INSIDE_AREA_EVENT", "MOTION_EVENT", "MAXIMUM_INTERVAL_EXPIRATION_EVENT", "LOCATION_CANCELLATION_EVENT" };
    size_t sizeofArray = sizeof(reported_event_type_any_ofArray) / sizeof(reported_event_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reported_event_type_any_of, reported_event_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

