
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "report_interval_mdt.h"

char* OpenAPI_report_interval_mdt_ToString(OpenAPI_report_interval_mdt_e report_interval_mdt)
{
    const char *report_interval_mdtArray[] =  { "NULL", "120", "240", "480", "640", "1024", "2048", "5120", "10240", "60000", "360000", "720000", "1800000", "3600000" };
    size_t sizeofArray = sizeof(report_interval_mdtArray) / sizeof(report_interval_mdtArray[0]);
    if (report_interval_mdt < sizeofArray)
        return (char *)report_interval_mdtArray[report_interval_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_report_interval_mdt_e OpenAPI_report_interval_mdt_FromString(char* report_interval_mdt)
{
    int stringToReturn = 0;
    const char *report_interval_mdtArray[] =  { "NULL", "120", "240", "480", "640", "1024", "2048", "5120", "10240", "60000", "360000", "720000", "1800000", "3600000" };
    size_t sizeofArray = sizeof(report_interval_mdtArray) / sizeof(report_interval_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(report_interval_mdt, report_interval_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

