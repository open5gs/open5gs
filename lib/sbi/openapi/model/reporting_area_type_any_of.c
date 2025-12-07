
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_area_type_any_of.h"

char* OpenAPI_reporting_area_type_any_of_ToString(OpenAPI_reporting_area_type_any_of_e reporting_area_type_any_of)
{
    const char *reporting_area_type_any_ofArray[] =  { "NULL", "EPS_TRACKING_AREA_IDENTITY", "E-UTRAN_CELL_GLOBAL_IDENTIFICATION", "5GS_TRACKING_AREA_IDENTITY", "NR_CELL_GLOBAL_IDENTITY" };
    size_t sizeofArray = sizeof(reporting_area_type_any_ofArray) / sizeof(reporting_area_type_any_ofArray[0]);
    if (reporting_area_type_any_of < sizeofArray)
        return (char *)reporting_area_type_any_ofArray[reporting_area_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_reporting_area_type_any_of_e OpenAPI_reporting_area_type_any_of_FromString(char* reporting_area_type_any_of)
{
    int stringToReturn = 0;
    const char *reporting_area_type_any_ofArray[] =  { "NULL", "EPS_TRACKING_AREA_IDENTITY", "E-UTRAN_CELL_GLOBAL_IDENTIFICATION", "5GS_TRACKING_AREA_IDENTITY", "NR_CELL_GLOBAL_IDENTITY" };
    size_t sizeofArray = sizeof(reporting_area_type_any_ofArray) / sizeof(reporting_area_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reporting_area_type_any_of, reporting_area_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

