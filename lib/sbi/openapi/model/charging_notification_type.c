
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_notification_type.h"

char* OpenAPI_charging_notification_type_ToString(OpenAPI_charging_notification_type_e charging_notification_type)
{
    const char *charging_notification_typeArray[] =  { "NULL", "REAUTHORIZATION", "ABORT_CHARGING" };
    size_t sizeofArray = sizeof(charging_notification_typeArray) / sizeof(charging_notification_typeArray[0]);
    if (charging_notification_type < sizeofArray)
        return (char *)charging_notification_typeArray[charging_notification_type];
    else
        return (char *)"Unknown";
}

OpenAPI_charging_notification_type_e OpenAPI_charging_notification_type_FromString(char* charging_notification_type)
{
    int stringToReturn = 0;
    const char *charging_notification_typeArray[] =  { "NULL", "REAUTHORIZATION", "ABORT_CHARGING" };
    size_t sizeofArray = sizeof(charging_notification_typeArray) / sizeof(charging_notification_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(charging_notification_type, charging_notification_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

