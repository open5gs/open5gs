
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sip_forking_indication.h"

char* OpenAPI_sip_forking_indication_ToString(OpenAPI_sip_forking_indication_e sip_forking_indication)
{
    const char *sip_forking_indicationArray[] =  { "NULL", "SINGLE_DIALOGUE", "SEVERAL_DIALOGUES" };
    size_t sizeofArray = sizeof(sip_forking_indicationArray) / sizeof(sip_forking_indicationArray[0]);
    if (sip_forking_indication < sizeofArray)
        return (char *)sip_forking_indicationArray[sip_forking_indication];
    else
        return (char *)"Unknown";
}

OpenAPI_sip_forking_indication_e OpenAPI_sip_forking_indication_FromString(char* sip_forking_indication)
{
    int stringToReturn = 0;
    const char *sip_forking_indicationArray[] =  { "NULL", "SINGLE_DIALOGUE", "SEVERAL_DIALOGUES" };
    size_t sizeofArray = sizeof(sip_forking_indicationArray) / sizeof(sip_forking_indicationArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sip_forking_indication, sip_forking_indicationArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

