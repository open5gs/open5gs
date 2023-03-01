
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_capability.h"

char* OpenAPI_scp_capability_ToString(OpenAPI_scp_capability_e scp_capability)
{
    const char *scp_capabilityArray[] =  { "NULL", "INDIRECT_COM_WITH_DELEG_DISC" };
    size_t sizeofArray = sizeof(scp_capabilityArray) / sizeof(scp_capabilityArray[0]);
    if (scp_capability < sizeofArray)
        return (char *)scp_capabilityArray[scp_capability];
    else
        return (char *)"Unknown";
}

OpenAPI_scp_capability_e OpenAPI_scp_capability_FromString(char* scp_capability)
{
    int stringToReturn = 0;
    const char *scp_capabilityArray[] =  { "NULL", "INDIRECT_COM_WITH_DELEG_DISC" };
    size_t sizeofArray = sizeof(scp_capabilityArray) / sizeof(scp_capabilityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(scp_capability, scp_capabilityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

