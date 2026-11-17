
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_transfer_type.h"

char* OpenAPI_ue_transfer_type_ToString(OpenAPI_ue_transfer_type_e ue_transfer_type)
{
    const char *ue_transfer_typeArray[] =  { "NULL", "INTRA_UE", "INTER_UE" };
    size_t sizeofArray = sizeof(ue_transfer_typeArray) / sizeof(ue_transfer_typeArray[0]);
    if (ue_transfer_type < sizeofArray)
        return (char *)ue_transfer_typeArray[ue_transfer_type];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_transfer_type_e OpenAPI_ue_transfer_type_FromString(char* ue_transfer_type)
{
    int stringToReturn = 0;
    const char *ue_transfer_typeArray[] =  { "NULL", "INTRA_UE", "INTER_UE" };
    size_t sizeofArray = sizeof(ue_transfer_typeArray) / sizeof(ue_transfer_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_transfer_type, ue_transfer_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

