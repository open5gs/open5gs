
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "logging_duration_nr_mdt.h"

char* OpenAPI_logging_duration_nr_mdt_ToString(OpenAPI_logging_duration_nr_mdt_e logging_duration_nr_mdt)
{
    const char *logging_duration_nr_mdtArray[] =  { "NULL", "_600", "_1200", "_2400", "_3600", "_5400", "_7200" };
    size_t sizeofArray = sizeof(logging_duration_nr_mdtArray) / sizeof(logging_duration_nr_mdtArray[0]);
    if (logging_duration_nr_mdt < sizeofArray)
        return (char *)logging_duration_nr_mdtArray[logging_duration_nr_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_logging_duration_nr_mdt_e OpenAPI_logging_duration_nr_mdt_FromString(char* logging_duration_nr_mdt)
{
    int stringToReturn = 0;
    const char *logging_duration_nr_mdtArray[] =  { "NULL", "_600", "_1200", "_2400", "_3600", "_5400", "_7200" };
    size_t sizeofArray = sizeof(logging_duration_nr_mdtArray) / sizeof(logging_duration_nr_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(logging_duration_nr_mdt, logging_duration_nr_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

