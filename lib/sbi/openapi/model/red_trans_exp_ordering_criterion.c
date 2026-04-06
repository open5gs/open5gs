
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "red_trans_exp_ordering_criterion.h"

char* OpenAPI_red_trans_exp_ordering_criterion_ToString(OpenAPI_red_trans_exp_ordering_criterion_e red_trans_exp_ordering_criterion)
{
    const char *red_trans_exp_ordering_criterionArray[] =  { "NULL", "TIME_SLOT_START", "RED_TRANS_EXP" };
    size_t sizeofArray = sizeof(red_trans_exp_ordering_criterionArray) / sizeof(red_trans_exp_ordering_criterionArray[0]);
    if (red_trans_exp_ordering_criterion < sizeofArray)
        return (char *)red_trans_exp_ordering_criterionArray[red_trans_exp_ordering_criterion];
    else
        return (char *)"Unknown";
}

OpenAPI_red_trans_exp_ordering_criterion_e OpenAPI_red_trans_exp_ordering_criterion_FromString(char* red_trans_exp_ordering_criterion)
{
    int stringToReturn = 0;
    const char *red_trans_exp_ordering_criterionArray[] =  { "NULL", "TIME_SLOT_START", "RED_TRANS_EXP" };
    size_t sizeofArray = sizeof(red_trans_exp_ordering_criterionArray) / sizeof(red_trans_exp_ordering_criterionArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(red_trans_exp_ordering_criterion, red_trans_exp_ordering_criterionArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

