
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eps_interworking_indication.h"

char* OpenAPI_eps_interworking_indication_ToString(OpenAPI_eps_interworking_indication_e eps_interworking_indication)
{
    const char *eps_interworking_indicationArray[] =  { "NULL", "NONE", "WITH_N26", "WITHOUT_N26", "IWK_NON_3GPP" };
    size_t sizeofArray = sizeof(eps_interworking_indicationArray) / sizeof(eps_interworking_indicationArray[0]);
    if (eps_interworking_indication < sizeofArray)
        return (char *)eps_interworking_indicationArray[eps_interworking_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_eps_interworking_indication_e OpenAPI_eps_interworking_indication_FromString(char* eps_interworking_indication)
{
    int stringToReturn = 0;
    const char *eps_interworking_indicationArray[] =  { "NULL", "NONE", "WITH_N26", "WITHOUT_N26", "IWK_NON_3GPP" };
    size_t sizeofArray = sizeof(eps_interworking_indicationArray) / sizeof(eps_interworking_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(eps_interworking_indication, eps_interworking_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

