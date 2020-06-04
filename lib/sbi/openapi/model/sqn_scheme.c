
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqn_scheme.h"

char* OpenAPI_sqn_scheme_ToString(OpenAPI_sqn_scheme_e sqn_scheme)
{
    const char *sqn_schemeArray[] =  { "NULL", "GENERAL", "NON_TIME_BASED", "TIME_BASED" };
    size_t sizeofArray = sizeof(sqn_schemeArray) / sizeof(sqn_schemeArray[0]);
    if (sqn_scheme < sizeofArray)
        return (char *)sqn_schemeArray[sqn_scheme];
    else
        return (char *)"Unknown";
}

OpenAPI_sqn_scheme_e OpenAPI_sqn_scheme_FromString(char* sqn_scheme)
{
    int stringToReturn = 0;
    const char *sqn_schemeArray[] =  { "NULL", "GENERAL", "NON_TIME_BASED", "TIME_BASED" };
    size_t sizeofArray = sizeof(sqn_schemeArray) / sizeof(sqn_schemeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sqn_scheme, sqn_schemeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

