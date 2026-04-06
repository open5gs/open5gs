
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udm_data_to_resynchronize.h"

char* OpenAPI_udm_data_to_resynchronize_ToString(OpenAPI_udm_data_to_resynchronize_e udm_data_to_resynchronize)
{
    const char *udm_data_to_resynchronizeArray[] =  { "NULL", "UDM_UECM_REGISTRATION", "UDM_SDM_SUBSCRIBE", "UDM_EE_SUBSCRIBE" };
    size_t sizeofArray = sizeof(udm_data_to_resynchronizeArray) / sizeof(udm_data_to_resynchronizeArray[0]);
    if (udm_data_to_resynchronize < sizeofArray)
        return (char *)udm_data_to_resynchronizeArray[udm_data_to_resynchronize];
    else
        return (char *)"Unknown";
}

OpenAPI_udm_data_to_resynchronize_e OpenAPI_udm_data_to_resynchronize_FromString(char* udm_data_to_resynchronize)
{
    int stringToReturn = 0;
    const char *udm_data_to_resynchronizeArray[] =  { "NULL", "UDM_UECM_REGISTRATION", "UDM_SDM_SUBSCRIBE", "UDM_EE_SUBSCRIBE" };
    size_t sizeofArray = sizeof(udm_data_to_resynchronizeArray) / sizeof(udm_data_to_resynchronizeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(udm_data_to_resynchronize, udm_data_to_resynchronizeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

