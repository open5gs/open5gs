/*
 * user_data_con_order_crit.h
 *
 * 
 */

#ifndef _OpenAPI_user_data_con_order_crit_H_
#define _OpenAPI_user_data_con_order_crit_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_user_data_con_order_crit_NULL = 0, OpenAPI_user_data_con_order_crit_APPLICABLE_TIME_WINDOW, OpenAPI_user_data_con_order_crit_NETWORK_STATUS_INDICATION } OpenAPI_user_data_con_order_crit_e;

char* OpenAPI_user_data_con_order_crit_ToString(OpenAPI_user_data_con_order_crit_e user_data_con_order_crit);

OpenAPI_user_data_con_order_crit_e OpenAPI_user_data_con_order_crit_FromString(char* user_data_con_order_crit);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_user_data_con_order_crit_H_ */

