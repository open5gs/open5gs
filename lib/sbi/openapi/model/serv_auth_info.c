
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "serv_auth_info.h"

char* OpenAPI_serv_auth_info_ToString(OpenAPI_serv_auth_info_e serv_auth_info)
{
    const char *serv_auth_infoArray[] =  { "NULL", "TP_NOT_KNOWN", "TP_EXPIRED", "TP_NOT_YET_OCURRED" };
    size_t sizeofArray = sizeof(serv_auth_infoArray) / sizeof(serv_auth_infoArray[0]);
    if (serv_auth_info < sizeofArray)
        return (char *)serv_auth_infoArray[serv_auth_info];
    else
        return (char *)"Unknown";
}

OpenAPI_serv_auth_info_e OpenAPI_serv_auth_info_FromString(char* serv_auth_info)
{
    int stringToReturn = 0;
    const char *serv_auth_infoArray[] =  { "NULL", "TP_NOT_KNOWN", "TP_EXPIRED", "TP_NOT_YET_OCURRED" };
    size_t sizeofArray = sizeof(serv_auth_infoArray) / sizeof(serv_auth_infoArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(serv_auth_info, serv_auth_infoArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

