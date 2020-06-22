
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_access_type.h"

char* OpenAPI_qos_flow_access_type_ToString(OpenAPI_qos_flow_access_type_e qos_flow_access_type)
{
    const char *qos_flow_access_typeArray[] =  { "NULL", "_3GPP", "NON_3GPP", "_3GPP_AND_NON_3GPP" };
    size_t sizeofArray = sizeof(qos_flow_access_typeArray) / sizeof(qos_flow_access_typeArray[0]);
    if (qos_flow_access_type < sizeofArray)
        return (char *)qos_flow_access_typeArray[qos_flow_access_type];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_flow_access_type_e OpenAPI_qos_flow_access_type_FromString(char* qos_flow_access_type)
{
    int stringToReturn = 0;
    const char *qos_flow_access_typeArray[] =  { "NULL", "_3GPP", "NON_3GPP", "_3GPP_AND_NON_3GPP" };
    size_t sizeofArray = sizeof(qos_flow_access_typeArray) / sizeof(qos_flow_access_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_flow_access_type, qos_flow_access_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

