
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trigger_category.h"

char* OpenAPI_trigger_category_ToString(OpenAPI_trigger_category_e trigger_category)
{
    const char *trigger_categoryArray[] =  { "NULL", "IMMEDIATE_REPORT", "DEFERRED_REPORT" };
    size_t sizeofArray = sizeof(trigger_categoryArray) / sizeof(trigger_categoryArray[0]);
    if (trigger_category < sizeofArray)
        return (char *)trigger_categoryArray[trigger_category];
    else
        return (char *)"Unknown";
}

OpenAPI_trigger_category_e OpenAPI_trigger_category_FromString(char* trigger_category)
{
    int stringToReturn = 0;
    const char *trigger_categoryArray[] =  { "NULL", "IMMEDIATE_REPORT", "DEFERRED_REPORT" };
    size_t sizeofArray = sizeof(trigger_categoryArray) / sizeof(trigger_categoryArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(trigger_category, trigger_categoryArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

