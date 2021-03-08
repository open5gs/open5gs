
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_integrity.h"

char* OpenAPI_up_integrity_ToString(OpenAPI_up_integrity_e up_integrity)
{
    const char *up_integrityArray[] =  { "NULL", "REQUIRED", "PREFERRED", "NOT_NEEDED" };
    size_t sizeofArray = sizeof(up_integrityArray) / sizeof(up_integrityArray[0]);
    if (up_integrity < sizeofArray)
        return (char *)up_integrityArray[up_integrity];
    else
        return (char *)"Unknown";
}

OpenAPI_up_integrity_e OpenAPI_up_integrity_FromString(char* up_integrity)
{
    int stringToReturn = 0;
    const char *up_integrityArray[] =  { "NULL", "REQUIRED", "PREFERRED", "NOT_NEEDED" };
    size_t sizeofArray = sizeof(up_integrityArray) / sizeof(up_integrityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(up_integrity, up_integrityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

