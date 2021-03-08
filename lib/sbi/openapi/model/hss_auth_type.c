
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_auth_type.h"

char* OpenAPI_hss_auth_type_ToString(OpenAPI_hss_auth_type_e hss_auth_type)
{
    const char *hss_auth_typeArray[] =  { "NULL", "EPS_AKA", "EAP_AKA", "EAP_AKA_PRIME", "IMS_AKA", "GBA_AKA", "UMTS_AKA" };
    size_t sizeofArray = sizeof(hss_auth_typeArray) / sizeof(hss_auth_typeArray[0]);
    if (hss_auth_type < sizeofArray)
        return (char *)hss_auth_typeArray[hss_auth_type];
    else
        return (char *)"Unknown";
}

OpenAPI_hss_auth_type_e OpenAPI_hss_auth_type_FromString(char* hss_auth_type)
{
    int stringToReturn = 0;
    const char *hss_auth_typeArray[] =  { "NULL", "EPS_AKA", "EAP_AKA", "EAP_AKA_PRIME", "IMS_AKA", "GBA_AKA", "UMTS_AKA" };
    size_t sizeofArray = sizeof(hss_auth_typeArray) / sizeof(hss_auth_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(hss_auth_type, hss_auth_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

