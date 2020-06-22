
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_selection_mode.h"

char* OpenAPI_dnn_selection_mode_ToString(OpenAPI_dnn_selection_mode_e dnn_selection_mode)
{
    const char *dnn_selection_modeArray[] =  { "NULL", "VERIFIED", "UE_DNN_NOT_VERIFIED", "NW_DNN_NOT_VERIFIED" };
    size_t sizeofArray = sizeof(dnn_selection_modeArray) / sizeof(dnn_selection_modeArray[0]);
    if (dnn_selection_mode < sizeofArray)
        return (char *)dnn_selection_modeArray[dnn_selection_mode];
    else
        return (char *)"Unknown";
}

OpenAPI_dnn_selection_mode_e OpenAPI_dnn_selection_mode_FromString(char* dnn_selection_mode)
{
    int stringToReturn = 0;
    const char *dnn_selection_modeArray[] =  { "NULL", "VERIFIED", "UE_DNN_NOT_VERIFIED", "NW_DNN_NOT_VERIFIED" };
    size_t sizeofArray = sizeof(dnn_selection_modeArray) / sizeof(dnn_selection_modeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(dnn_selection_mode, dnn_selection_modeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

