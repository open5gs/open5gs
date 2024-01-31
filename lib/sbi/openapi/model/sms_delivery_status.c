
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_delivery_status.h"

char* OpenAPI_sms_delivery_status_ToString(OpenAPI_sms_delivery_status_e sms_delivery_status)
{
    const char *sms_delivery_statusArray[] =  { "NULL", "SMS_DELIVERY_PENDING", "SMS_DELIVERY_COMPLETED", "SMS_DELIVERY_FAILED", "SMS_DELIVERY_SMSF_ACCEPTED" };
    size_t sizeofArray = sizeof(sms_delivery_statusArray) / sizeof(sms_delivery_statusArray[0]);
    if (sms_delivery_status < sizeofArray)
        return (char *)sms_delivery_statusArray[sms_delivery_status];
    else
        return (char *)"Unknown";
}

OpenAPI_sms_delivery_status_e OpenAPI_sms_delivery_status_FromString(char* sms_delivery_status)
{
    int stringToReturn = 0;
    const char *sms_delivery_statusArray[] =  { "NULL", "SMS_DELIVERY_PENDING", "SMS_DELIVERY_COMPLETED", "SMS_DELIVERY_FAILED", "SMS_DELIVERY_SMSF_ACCEPTED" };
    size_t sizeofArray = sizeof(sms_delivery_statusArray) / sizeof(sms_delivery_statusArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sms_delivery_status, sms_delivery_statusArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

