
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "http_method.h"

char* OpenAPI_http_method_ToString(OpenAPI_http_method_e http_method)
{
    const char *http_methodArray[] =  { "NULL", "GET", "PUT", "POST", "DELETE", "PATCH", "HEAD", "OPTIONS", "CONNECT", "TRACE" };
    size_t sizeofArray = sizeof(http_methodArray) / sizeof(http_methodArray[0]);
    if (http_method < sizeofArray)
        return (char *)http_methodArray[http_method];
    else
        return (char *)"Unknown";
}

OpenAPI_http_method_e OpenAPI_http_method_FromString(char* http_method)
{
    int stringToReturn = 0;
    const char *http_methodArray[] =  { "NULL", "GET", "PUT", "POST", "DELETE", "PATCH", "HEAD", "OPTIONS", "CONNECT", "TRACE" };
    size_t sizeofArray = sizeof(http_methodArray) / sizeof(http_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(http_method, http_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

