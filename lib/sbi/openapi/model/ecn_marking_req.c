
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecn_marking_req.h"

char* OpenAPI_ecn_marking_req_ToString(OpenAPI_ecn_marking_req_e ecn_marking_req)
{
    const char *ecn_marking_reqArray[] =  { "NULL", "UL", "DL", "BOTH", "STOP" };
    size_t sizeofArray = sizeof(ecn_marking_reqArray) / sizeof(ecn_marking_reqArray[0]);
    if (ecn_marking_req < sizeofArray)
        return (char *)ecn_marking_reqArray[ecn_marking_req];
    else
        return (char *)"Unknown";
}

OpenAPI_ecn_marking_req_e OpenAPI_ecn_marking_req_FromString(char* ecn_marking_req)
{
    int stringToReturn = 0;
    const char *ecn_marking_reqArray[] =  { "NULL", "UL", "DL", "BOTH", "STOP" };
    size_t sizeofArray = sizeof(ecn_marking_reqArray) / sizeof(ecn_marking_reqArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ecn_marking_req, ecn_marking_reqArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

