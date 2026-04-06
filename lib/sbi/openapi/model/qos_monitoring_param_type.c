
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_param_type.h"

char* OpenAPI_qos_monitoring_param_type_ToString(OpenAPI_qos_monitoring_param_type_e qos_monitoring_param_type)
{
    const char *qos_monitoring_param_typeArray[] =  { "NULL", "PACKET_DELAY", "CONGESTION", "DATA_RATE", "AVAILABLE_BITRATE" };
    size_t sizeofArray = sizeof(qos_monitoring_param_typeArray) / sizeof(qos_monitoring_param_typeArray[0]);
    if (qos_monitoring_param_type < sizeofArray)
        return (char *)qos_monitoring_param_typeArray[qos_monitoring_param_type];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_monitoring_param_type_e OpenAPI_qos_monitoring_param_type_FromString(char* qos_monitoring_param_type)
{
    int stringToReturn = 0;
    const char *qos_monitoring_param_typeArray[] =  { "NULL", "PACKET_DELAY", "CONGESTION", "DATA_RATE", "AVAILABLE_BITRATE" };
    size_t sizeofArray = sizeof(qos_monitoring_param_typeArray) / sizeof(qos_monitoring_param_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_monitoring_param_type, qos_monitoring_param_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

