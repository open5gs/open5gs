
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_ind_any_of.h"

char* OpenAPI_data_ind_any_of_ToString(OpenAPI_data_ind_any_of_e data_ind_any_of)
{
    const char *data_ind_any_ofArray[] =  { "NULL", "PFD", "IPTV", "BDT", "SVC_PARAM" };
    size_t sizeofArray = sizeof(data_ind_any_ofArray) / sizeof(data_ind_any_ofArray[0]);
    if (data_ind_any_of < sizeofArray)
        return (char *)data_ind_any_ofArray[data_ind_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_data_ind_any_of_e OpenAPI_data_ind_any_of_FromString(char* data_ind_any_of)
{
    int stringToReturn = 0;
    const char *data_ind_any_ofArray[] =  { "NULL", "PFD", "IPTV", "BDT", "SVC_PARAM" };
    size_t sizeofArray = sizeof(data_ind_any_ofArray) / sizeof(data_ind_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(data_ind_any_of, data_ind_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

