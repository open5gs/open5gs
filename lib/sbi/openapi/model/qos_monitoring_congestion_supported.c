
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_congestion_supported.h"

char* OpenAPI_qos_monitoring_congestion_supported_ToString(OpenAPI_qos_monitoring_congestion_supported_e qos_monitoring_congestion_supported)
{
    const char *qos_monitoring_congestion_supportedArray[] =  { "NULL", "SUPPORTED", "NOT_SUPPORTED", "UNKNOWN" };
    size_t sizeofArray = sizeof(qos_monitoring_congestion_supportedArray) / sizeof(qos_monitoring_congestion_supportedArray[0]);
    if (qos_monitoring_congestion_supported < sizeofArray)
        return (char *)qos_monitoring_congestion_supportedArray[qos_monitoring_congestion_supported];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_monitoring_congestion_supported_e OpenAPI_qos_monitoring_congestion_supported_FromString(char* qos_monitoring_congestion_supported)
{
    int stringToReturn = 0;
    const char *qos_monitoring_congestion_supportedArray[] =  { "NULL", "SUPPORTED", "NOT_SUPPORTED", "UNKNOWN" };
    size_t sizeofArray = sizeof(qos_monitoring_congestion_supportedArray) / sizeof(qos_monitoring_congestion_supportedArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_monitoring_congestion_supported, qos_monitoring_congestion_supportedArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

