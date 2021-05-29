
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_reason.h"

char* OpenAPI_registration_reason_ToString(OpenAPI_registration_reason_e registration_reason)
{
    const char *registration_reasonArray[] =  { "NULL", "SMF_CONTEXT_TRANSFERRED" };
    size_t sizeofArray = sizeof(registration_reasonArray) / sizeof(registration_reasonArray[0]);
    if (registration_reason < sizeofArray)
        return (char *)registration_reasonArray[registration_reason];
    else
        return (char *)"Unknown";
}

OpenAPI_registration_reason_e OpenAPI_registration_reason_FromString(char* registration_reason)
{
    int stringToReturn = 0;
    const char *registration_reasonArray[] =  { "NULL", "SMF_CONTEXT_TRANSFERRED" };
    size_t sizeofArray = sizeof(registration_reasonArray) / sizeof(registration_reasonArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(registration_reason, registration_reasonArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

