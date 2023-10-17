
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "security_capability.h"

char* OpenAPI_security_capability_ToString(OpenAPI_security_capability_e security_capability)
{
    const char *security_capabilityArray[] =  { "NULL", "TLS", "PRINS", "NONE" };
    size_t sizeofArray = sizeof(security_capabilityArray) / sizeof(security_capabilityArray[0]);
    if (security_capability < sizeofArray)
        return (char *)security_capabilityArray[security_capability];
    else
        return (char *)"Unknown";
}

OpenAPI_security_capability_e OpenAPI_security_capability_FromString(char* security_capability)
{
    int stringToReturn = 0;
    const char *security_capabilityArray[] =  { "NULL", "TLS", "PRINS", "NONE" };
    size_t sizeofArray = sizeof(security_capabilityArray) / sizeof(security_capabilityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(security_capability, security_capabilityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

