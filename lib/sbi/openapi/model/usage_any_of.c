
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_any_of.h"

char* OpenAPI_usage_any_of_ToString(OpenAPI_usage_any_of_e usage_any_of)
{
    const char *usage_any_ofArray[] =  { "NULL", "UNSUCCESS", "SUCCESS_RESULTS_NOT_USED", "SUCCESS_RESULTS_USED_TO_VERIFY_LOCATION", "SUCCESS_RESULTS_USED_TO_GENERATE_LOCATION", "SUCCESS_METHOD_NOT_DETERMINED" };
    size_t sizeofArray = sizeof(usage_any_ofArray) / sizeof(usage_any_ofArray[0]);
    if (usage_any_of < sizeofArray)
        return (char *)usage_any_ofArray[usage_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_usage_any_of_e OpenAPI_usage_any_of_FromString(char* usage_any_of)
{
    int stringToReturn = 0;
    const char *usage_any_ofArray[] =  { "NULL", "UNSUCCESS", "SUCCESS_RESULTS_NOT_USED", "SUCCESS_RESULTS_USED_TO_VERIFY_LOCATION", "SUCCESS_RESULTS_USED_TO_GENERATE_LOCATION", "SUCCESS_METHOD_NOT_DETERMINED" };
    size_t sizeofArray = sizeof(usage_any_ofArray) / sizeof(usage_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(usage_any_of, usage_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

