
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_pos_allowed.h"

char* OpenAPI_ranging_sl_pos_allowed_ToString(OpenAPI_ranging_sl_pos_allowed_e ranging_sl_pos_allowed)
{
    const char *ranging_sl_pos_allowedArray[] =  { "NULL", "PC5", "ANNOUNCE", "MONITOR", "DISCOVERER", "DISCOVEREE", "LOCATED_UE", "SL_SERVER_UE", "SL_CLIENT_UE" };
    size_t sizeofArray = sizeof(ranging_sl_pos_allowedArray) / sizeof(ranging_sl_pos_allowedArray[0]);
    if (ranging_sl_pos_allowed < sizeofArray)
        return (char *)ranging_sl_pos_allowedArray[ranging_sl_pos_allowed];
    else
        return (char *)"Unknown";
}

OpenAPI_ranging_sl_pos_allowed_e OpenAPI_ranging_sl_pos_allowed_FromString(char* ranging_sl_pos_allowed)
{
    int stringToReturn = 0;
    const char *ranging_sl_pos_allowedArray[] =  { "NULL", "PC5", "ANNOUNCE", "MONITOR", "DISCOVERER", "DISCOVEREE", "LOCATED_UE", "SL_SERVER_UE", "SL_CLIENT_UE" };
    size_t sizeofArray = sizeof(ranging_sl_pos_allowedArray) / sizeof(ranging_sl_pos_allowedArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ranging_sl_pos_allowed, ranging_sl_pos_allowedArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

