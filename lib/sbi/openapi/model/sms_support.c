
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_support.h"

char* OpenAPI_sms_support_ToString(OpenAPI_sms_support_e sms_support)
{
    const char *sms_supportArray[] =  { "NULL", "3GPP", "NON_3GPP", "BOTH", "NONE" };
    size_t sizeofArray = sizeof(sms_supportArray) / sizeof(sms_supportArray[0]);
    if (sms_support < sizeofArray)
        return (char *)sms_supportArray[sms_support];
    else
        return (char *)"Unknown";
}

OpenAPI_sms_support_e OpenAPI_sms_support_FromString(char* sms_support)
{
    int stringToReturn = 0;
    const char *sms_supportArray[] =  { "NULL", "3GPP", "NON_3GPP", "BOTH", "NONE" };
    size_t sizeofArray = sizeof(sms_supportArray) / sizeof(sms_supportArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sms_support, sms_supportArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

