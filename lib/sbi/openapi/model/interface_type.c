
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "interface_type.h"

char* OpenAPI_interface_type_ToString(OpenAPI_interface_type_e interface_type)
{
    const char *interface_typeArray[] =  { "NULL", "UNKNOWN", "MOBILE_ORIGINATING", "MOBILE_TERMINATING", "APPLICATION_ORIGINATING", "APPLICATION_TERMINATING" };
    size_t sizeofArray = sizeof(interface_typeArray) / sizeof(interface_typeArray[0]);
    if (interface_type < sizeofArray)
        return (char *)interface_typeArray[interface_type];
    else
        return (char *)"Unknown";
}

OpenAPI_interface_type_e OpenAPI_interface_type_FromString(char* interface_type)
{
    int stringToReturn = 0;
    const char *interface_typeArray[] =  { "NULL", "UNKNOWN", "MOBILE_ORIGINATING", "MOBILE_TERMINATING", "APPLICATION_ORIGINATING", "APPLICATION_TERMINATING" };
    size_t sizeofArray = sizeof(interface_typeArray) / sizeof(interface_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(interface_type, interface_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

