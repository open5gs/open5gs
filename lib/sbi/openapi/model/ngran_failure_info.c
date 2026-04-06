
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ngran_failure_info.h"

char* OpenAPI_ngran_failure_info_ToString(OpenAPI_ngran_failure_info_e ngran_failure_info)
{
    const char *ngran_failure_infoArray[] =  { "NULL", "NG_RAN_NOT_REACHABLE", "NG_RAN_FAILURE_WITH_RESTART", "NG_RAN_FAILURE_WITHOUT_RESTART" };
    size_t sizeofArray = sizeof(ngran_failure_infoArray) / sizeof(ngran_failure_infoArray[0]);
    if (ngran_failure_info < sizeofArray)
        return (char *)ngran_failure_infoArray[ngran_failure_info];
    else
        return (char *)"Unknown";
}

OpenAPI_ngran_failure_info_e OpenAPI_ngran_failure_info_FromString(char* ngran_failure_info)
{
    int stringToReturn = 0;
    const char *ngran_failure_infoArray[] =  { "NULL", "NG_RAN_NOT_REACHABLE", "NG_RAN_FAILURE_WITH_RESTART", "NG_RAN_FAILURE_WITHOUT_RESTART" };
    size_t sizeofArray = sizeof(ngran_failure_infoArray) / sizeof(ngran_failure_infoArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ngran_failure_info, ngran_failure_infoArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

