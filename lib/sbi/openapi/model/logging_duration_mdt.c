
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "logging_duration_mdt.h"

char* OpenAPI_logging_duration_mdt_ToString(OpenAPI_logging_duration_mdt_e logging_duration_mdt)
{
    const char *logging_duration_mdtArray[] =  { "NULL", "600", "1200", "2400", "3600", "5400", "7200" };
    size_t sizeofArray = sizeof(logging_duration_mdtArray) / sizeof(logging_duration_mdtArray[0]);
    if (logging_duration_mdt < sizeofArray)
        return (char *)logging_duration_mdtArray[logging_duration_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_logging_duration_mdt_e OpenAPI_logging_duration_mdt_FromString(char* logging_duration_mdt)
{
    int stringToReturn = 0;
    const char *logging_duration_mdtArray[] =  { "NULL", "600", "1200", "2400", "3600", "5400", "7200" };
    size_t sizeofArray = sizeof(logging_duration_mdtArray) / sizeof(logging_duration_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(logging_duration_mdt, logging_duration_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

