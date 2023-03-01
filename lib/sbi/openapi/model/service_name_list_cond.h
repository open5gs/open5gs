/*
 * service_name_list_cond.h
 *
 * Subscription to a set of NFs based on their support for a Service Name in the Servic Name list 
 */

#ifndef _OpenAPI_service_name_list_cond_H_
#define _OpenAPI_service_name_list_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_name_list_cond_s OpenAPI_service_name_list_cond_t;
typedef enum { OpenAPI_service_name_list_cond_CONDITIONTYPE_NULL = 0, OpenAPI_service_name_list_cond_CONDITIONTYPE_SERVICE_NAME_LIST_COND } OpenAPI_service_name_list_cond_condition_type_e;

char* OpenAPI_service_name_list_cond_condition_type_ToString(OpenAPI_service_name_list_cond_condition_type_e condition_type);

OpenAPI_service_name_list_cond_condition_type_e OpenAPI_service_name_list_cond_condition_type_FromString(char* condition_type);
typedef struct OpenAPI_service_name_list_cond_s {
    OpenAPI_service_name_list_cond_condition_type_e condition_type;
    OpenAPI_list_t *service_name_list;
} OpenAPI_service_name_list_cond_t;

OpenAPI_service_name_list_cond_t *OpenAPI_service_name_list_cond_create(
    OpenAPI_service_name_list_cond_condition_type_e condition_type,
    OpenAPI_list_t *service_name_list
);
void OpenAPI_service_name_list_cond_free(OpenAPI_service_name_list_cond_t *service_name_list_cond);
OpenAPI_service_name_list_cond_t *OpenAPI_service_name_list_cond_parseFromJSON(cJSON *service_name_list_condJSON);
cJSON *OpenAPI_service_name_list_cond_convertToJSON(OpenAPI_service_name_list_cond_t *service_name_list_cond);
OpenAPI_service_name_list_cond_t *OpenAPI_service_name_list_cond_copy(OpenAPI_service_name_list_cond_t *dst, OpenAPI_service_name_list_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_name_list_cond_H_ */

