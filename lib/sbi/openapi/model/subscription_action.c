
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_action.h"

char* OpenAPI_subscription_action_ToString(OpenAPI_subscription_action_e subscription_action)
{
    const char *subscription_actionArray[] =  { "NULL", "CLOSE", "CONTINUE_WITH_MUTING", "CONTINUE_WITHOUT_MUTING" };
    size_t sizeofArray = sizeof(subscription_actionArray) / sizeof(subscription_actionArray[0]);
    if (subscription_action < sizeofArray)
        return (char *)subscription_actionArray[subscription_action];
    else
        return (char *)"Unknown";
}

OpenAPI_subscription_action_e OpenAPI_subscription_action_FromString(char* subscription_action)
{
    int stringToReturn = 0;
    const char *subscription_actionArray[] =  { "NULL", "CLOSE", "CONTINUE_WITH_MUTING", "CONTINUE_WITHOUT_MUTING" };
    size_t sizeofArray = sizeof(subscription_actionArray) / sizeof(subscription_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(subscription_action, subscription_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

