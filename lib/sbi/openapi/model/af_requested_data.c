
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_requested_data.h"

char* OpenAPI_af_requested_data_ToString(OpenAPI_af_requested_data_e af_requested_data)
{
    const char *af_requested_dataArray[] =  { "NULL", "UE_IDENTITY" };
    size_t sizeofArray = sizeof(af_requested_dataArray) / sizeof(af_requested_dataArray[0]);
    if (af_requested_data < sizeofArray)
        return (char *)af_requested_dataArray[af_requested_data];
    else
        return (char *)"Unknown";
}

OpenAPI_af_requested_data_e OpenAPI_af_requested_data_FromString(char* af_requested_data)
{
    int stringToReturn = 0;
    const char *af_requested_dataArray[] =  { "NULL", "UE_IDENTITY" };
    size_t sizeofArray = sizeof(af_requested_dataArray) / sizeof(af_requested_dataArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(af_requested_data, af_requested_dataArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

