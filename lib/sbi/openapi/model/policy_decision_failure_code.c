
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_decision_failure_code.h"

char* OpenAPI_policy_decision_failure_code_ToString(OpenAPI_policy_decision_failure_code_e policy_decision_failure_code)
{
    const char *policy_decision_failure_codeArray[] =  { "NULL", "TRA_CTRL_DECS_ERR", "QOS_DECS_ERR", "CHG_DECS_ERR", "USA_MON_DECS_ERR", "QOS_MON_DECS_ERR", "CON_DATA_ERR" };
    size_t sizeofArray = sizeof(policy_decision_failure_codeArray) / sizeof(policy_decision_failure_codeArray[0]);
    if (policy_decision_failure_code < sizeofArray)
        return (char *)policy_decision_failure_codeArray[policy_decision_failure_code];
    else
        return (char *)"Unknown";
}

OpenAPI_policy_decision_failure_code_e OpenAPI_policy_decision_failure_code_FromString(char* policy_decision_failure_code)
{
    int stringToReturn = 0;
    const char *policy_decision_failure_codeArray[] =  { "NULL", "TRA_CTRL_DECS_ERR", "QOS_DECS_ERR", "CHG_DECS_ERR", "USA_MON_DECS_ERR", "QOS_MON_DECS_ERR", "CON_DATA_ERR" };
    size_t sizeofArray = sizeof(policy_decision_failure_codeArray) / sizeof(policy_decision_failure_codeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(policy_decision_failure_code, policy_decision_failure_codeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

