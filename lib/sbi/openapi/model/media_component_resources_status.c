
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_component_resources_status.h"

char* OpenAPI_media_component_resources_status_ToString(OpenAPI_media_component_resources_status_e media_component_resources_status)
{
    const char *media_component_resources_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(media_component_resources_statusArray) / sizeof(media_component_resources_statusArray[0]);
    if (media_component_resources_status < sizeofArray)
        return (char *)media_component_resources_statusArray[media_component_resources_status];
    else
        return (char *)"Unknown";
}

OpenAPI_media_component_resources_status_e OpenAPI_media_component_resources_status_FromString(char* media_component_resources_status)
{
    int stringToReturn = 0;
    const char *media_component_resources_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(media_component_resources_statusArray) / sizeof(media_component_resources_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(media_component_resources_status, media_component_resources_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

