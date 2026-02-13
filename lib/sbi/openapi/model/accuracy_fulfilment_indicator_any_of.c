
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accuracy_fulfilment_indicator_any_of.h"

char* OpenAPI_accuracy_fulfilment_indicator_any_of_ToString(OpenAPI_accuracy_fulfilment_indicator_any_of_e accuracy_fulfilment_indicator_any_of)
{
    const char *accuracy_fulfilment_indicator_any_ofArray[] =  { "NULL", "REQUESTED_ACCURACY_FULFILLED", "REQUESTED_ACCURACY_NOT_FULFILLED" };
    size_t sizeofArray = sizeof(accuracy_fulfilment_indicator_any_ofArray) / sizeof(accuracy_fulfilment_indicator_any_ofArray[0]);
    if (accuracy_fulfilment_indicator_any_of < sizeofArray)
        return (char *)accuracy_fulfilment_indicator_any_ofArray[accuracy_fulfilment_indicator_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_accuracy_fulfilment_indicator_any_of_e OpenAPI_accuracy_fulfilment_indicator_any_of_FromString(char* accuracy_fulfilment_indicator_any_of)
{
    int stringToReturn = 0;
    const char *accuracy_fulfilment_indicator_any_ofArray[] =  { "NULL", "REQUESTED_ACCURACY_FULFILLED", "REQUESTED_ACCURACY_NOT_FULFILLED" };
    size_t sizeofArray = sizeof(accuracy_fulfilment_indicator_any_ofArray) / sizeof(accuracy_fulfilment_indicator_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(accuracy_fulfilment_indicator_any_of, accuracy_fulfilment_indicator_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

