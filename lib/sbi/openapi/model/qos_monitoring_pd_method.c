
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_pd_method.h"

char* OpenAPI_qos_monitoring_pd_method_ToString(OpenAPI_qos_monitoring_pd_method_e qos_monitoring_pd_method)
{
    const char *qos_monitoring_pd_methodArray[] =  { "NULL", "USING_TIMESTAMP", "USING_GTP_U_PATH" };
    size_t sizeofArray = sizeof(qos_monitoring_pd_methodArray) / sizeof(qos_monitoring_pd_methodArray[0]);
    if (qos_monitoring_pd_method < sizeofArray)
        return (char *)qos_monitoring_pd_methodArray[qos_monitoring_pd_method];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_monitoring_pd_method_e OpenAPI_qos_monitoring_pd_method_FromString(char* qos_monitoring_pd_method)
{
    int stringToReturn = 0;
    const char *qos_monitoring_pd_methodArray[] =  { "NULL", "USING_TIMESTAMP", "USING_GTP_U_PATH" };
    size_t sizeofArray = sizeof(qos_monitoring_pd_methodArray) / sizeof(qos_monitoring_pd_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_monitoring_pd_method, qos_monitoring_pd_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

