
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "report_amount_mdt.h"

char* OpenAPI_report_amount_mdt_ToString(OpenAPI_report_amount_mdt_e report_amount_mdt)
{
    const char *report_amount_mdtArray[] =  { "NULL", "_1", "_2", "_4", "_8", "_16", "_32", "_64", "infinity" };
    size_t sizeofArray = sizeof(report_amount_mdtArray) / sizeof(report_amount_mdtArray[0]);
    if (report_amount_mdt < sizeofArray)
        return (char *)report_amount_mdtArray[report_amount_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_report_amount_mdt_e OpenAPI_report_amount_mdt_FromString(char* report_amount_mdt)
{
    int stringToReturn = 0;
    const char *report_amount_mdtArray[] =  { "NULL", "_1", "_2", "_4", "_8", "_16", "_32", "_64", "infinity" };
    size_t sizeofArray = sizeof(report_amount_mdtArray) / sizeof(report_amount_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(report_amount_mdt, report_amount_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

