
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "periodicity.h"

char* OpenAPI_periodicity_ToString(OpenAPI_periodicity_e periodicity)
{
    const char *periodicityArray[] =  { "NULL", "YEARLY", "MONTHLY", "WEEKLY", "DAILY", "HOURLY" };
    size_t sizeofArray = sizeof(periodicityArray) / sizeof(periodicityArray[0]);
    if (periodicity < sizeofArray)
        return (char *)periodicityArray[periodicity];
    else
        return (char *)"Unknown";
}

OpenAPI_periodicity_e OpenAPI_periodicity_FromString(char* periodicity)
{
    int stringToReturn = 0;
    const char *periodicityArray[] =  { "NULL", "YEARLY", "MONTHLY", "WEEKLY", "DAILY", "HOURLY" };
    size_t sizeofArray = sizeof(periodicityArray) / sizeof(periodicityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(periodicity, periodicityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

