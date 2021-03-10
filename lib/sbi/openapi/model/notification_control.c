
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_control.h"

char* OpenAPI_notification_control_ToString(OpenAPI_notification_control_e notification_control)
{
    const char *notification_controlArray[] =  { "NULL", "REQUESTED", "NOT_REQUESTED" };
    size_t sizeofArray = sizeof(notification_controlArray) / sizeof(notification_controlArray[0]);
    if (notification_control < sizeofArray)
        return (char *)notification_controlArray[notification_control];
    else
        return (char *)"Unknown";
}

OpenAPI_notification_control_e OpenAPI_notification_control_FromString(char* notification_control)
{
    int stringToReturn = 0;
    const char *notification_controlArray[] =  { "NULL", "REQUESTED", "NOT_REQUESTED" };
    size_t sizeofArray = sizeof(notification_controlArray) / sizeof(notification_controlArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notification_control, notification_controlArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

