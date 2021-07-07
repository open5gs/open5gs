
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_control_indication.h"

char* OpenAPI_notification_control_indication_ToString(OpenAPI_notification_control_indication_e notification_control_indication)
{
    const char *notification_control_indicationArray[] =  { "NULL", "DDN_FAILURE", "DDD_STATUS" };
    size_t sizeofArray = sizeof(notification_control_indicationArray) / sizeof(notification_control_indicationArray[0]);
    if (notification_control_indication < sizeofArray)
        return (char *)notification_control_indicationArray[notification_control_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_notification_control_indication_e OpenAPI_notification_control_indication_FromString(char* notification_control_indication)
{
    int stringToReturn = 0;
    const char *notification_control_indicationArray[] =  { "NULL", "DDN_FAILURE", "DDD_STATUS" };
    size_t sizeofArray = sizeof(notification_control_indicationArray) / sizeof(notification_control_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notification_control_indication, notification_control_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

