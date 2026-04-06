
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_for_sms_configuration.h"

char* OpenAPI_reachability_for_sms_configuration_ToString(OpenAPI_reachability_for_sms_configuration_e reachability_for_sms_configuration)
{
    const char *reachability_for_sms_configurationArray[] =  { "NULL", "REACHABILITY_FOR_SMS_OVER_NAS", "REACHABILITY_FOR_SMS_OVER_IP" };
    size_t sizeofArray = sizeof(reachability_for_sms_configurationArray) / sizeof(reachability_for_sms_configurationArray[0]);
    if (reachability_for_sms_configuration < sizeofArray)
        return (char *)reachability_for_sms_configurationArray[reachability_for_sms_configuration];
    else
        return (char *)"Unknown";
}

OpenAPI_reachability_for_sms_configuration_e OpenAPI_reachability_for_sms_configuration_FromString(char* reachability_for_sms_configuration)
{
    int stringToReturn = 0;
    const char *reachability_for_sms_configurationArray[] =  { "NULL", "REACHABILITY_FOR_SMS_OVER_NAS", "REACHABILITY_FOR_SMS_OVER_IP" };
    size_t sizeofArray = sizeof(reachability_for_sms_configurationArray) / sizeof(reachability_for_sms_configurationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(reachability_for_sms_configuration, reachability_for_sms_configurationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

