
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception_trend_any_of.h"

char* OpenAPI_exception_trend_any_of_ToString(OpenAPI_exception_trend_any_of_e exception_trend_any_of)
{
    const char *exception_trend_any_ofArray[] =  { "NULL", "UP", "DOWN", "UNKNOW", "STABLE" };
    size_t sizeofArray = sizeof(exception_trend_any_ofArray) / sizeof(exception_trend_any_ofArray[0]);
    if (exception_trend_any_of < sizeofArray)
        return (char *)exception_trend_any_ofArray[exception_trend_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_exception_trend_any_of_e OpenAPI_exception_trend_any_of_FromString(char* exception_trend_any_of)
{
    int stringToReturn = 0;
    const char *exception_trend_any_ofArray[] =  { "NULL", "UP", "DOWN", "UNKNOW", "STABLE" };
    size_t sizeofArray = sizeof(exception_trend_any_ofArray) / sizeof(exception_trend_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(exception_trend_any_of, exception_trend_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

