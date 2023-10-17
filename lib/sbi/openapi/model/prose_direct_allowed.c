
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prose_direct_allowed.h"

char* OpenAPI_prose_direct_allowed_ToString(OpenAPI_prose_direct_allowed_e prose_direct_allowed)
{
    const char *prose_direct_allowedArray[] =  { "NULL", "ANNOUNCE", "MONITOR", "RESTRICTD_ANNOUNCE", "RESTRICTD_MONITOR", "DISCOVERER", "DISCOVEREE", "BROADCAST", "GROUPCAST", "UNICAST", "LAYER2_RELAY", "LAYER3_RELAY" };
    size_t sizeofArray = sizeof(prose_direct_allowedArray) / sizeof(prose_direct_allowedArray[0]);
    if (prose_direct_allowed < sizeofArray)
        return (char *)prose_direct_allowedArray[prose_direct_allowed];
    else
        return (char *)"Unknown";
}

OpenAPI_prose_direct_allowed_e OpenAPI_prose_direct_allowed_FromString(char* prose_direct_allowed)
{
    int stringToReturn = 0;
    const char *prose_direct_allowedArray[] =  { "NULL", "ANNOUNCE", "MONITOR", "RESTRICTD_ANNOUNCE", "RESTRICTD_MONITOR", "DISCOVERER", "DISCOVEREE", "BROADCAST", "GROUPCAST", "UNICAST", "LAYER2_RELAY", "LAYER3_RELAY" };
    size_t sizeofArray = sizeof(prose_direct_allowedArray) / sizeof(prose_direct_allowedArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(prose_direct_allowed, prose_direct_allowedArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

