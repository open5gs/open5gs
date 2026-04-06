
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ims_event.h"

char* OpenAPI_ims_event_ToString(OpenAPI_ims_event_e ims_event)
{
    const char *ims_eventArray[] =  { "NULL", "ADC_ESTABLISHMENT", "ADC_RELEASE", "BDC_ESTABLISHMENT", "BDC_RELEASE", "ADC_IWK_ESTABLISHMENT" };
    size_t sizeofArray = sizeof(ims_eventArray) / sizeof(ims_eventArray[0]);
    if (ims_event < sizeofArray)
        return (char *)ims_eventArray[ims_event];
    else
        return (char *)"Unknown";
}

OpenAPI_ims_event_e OpenAPI_ims_event_FromString(char* ims_event)
{
    int stringToReturn = 0;
    const char *ims_eventArray[] =  { "NULL", "ADC_ESTABLISHMENT", "ADC_RELEASE", "BDC_ESTABLISHMENT", "BDC_RELEASE", "ADC_IWK_ESTABLISHMENT" };
    size_t sizeofArray = sizeof(ims_eventArray) / sizeof(ims_eventArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ims_event, ims_eventArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

