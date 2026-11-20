
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "role_of_ims_node.h"

char* OpenAPI_role_of_ims_node_ToString(OpenAPI_role_of_ims_node_e role_of_ims_node)
{
    const char *role_of_ims_nodeArray[] =  { "NULL", "ORIGINATING", "TERMINATING", "FORWARDING" };
    size_t sizeofArray = sizeof(role_of_ims_nodeArray) / sizeof(role_of_ims_nodeArray[0]);
    if (role_of_ims_node < sizeofArray)
        return (char *)role_of_ims_nodeArray[role_of_ims_node];
    else
        return (char *)"Unknown";
}

OpenAPI_role_of_ims_node_e OpenAPI_role_of_ims_node_FromString(char* role_of_ims_node)
{
    int stringToReturn = 0;
    const char *role_of_ims_nodeArray[] =  { "NULL", "ORIGINATING", "TERMINATING", "FORWARDING" };
    size_t sizeofArray = sizeof(role_of_ims_nodeArray) / sizeof(role_of_ims_nodeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(role_of_ims_node, role_of_ims_nodeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

