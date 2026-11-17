
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_indication.h"

char* OpenAPI_sms_indication_ToString(OpenAPI_sms_indication_e sms_indication)
{
    const char *sms_indicationArray[] =  { "NULL", "SMS_SUPPORTED", "SMS_NOT_SUPPORTED" };
    size_t sizeofArray = sizeof(sms_indicationArray) / sizeof(sms_indicationArray[0]);
    if (sms_indication < sizeofArray)
        return (char *)sms_indicationArray[sms_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_sms_indication_e OpenAPI_sms_indication_FromString(char* sms_indication)
{
    int stringToReturn = 0;
    const char *sms_indicationArray[] =  { "NULL", "SMS_SUPPORTED", "SMS_NOT_SUPPORTED" };
    size_t sizeofArray = sizeof(sms_indicationArray) / sizeof(sms_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sms_indication, sms_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

