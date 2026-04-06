
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_accuracy.h"

char* OpenAPI_location_accuracy_ToString(OpenAPI_location_accuracy_e location_accuracy)
{
    const char *location_accuracyArray[] =  { "NULL", "CELL_LEVEL", "RAN_NODE_LEVEL", "TA_LEVEL", "PLMN_LEVEL", "N3IWF_LEVEL", "UE_IP", "UE_PORT" };
    size_t sizeofArray = sizeof(location_accuracyArray) / sizeof(location_accuracyArray[0]);
    if (location_accuracy < sizeofArray)
        return (char *)location_accuracyArray[location_accuracy];
    else
        return (char *)"Unknown";
}

OpenAPI_location_accuracy_e OpenAPI_location_accuracy_FromString(char* location_accuracy)
{
    int stringToReturn = 0;
    const char *location_accuracyArray[] =  { "NULL", "CELL_LEVEL", "RAN_NODE_LEVEL", "TA_LEVEL", "PLMN_LEVEL", "N3IWF_LEVEL", "UE_IP", "UE_PORT" };
    size_t sizeofArray = sizeof(location_accuracyArray) / sizeof(location_accuracyArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(location_accuracy, location_accuracyArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

