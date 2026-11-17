
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roamer_in_out.h"

char* OpenAPI_roamer_in_out_ToString(OpenAPI_roamer_in_out_e roamer_in_out)
{
    const char *roamer_in_outArray[] =  { "NULL", "IN_BOUND", "OUT_BOUND" };
    size_t sizeofArray = sizeof(roamer_in_outArray) / sizeof(roamer_in_outArray[0]);
    if (roamer_in_out < sizeofArray)
        return (char *)roamer_in_outArray[roamer_in_out];
    else
        return (char *)"Unknown";
}

OpenAPI_roamer_in_out_e OpenAPI_roamer_in_out_FromString(char* roamer_in_out)
{
    int stringToReturn = 0;
    const char *roamer_in_outArray[] =  { "NULL", "IN_BOUND", "OUT_BOUND" };
    size_t sizeofArray = sizeof(roamer_in_outArray) / sizeof(roamer_in_outArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(roamer_in_out, roamer_in_outArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

