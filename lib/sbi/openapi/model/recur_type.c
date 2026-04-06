
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "recur_type.h"

char* OpenAPI_recur_type_ToString(OpenAPI_recur_type_e recur_type)
{
    const char *recur_typeArray[] =  { "NULL", "DAILY", "WEEKLY", "MONTHLY_BY_DATE", "MONTHLY_BY_DAY", "YEARLY", "WEEKDAYS_ONLY", "WEEKENDS_ONLY" };
    size_t sizeofArray = sizeof(recur_typeArray) / sizeof(recur_typeArray[0]);
    if (recur_type < sizeofArray)
        return (char *)recur_typeArray[recur_type];
    else
        return (char *)"Unknown";
}

OpenAPI_recur_type_e OpenAPI_recur_type_FromString(char* recur_type)
{
    int stringToReturn = 0;
    const char *recur_typeArray[] =  { "NULL", "DAILY", "WEEKLY", "MONTHLY_BY_DATE", "MONTHLY_BY_DAY", "YEARLY", "WEEKDAYS_ONLY", "WEEKENDS_ONLY" };
    size_t sizeofArray = sizeof(recur_typeArray) / sizeof(recur_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(recur_type, recur_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

