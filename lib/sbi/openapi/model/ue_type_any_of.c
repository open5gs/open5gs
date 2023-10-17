
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_type_any_of.h"

char* OpenAPI_ue_type_any_of_ToString(OpenAPI_ue_type_any_of_e ue_type_any_of)
{
    const char *ue_type_any_ofArray[] =  { "NULL", "AERIAL_UE" };
    size_t sizeofArray = sizeof(ue_type_any_ofArray) / sizeof(ue_type_any_ofArray[0]);
    if (ue_type_any_of < sizeofArray)
        return (char *)ue_type_any_ofArray[ue_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_type_any_of_e OpenAPI_ue_type_any_of_FromString(char* ue_type_any_of)
{
    int stringToReturn = 0;
    const char *ue_type_any_ofArray[] =  { "NULL", "AERIAL_UE" };
    size_t sizeofArray = sizeof(ue_type_any_ofArray) / sizeof(ue_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_type_any_of, ue_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

