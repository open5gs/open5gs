
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "association_type_any_of.h"

char* OpenAPI_association_type_any_of_ToString(OpenAPI_association_type_any_of_e association_type_any_of)
{
    const char *association_type_any_ofArray[] =  { "NULL", "IMEI_CHANGE", "IMEISV_CHANGE" };
    size_t sizeofArray = sizeof(association_type_any_ofArray) / sizeof(association_type_any_ofArray[0]);
    if (association_type_any_of < sizeofArray)
        return (char *)association_type_any_ofArray[association_type_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_association_type_any_of_e OpenAPI_association_type_any_of_FromString(char* association_type_any_of)
{
    int stringToReturn = 0;
    const char *association_type_any_ofArray[] =  { "NULL", "IMEI_CHANGE", "IMEISV_CHANGE" };
    size_t sizeofArray = sizeof(association_type_any_ofArray) / sizeof(association_type_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(association_type_any_of, association_type_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

