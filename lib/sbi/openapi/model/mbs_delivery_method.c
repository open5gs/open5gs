
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_delivery_method.h"

char* OpenAPI_mbs_delivery_method_ToString(OpenAPI_mbs_delivery_method_e mbs_delivery_method)
{
    const char *mbs_delivery_methodArray[] =  { "NULL", "SHARED", "INDIVIDUAL" };
    size_t sizeofArray = sizeof(mbs_delivery_methodArray) / sizeof(mbs_delivery_methodArray[0]);
    if (mbs_delivery_method < sizeofArray)
        return (char *)mbs_delivery_methodArray[mbs_delivery_method];
    else
        return (char *)"Unknown";
}

OpenAPI_mbs_delivery_method_e OpenAPI_mbs_delivery_method_FromString(char* mbs_delivery_method)
{
    int stringToReturn = 0;
    const char *mbs_delivery_methodArray[] =  { "NULL", "SHARED", "INDIVIDUAL" };
    size_t sizeofArray = sizeof(mbs_delivery_methodArray) / sizeof(mbs_delivery_methodArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(mbs_delivery_method, mbs_delivery_methodArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

