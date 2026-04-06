
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_method.h"

char* OpenAPI_notification_method_ToString(OpenAPI_notification_method_e notification_method)
{
    const char *notification_methodArray[] =  { "NULL", "PERIODIC", "THRESHOLD" };
    size_t sizeofArray = sizeof(notification_methodArray) / sizeof(notification_methodArray[0]);
    if (notification_method < sizeofArray)
        return (char *)notification_methodArray[notification_method];
    else
        return (char *)"Unknown";
}

OpenAPI_notification_method_e OpenAPI_notification_method_FromString(char* notification_method)
{
    int stringToReturn = 0;
    const char *notification_methodArray[] =  { "NULL", "PERIODIC", "THRESHOLD" };
    size_t sizeofArray = sizeof(notification_methodArray) / sizeof(notification_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notification_method, notification_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

