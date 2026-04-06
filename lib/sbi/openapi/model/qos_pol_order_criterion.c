
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_pol_order_criterion.h"

char* OpenAPI_qos_pol_order_criterion_ToString(OpenAPI_qos_pol_order_criterion_e qos_pol_order_criterion)
{
    const char *qos_pol_order_criterionArray[] =  { "NULL", "QOE", "USAGE_DURATION", "NUMBER_OF_USAGES" };
    size_t sizeofArray = sizeof(qos_pol_order_criterionArray) / sizeof(qos_pol_order_criterionArray[0]);
    if (qos_pol_order_criterion < sizeofArray)
        return (char *)qos_pol_order_criterionArray[qos_pol_order_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_qos_pol_order_criterion_e OpenAPI_qos_pol_order_criterion_FromString(char* qos_pol_order_criterion)
{
    int stringToReturn = 0;
    const char *qos_pol_order_criterionArray[] =  { "NULL", "QOE", "USAGE_DURATION", "NUMBER_OF_USAGES" };
    size_t sizeofArray = sizeof(qos_pol_order_criterionArray) / sizeof(qos_pol_order_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(qos_pol_order_criterion, qos_pol_order_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

