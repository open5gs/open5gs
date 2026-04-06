
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_parameter_meas.h"

char* OpenAPI_traffic_parameter_meas_ToString(OpenAPI_traffic_parameter_meas_e traffic_parameter_meas)
{
    const char *traffic_parameter_measArray[] =  { "NULL", "DL_N6_JITTER", "DL_PERIOD", "UL_PERIOD" };
    size_t sizeofArray = sizeof(traffic_parameter_measArray) / sizeof(traffic_parameter_measArray[0]);
    if (traffic_parameter_meas < sizeofArray)
        return (char *)traffic_parameter_measArray[traffic_parameter_meas];
    else
        return (char *)"Unknown";
}

OpenAPI_traffic_parameter_meas_e OpenAPI_traffic_parameter_meas_FromString(char* traffic_parameter_meas)
{
    int stringToReturn = 0;
    const char *traffic_parameter_measArray[] =  { "NULL", "DL_N6_JITTER", "DL_PERIOD", "UL_PERIOD" };
    size_t sizeofArray = sizeof(traffic_parameter_measArray) / sizeof(traffic_parameter_measArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(traffic_parameter_meas, traffic_parameter_measArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

