
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_notify_data_type.h"

char* OpenAPI_event_notify_data_type_ToString(OpenAPI_event_notify_data_type_e event_notify_data_type)
{
    const char *event_notify_data_typeArray[] =  { "NULL", "UE_AVAILABLE", "PERIODIC", "ENTERING_INTO_AREA", "LEAVING_FROM_AREA", "BEING_INSIDE_AREA", "MOTION", "MAXIMUM_INTERVAL_EXPIRATION_EVENT", "LOCATION_CANCELLATION_EVENT", "ACTIVATION_OF_DEFERRED_LOCATION", "UE_MOBILITY_FOR_DEFERRED_LOCATION", "5GC_MT_LR", "DIRECT_REPORT_EVENT", "CUMULATIVE_EVENT_REPORT" };
    size_t sizeofArray = sizeof(event_notify_data_typeArray) / sizeof(event_notify_data_typeArray[0]);
    if (event_notify_data_type < sizeofArray)
        return (char *)event_notify_data_typeArray[event_notify_data_type];
    else
        return (char *)"Unknown";
}

OpenAPI_event_notify_data_type_e OpenAPI_event_notify_data_type_FromString(char* event_notify_data_type)
{
    int stringToReturn = 0;
    const char *event_notify_data_typeArray[] =  { "NULL", "UE_AVAILABLE", "PERIODIC", "ENTERING_INTO_AREA", "LEAVING_FROM_AREA", "BEING_INSIDE_AREA", "MOTION", "MAXIMUM_INTERVAL_EXPIRATION_EVENT", "LOCATION_CANCELLATION_EVENT", "ACTIVATION_OF_DEFERRED_LOCATION", "UE_MOBILITY_FOR_DEFERRED_LOCATION", "5GC_MT_LR", "DIRECT_REPORT_EVENT", "CUMULATIVE_EVENT_REPORT" };
    size_t sizeofArray = sizeof(event_notify_data_typeArray) / sizeof(event_notify_data_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(event_notify_data_type, event_notify_data_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

