
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "node_type.h"

char* OpenAPI_node_type_ToString(OpenAPI_node_type_e node_type)
{
    const char *node_typeArray[] =  { "NULL", "AUSF", "VLR", "SGSN", "S_CSCF", "BSF", "GAN_AAA_SERVER", "WLAN_AAA_SERVER", "MME" };
    size_t sizeofArray = sizeof(node_typeArray) / sizeof(node_typeArray[0]);
    if (node_type < sizeofArray)
        return (char *)node_typeArray[node_type];
    else
        return (char *)"Unknown";
}

OpenAPI_node_type_e OpenAPI_node_type_FromString(char* node_type)
{
    int stringToReturn = 0;
    const char *node_typeArray[] =  { "NULL", "AUSF", "VLR", "SGSN", "S_CSCF", "BSF", "GAN_AAA_SERVER", "WLAN_AAA_SERVER", "MME" };
    size_t sizeofArray = sizeof(node_typeArray) / sizeof(node_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(node_type, node_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

