
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception_trend.h"

char* OpenAPI_exception_trend_ToString(OpenAPI_exception_trend_e exception_trend)
{
    const char *exception_trendArray[] =  { "NULL", "UP", "DOWN", "UNKNOW", "STABLE" };
    size_t sizeofArray = sizeof(exception_trendArray) / sizeof(exception_trendArray[0]);
    if (exception_trend < sizeofArray)
        return (char *)exception_trendArray[exception_trend];
    else
        return (char *)"Unknown";
}

OpenAPI_exception_trend_e OpenAPI_exception_trend_FromString(char* exception_trend)
{
    int stringToReturn = 0;
    const char *exception_trendArray[] =  { "NULL", "UP", "DOWN", "UNKNOW", "STABLE" };
    size_t sizeofArray = sizeof(exception_trendArray) / sizeof(exception_trendArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(exception_trend, exception_trendArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

