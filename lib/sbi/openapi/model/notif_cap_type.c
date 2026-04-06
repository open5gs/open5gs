
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notif_cap_type.h"

char* OpenAPI_notif_cap_type_ToString(OpenAPI_notif_cap_type_e notif_cap_type)
{
    const char *notif_cap_typeArray[] =  { "NULL", "PACKET_DELAY", "CONGESTION", "AVAILABLE_BITRATE" };
    size_t sizeofArray = sizeof(notif_cap_typeArray) / sizeof(notif_cap_typeArray[0]);
    if (notif_cap_type < sizeofArray)
        return (char *)notif_cap_typeArray[notif_cap_type];
    else
        return (char *)"Unknown";
}

OpenAPI_notif_cap_type_e OpenAPI_notif_cap_type_FromString(char* notif_cap_type)
{
    int stringToReturn = 0;
    const char *notif_cap_typeArray[] =  { "NULL", "PACKET_DELAY", "CONGESTION", "AVAILABLE_BITRATE" };
    size_t sizeofArray = sizeof(notif_cap_typeArray) / sizeof(notif_cap_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(notif_cap_type, notif_cap_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

