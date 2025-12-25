
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_priority_any_of.h"

char* OpenAPI_lcs_priority_any_of_ToString(OpenAPI_lcs_priority_any_of_e lcs_priority_any_of)
{
    const char *lcs_priority_any_ofArray[] =  { "NULL", "HIGHEST_PRIORITY", "NORMAL_PRIORITY" };
    size_t sizeofArray = sizeof(lcs_priority_any_ofArray) / sizeof(lcs_priority_any_ofArray[0]);
    if (lcs_priority_any_of < sizeofArray)
        return (char *)lcs_priority_any_ofArray[lcs_priority_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_lcs_priority_any_of_e OpenAPI_lcs_priority_any_of_FromString(char* lcs_priority_any_of)
{
    int stringToReturn = 0;
    const char *lcs_priority_any_ofArray[] =  { "NULL", "HIGHEST_PRIORITY", "NORMAL_PRIORITY" };
    size_t sizeofArray = sizeof(lcs_priority_any_ofArray) / sizeof(lcs_priority_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(lcs_priority_any_of, lcs_priority_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

