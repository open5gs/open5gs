
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "positioning_method_mdt.h"

char* OpenAPI_positioning_method_mdt_ToString(OpenAPI_positioning_method_mdt_e positioning_method_mdt)
{
    const char *positioning_method_mdtArray[] =  { "NULL", "GNSS", "E_CELL_ID" };
    size_t sizeofArray = sizeof(positioning_method_mdtArray) / sizeof(positioning_method_mdtArray[0]);
    if (positioning_method_mdt < sizeofArray)
        return (char *)positioning_method_mdtArray[positioning_method_mdt];
    else
        return (char *)"Unknown";
}

OpenAPI_positioning_method_mdt_e OpenAPI_positioning_method_mdt_FromString(char* positioning_method_mdt)
{
    int stringToReturn = 0;
    const char *positioning_method_mdtArray[] =  { "NULL", "GNSS", "E_CELL_ID" };
    size_t sizeofArray = sizeof(positioning_method_mdtArray) / sizeof(positioning_method_mdtArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(positioning_method_mdt, positioning_method_mdtArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

