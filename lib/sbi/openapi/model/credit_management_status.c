
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "credit_management_status.h"

char* OpenAPI_credit_management_status_ToString(OpenAPI_credit_management_status_e credit_management_status)
{
    const char *credit_management_statusArray[] =  { "NULL", "END_USER_SER_DENIED", "CREDIT_CTRL_NOT_APP", "AUTH_REJECTED", "USER_UNKNOWN", "RATING_FAILED" };
    size_t sizeofArray = sizeof(credit_management_statusArray) / sizeof(credit_management_statusArray[0]);
    if (credit_management_status < sizeofArray)
        return (char *)credit_management_statusArray[credit_management_status];
    else
        return (char *)"Unknown";
}

OpenAPI_credit_management_status_e OpenAPI_credit_management_status_FromString(char* credit_management_status)
{
    int stringToReturn = 0;
    const char *credit_management_statusArray[] =  { "NULL", "END_USER_SER_DENIED", "CREDIT_CTRL_NOT_APP", "AUTH_REJECTED", "USER_UNKNOWN", "RATING_FAILED" };
    size_t sizeofArray = sizeof(credit_management_statusArray) / sizeof(credit_management_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(credit_management_status, credit_management_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

