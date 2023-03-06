
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accuracy_any_of.h"

char* OpenAPI_accuracy_any_of_ToString(OpenAPI_accuracy_any_of_e accuracy_any_of)
{
    const char *accuracy_any_ofArray[] =  { "NULL", "LOW", "HIGH" };
    size_t sizeofArray = sizeof(accuracy_any_ofArray) / sizeof(accuracy_any_ofArray[0]);
    if (accuracy_any_of < sizeofArray)
        return (char *)accuracy_any_ofArray[accuracy_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_accuracy_any_of_e OpenAPI_accuracy_any_of_FromString(char* accuracy_any_of)
{
    int stringToReturn = 0;
    const char *accuracy_any_ofArray[] =  { "NULL", "LOW", "HIGH" };
    size_t sizeofArray = sizeof(accuracy_any_ofArray) / sizeof(accuracy_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(accuracy_any_of, accuracy_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

