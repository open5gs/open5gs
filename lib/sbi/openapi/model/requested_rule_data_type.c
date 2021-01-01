
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_rule_data_type.h"

char* OpenAPI_requested_rule_data_type_ToString(OpenAPI_requested_rule_data_type_e requested_rule_data_type)
{
    const char *requested_rule_data_typeArray[] =  { "NULL", "CH_ID", "MS_TIME_ZONE", "USER_LOC_INFO", "RES_RELEASE", "SUCC_RES_ALLO", "EPS_FALLBACK" };
    size_t sizeofArray = sizeof(requested_rule_data_typeArray) / sizeof(requested_rule_data_typeArray[0]);
    if (requested_rule_data_type < sizeofArray)
        return (char *)requested_rule_data_typeArray[requested_rule_data_type];
    else
        return (char *)"Unknown";
}

OpenAPI_requested_rule_data_type_e OpenAPI_requested_rule_data_type_FromString(char* requested_rule_data_type)
{
    int stringToReturn = 0;
    const char *requested_rule_data_typeArray[] =  { "NULL", "CH_ID", "MS_TIME_ZONE", "USER_LOC_INFO", "RES_RELEASE", "SUCC_RES_ALLO", "EPS_FALLBACK" };
    size_t sizeofArray = sizeof(requested_rule_data_typeArray) / sizeof(requested_rule_data_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(requested_rule_data_type, requested_rule_data_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

