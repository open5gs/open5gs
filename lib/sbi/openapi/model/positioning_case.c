
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "positioning_case.h"

char* OpenAPI_positioning_case_ToString(OpenAPI_positioning_case_e positioning_case)
{
    const char *positioning_caseArray[] =  { "NULL", "NG_RAN_ASSISTED_POSITIONING" };
    size_t sizeofArray = sizeof(positioning_caseArray) / sizeof(positioning_caseArray[0]);
    if (positioning_case < sizeofArray)
        return (char *)positioning_caseArray[positioning_case];
    else
        return (char *)"Unknown";
}

OpenAPI_positioning_case_e OpenAPI_positioning_case_FromString(char* positioning_case)
{
    int stringToReturn = 0;
    const char *positioning_caseArray[] =  { "NULL", "NG_RAN_ASSISTED_POSITIONING" };
    size_t sizeofArray = sizeof(positioning_caseArray) / sizeof(positioning_caseArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(positioning_case, positioning_caseArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

