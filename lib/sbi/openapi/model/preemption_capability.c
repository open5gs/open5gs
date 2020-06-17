
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "preemption_capability.h"

char* OpenAPI_preemption_capability_ToString(OpenAPI_preemption_capability_e preemption_capability)
{
    const char *preemption_capabilityArray[] =  { "NULL", "NOT_PREEMPT", "MAY_PREEMPT" };
    size_t sizeofArray = sizeof(preemption_capabilityArray) / sizeof(preemption_capabilityArray[0]);
    if (preemption_capability < sizeofArray)
        return (char *)preemption_capabilityArray[preemption_capability];
    else
        return (char *)"Unknown";
}

OpenAPI_preemption_capability_e OpenAPI_preemption_capability_FromString(char* preemption_capability)
{
    int stringToReturn = 0;
    const char *preemption_capabilityArray[] =  { "NULL", "NOT_PREEMPT", "MAY_PREEMPT" };
    size_t sizeofArray = sizeof(preemption_capabilityArray) / sizeof(preemption_capabilityArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(preemption_capability, preemption_capabilityArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

