
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "l4s_notif_type.h"

char* OpenAPI_l4s_notif_type_ToString(OpenAPI_l4s_notif_type_e l4s_notif_type)
{
    const char *l4s_notif_typeArray[] =  { "NULL", "AVAILABLE", "NOT_AVAILABLE" };
    size_t sizeofArray = sizeof(l4s_notif_typeArray) / sizeof(l4s_notif_typeArray[0]);
    if (l4s_notif_type < sizeofArray)
        return (char *)l4s_notif_typeArray[l4s_notif_type];
    else
        return (char *)"Unknown";
}

OpenAPI_l4s_notif_type_e OpenAPI_l4s_notif_type_FromString(char* l4s_notif_type)
{
    int stringToReturn = 0;
    const char *l4s_notif_typeArray[] =  { "NULL", "AVAILABLE", "NOT_AVAILABLE" };
    size_t sizeofArray = sizeof(l4s_notif_typeArray) / sizeof(l4s_notif_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(l4s_notif_type, l4s_notif_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

