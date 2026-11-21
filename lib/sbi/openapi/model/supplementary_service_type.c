
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supplementary_service_type.h"

char* OpenAPI_supplementary_service_type_ToString(OpenAPI_supplementary_service_type_e supplementary_service_type)
{
    const char *supplementary_service_typeArray[] =  { "NULL", "OIP", "OIR", "TIP", "TIR", "HOLD", "CB", "CDIV", "CW", "MWI", "CONF", "FA", "CCBS", "CCNR", "MCID", "CAT", "CUG", "PNM", "CRS", "ECT" };
    size_t sizeofArray = sizeof(supplementary_service_typeArray) / sizeof(supplementary_service_typeArray[0]);
    if (supplementary_service_type < sizeofArray)
        return (char *)supplementary_service_typeArray[supplementary_service_type];
    else
        return (char *)"Unknown";
}

OpenAPI_supplementary_service_type_e OpenAPI_supplementary_service_type_FromString(char* supplementary_service_type)
{
    int stringToReturn = 0;
    const char *supplementary_service_typeArray[] =  { "NULL", "OIP", "OIR", "TIP", "TIR", "HOLD", "CB", "CDIV", "CW", "MWI", "CONF", "FA", "CCBS", "CCNR", "MCID", "CAT", "CUG", "PNM", "CRS", "ECT" };
    size_t sizeofArray = sizeof(supplementary_service_typeArray) / sizeof(supplementary_service_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(supplementary_service_type, supplementary_service_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

