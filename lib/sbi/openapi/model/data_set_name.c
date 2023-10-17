
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_set_name.h"

char* OpenAPI_data_set_name_ToString(OpenAPI_data_set_name_e data_set_name)
{
    const char *data_set_nameArray[] =  { "NULL", "AM", "SMF_SEL", "UEC_SMF", "UEC_SMSF", "SMS_SUB", "SM", "TRACE", "SMS_MNG", "LCS_PRIVACY", "LCS_MO", "UEC_AMF", "V2X", "LCS_BCA", "PROSE", "UC", "MBS" };
    size_t sizeofArray = sizeof(data_set_nameArray) / sizeof(data_set_nameArray[0]);
    if (data_set_name < sizeofArray)
        return (char *)data_set_nameArray[data_set_name];
    else
        return (char *)"Unknown";
}

OpenAPI_data_set_name_e OpenAPI_data_set_name_FromString(char* data_set_name)
{
    int stringToReturn = 0;
    const char *data_set_nameArray[] =  { "NULL", "AM", "SMF_SEL", "UEC_SMF", "UEC_SMSF", "SMS_SUB", "SM", "TRACE", "SMS_MNG", "LCS_PRIVACY", "LCS_MO", "UEC_AMF", "V2X", "LCS_BCA", "PROSE", "UC", "MBS" };
    size_t sizeofArray = sizeof(data_set_nameArray) / sizeof(data_set_nameArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(data_set_name, data_set_nameArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

