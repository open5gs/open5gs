
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "http_method_any_of.h"

char* OpenAPI_http_method_any_of_ToString(OpenAPI_http_method_any_of_e http_method_any_of)
{
    const char *http_method_any_ofArray[] =  { "NULL", "GET", "PUT", "POST", "DELETE", "PATCH", "HEAD", "OPTIONS", "CONNECT", "TRACE" };
    size_t sizeofArray = sizeof(http_method_any_ofArray) / sizeof(http_method_any_ofArray[0]);
    if (http_method_any_of < sizeofArray)
        return (char *)http_method_any_ofArray[http_method_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_http_method_any_of_e OpenAPI_http_method_any_of_FromString(char* http_method_any_of)
{
    int stringToReturn = 0;
    const char *http_method_any_ofArray[] =  { "NULL", "GET", "PUT", "POST", "DELETE", "PATCH", "HEAD", "OPTIONS", "CONNECT", "TRACE" };
    size_t sizeofArray = sizeof(http_method_any_ofArray) / sizeof(http_method_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(http_method_any_of, http_method_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

