
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "occurrence_info_any_of.h"

char* OpenAPI_occurrence_info_any_of_ToString(OpenAPI_occurrence_info_any_of_e occurrence_info_any_of)
{
    const char *occurrence_info_any_ofArray[] =  { "NULL", "ONE_TIME_EVENT", "MULTIPLE_TIME_EVENT" };
    size_t sizeofArray = sizeof(occurrence_info_any_ofArray) / sizeof(occurrence_info_any_ofArray[0]);
    if (occurrence_info_any_of < sizeofArray)
        return (char *)occurrence_info_any_ofArray[occurrence_info_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_occurrence_info_any_of_e OpenAPI_occurrence_info_any_of_FromString(char* occurrence_info_any_of)
{
    int stringToReturn = 0;
    const char *occurrence_info_any_ofArray[] =  { "NULL", "ONE_TIME_EVENT", "MULTIPLE_TIME_EVENT" };
    size_t sizeofArray = sizeof(occurrence_info_any_ofArray) / sizeof(occurrence_info_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(occurrence_info_any_of, occurrence_info_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

