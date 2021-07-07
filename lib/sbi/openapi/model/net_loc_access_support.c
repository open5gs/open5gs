
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "net_loc_access_support.h"

char* OpenAPI_net_loc_access_support_ToString(OpenAPI_net_loc_access_support_e net_loc_access_support)
{
    const char *net_loc_access_supportArray[] =  { "NULL", "ANR_NOT_SUPPORTED", "TZR_NOT_SUPPORTED", "LOC_NOT_SUPPORTED" };
    size_t sizeofArray = sizeof(net_loc_access_supportArray) / sizeof(net_loc_access_supportArray[0]);
    if (net_loc_access_support < sizeofArray)
        return (char *)net_loc_access_supportArray[net_loc_access_support];
    else
        return (char *)"Unknown";
}

OpenAPI_net_loc_access_support_e OpenAPI_net_loc_access_support_FromString(char* net_loc_access_support)
{
    int stringToReturn = 0;
    const char *net_loc_access_supportArray[] =  { "NULL", "ANR_NOT_SUPPORTED", "TZR_NOT_SUPPORTED", "LOC_NOT_SUPPORTED" };
    size_t sizeofArray = sizeof(net_loc_access_supportArray) / sizeof(net_loc_access_supportArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(net_loc_access_support, net_loc_access_supportArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

