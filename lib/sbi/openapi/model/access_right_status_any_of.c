
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_right_status_any_of.h"

char* OpenAPI_access_right_status_any_of_ToString(OpenAPI_access_right_status_any_of_e access_right_status_any_of)
{
    const char *access_right_status_any_ofArray[] =  { "NULL", "FULLY_ALLOWED", "PREVIEW_ALLOWED", "NO_ALLOWED" };
    size_t sizeofArray = sizeof(access_right_status_any_ofArray) / sizeof(access_right_status_any_ofArray[0]);
    if (access_right_status_any_of < sizeofArray)
        return (char *)access_right_status_any_ofArray[access_right_status_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_access_right_status_any_of_e OpenAPI_access_right_status_any_of_FromString(char* access_right_status_any_of)
{
    int stringToReturn = 0;
    const char *access_right_status_any_ofArray[] =  { "NULL", "FULLY_ALLOWED", "PREVIEW_ALLOWED", "NO_ALLOWED" };
    size_t sizeofArray = sizeof(access_right_status_any_ofArray) / sizeof(access_right_status_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(access_right_status_any_of, access_right_status_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

