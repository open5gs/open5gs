
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_gvn_group_communication_type.h"

char* OpenAPI_model_5_gvn_group_communication_type_ToString(OpenAPI_model_5_gvn_group_communication_type_e model_5_gvn_group_communication_type)
{
    const char *model_5_gvn_group_communication_typeArray[] =  { "NULL", "WITH_N6_BASED_FORWARDING", "WITHOUT_N6_BASED_FORWARDING" };
    size_t sizeofArray = sizeof(model_5_gvn_group_communication_typeArray) / sizeof(model_5_gvn_group_communication_typeArray[0]);
    if (model_5_gvn_group_communication_type < sizeofArray)
        return (char *)model_5_gvn_group_communication_typeArray[model_5_gvn_group_communication_type];
    else
        return (char *)"Unknown";
}

OpenAPI_model_5_gvn_group_communication_type_e OpenAPI_model_5_gvn_group_communication_type_FromString(char* model_5_gvn_group_communication_type)
{
    int stringToReturn = 0;
    const char *model_5_gvn_group_communication_typeArray[] =  { "NULL", "WITH_N6_BASED_FORWARDING", "WITHOUT_N6_BASED_FORWARDING" };
    size_t sizeofArray = sizeof(model_5_gvn_group_communication_typeArray) / sizeof(model_5_gvn_group_communication_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(model_5_gvn_group_communication_type, model_5_gvn_group_communication_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

