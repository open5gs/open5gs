
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_qos_class.h"

char* OpenAPI_lcs_qos_class_ToString(OpenAPI_lcs_qos_class_e lcs_qos_class)
{
    const char *lcs_qos_classArray[] =  { "NULL", "BEST_EFFORT", "ASSURED", "MULTIPLE_QOS" };
    size_t sizeofArray = sizeof(lcs_qos_classArray) / sizeof(lcs_qos_classArray[0]);
    if (lcs_qos_class < sizeofArray)
        return (char *)lcs_qos_classArray[lcs_qos_class];
    else
        return (char *)"Unknown";
}

OpenAPI_lcs_qos_class_e OpenAPI_lcs_qos_class_FromString(char* lcs_qos_class)
{
    int stringToReturn = 0;
    const char *lcs_qos_classArray[] =  { "NULL", "BEST_EFFORT", "ASSURED", "MULTIPLE_QOS" };
    size_t sizeofArray = sizeof(lcs_qos_classArray) / sizeof(lcs_qos_classArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(lcs_qos_class, lcs_qos_classArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

