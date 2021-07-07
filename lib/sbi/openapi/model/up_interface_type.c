
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_interface_type.h"

char* OpenAPI_up_interface_type_ToString(OpenAPI_up_interface_type_e up_interface_type)
{
    const char *up_interface_typeArray[] =  { "NULL", "N3", "N6", "N9", "DATA_FORWARDING" };
    size_t sizeofArray = sizeof(up_interface_typeArray) / sizeof(up_interface_typeArray[0]);
    if (up_interface_type < sizeofArray)
        return (char *)up_interface_typeArray[up_interface_type];
    else
        return (char *)"Unknown";
}

OpenAPI_up_interface_type_e OpenAPI_up_interface_type_FromString(char* up_interface_type)
{
    int stringToReturn = 0;
    const char *up_interface_typeArray[] =  { "NULL", "N3", "N6", "N9", "DATA_FORWARDING" };
    size_t sizeofArray = sizeof(up_interface_typeArray) / sizeof(up_interface_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(up_interface_type, up_interface_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

