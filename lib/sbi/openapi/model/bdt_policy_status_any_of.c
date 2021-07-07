
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_policy_status_any_of.h"

char* OpenAPI_bdt_policy_status_any_of_ToString(OpenAPI_bdt_policy_status_any_of_e bdt_policy_status_any_of)
{
    const char *bdt_policy_status_any_ofArray[] =  { "NULL", "INVALID", "VALID" };
    size_t sizeofArray = sizeof(bdt_policy_status_any_ofArray) / sizeof(bdt_policy_status_any_ofArray[0]);
    if (bdt_policy_status_any_of < sizeofArray)
        return (char *)bdt_policy_status_any_ofArray[bdt_policy_status_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_bdt_policy_status_any_of_e OpenAPI_bdt_policy_status_any_of_FromString(char* bdt_policy_status_any_of)
{
    int stringToReturn = 0;
    const char *bdt_policy_status_any_ofArray[] =  { "NULL", "INVALID", "VALID" };
    size_t sizeofArray = sizeof(bdt_policy_status_any_ofArray) / sizeof(bdt_policy_status_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(bdt_policy_status_any_of, bdt_policy_status_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

