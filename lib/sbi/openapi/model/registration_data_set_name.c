
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_data_set_name.h"

char* OpenAPI_registration_data_set_name_ToString(OpenAPI_registration_data_set_name_e registration_data_set_name)
{
    const char *registration_data_set_nameArray[] =  { "NULL", "AMF_3GPP", "AMF_NON_3GPP", "SMF_PDU_SESSIONS", "SMSF_3GPP", "SMSF_NON_3GPP" };
    size_t sizeofArray = sizeof(registration_data_set_nameArray) / sizeof(registration_data_set_nameArray[0]);
    if (registration_data_set_name < sizeofArray)
        return (char *)registration_data_set_nameArray[registration_data_set_name];
    else
        return (char *)"Unknown";
}

OpenAPI_registration_data_set_name_e OpenAPI_registration_data_set_name_FromString(char* registration_data_set_name)
{
    int stringToReturn = 0;
    const char *registration_data_set_nameArray[] =  { "NULL", "AMF_3GPP", "AMF_NON_3GPP", "SMF_PDU_SESSIONS", "SMSF_3GPP", "SMSF_NON_3GPP" };
    size_t sizeofArray = sizeof(registration_data_set_nameArray) / sizeof(registration_data_set_nameArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(registration_data_set_name, registration_data_set_nameArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

