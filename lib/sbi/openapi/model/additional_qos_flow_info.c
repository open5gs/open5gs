
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_qos_flow_info.h"

char* OpenAPI_additional_qos_flow_info_ToString(OpenAPI_additional_qos_flow_info_e additional_qos_flow_info)
{
    const char *additional_qos_flow_infoArray[] =  { "NULL", "MORE_LIKELY" };
    size_t sizeofArray = sizeof(additional_qos_flow_infoArray) / sizeof(additional_qos_flow_infoArray[0]);
    if (additional_qos_flow_info < sizeofArray)
        return (char *)additional_qos_flow_infoArray[additional_qos_flow_info];
    else
        return (char *)"Unknown";
}

OpenAPI_additional_qos_flow_info_e OpenAPI_additional_qos_flow_info_FromString(char* additional_qos_flow_info)
{
    int stringToReturn = 0;
    const char *additional_qos_flow_infoArray[] =  { "NULL", "MORE_LIKELY" };
    size_t sizeofArray = sizeof(additional_qos_flow_infoArray) / sizeof(additional_qos_flow_infoArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(additional_qos_flow_info, additional_qos_flow_infoArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

