
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "metering_method.h"

char* OpenAPI_metering_method_ToString(OpenAPI_metering_method_e metering_method)
{
    const char *metering_methodArray[] =  { "NULL", "DURATION", "VOLUME", "DURATION_VOLUME", "EVENT" };
    size_t sizeofArray = sizeof(metering_methodArray) / sizeof(metering_methodArray[0]);
    if (metering_method < sizeofArray)
        return (char *)metering_methodArray[metering_method];
    else
        return (char *)"Unknown";
}

OpenAPI_metering_method_e OpenAPI_metering_method_FromString(char* metering_method)
{
    int stringToReturn = 0;
    const char *metering_methodArray[] =  { "NULL", "DURATION", "VOLUME", "DURATION_VOLUME", "EVENT" };
    size_t sizeofArray = sizeof(metering_methodArray) / sizeof(metering_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(metering_method, metering_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

