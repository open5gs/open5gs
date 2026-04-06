
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vlan_tag_type.h"

char* OpenAPI_vlan_tag_type_ToString(OpenAPI_vlan_tag_type_e vlan_tag_type)
{
    const char *vlan_tag_typeArray[] =  { "NULL", "C_VLAN_TAG", "S_VLAN_TAG" };
    size_t sizeofArray = sizeof(vlan_tag_typeArray) / sizeof(vlan_tag_typeArray[0]);
    if (vlan_tag_type < sizeofArray)
        return (char *)vlan_tag_typeArray[vlan_tag_type];
    else
        return (char *)"Unknown";
}

OpenAPI_vlan_tag_type_e OpenAPI_vlan_tag_type_FromString(char* vlan_tag_type)
{
    int stringToReturn = 0;
    const char *vlan_tag_typeArray[] =  { "NULL", "C_VLAN_TAG", "S_VLAN_TAG" };
    size_t sizeofArray = sizeof(vlan_tag_typeArray) / sizeof(vlan_tag_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(vlan_tag_type, vlan_tag_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

