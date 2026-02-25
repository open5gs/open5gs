
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ldr_type_any_of.h"

char* OpenAPI_ldr_type_any_of_ToString(OpenAPI_ldr_type_any_of_e ldr_type_any_of)
{
    const char *ldr_type_any_ofArray[] =  { "NULL", "UE_AVAILABLE", "PERIODIC", "ENTERING_INTO_AREA", "LEAVING_FROM_AREA", "BEING_INSIDE_AREA", "MOTION" };
    size_t sizeofArray = sizeof(ldr_type_any_ofArray) / sizeof(ldr_type_any_ofArray[0]);
    if (ldr_type_any_of < sizeofArray)
        return (char *)ldr_type_any_ofArray[ldr_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_ldr_type_any_of_e OpenAPI_ldr_type_any_of_FromString(char* ldr_type_any_of)
{
    int stringToReturn = 0;
    const char *ldr_type_any_ofArray[] =  { "NULL", "UE_AVAILABLE", "PERIODIC", "ENTERING_INTO_AREA", "LEAVING_FROM_AREA", "BEING_INSIDE_AREA", "MOTION" };
    size_t sizeofArray = sizeof(ldr_type_any_ofArray) / sizeof(ldr_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ldr_type_any_of, ldr_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

