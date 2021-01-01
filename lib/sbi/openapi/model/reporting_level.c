
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_level.h"

char* OpenAPI_reporting_level_ToString(OpenAPI_reporting_level_e reporting_level)
{
    const char *reporting_levelArray[] =  { "NULL", "SER_ID_LEVEL", "RAT_GR_LEVEL", "SPON_CON_LEVEL" };
    size_t sizeofArray = sizeof(reporting_levelArray) / sizeof(reporting_levelArray[0]);
    if (reporting_level < sizeofArray)
        return (char *)reporting_levelArray[reporting_level];
    else
        return (char *)"Unknown";
}

OpenAPI_reporting_level_e OpenAPI_reporting_level_FromString(char* reporting_level)
{
    int stringToReturn = 0;
    const char *reporting_levelArray[] =  { "NULL", "SER_ID_LEVEL", "RAT_GR_LEVEL", "SPON_CON_LEVEL" };
    size_t sizeofArray = sizeof(reporting_levelArray) / sizeof(reporting_levelArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reporting_level, reporting_levelArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

