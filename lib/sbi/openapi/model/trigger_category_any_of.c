
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trigger_category_any_of.h"

char* OpenAPI_trigger_category_any_of_ToString(OpenAPI_trigger_category_any_of_e trigger_category_any_of)
{
    const char *trigger_category_any_ofArray[] =  { "NULL", "IMMEDIATE_REPORT", "DEFERRED_REPORT" };
    size_t sizeofArray = sizeof(trigger_category_any_ofArray) / sizeof(trigger_category_any_ofArray[0]);
    if (trigger_category_any_of < sizeofArray)
        return (char *)trigger_category_any_ofArray[trigger_category_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_trigger_category_any_of_e OpenAPI_trigger_category_any_of_FromString(char* trigger_category_any_of)
{
    int stringToReturn = 0;
    const char *trigger_category_any_ofArray[] =  { "NULL", "IMMEDIATE_REPORT", "DEFERRED_REPORT" };
    size_t sizeofArray = sizeof(trigger_category_any_ofArray) / sizeof(trigger_category_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(trigger_category_any_of, trigger_category_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

