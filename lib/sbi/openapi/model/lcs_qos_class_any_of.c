
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_qos_class_any_of.h"

char* OpenAPI_lcs_qos_class_any_of_ToString(OpenAPI_lcs_qos_class_any_of_e lcs_qos_class_any_of)
{
    const char *lcs_qos_class_any_ofArray[] =  { "NULL", "BEST_EFFORT", "ASSURED", "MULTIPLE_QOS" };
    size_t sizeofArray = sizeof(lcs_qos_class_any_ofArray) / sizeof(lcs_qos_class_any_ofArray[0]);
    if (lcs_qos_class_any_of < sizeofArray)
        return (char *)lcs_qos_class_any_ofArray[lcs_qos_class_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_lcs_qos_class_any_of_e OpenAPI_lcs_qos_class_any_of_FromString(char* lcs_qos_class_any_of)
{
    int stringToReturn = 0;
    const char *lcs_qos_class_any_ofArray[] =  { "NULL", "BEST_EFFORT", "ASSURED", "MULTIPLE_QOS" };
    size_t sizeofArray = sizeof(lcs_qos_class_any_ofArray) / sizeof(lcs_qos_class_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(lcs_qos_class_any_of, lcs_qos_class_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

