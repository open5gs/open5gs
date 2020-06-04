
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sign.h"

char* OpenAPI_sign_ToString(OpenAPI_sign_e sign)
{
    const char *signArray[] =  { "NULL", "POSITIVE", "NEGATIVE" };
    size_t sizeofArray = sizeof(signArray) / sizeof(signArray[0]);
    if (sign < sizeofArray)
        return (char *)signArray[sign];
    else
        return (char *)"Unknown";
}

OpenAPI_sign_e OpenAPI_sign_FromString(char* sign)
{
    int stringToReturn = 0;
    const char *signArray[] =  { "NULL", "POSITIVE", "NEGATIVE" };
    size_t sizeofArray = sizeof(signArray) / sizeof(signArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sign, signArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

