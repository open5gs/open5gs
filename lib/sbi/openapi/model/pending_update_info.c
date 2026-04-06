
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pending_update_info.h"

char* OpenAPI_pending_update_info_ToString(OpenAPI_pending_update_info_e pending_update_info)
{
    const char *pending_update_infoArray[] =  { "NULL", "UE_LOCATION", "TIMEZONE", "ACCESS_TYPE", "RAT_TYPE", "AMF_ID" };
    size_t sizeofArray = sizeof(pending_update_infoArray) / sizeof(pending_update_infoArray[0]);
    if (pending_update_info < sizeofArray)
        return (char *)pending_update_infoArray[pending_update_info];
    else
        return (char *)"Unknown";
}

OpenAPI_pending_update_info_e OpenAPI_pending_update_info_FromString(char* pending_update_info)
{
    int stringToReturn = 0;
    const char *pending_update_infoArray[] =  { "NULL", "UE_LOCATION", "TIMEZONE", "ACCESS_TYPE", "RAT_TYPE", "AMF_ID" };
    size_t sizeofArray = sizeof(pending_update_infoArray) / sizeof(pending_update_infoArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(pending_update_info, pending_update_infoArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

