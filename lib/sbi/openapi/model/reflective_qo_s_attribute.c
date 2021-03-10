
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reflective_qo_s_attribute.h"

char* OpenAPI_reflective_qo_s_attribute_ToString(OpenAPI_reflective_qo_s_attribute_e reflective_qo_s_attribute)
{
    const char *reflective_qo_s_attributeArray[] =  { "NULL", "RQOS", "NO_RQOS" };
    size_t sizeofArray = sizeof(reflective_qo_s_attributeArray) / sizeof(reflective_qo_s_attributeArray[0]);
    if (reflective_qo_s_attribute < sizeofArray)
        return (char *)reflective_qo_s_attributeArray[reflective_qo_s_attribute];
    else
        return (char *)"Unknown";
}

OpenAPI_reflective_qo_s_attribute_e OpenAPI_reflective_qo_s_attribute_FromString(char* reflective_qo_s_attribute)
{
    int stringToReturn = 0;
    const char *reflective_qo_s_attributeArray[] =  { "NULL", "RQOS", "NO_RQOS" };
    size_t sizeofArray = sizeof(reflective_qo_s_attributeArray) / sizeof(reflective_qo_s_attributeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reflective_qo_s_attribute, reflective_qo_s_attributeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

