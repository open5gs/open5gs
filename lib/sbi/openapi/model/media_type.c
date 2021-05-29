
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_type.h"

char* OpenAPI_media_type_ToString(OpenAPI_media_type_e media_type)
{
    const char *media_typeArray[] =  { "NULL", "AUDIO", "VIDEO", "DATA", "APPLICATION", "CONTROL", "TEXT", "MESSAGE", "OTHER" };
    size_t sizeofArray = sizeof(media_typeArray) / sizeof(media_typeArray[0]);
    if (media_type < sizeofArray)
        return (char *)media_typeArray[media_type];
    else
        return (char *)"Unknown";
}

OpenAPI_media_type_e OpenAPI_media_type_FromString(char* media_type)
{
    int stringToReturn = 0;
    const char *media_typeArray[] =  { "NULL", "AUDIO", "VIDEO", "DATA", "APPLICATION", "CONTROL", "TEXT", "MESSAGE", "OTHER" };
    size_t sizeofArray = sizeof(media_typeArray) / sizeof(media_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(media_type, media_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

