
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_access_type_any_of.h"

char* OpenAPI_reporting_access_type_any_of_ToString(OpenAPI_reporting_access_type_any_of_e reporting_access_type_any_of)
{
    const char *reporting_access_type_any_ofArray[] =  { "NULL", "NR", "EUTRA_CONNECTED_TO_5GC", "NON_3GPP_CONNECTED_TO_5GC", "NR_LEO", "NR_MEO", "NR_GEO", "NR_OTHER_SAT" };
    size_t sizeofArray = sizeof(reporting_access_type_any_ofArray) / sizeof(reporting_access_type_any_ofArray[0]);
    if (reporting_access_type_any_of < sizeofArray)
        return (char *)reporting_access_type_any_ofArray[reporting_access_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_reporting_access_type_any_of_e OpenAPI_reporting_access_type_any_of_FromString(char* reporting_access_type_any_of)
{
    int stringToReturn = 0;
    const char *reporting_access_type_any_ofArray[] =  { "NULL", "NR", "EUTRA_CONNECTED_TO_5GC", "NON_3GPP_CONNECTED_TO_5GC", "NR_LEO", "NR_MEO", "NR_GEO", "NR_OTHER_SAT" };
    size_t sizeofArray = sizeof(reporting_access_type_any_ofArray) / sizeof(reporting_access_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reporting_access_type_any_of, reporting_access_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

