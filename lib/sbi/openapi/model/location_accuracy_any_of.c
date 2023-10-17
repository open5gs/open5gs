
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_accuracy_any_of.h"

char* OpenAPI_location_accuracy_any_of_ToString(OpenAPI_location_accuracy_any_of_e location_accuracy_any_of)
{
    const char *location_accuracy_any_ofArray[] =  { "NULL", "CELL_LEVEL", "RAN_NODE_LEVEL", "TA_LEVEL", "N3IWF_LEVEL", "UE_IP", "UE_PORT" };
    size_t sizeofArray = sizeof(location_accuracy_any_ofArray) / sizeof(location_accuracy_any_ofArray[0]);
    if (location_accuracy_any_of < sizeofArray)
        return (char *)location_accuracy_any_ofArray[location_accuracy_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_location_accuracy_any_of_e OpenAPI_location_accuracy_any_of_FromString(char* location_accuracy_any_of)
{
    int stringToReturn = 0;
    const char *location_accuracy_any_ofArray[] =  { "NULL", "CELL_LEVEL", "RAN_NODE_LEVEL", "TA_LEVEL", "N3IWF_LEVEL", "UE_IP", "UE_PORT" };
    size_t sizeofArray = sizeof(location_accuracy_any_ofArray) / sizeof(location_accuracy_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(location_accuracy_any_of, location_accuracy_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

