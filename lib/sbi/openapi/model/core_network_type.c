
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "core_network_type.h"

char* OpenAPI_core_network_type_ToString(OpenAPI_core_network_type_e core_network_type)
{
    const char *core_network_typeArray[] =  { "NULL", "5GC", "EPC" };
    size_t sizeofArray = sizeof(core_network_typeArray) / sizeof(core_network_typeArray[0]);
    if (core_network_type < sizeofArray)
        return (char *)core_network_typeArray[core_network_type];
    else
        return (char *)"Unknown";
}

OpenAPI_core_network_type_e OpenAPI_core_network_type_FromString(char* core_network_type)
{
    int stringToReturn = 0;
    const char *core_network_typeArray[] =  { "NULL", "5GC", "EPC" };
    size_t sizeofArray = sizeof(core_network_typeArray) / sizeof(core_network_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(core_network_type, core_network_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

