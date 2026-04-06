
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_mobility_order_criterion.h"

char* OpenAPI_ue_mobility_order_criterion_ToString(OpenAPI_ue_mobility_order_criterion_e ue_mobility_order_criterion)
{
    const char *ue_mobility_order_criterionArray[] =  { "NULL", "TIME_SLOT" };
    size_t sizeofArray = sizeof(ue_mobility_order_criterionArray) / sizeof(ue_mobility_order_criterionArray[0]);
    if (ue_mobility_order_criterion < sizeofArray)
        return (char *)ue_mobility_order_criterionArray[ue_mobility_order_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_ue_mobility_order_criterion_e OpenAPI_ue_mobility_order_criterion_FromString(char* ue_mobility_order_criterion)
{
    int stringToReturn = 0;
    const char *ue_mobility_order_criterionArray[] =  { "NULL", "TIME_SLOT" };
    size_t sizeofArray = sizeof(ue_mobility_order_criterionArray) / sizeof(ue_mobility_order_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(ue_mobility_order_criterion, ue_mobility_order_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

