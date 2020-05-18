
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uri_scheme.h"

char* OpenAPI_uri_scheme_ToString(OpenAPI_uri_scheme_e uri_scheme)
{
    const char *uri_schemeArray[] =  { "NULL", "http", "https" };
    size_t sizeofArray = sizeof(uri_schemeArray) / sizeof(uri_schemeArray[0]);
    if (uri_scheme < sizeofArray)
        return (char *)uri_schemeArray[uri_scheme];
    else
        return (char *)"Unknown";
}

OpenAPI_uri_scheme_e OpenAPI_uri_scheme_FromString(char* uri_scheme)
{
    int stringToReturn = 0;
    const char *uri_schemeArray[] =  { "NULL", "http", "https" };
    size_t sizeofArray = sizeof(uri_schemeArray) / sizeof(uri_schemeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(uri_scheme, uri_schemeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

