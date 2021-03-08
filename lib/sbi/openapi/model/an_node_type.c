
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "an_node_type.h"

char* OpenAPI_an_node_type_ToString(OpenAPI_an_node_type_e an_node_type)
{
    const char *an_node_typeArray[] =  { "NULL", "GNB", "NG_ENB" };
    size_t sizeofArray = sizeof(an_node_typeArray) / sizeof(an_node_typeArray[0]);
    if (an_node_type < sizeofArray)
        return (char *)an_node_typeArray[an_node_type];
    else
        return (char *)"Unknown";
}

OpenAPI_an_node_type_e OpenAPI_an_node_type_FromString(char* an_node_type)
{
    int stringToReturn = 0;
    const char *an_node_typeArray[] =  { "NULL", "GNB", "NG_ENB" };
    size_t sizeofArray = sizeof(an_node_typeArray) / sizeof(an_node_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(an_node_type, an_node_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

