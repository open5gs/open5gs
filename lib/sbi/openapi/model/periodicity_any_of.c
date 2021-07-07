
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "periodicity_any_of.h"

char* OpenAPI_periodicity_any_of_ToString(OpenAPI_periodicity_any_of_e periodicity_any_of)
{
    const char *periodicity_any_ofArray[] =  { "NULL", "YEARLY", "MONTHLY", "WEEKLY", "DAILY", "HOURLY" };
    size_t sizeofArray = sizeof(periodicity_any_ofArray) / sizeof(periodicity_any_ofArray[0]);
    if (periodicity_any_of < sizeofArray)
        return (char *)periodicity_any_ofArray[periodicity_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_periodicity_any_of_e OpenAPI_periodicity_any_of_FromString(char* periodicity_any_of)
{
    int stringToReturn = 0;
    const char *periodicity_any_ofArray[] =  { "NULL", "YEARLY", "MONTHLY", "WEEKLY", "DAILY", "HOURLY" };
    size_t sizeofArray = sizeof(periodicity_any_ofArray) / sizeof(periodicity_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(periodicity_any_of, periodicity_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

