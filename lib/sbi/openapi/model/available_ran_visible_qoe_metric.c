
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "available_ran_visible_qoe_metric.h"

char* OpenAPI_available_ran_visible_qoe_metric_ToString(OpenAPI_available_ran_visible_qoe_metric_e available_ran_visible_qoe_metric)
{
    const char *available_ran_visible_qoe_metricArray[] =  { "NULL", "APPLICATION_LAYER_BUFFER_LEVEL_LIST", "PLAYOUT_DELAY_FOR_MEDIA_STARTUP" };
    size_t sizeofArray = sizeof(available_ran_visible_qoe_metricArray) / sizeof(available_ran_visible_qoe_metricArray[0]);
    if (available_ran_visible_qoe_metric < sizeofArray)
        return (char *)available_ran_visible_qoe_metricArray[available_ran_visible_qoe_metric];
    else
        return (char *)"Unknown";
}

OpenAPI_available_ran_visible_qoe_metric_e OpenAPI_available_ran_visible_qoe_metric_FromString(char* available_ran_visible_qoe_metric)
{
    int stringToReturn = 0;
    const char *available_ran_visible_qoe_metricArray[] =  { "NULL", "APPLICATION_LAYER_BUFFER_LEVEL_LIST", "PLAYOUT_DELAY_FOR_MEDIA_STARTUP" };
    size_t sizeofArray = sizeof(available_ran_visible_qoe_metricArray) / sizeof(available_ran_visible_qoe_metricArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(available_ran_visible_qoe_metric, available_ran_visible_qoe_metricArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

