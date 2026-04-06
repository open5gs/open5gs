
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rule_set_action.h"

char* OpenAPI_rule_set_action_ToString(OpenAPI_rule_set_action_e rule_set_action)
{
    const char *rule_set_actionArray[] =  { "NULL", "ALLOW", "DENY" };
    size_t sizeofArray = sizeof(rule_set_actionArray) / sizeof(rule_set_actionArray[0]);
    if (rule_set_action < sizeofArray)
        return (char *)rule_set_actionArray[rule_set_action];
    else
        return (char *)"Unknown";
}

OpenAPI_rule_set_action_e OpenAPI_rule_set_action_FromString(char* rule_set_action)
{
    int stringToReturn = 0;
    const char *rule_set_actionArray[] =  { "NULL", "ALLOW", "DENY" };
    size_t sizeofArray = sizeof(rule_set_actionArray) / sizeof(rule_set_actionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rule_set_action, rule_set_actionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

