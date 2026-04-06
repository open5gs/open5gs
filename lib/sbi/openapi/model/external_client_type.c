
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "external_client_type.h"

char* OpenAPI_external_client_type_ToString(OpenAPI_external_client_type_e external_client_type)
{
    const char *external_client_typeArray[] =  { "NULL", "EMERGENCY_SERVICES", "VALUE_ADDED_SERVICES", "PLMN_OPERATOR_SERVICES", "LAWFUL_INTERCEPT_SERVICES", "PLMN_OPERATOR_BROADCAST_SERVICES", "PLMN_OPERATOR_OM", "PLMN_OPERATOR_ANONYMOUS_STATISTICS", "PLMN_OPERATOR_TARGET_MS_SERVICE_SUPPORT", "SL_POS" };
    size_t sizeofArray = sizeof(external_client_typeArray) / sizeof(external_client_typeArray[0]);
    if (external_client_type < sizeofArray)
        return (char *)external_client_typeArray[external_client_type];
    else
        return (char *)"Unknown";
}

OpenAPI_external_client_type_e OpenAPI_external_client_type_FromString(char* external_client_type)
{
    int stringToReturn = 0;
    const char *external_client_typeArray[] =  { "NULL", "EMERGENCY_SERVICES", "VALUE_ADDED_SERVICES", "PLMN_OPERATOR_SERVICES", "LAWFUL_INTERCEPT_SERVICES", "PLMN_OPERATOR_BROADCAST_SERVICES", "PLMN_OPERATOR_OM", "PLMN_OPERATOR_ANONYMOUS_STATISTICS", "PLMN_OPERATOR_TARGET_MS_SERVICE_SUPPORT", "SL_POS" };
    size_t sizeofArray = sizeof(external_client_typeArray) / sizeof(external_client_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(external_client_type, external_client_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

