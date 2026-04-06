
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_ind.h"

char* OpenAPI_data_ind_ToString(OpenAPI_data_ind_e data_ind)
{
    const char *data_indArray[] =  { "NULL", "PFD", "IPTV", "BDT", "SVC_PARAM", "AM", "DNAI_EAS", "REQ_QOS", "ECS", "N3GDEV" };
    size_t sizeofArray = sizeof(data_indArray) / sizeof(data_indArray[0]);
    if (data_ind < sizeofArray)
        return (char *)data_indArray[data_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_data_ind_e OpenAPI_data_ind_FromString(char* data_ind)
{
    int stringToReturn = 0;
    const char *data_indArray[] =  { "NULL", "PFD", "IPTV", "BDT", "SVC_PARAM", "AM", "DNAI_EAS", "REQ_QOS", "ECS", "N3GDEV" };
    size_t sizeofArray = sizeof(data_indArray) / sizeof(data_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(data_ind, data_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

