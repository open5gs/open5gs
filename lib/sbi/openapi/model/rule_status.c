
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rule_status.h"

char* OpenAPI_rule_status_ToString(OpenAPI_rule_status_e rule_status)
{
    const char *rule_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(rule_statusArray) / sizeof(rule_statusArray[0]);
    if (rule_status < sizeofArray)
        return (char *)rule_statusArray[rule_status];
    else
        return (char *)"Unknown";
}

OpenAPI_rule_status_e OpenAPI_rule_status_FromString(char* rule_status)
{
    int stringToReturn = 0;
    const char *rule_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(rule_statusArray) / sizeof(rule_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rule_status, rule_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

