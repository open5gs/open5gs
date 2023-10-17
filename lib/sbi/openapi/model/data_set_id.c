
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_set_id.h"

char* OpenAPI_data_set_id_ToString(OpenAPI_data_set_id_e data_set_id)
{
    const char *data_set_idArray[] =  { "NULL", "SUBSCRIPTION", "POLICY", "EXPOSURE", "APPLICATION", "A_PFD", "A_AFTI", "A_IPTV", "A_BDT", "A_SPD", "A_EASD", "A_AMI", "P_UE", "P_SCD", "P_BDT", "P_PLMNUE", "P_NSSCD" };
    size_t sizeofArray = sizeof(data_set_idArray) / sizeof(data_set_idArray[0]);
    if (data_set_id < sizeofArray)
        return (char *)data_set_idArray[data_set_id];
    else
        return (char *)"Unknown";
}

OpenAPI_data_set_id_e OpenAPI_data_set_id_FromString(char* data_set_id)
{
    int stringToReturn = 0;
    const char *data_set_idArray[] =  { "NULL", "SUBSCRIPTION", "POLICY", "EXPOSURE", "APPLICATION", "A_PFD", "A_AFTI", "A_IPTV", "A_BDT", "A_SPD", "A_EASD", "A_AMI", "P_UE", "P_SCD", "P_BDT", "P_PLMNUE", "P_NSSCD" };
    size_t sizeofArray = sizeof(data_set_idArray) / sizeof(data_set_idArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(data_set_id, data_set_idArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

