
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_repl_type.h"

char* OpenAPI_slice_repl_type_ToString(OpenAPI_slice_repl_type_e slice_repl_type)
{
    const char *slice_repl_typeArray[] =  { "NULL", "INITIATE_AF_REQ_SLICE_REP", "TERMINATE_AF_REQ_SLICE_REP" };
    size_t sizeofArray = sizeof(slice_repl_typeArray) / sizeof(slice_repl_typeArray[0]);
    if (slice_repl_type < sizeofArray)
        return (char *)slice_repl_typeArray[slice_repl_type];
    else
        return (char *)"Unknown";
}

OpenAPI_slice_repl_type_e OpenAPI_slice_repl_type_FromString(char* slice_repl_type)
{
    int stringToReturn = 0;
    const char *slice_repl_typeArray[] =  { "NULL", "INITIATE_AF_REQ_SLICE_REP", "TERMINATE_AF_REQ_SLICE_REP" };
    size_t sizeofArray = sizeof(slice_repl_typeArray) / sizeof(slice_repl_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(slice_repl_type, slice_repl_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

