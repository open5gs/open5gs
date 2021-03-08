
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "report_type_mdt.h"

char* OpenAPI_report_type_mdt_ToString(OpenAPI_report_type_mdt_e report_type_mdt)
{
    const char *report_type_mdtArray[] =  { "NULL", "PERIODICAL", "EVENT_TRIGGED" };
    size_t sizeofArray = sizeof(report_type_mdtArray) / sizeof(report_type_mdtArray[0]);
    if (report_type_mdt < sizeofArray)
        return (char *)report_type_mdtArray[report_type_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_report_type_mdt_e OpenAPI_report_type_mdt_FromString(char* report_type_mdt)
{
    int stringToReturn = 0;
    const char *report_type_mdtArray[] =  { "NULL", "PERIODICAL", "EVENT_TRIGGED" };
    size_t sizeofArray = sizeof(report_type_mdtArray) / sizeof(report_type_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(report_type_mdt, report_type_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

