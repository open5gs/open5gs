
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_accuracy_indication.h"

char* OpenAPI_analytics_accuracy_indication_ToString(OpenAPI_analytics_accuracy_indication_e analytics_accuracy_indication)
{
    const char *analytics_accuracy_indicationArray[] =  { "NULL", "MEET", "NOT_MEET" };
    size_t sizeofArray = sizeof(analytics_accuracy_indicationArray) / sizeof(analytics_accuracy_indicationArray[0]);
    if (analytics_accuracy_indication < sizeofArray)
        return (char *)analytics_accuracy_indicationArray[analytics_accuracy_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_analytics_accuracy_indication_e OpenAPI_analytics_accuracy_indication_FromString(char* analytics_accuracy_indication)
{
    int stringToReturn = 0;
    const char *analytics_accuracy_indicationArray[] =  { "NULL", "MEET", "NOT_MEET" };
    size_t sizeofArray = sizeof(analytics_accuracy_indicationArray) / sizeof(analytics_accuracy_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(analytics_accuracy_indication, analytics_accuracy_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

