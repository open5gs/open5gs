
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_notif_method.h"

char* OpenAPI_af_notif_method_ToString(OpenAPI_af_notif_method_e af_notif_method)
{
    const char *af_notif_methodArray[] =  { "NULL", "EVENT_DETECTION", "ONE_TIME", "PERIODIC", "PDU_SESSION_RELEASE" };
    size_t sizeofArray = sizeof(af_notif_methodArray) / sizeof(af_notif_methodArray[0]);
    if (af_notif_method < sizeofArray)
        return (char *)af_notif_methodArray[af_notif_method];
    else
        return (char *)"Unknown";
}

OpenAPI_af_notif_method_e OpenAPI_af_notif_method_FromString(char* af_notif_method)
{
    int stringToReturn = 0;
    const char *af_notif_methodArray[] =  { "NULL", "EVENT_DETECTION", "ONE_TIME", "PERIODIC", "PDU_SESSION_RELEASE" };
    size_t sizeofArray = sizeof(af_notif_methodArray) / sizeof(af_notif_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(af_notif_method, af_notif_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

