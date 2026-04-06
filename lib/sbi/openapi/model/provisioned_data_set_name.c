
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "provisioned_data_set_name.h"

char* OpenAPI_provisioned_data_set_name_ToString(OpenAPI_provisioned_data_set_name_e provisioned_data_set_name)
{
    const char *provisioned_data_set_nameArray[] =  { "NULL", "AM", "SMF_SEL", "SMS_SUB", "SM", "TRACE", "SMS_MNG", "LCS_PRIVACY", "LCS_MO", "LCS_BCA", "LCS_SUB", "V2X", "PROSE", "ODB", "EE_PROF", "PP_PROF", "NIDD_AUTH", "USER_CONSENT", "MBS", "PP_DATA", "A2X", "RANGINGSL_PRIVACY" };
    size_t sizeofArray = sizeof(provisioned_data_set_nameArray) / sizeof(provisioned_data_set_nameArray[0]);
    if (provisioned_data_set_name < sizeofArray)
        return (char *)provisioned_data_set_nameArray[provisioned_data_set_name];
    else
        return (char *)"Unknown";
}

OpenAPI_provisioned_data_set_name_e OpenAPI_provisioned_data_set_name_FromString(char* provisioned_data_set_name)
{
    int stringToReturn = 0;
    const char *provisioned_data_set_nameArray[] =  { "NULL", "AM", "SMF_SEL", "SMS_SUB", "SM", "TRACE", "SMS_MNG", "LCS_PRIVACY", "LCS_MO", "LCS_BCA", "LCS_SUB", "V2X", "PROSE", "ODB", "EE_PROF", "PP_PROF", "NIDD_AUTH", "USER_CONSENT", "MBS", "PP_DATA", "A2X", "RANGINGSL_PRIVACY" };
    size_t sizeofArray = sizeof(provisioned_data_set_nameArray) / sizeof(provisioned_data_set_nameArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(provisioned_data_set_name, provisioned_data_set_nameArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

