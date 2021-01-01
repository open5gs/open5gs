
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_frequency.h"

char* OpenAPI_reporting_frequency_ToString(OpenAPI_reporting_frequency_e reporting_frequency)
{
    const char *reporting_frequencyArray[] =  { "NULL", "EVENT_TRIGGERED", "PERIODIC", "SESSION_RELEASE" };
    size_t sizeofArray = sizeof(reporting_frequencyArray) / sizeof(reporting_frequencyArray[0]);
    if (reporting_frequency < sizeofArray)
        return (char *)reporting_frequencyArray[reporting_frequency];
    else
        return (char *)"Unknown";
}

OpenAPI_reporting_frequency_e OpenAPI_reporting_frequency_FromString(char* reporting_frequency)
{
    int stringToReturn = 0;
    const char *reporting_frequencyArray[] =  { "NULL", "EVENT_TRIGGERED", "PERIODIC", "SESSION_RELEASE" };
    size_t sizeofArray = sizeof(reporting_frequencyArray) / sizeof(reporting_frequencyArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reporting_frequency, reporting_frequencyArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

