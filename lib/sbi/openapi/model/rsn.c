
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rsn.h"

char* OpenAPI_rsn_ToString(OpenAPI_rsn_e rsn)
{
    const char *rsnArray[] =  { "NULL", "V1", "V2", "NONE" };
    size_t sizeofArray = sizeof(rsnArray) / sizeof(rsnArray[0]);
    if (rsn < sizeofArray)
        return (char *)rsnArray[rsn];
    else
        return (char *)"Unknown";
}

OpenAPI_rsn_e OpenAPI_rsn_FromString(char* rsn)
{
    int stringToReturn = 0;
    const char *rsnArray[] =  { "NULL", "V1", "V2", "NONE" };
    size_t sizeofArray = sizeof(rsnArray) / sizeof(rsnArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rsn, rsnArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

