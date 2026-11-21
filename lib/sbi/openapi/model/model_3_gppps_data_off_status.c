
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_3_gppps_data_off_status.h"

char* OpenAPI_model_3_gppps_data_off_status_ToString(OpenAPI_model_3_gppps_data_off_status_e model_3_gppps_data_off_status)
{
    const char *model_3_gppps_data_off_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(model_3_gppps_data_off_statusArray) / sizeof(model_3_gppps_data_off_statusArray[0]);
    if (model_3_gppps_data_off_status < sizeofArray)
        return (char *)model_3_gppps_data_off_statusArray[model_3_gppps_data_off_status];
    else
        return (char *)"Unknown";
}

OpenAPI_model_3_gppps_data_off_status_e OpenAPI_model_3_gppps_data_off_status_FromString(char* model_3_gppps_data_off_status)
{
    int stringToReturn = 0;
    const char *model_3_gppps_data_off_statusArray[] =  { "NULL", "ACTIVE", "INACTIVE" };
    size_t sizeofArray = sizeof(model_3_gppps_data_off_statusArray) / sizeof(model_3_gppps_data_off_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(model_3_gppps_data_off_status, model_3_gppps_data_off_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

