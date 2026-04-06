
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fl_capability_type.h"

char* OpenAPI_fl_capability_type_ToString(OpenAPI_fl_capability_type_e fl_capability_type)
{
    const char *fl_capability_typeArray[] =  { "NULL", "FL_SERVER", "FL_CLIENT", "FL_SERVER_AND_CLIENT" };
    size_t sizeofArray = sizeof(fl_capability_typeArray) / sizeof(fl_capability_typeArray[0]);
    if (fl_capability_type < sizeofArray)
        return (char *)fl_capability_typeArray[fl_capability_type];
    else
        return (char *)"Unknown";
}

OpenAPI_fl_capability_type_e OpenAPI_fl_capability_type_FromString(char* fl_capability_type)
{
    int stringToReturn = 0;
    const char *fl_capability_typeArray[] =  { "NULL", "FL_SERVER", "FL_CLIENT", "FL_SERVER_AND_CLIENT" };
    size_t sizeofArray = sizeof(fl_capability_typeArray) / sizeof(fl_capability_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(fl_capability_type, fl_capability_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

