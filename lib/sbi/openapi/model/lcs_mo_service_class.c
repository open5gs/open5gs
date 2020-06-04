
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_mo_service_class.h"

char* OpenAPI_lcs_mo_service_class_ToString(OpenAPI_lcs_mo_service_class_e lcs_mo_service_class)
{
    const char *lcs_mo_service_classArray[] =  { "NULL", "BASIC_SELF_LOCATION", "AUTONOMOUS_SELF_LOCATION", "TRANSFER_TO_THIRD_PARTY" };
    size_t sizeofArray = sizeof(lcs_mo_service_classArray) / sizeof(lcs_mo_service_classArray[0]);
    if (lcs_mo_service_class < sizeofArray)
        return (char *)lcs_mo_service_classArray[lcs_mo_service_class];
    else
        return (char *)"Unknown";
}

OpenAPI_lcs_mo_service_class_e OpenAPI_lcs_mo_service_class_FromString(char* lcs_mo_service_class)
{
    int stringToReturn = 0;
    const char *lcs_mo_service_classArray[] =  { "NULL", "BASIC_SELF_LOCATION", "AUTONOMOUS_SELF_LOCATION", "TRANSFER_TO_THIRD_PARTY" };
    size_t sizeofArray = sizeof(lcs_mo_service_classArray) / sizeof(lcs_mo_service_classArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(lcs_mo_service_class, lcs_mo_service_classArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

