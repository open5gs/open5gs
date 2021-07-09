
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "partial_record_method_any_of.h"

char* OpenAPI_partial_record_method_any_of_ToString(OpenAPI_partial_record_method_any_of_e partial_record_method_any_of)
{
    const char *partial_record_method_any_ofArray[] =  { "NULL", "_DEFAULT", "INDIVIDUAL" };
    size_t sizeofArray = sizeof(partial_record_method_any_ofArray) / sizeof(partial_record_method_any_ofArray[0]);
    if (partial_record_method_any_of < sizeofArray)
        return (char *)partial_record_method_any_ofArray[partial_record_method_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_partial_record_method_any_of_e OpenAPI_partial_record_method_any_of_FromString(char* partial_record_method_any_of)
{
    int stringToReturn = 0;
    const char *partial_record_method_any_ofArray[] =  { "NULL", "_DEFAULT", "INDIVIDUAL" };
    size_t sizeofArray = sizeof(partial_record_method_any_ofArray) / sizeof(partial_record_method_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(partial_record_method_any_of, partial_record_method_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

