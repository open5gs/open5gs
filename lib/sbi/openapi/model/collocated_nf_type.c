
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "collocated_nf_type.h"

char* OpenAPI_collocated_nf_type_ToString(OpenAPI_collocated_nf_type_e collocated_nf_type)
{
    const char *collocated_nf_typeArray[] =  { "NULL", "UPF", "SMF", "MB_UPF", "MB_SMF" };
    size_t sizeofArray = sizeof(collocated_nf_typeArray) / sizeof(collocated_nf_typeArray[0]);
    if (collocated_nf_type < sizeofArray)
        return (char *)collocated_nf_typeArray[collocated_nf_type];
    else
        return (char *)"Unknown";
}

OpenAPI_collocated_nf_type_e OpenAPI_collocated_nf_type_FromString(char* collocated_nf_type)
{
    int stringToReturn = 0;
    const char *collocated_nf_typeArray[] =  { "NULL", "UPF", "SMF", "MB_UPF", "MB_SMF" };
    size_t sizeofArray = sizeof(collocated_nf_typeArray) / sizeof(collocated_nf_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(collocated_nf_type, collocated_nf_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

