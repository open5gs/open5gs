
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rule_operation.h"

char* OpenAPI_rule_operation_ToString(OpenAPI_rule_operation_e rule_operation)
{
    const char *rule_operationArray[] =  { "NULL", "CREATE_PCC_RULE", "DELETE_PCC_RULE", "MODIFY_PCC_RULE_AND_ADD_PACKET_FILTERS", "MODIFY_PCC_RULE_AND_REPLACE_PACKET_FILTERS", "MODIFY_PCC_RULE_AND_DELETE_PACKET_FILTERS", "MODIFY_PCC_RULE_WITHOUT_MODIFY_PACKET_FILTERS" };
    size_t sizeofArray = sizeof(rule_operationArray) / sizeof(rule_operationArray[0]);
    if (rule_operation < sizeofArray)
        return (char *)rule_operationArray[rule_operation];
    else
        return (char *)"Unknown";
}

OpenAPI_rule_operation_e OpenAPI_rule_operation_FromString(char* rule_operation)
{
    int stringToReturn = 0;
    const char *rule_operationArray[] =  { "NULL", "CREATE_PCC_RULE", "DELETE_PCC_RULE", "MODIFY_PCC_RULE_AND_ADD_PACKET_FILTERS", "MODIFY_PCC_RULE_AND_REPLACE_PACKET_FILTERS", "MODIFY_PCC_RULE_AND_DELETE_PACKET_FILTERS", "MODIFY_PCC_RULE_WITHOUT_MODIFY_PACKET_FILTERS" };
    size_t sizeofArray = sizeof(rule_operationArray) / sizeof(rule_operationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(rule_operation, rule_operationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

