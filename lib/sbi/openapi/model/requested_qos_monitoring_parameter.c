
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_qos_monitoring_parameter.h"

char* OpenAPI_requested_qos_monitoring_parameter_ToString(OpenAPI_requested_qos_monitoring_parameter_e requested_qos_monitoring_parameter)
{
    const char *requested_qos_monitoring_parameterArray[] =  { "NULL", "DOWNLINK", "UPLINK", "ROUND_TRIP", "DOWNLINK_DATA_RATE", "UPLINK_DATA_RATE", "DOWNLINK_CONGESTION", "UPLINK_CONGESTION", "DOWNLINK_AVAILABLE_BITRATE", "UPLINK_AVAILABLE_BITRATE" };
    size_t sizeofArray = sizeof(requested_qos_monitoring_parameterArray) / sizeof(requested_qos_monitoring_parameterArray[0]);
    if (requested_qos_monitoring_parameter < sizeofArray)
        return (char *)requested_qos_monitoring_parameterArray[requested_qos_monitoring_parameter];
    else
        return (char *)"Unknown";
}

OpenAPI_requested_qos_monitoring_parameter_e OpenAPI_requested_qos_monitoring_parameter_FromString(char* requested_qos_monitoring_parameter)
{
    int stringToReturn = 0;
    const char *requested_qos_monitoring_parameterArray[] =  { "NULL", "DOWNLINK", "UPLINK", "ROUND_TRIP", "DOWNLINK_DATA_RATE", "UPLINK_DATA_RATE", "DOWNLINK_CONGESTION", "UPLINK_CONGESTION", "DOWNLINK_AVAILABLE_BITRATE", "UPLINK_AVAILABLE_BITRATE" };
    size_t sizeofArray = sizeof(requested_qos_monitoring_parameterArray) / sizeof(requested_qos_monitoring_parameterArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(requested_qos_monitoring_parameter, requested_qos_monitoring_parameterArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

