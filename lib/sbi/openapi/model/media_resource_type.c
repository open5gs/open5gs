
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_resource_type.h"

char* OpenAPI_media_resource_type_ToString(OpenAPI_media_resource_type_e media_resource_type)
{
    const char *media_resource_typeArray[] =  { "NULL", "DC", "AUDIO", "VIDEO" };
    size_t sizeofArray = sizeof(media_resource_typeArray) / sizeof(media_resource_typeArray[0]);
    if (media_resource_type < sizeofArray)
        return (char *)media_resource_typeArray[media_resource_type];
    else
        return (char *)"Unknown";
}

OpenAPI_media_resource_type_e OpenAPI_media_resource_type_FromString(char* media_resource_type)
{
    int stringToReturn = 0;
    const char *media_resource_typeArray[] =  { "NULL", "DC", "AUDIO", "VIDEO" };
    size_t sizeofArray = sizeof(media_resource_typeArray) / sizeof(media_resource_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(media_resource_type, media_resource_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

