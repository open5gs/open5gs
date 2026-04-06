
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accuracy.h"

char* OpenAPI_accuracy_ToString(OpenAPI_accuracy_e accuracy)
{
    const char *accuracyArray[] =  { "NULL", "LOW", "MEDIUM", "HIGH", "HIGHEST" };
    size_t sizeofArray = sizeof(accuracyArray) / sizeof(accuracyArray[0]);
    if (accuracy < sizeofArray)
        return (char *)accuracyArray[accuracy];
    else
        return (char *)"Unknown";
}

OpenAPI_accuracy_e OpenAPI_accuracy_FromString(char* accuracy)
{
    int stringToReturn = 0;
    const char *accuracyArray[] =  { "NULL", "LOW", "MEDIUM", "HIGH", "HIGHEST" };
    size_t sizeofArray = sizeof(accuracyArray) / sizeof(accuracyArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(accuracy, accuracyArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

