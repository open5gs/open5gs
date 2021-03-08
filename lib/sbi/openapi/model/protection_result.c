
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "protection_result.h"

char* OpenAPI_protection_result_ToString(OpenAPI_protection_result_e protection_result)
{
    const char *protection_resultArray[] =  { "NULL", "PERFORMED", "NOT_PERFORMED" };
    size_t sizeofArray = sizeof(protection_resultArray) / sizeof(protection_resultArray[0]);
    if (protection_result < sizeofArray)
        return (char *)protection_resultArray[protection_result];
    else
        return (char *)"Unknown";
}

OpenAPI_protection_result_e OpenAPI_protection_result_FromString(char* protection_result)
{
    int stringToReturn = 0;
    const char *protection_resultArray[] =  { "NULL", "PERFORMED", "NOT_PERFORMED" };
    size_t sizeofArray = sizeof(protection_resultArray) / sizeof(protection_resultArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(protection_result, protection_resultArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

