
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matching_direction_any_of.h"

char* OpenAPI_matching_direction_any_of_ToString(OpenAPI_matching_direction_any_of_e matching_direction_any_of)
{
    const char *matching_direction_any_ofArray[] =  { "NULL", "ASCENDING", "DESCENDING", "CROSSED" };
    size_t sizeofArray = sizeof(matching_direction_any_ofArray) / sizeof(matching_direction_any_ofArray[0]);
    if (matching_direction_any_of < sizeofArray)
        return (char *)matching_direction_any_ofArray[matching_direction_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_matching_direction_any_of_e OpenAPI_matching_direction_any_of_FromString(char* matching_direction_any_of)
{
    int stringToReturn = 0;
    const char *matching_direction_any_ofArray[] =  { "NULL", "ASCENDING", "DESCENDING", "CROSSED" };
    size_t sizeofArray = sizeof(matching_direction_any_ofArray) / sizeof(matching_direction_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(matching_direction_any_of, matching_direction_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

