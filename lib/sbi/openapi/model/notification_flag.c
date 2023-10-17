
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_flag.h"

char* OpenAPI_notification_flag_ToString(OpenAPI_notification_flag_e notification_flag)
{
    const char *notification_flagArray[] =  { "NULL", "ACTIVATE", "DEACTIVATE", "RETRIEVAL" };
    size_t sizeofArray = sizeof(notification_flagArray) / sizeof(notification_flagArray[0]);
    if (notification_flag < sizeofArray)
        return (char *)notification_flagArray[notification_flag];
    else
        return (char *)"Unknown";
}

OpenAPI_notification_flag_e OpenAPI_notification_flag_FromString(char* notification_flag)
{
    int stringToReturn = 0;
    const char *notification_flagArray[] =  { "NULL", "ACTIVATE", "DEACTIVATE", "RETRIEVAL" };
    size_t sizeofArray = sizeof(notification_flagArray) / sizeof(notification_flagArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notification_flag, notification_flagArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

