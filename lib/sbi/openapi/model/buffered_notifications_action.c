
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "buffered_notifications_action.h"

char* OpenAPI_buffered_notifications_action_ToString(OpenAPI_buffered_notifications_action_e buffered_notifications_action)
{
    const char *buffered_notifications_actionArray[] =  { "NULL", "SEND_ALL", "DISCARD_ALL", "DROP_OLD" };
    size_t sizeofArray = sizeof(buffered_notifications_actionArray) / sizeof(buffered_notifications_actionArray[0]);
    if (buffered_notifications_action < sizeofArray)
        return (char *)buffered_notifications_actionArray[buffered_notifications_action];
    else
        return (char *)"Unknown";
}

OpenAPI_buffered_notifications_action_e OpenAPI_buffered_notifications_action_FromString(char* buffered_notifications_action)
{
    int stringToReturn = 0;
    const char *buffered_notifications_actionArray[] =  { "NULL", "SEND_ALL", "DISCARD_ALL", "DROP_OLD" };
    size_t sizeofArray = sizeof(buffered_notifications_actionArray) / sizeof(buffered_notifications_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(buffered_notifications_action, buffered_notifications_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

