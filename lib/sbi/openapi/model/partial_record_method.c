
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "partial_record_method.h"

char* OpenAPI_partial_record_method_ToString(OpenAPI_partial_record_method_e partial_record_method)
{
    const char *partial_record_methodArray[] =  { "NULL", "DEFAULT", "INDIVIDUAL" };
    size_t sizeofArray = sizeof(partial_record_methodArray) / sizeof(partial_record_methodArray[0]);
    if (partial_record_method < sizeofArray)
        return (char *)partial_record_methodArray[partial_record_method];
    else
        return (char *)"Unknown";
}

OpenAPI_partial_record_method_e OpenAPI_partial_record_method_FromString(char* partial_record_method)
{
    int stringToReturn = 0;
    const char *partial_record_methodArray[] =  { "NULL", "DEFAULT", "INDIVIDUAL" };
    size_t sizeofArray = sizeof(partial_record_methodArray) / sizeof(partial_record_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(partial_record_method, partial_record_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

