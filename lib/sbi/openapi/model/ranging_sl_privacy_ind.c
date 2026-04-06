
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_privacy_ind.h"

char* OpenAPI_ranging_sl_privacy_ind_ToString(OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_ind)
{
    const char *ranging_sl_privacy_indArray[] =  { "NULL", "RANGINGSL_DISALLOWED", "RANGINGSL_ALLOWED" };
    size_t sizeofArray = sizeof(ranging_sl_privacy_indArray) / sizeof(ranging_sl_privacy_indArray[0]);
    if (ranging_sl_privacy_ind < sizeofArray)
        return (char *)ranging_sl_privacy_indArray[ranging_sl_privacy_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_ranging_sl_privacy_ind_e OpenAPI_ranging_sl_privacy_ind_FromString(char* ranging_sl_privacy_ind)
{
    int stringToReturn = 0;
    const char *ranging_sl_privacy_indArray[] =  { "NULL", "RANGINGSL_DISALLOWED", "RANGINGSL_ALLOWED" };
    size_t sizeofArray = sizeof(ranging_sl_privacy_indArray) / sizeof(ranging_sl_privacy_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ranging_sl_privacy_ind, ranging_sl_privacy_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

