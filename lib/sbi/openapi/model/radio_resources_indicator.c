
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "radio_resources_indicator.h"

char* OpenAPI_radio_resources_indicator_ToString(OpenAPI_radio_resources_indicator_e radio_resources_indicator)
{
    const char *radio_resources_indicatorArray[] =  { "NULL", "OPERATOR_PROVIDED", "CONFIGURED" };
    size_t sizeofArray = sizeof(radio_resources_indicatorArray) / sizeof(radio_resources_indicatorArray[0]);
    if (radio_resources_indicator < sizeofArray)
        return (char *)radio_resources_indicatorArray[radio_resources_indicator];
    else
        return (char *)"Unknown";
}

OpenAPI_radio_resources_indicator_e OpenAPI_radio_resources_indicator_FromString(char* radio_resources_indicator)
{
    int stringToReturn = 0;
    const char *radio_resources_indicatorArray[] =  { "NULL", "OPERATOR_PROVIDED", "CONFIGURED" };
    size_t sizeofArray = sizeof(radio_resources_indicatorArray) / sizeof(radio_resources_indicatorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(radio_resources_indicator, radio_resources_indicatorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

