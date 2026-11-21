
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdp_type.h"

char* OpenAPI_sdp_type_ToString(OpenAPI_sdp_type_e sdp_type)
{
    const char *sdp_typeArray[] =  { "NULL", "OFFER", "ANSWER" };
    size_t sizeofArray = sizeof(sdp_typeArray) / sizeof(sdp_typeArray[0]);
    if (sdp_type < sizeofArray)
        return (char *)sdp_typeArray[sdp_type];
    else
        return (char *)"Unknown";
}

OpenAPI_sdp_type_e OpenAPI_sdp_type_FromString(char* sdp_type)
{
    int stringToReturn = 0;
    const char *sdp_typeArray[] =  { "NULL", "OFFER", "ANSWER" };
    size_t sizeofArray = sizeof(sdp_typeArray) / sizeof(sdp_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sdp_type, sdp_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

