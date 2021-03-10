
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_req.h"

char* OpenAPI_qos_monitoring_req_ToString(OpenAPI_qos_monitoring_req_e qos_monitoring_req)
{
    const char *qos_monitoring_reqArray[] =  { "NULL", "UL", "DL", "BOTH", "NONE" };
    size_t sizeofArray = sizeof(qos_monitoring_reqArray) / sizeof(qos_monitoring_reqArray[0]);
    if (qos_monitoring_req < sizeofArray)
        return (char *)qos_monitoring_reqArray[qos_monitoring_req];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_monitoring_req_e OpenAPI_qos_monitoring_req_FromString(char* qos_monitoring_req)
{
    int stringToReturn = 0;
    const char *qos_monitoring_reqArray[] =  { "NULL", "UL", "DL", "BOTH", "NONE" };
    size_t sizeofArray = sizeof(qos_monitoring_reqArray) / sizeof(qos_monitoring_reqArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_monitoring_req, qos_monitoring_reqArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

