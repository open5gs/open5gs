
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "final_unit_action_any_of.h"

char* OpenAPI_final_unit_action_any_of_ToString(OpenAPI_final_unit_action_any_of_e final_unit_action_any_of)
{
    const char *final_unit_action_any_ofArray[] =  { "NULL", "TERMINATE", "REDIRECT", "RESTRICT_ACCESS" };
    size_t sizeofArray = sizeof(final_unit_action_any_ofArray) / sizeof(final_unit_action_any_ofArray[0]);
    if (final_unit_action_any_of < sizeofArray)
        return (char *)final_unit_action_any_ofArray[final_unit_action_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_final_unit_action_any_of_e OpenAPI_final_unit_action_any_of_FromString(char* final_unit_action_any_of)
{
    int stringToReturn = 0;
    const char *final_unit_action_any_ofArray[] =  { "NULL", "TERMINATE", "REDIRECT", "RESTRICT_ACCESS" };
    size_t sizeofArray = sizeof(final_unit_action_any_ofArray) / sizeof(final_unit_action_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(final_unit_action_any_of, final_unit_action_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

