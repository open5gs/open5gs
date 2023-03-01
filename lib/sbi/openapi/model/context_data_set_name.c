
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "context_data_set_name.h"

char* OpenAPI_context_data_set_name_ToString(OpenAPI_context_data_set_name_e context_data_set_name)
{
    const char *context_data_set_nameArray[] =  { "NULL", "AMF_3GPP", "AMF_NON_3GPP", "SDM_SUBSCRIPTIONS", "EE_SUBSCRIPTIONS", "SMSF_3GPP", "SMSF_NON_3GPP", "SUBS_TO_NOTIFY", "SMF_REG", "IP_SM_GW", "ROAMING_INFO", "PEI_INFO" };
    size_t sizeofArray = sizeof(context_data_set_nameArray) / sizeof(context_data_set_nameArray[0]);
    if (context_data_set_name < sizeofArray)
        return (char *)context_data_set_nameArray[context_data_set_name];
    else
        return (char *)"Unknown";
}

OpenAPI_context_data_set_name_e OpenAPI_context_data_set_name_FromString(char* context_data_set_name)
{
    int stringToReturn = 0;
    const char *context_data_set_nameArray[] =  { "NULL", "AMF_3GPP", "AMF_NON_3GPP", "SDM_SUBSCRIPTIONS", "EE_SUBSCRIPTIONS", "SMSF_3GPP", "SMSF_NON_3GPP", "SUBS_TO_NOTIFY", "SMF_REG", "IP_SM_GW", "ROAMING_INFO", "PEI_INFO" };
    size_t sizeofArray = sizeof(context_data_set_nameArray) / sizeof(context_data_set_nameArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(context_data_set_name, context_data_set_nameArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

