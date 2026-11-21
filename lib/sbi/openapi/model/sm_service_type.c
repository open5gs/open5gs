
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_service_type.h"

char* OpenAPI_sm_service_type_ToString(OpenAPI_sm_service_type_e sm_service_type)
{
    const char *sm_service_typeArray[] =  { "NULL", "VAS4SMS_SHORT_MESSAGE_CONTENT_PROCESSING", "VAS4SMS_SHORT_MESSAGE_FORWARDING", "VAS4SMS_SHORT_MESSAGE_FORWARDING_MULTIPLE_SUBSCRIPTIONS", "VAS4SMS_SHORT_MESSAGE_FILTERING", "VAS4SMS_SHORT_MESSAGE_RECEIPT", "VAS4SMS_SHORT_MESSAGE_NETWORK_STORAGE", "VAS4SMS_SHORT_MESSAGE_TO_MULTIPLE_DESTINATIONS", "VAS4SMS_SHORT_MESSAGE_VIRTUAL_PRIVATE_NETWORK_VPN", "VAS4SMS_SHORT_MESSAGE_AUTO_REPLY", "VAS4SMS_SHORT_MESSAGE_PERSONAL_SIGNATURE", "VAS4SMS_SHORT_MESSAGE_DEFERRED_DELIVERY" };
    size_t sizeofArray = sizeof(sm_service_typeArray) / sizeof(sm_service_typeArray[0]);
    if (sm_service_type < sizeofArray)
        return (char *)sm_service_typeArray[sm_service_type];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_service_type_e OpenAPI_sm_service_type_FromString(char* sm_service_type)
{
    int stringToReturn = 0;
    const char *sm_service_typeArray[] =  { "NULL", "VAS4SMS_SHORT_MESSAGE_CONTENT_PROCESSING", "VAS4SMS_SHORT_MESSAGE_FORWARDING", "VAS4SMS_SHORT_MESSAGE_FORWARDING_MULTIPLE_SUBSCRIPTIONS", "VAS4SMS_SHORT_MESSAGE_FILTERING", "VAS4SMS_SHORT_MESSAGE_RECEIPT", "VAS4SMS_SHORT_MESSAGE_NETWORK_STORAGE", "VAS4SMS_SHORT_MESSAGE_TO_MULTIPLE_DESTINATIONS", "VAS4SMS_SHORT_MESSAGE_VIRTUAL_PRIVATE_NETWORK_VPN", "VAS4SMS_SHORT_MESSAGE_AUTO_REPLY", "VAS4SMS_SHORT_MESSAGE_PERSONAL_SIGNATURE", "VAS4SMS_SHORT_MESSAGE_DEFERRED_DELIVERY" };
    size_t sizeofArray = sizeof(sm_service_typeArray) / sizeof(sm_service_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_service_type, sm_service_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

