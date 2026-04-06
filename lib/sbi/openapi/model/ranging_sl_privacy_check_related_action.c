
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_privacy_check_related_action.h"

char* OpenAPI_ranging_sl_privacy_check_related_action_ToString(OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action)
{
    const char *ranging_sl_privacy_check_related_actionArray[] =  { "NULL", "RANGINGSL_NOT_ALLOWED", "RANGINGSL_ALLOWED_WITH_NOTIFICATION", "RANGINGSL_ALLOWED_WITHOUT_NOTIFICATION", "RANGINGSL_ALLOWED_WITHOUT_RESPONSE", "RANGINGSL_RESTRICTED_WITHOUT_RESPONSE" };
    size_t sizeofArray = sizeof(ranging_sl_privacy_check_related_actionArray) / sizeof(ranging_sl_privacy_check_related_actionArray[0]);
    if (ranging_sl_privacy_check_related_action < sizeofArray)
        return (char *)ranging_sl_privacy_check_related_actionArray[ranging_sl_privacy_check_related_action];
    else
        return (char *)"Unknown";
}

OpenAPI_ranging_sl_privacy_check_related_action_e OpenAPI_ranging_sl_privacy_check_related_action_FromString(char* ranging_sl_privacy_check_related_action)
{
    int stringToReturn = 0;
    const char *ranging_sl_privacy_check_related_actionArray[] =  { "NULL", "RANGINGSL_NOT_ALLOWED", "RANGINGSL_ALLOWED_WITH_NOTIFICATION", "RANGINGSL_ALLOWED_WITHOUT_NOTIFICATION", "RANGINGSL_ALLOWED_WITHOUT_RESPONSE", "RANGINGSL_RESTRICTED_WITHOUT_RESPONSE" };
    size_t sizeofArray = sizeof(ranging_sl_privacy_check_related_actionArray) / sizeof(ranging_sl_privacy_check_related_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ranging_sl_privacy_check_related_action, ranging_sl_privacy_check_related_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

