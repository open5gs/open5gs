
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_confidentiality.h"

char* OpenAPI_up_confidentiality_ToString(OpenAPI_up_confidentiality_e up_confidentiality)
{
    const char *up_confidentialityArray[] =  { "NULL", "REQUIRED", "PREFERRED", "NOT_NEEDED" };
    size_t sizeofArray = sizeof(up_confidentialityArray) / sizeof(up_confidentialityArray[0]);
    if (up_confidentiality < sizeofArray)
        return (char *)up_confidentialityArray[up_confidentiality];
    else
        return (char *)"Unknown";
}

OpenAPI_up_confidentiality_e OpenAPI_up_confidentiality_FromString(char* up_confidentiality)
{
    int stringToReturn = 0;
    const char *up_confidentialityArray[] =  { "NULL", "REQUIRED", "PREFERRED", "NOT_NEEDED" };
    size_t sizeofArray = sizeof(up_confidentialityArray) / sizeof(up_confidentialityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(up_confidentiality, up_confidentialityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

