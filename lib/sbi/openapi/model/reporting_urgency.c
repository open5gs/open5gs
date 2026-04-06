
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_urgency.h"

char* OpenAPI_reporting_urgency_ToString(OpenAPI_reporting_urgency_e reporting_urgency)
{
    const char *reporting_urgencyArray[] =  { "NULL", "DELAY_TOLERANT", "NON_DELAY_TOLERANT" };
    size_t sizeofArray = sizeof(reporting_urgencyArray) / sizeof(reporting_urgencyArray[0]);
    if (reporting_urgency < sizeofArray)
        return (char *)reporting_urgencyArray[reporting_urgency];
    else
        return (char *)"Unknown";
}

OpenAPI_reporting_urgency_e OpenAPI_reporting_urgency_FromString(char* reporting_urgency)
{
    int stringToReturn = 0;
    const char *reporting_urgencyArray[] =  { "NULL", "DELAY_TOLERANT", "NON_DELAY_TOLERANT" };
    size_t sizeofArray = sizeof(reporting_urgencyArray) / sizeof(reporting_urgencyArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reporting_urgency, reporting_urgencyArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

