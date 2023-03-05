
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "security_capability_any_of.h"

char* OpenAPI_security_capability_any_of_ToString(OpenAPI_security_capability_any_of_e security_capability_any_of)
{
    const char *security_capability_any_ofArray[] =  { "NULL", "TLS", "PRINS", "NONE" };
    size_t sizeofArray = sizeof(security_capability_any_ofArray) / sizeof(security_capability_any_ofArray[0]);
    if (security_capability_any_of < sizeofArray)
        return (char *)security_capability_any_ofArray[security_capability_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_security_capability_any_of_e OpenAPI_security_capability_any_of_FromString(char* security_capability_any_of)
{
    int stringToReturn = 0;
    const char *security_capability_any_ofArray[] =  { "NULL", "TLS", "PRINS", "NONE" };
    size_t sizeofArray = sizeof(security_capability_any_ofArray) / sizeof(security_capability_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(security_capability_any_of, security_capability_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

