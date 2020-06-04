
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "null_value.h"

char* OpenAPI_null_value_ToString(OpenAPI_null_value_e null_value)
{
    const char *null_valueArray[] =  { "NULL", "null" };
    size_t sizeofArray = sizeof(null_valueArray) / sizeof(null_valueArray[0]);
    if (null_value < sizeofArray)
        return (char *)null_valueArray[null_value];
    else
        return (char *)"Unknown";
}

OpenAPI_null_value_e OpenAPI_null_value_FromString(char* null_value)
{
    int stringToReturn = 0;
    const char *null_valueArray[] =  { "NULL", "null" };
    size_t sizeofArray = sizeof(null_valueArray) / sizeof(null_valueArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(null_value, null_valueArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

