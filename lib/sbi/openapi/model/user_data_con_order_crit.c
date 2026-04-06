
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "user_data_con_order_crit.h"

char* OpenAPI_user_data_con_order_crit_ToString(OpenAPI_user_data_con_order_crit_e user_data_con_order_crit)
{
    const char *user_data_con_order_critArray[] =  { "NULL", "APPLICABLE_TIME_WINDOW", "NETWORK_STATUS_INDICATION" };
    size_t sizeofArray = sizeof(user_data_con_order_critArray) / sizeof(user_data_con_order_critArray[0]);
    if (user_data_con_order_crit < sizeofArray)
        return (char *)user_data_con_order_critArray[user_data_con_order_crit];
    else
        return (char *)"Unknown";
}

OpenAPI_user_data_con_order_crit_e OpenAPI_user_data_con_order_crit_FromString(char* user_data_con_order_crit)
{
    int stringToReturn = 0;
    const char *user_data_con_order_critArray[] =  { "NULL", "APPLICABLE_TIME_WINDOW", "NETWORK_STATUS_INDICATION" };
    size_t sizeofArray = sizeof(user_data_con_order_critArray) / sizeof(user_data_con_order_critArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(user_data_con_order_crit, user_data_con_order_critArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}

