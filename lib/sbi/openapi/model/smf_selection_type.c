
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_selection_type.h"

char* OpenAPI_smf_selection_type_ToString(OpenAPI_smf_selection_type_e smf_selection_type)
{
    const char *smf_selection_typeArray[] =  { "NULL", "CURRENT_PDU_SESSION", "NEXT_PDU_SESSION" };
    size_t sizeofArray = sizeof(smf_selection_typeArray) / sizeof(smf_selection_typeArray[0]);
    if (smf_selection_type < sizeofArray)
        return (char *)smf_selection_typeArray[smf_selection_type];
    else
        return (char *)"Unknown";
}

OpenAPI_smf_selection_type_e OpenAPI_smf_selection_type_FromString(char* smf_selection_type)
{
    int stringToReturn = 0;
    const char *smf_selection_typeArray[] =  { "NULL", "CURRENT_PDU_SESSION", "NEXT_PDU_SESSION" };
    size_t sizeofArray = sizeof(smf_selection_typeArray) / sizeof(smf_selection_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(smf_selection_type, smf_selection_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

