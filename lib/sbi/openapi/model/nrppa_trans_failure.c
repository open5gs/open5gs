
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrppa_trans_failure.h"

char* OpenAPI_nrppa_trans_failure_ToString(OpenAPI_nrppa_trans_failure_e nrppa_trans_failure)
{
    const char *nrppa_trans_failureArray[] =  { "NULL", "NG_RAN_NODE_NOT_REACHABLE", "NG_RAN_NODE_UNKNONWN" };
    size_t sizeofArray = sizeof(nrppa_trans_failureArray) / sizeof(nrppa_trans_failureArray[0]);
    if (nrppa_trans_failure < sizeofArray)
        return (char *)nrppa_trans_failureArray[nrppa_trans_failure];
    else
        return (char *)"Unknown";
}

OpenAPI_nrppa_trans_failure_e OpenAPI_nrppa_trans_failure_FromString(char* nrppa_trans_failure)
{
    int stringToReturn = 0;
    const char *nrppa_trans_failureArray[] =  { "NULL", "NG_RAN_NODE_NOT_REACHABLE", "NG_RAN_NODE_UNKNONWN" };
    size_t sizeofArray = sizeof(nrppa_trans_failureArray) / sizeof(nrppa_trans_failureArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nrppa_trans_failure, nrppa_trans_failureArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

