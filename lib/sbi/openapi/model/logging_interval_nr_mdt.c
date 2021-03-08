
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "logging_interval_nr_mdt.h"

char* OpenAPI_logging_interval_nr_mdt_ToString(OpenAPI_logging_interval_nr_mdt_e logging_interval_nr_mdt)
{
    const char *logging_interval_nr_mdtArray[] =  { "NULL", "_128", "_256", "_512", "_1024", "_2048", "_3072", "_4096", "_6144", "_320", "_640", "infinity" };
    size_t sizeofArray = sizeof(logging_interval_nr_mdtArray) / sizeof(logging_interval_nr_mdtArray[0]);
    if (logging_interval_nr_mdt < sizeofArray)
        return (char *)logging_interval_nr_mdtArray[logging_interval_nr_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_logging_interval_nr_mdt_e OpenAPI_logging_interval_nr_mdt_FromString(char* logging_interval_nr_mdt)
{
    int stringToReturn = 0;
    const char *logging_interval_nr_mdtArray[] =  { "NULL", "_128", "_256", "_512", "_1024", "_2048", "_3072", "_4096", "_6144", "_320", "_640", "infinity" };
    size_t sizeofArray = sizeof(logging_interval_nr_mdtArray) / sizeof(logging_interval_nr_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(logging_interval_nr_mdt, logging_interval_nr_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

