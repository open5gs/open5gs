
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_network_id.h"

char* OpenAPI_access_network_id_ToString(OpenAPI_access_network_id_e access_network_id)
{
    const char *access_network_idArray[] =  { "NULL", "HRPD", "WIMAX", "WLAN", "ETHERNET" };
    size_t sizeofArray = sizeof(access_network_idArray) / sizeof(access_network_idArray[0]);
    if (access_network_id < sizeofArray)
        return (char *)access_network_idArray[access_network_id];
    else
        return (char *)"Unknown";
}

OpenAPI_access_network_id_e OpenAPI_access_network_id_FromString(char* access_network_id)
{
    int stringToReturn = 0;
    const char *access_network_idArray[] =  { "NULL", "HRPD", "WIMAX", "WLAN", "ETHERNET" };
    size_t sizeofArray = sizeof(access_network_idArray) / sizeof(access_network_idArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_network_id, access_network_idArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

