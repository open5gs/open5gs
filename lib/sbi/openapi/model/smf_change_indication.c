
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_change_indication.h"

char* OpenAPI_smf_change_indication_ToString(OpenAPI_smf_change_indication_e smf_change_indication)
{
    const char *smf_change_indicationArray[] =  { "NULL", "CHANGED", "REMOVED" };
    size_t sizeofArray = sizeof(smf_change_indicationArray) / sizeof(smf_change_indicationArray[0]);
    if (smf_change_indication < sizeofArray)
        return (char *)smf_change_indicationArray[smf_change_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_smf_change_indication_e OpenAPI_smf_change_indication_FromString(char* smf_change_indication)
{
    int stringToReturn = 0;
    const char *smf_change_indicationArray[] =  { "NULL", "CHANGED", "REMOVED" };
    size_t sizeofArray = sizeof(smf_change_indicationArray) / sizeof(smf_change_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(smf_change_indication, smf_change_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

