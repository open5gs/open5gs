
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lp_hap_type.h"

char* OpenAPI_lp_hap_type_ToString(OpenAPI_lp_hap_type_e lp_hap_type)
{
    const char *lp_hap_typeArray[] =  { "NULL", "LOW_POW_HIGH_ACCU_POS" };
    size_t sizeofArray = sizeof(lp_hap_typeArray) / sizeof(lp_hap_typeArray[0]);
    if (lp_hap_type < sizeofArray)
        return (char *)lp_hap_typeArray[lp_hap_type];
    else
        return (char *)"Unknown";
}

OpenAPI_lp_hap_type_e OpenAPI_lp_hap_type_FromString(char* lp_hap_type)
{
    int stringToReturn = 0;
    const char *lp_hap_typeArray[] =  { "NULL", "LOW_POW_HIGH_ACCU_POS" };
    size_t sizeofArray = sizeof(lp_hap_typeArray) / sizeof(lp_hap_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(lp_hap_type, lp_hap_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

