
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "support.h"

char* OpenAPI_support_ToString(OpenAPI_support_e support)
{
    const char *supportArray[] =  { "NULL", "NOT_SUPPORTED", "SUPPORTED" };
    size_t sizeofArray = sizeof(supportArray) / sizeof(supportArray[0]);
    if (support < sizeofArray)
        return (char *)supportArray[support];
    else
        return (char *)"Unknown";
}

OpenAPI_support_e OpenAPI_support_FromString(char* support)
{
    int stringToReturn = 0;
    const char *supportArray[] =  { "NULL", "NOT_SUPPORTED", "SUPPORTED" };
    size_t sizeofArray = sizeof(supportArray) / sizeof(supportArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(support, supportArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

