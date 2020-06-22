
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_cause.h"

char* OpenAPI_notification_cause_ToString(OpenAPI_notification_cause_e notification_cause)
{
    const char *notification_causeArray[] =  { "NULL", "QOS_FULFILLED", "QOS_NOT_FULFILLED", "UP_SEC_FULFILLED", "UP_SEC_NOT_FULFILLED" };
    size_t sizeofArray = sizeof(notification_causeArray) / sizeof(notification_causeArray[0]);
    if (notification_cause < sizeofArray)
        return (char *)notification_causeArray[notification_cause];
    else
        return (char *)"Unknown";
}

OpenAPI_notification_cause_e OpenAPI_notification_cause_FromString(char* notification_cause)
{
    int stringToReturn = 0;
    const char *notification_causeArray[] =  { "NULL", "QOS_FULFILLED", "QOS_NOT_FULFILLED", "UP_SEC_FULFILLED", "UP_SEC_NOT_FULFILLED" };
    size_t sizeofArray = sizeof(notification_causeArray) / sizeof(notification_causeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notification_cause, notification_causeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

