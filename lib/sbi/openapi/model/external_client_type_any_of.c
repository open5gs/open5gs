
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "external_client_type_any_of.h"

char* OpenAPI_external_client_type_any_of_ToString(OpenAPI_external_client_type_any_of_e external_client_type_any_of)
{
    const char *external_client_type_any_ofArray[] =  { "NULL", "EMERGENCY_SERVICES", "VALUE_ADDED_SERVICES", "PLMN_OPERATOR_SERVICES", "LAWFUL_INTERCEPT_SERVICES", "PLMN_OPERATOR_BROADCAST_SERVICES", "PLMN_OPERATOR_OM", "PLMN_OPERATOR_ANONYMOUS_STATISTICS", "PLMN_OPERATOR_TARGET_MS_SERVICE_SUPPORT" };
    size_t sizeofArray = sizeof(external_client_type_any_ofArray) / sizeof(external_client_type_any_ofArray[0]);
    if (external_client_type_any_of < sizeofArray)
        return (char *)external_client_type_any_ofArray[external_client_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_external_client_type_any_of_e OpenAPI_external_client_type_any_of_FromString(char* external_client_type_any_of)
{
    int stringToReturn = 0;
    const char *external_client_type_any_ofArray[] =  { "NULL", "EMERGENCY_SERVICES", "VALUE_ADDED_SERVICES", "PLMN_OPERATOR_SERVICES", "LAWFUL_INTERCEPT_SERVICES", "PLMN_OPERATOR_BROADCAST_SERVICES", "PLMN_OPERATOR_OM", "PLMN_OPERATOR_ANONYMOUS_STATISTICS", "PLMN_OPERATOR_TARGET_MS_SERVICE_SUPPORT" };
    size_t sizeofArray = sizeof(external_client_type_any_ofArray) / sizeof(external_client_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(external_client_type_any_of, external_client_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

