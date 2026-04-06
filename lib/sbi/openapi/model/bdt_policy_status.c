
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_policy_status.h"

char* OpenAPI_bdt_policy_status_ToString(OpenAPI_bdt_policy_status_e bdt_policy_status)
{
    const char *bdt_policy_statusArray[] =  { "NULL", "INVALID", "VALID" };
    size_t sizeofArray = sizeof(bdt_policy_statusArray) / sizeof(bdt_policy_statusArray[0]);
    if (bdt_policy_status < sizeofArray)
        return (char *)bdt_policy_statusArray[bdt_policy_status];
    else
        return (char *)"Unknown";
}

OpenAPI_bdt_policy_status_e OpenAPI_bdt_policy_status_FromString(char* bdt_policy_status)
{
    int stringToReturn = 0;
    const char *bdt_policy_statusArray[] =  { "NULL", "INVALID", "VALID" };
    size_t sizeofArray = sizeof(bdt_policy_statusArray) / sizeof(bdt_policy_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(bdt_policy_status, bdt_policy_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

