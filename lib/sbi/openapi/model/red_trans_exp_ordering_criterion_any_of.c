
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "red_trans_exp_ordering_criterion_any_of.h"

char* OpenAPI_red_trans_exp_ordering_criterion_any_of_ToString(OpenAPI_red_trans_exp_ordering_criterion_any_of_e red_trans_exp_ordering_criterion_any_of)
{
    const char *red_trans_exp_ordering_criterion_any_ofArray[] =  { "NULL", "TIME_SLOT_START", "RED_TRANS_EXP" };
    size_t sizeofArray = sizeof(red_trans_exp_ordering_criterion_any_ofArray) / sizeof(red_trans_exp_ordering_criterion_any_ofArray[0]);
    if (red_trans_exp_ordering_criterion_any_of < sizeofArray)
        return (char *)red_trans_exp_ordering_criterion_any_ofArray[red_trans_exp_ordering_criterion_any_of];
    else
        return (char *)"Unknown";
}

OpenAPI_red_trans_exp_ordering_criterion_any_of_e OpenAPI_red_trans_exp_ordering_criterion_any_of_FromString(char* red_trans_exp_ordering_criterion_any_of)
{
    int stringToReturn = 0;
    const char *red_trans_exp_ordering_criterion_any_ofArray[] =  { "NULL", "TIME_SLOT_START", "RED_TRANS_EXP" };
    size_t sizeofArray = sizeof(red_trans_exp_ordering_criterion_any_ofArray) / sizeof(red_trans_exp_ordering_criterion_any_ofArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(red_trans_exp_ordering_criterion_any_of, red_trans_exp_ordering_criterion_any_ofArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

