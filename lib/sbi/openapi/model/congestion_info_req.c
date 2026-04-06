
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "congestion_info_req.h"

char* OpenAPI_congestion_info_req_ToString(OpenAPI_congestion_info_req_e congestion_info_req)
{
    const char *congestion_info_reqArray[] =  { "NULL", "UL", "DL", "BOTH", "STOP" };
    size_t sizeofArray = sizeof(congestion_info_reqArray) / sizeof(congestion_info_reqArray[0]);
    if (congestion_info_req < sizeofArray)
        return (char *)congestion_info_reqArray[congestion_info_req];
    else
        return (char *)"Unknown";
}

OpenAPI_congestion_info_req_e OpenAPI_congestion_info_req_FromString(char* congestion_info_req)
{
    int stringToReturn = 0;
    const char *congestion_info_reqArray[] =  { "NULL", "UL", "DL", "BOTH", "STOP" };
    size_t sizeofArray = sizeof(congestion_info_reqArray) / sizeof(congestion_info_reqArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(congestion_info_req, congestion_info_reqArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

