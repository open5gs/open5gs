
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "privacy_check_related_action.h"

char* OpenAPI_privacy_check_related_action_ToString(OpenAPI_privacy_check_related_action_e privacy_check_related_action)
{
    const char *privacy_check_related_actionArray[] =  { "NULL", "LOCATION_NOT_ALLOWED", "LOCATION_ALLOWED_WITH_NOTIFICATION", "LOCATION_ALLOWED_WITHOUT_NOTIFICATION", "LOCATION_ALLOWED_WITHOUT_RESPONSE", "LOCATION_RESTRICTED_WITHOUT_RESPONSE" };
    size_t sizeofArray = sizeof(privacy_check_related_actionArray) / sizeof(privacy_check_related_actionArray[0]);
    if (privacy_check_related_action < sizeofArray)
        return (char *)privacy_check_related_actionArray[privacy_check_related_action];
    else
        return (char *)"Unknown";
}

OpenAPI_privacy_check_related_action_e OpenAPI_privacy_check_related_action_FromString(char* privacy_check_related_action)
{
    int stringToReturn = 0;
    const char *privacy_check_related_actionArray[] =  { "NULL", "LOCATION_NOT_ALLOWED", "LOCATION_ALLOWED_WITH_NOTIFICATION", "LOCATION_ALLOWED_WITHOUT_NOTIFICATION", "LOCATION_ALLOWED_WITHOUT_RESPONSE", "LOCATION_RESTRICTED_WITHOUT_RESPONSE" };
    size_t sizeofArray = sizeof(privacy_check_related_actionArray) / sizeof(privacy_check_related_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(privacy_check_related_action, privacy_check_related_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

