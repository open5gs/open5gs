
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_method_any_of.h"

char* OpenAPI_notification_method_any_of_ToString(OpenAPI_notification_method_any_of_e notification_method_any_of)
{
    const char *notification_method_any_ofArray[] =  { "NULL", "PERIODIC", "THRESHOLD" };
    size_t sizeofArray = sizeof(notification_method_any_ofArray) / sizeof(notification_method_any_ofArray[0]);
    if (notification_method_any_of < sizeofArray)
        return (char *)notification_method_any_ofArray[notification_method_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_notification_method_any_of_e OpenAPI_notification_method_any_of_FromString(char* notification_method_any_of)
{
    int stringToReturn = 0;
    const char *notification_method_any_ofArray[] =  { "NULL", "PERIODIC", "THRESHOLD" };
    size_t sizeofArray = sizeof(notification_method_any_ofArray) / sizeof(notification_method_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notification_method_any_of, notification_method_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

