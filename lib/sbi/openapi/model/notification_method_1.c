
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_method_1.h"

char* OpenAPI_notification_method_1_ToString(OpenAPI_notification_method_1_e notification_method_1)
{
    const char *notification_method_1Array[] =  { "NULL", "PERIODIC", "ONE_TIME", "ON_EVENT_DETECTION" };
    size_t sizeofArray = sizeof(notification_method_1Array) / sizeof(notification_method_1Array[0]);
    if (notification_method_1 < sizeofArray)
        return (char *)notification_method_1Array[notification_method_1];
    else
        return (char *)"Unknown";
}

OpenAPI_notification_method_1_e OpenAPI_notification_method_1_FromString(char* notification_method_1)
{
    int stringToReturn = 0;
    const char *notification_method_1Array[] =  { "NULL", "PERIODIC", "ONE_TIME", "ON_EVENT_DETECTION" };
    size_t sizeofArray = sizeof(notification_method_1Array) / sizeof(notification_method_1Array[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notification_method_1, notification_method_1Array[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

