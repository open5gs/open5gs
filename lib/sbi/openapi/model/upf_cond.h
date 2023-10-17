/*
 * upf_cond.h
 *
 * Subscription to a set of NF Instances (UPFs), able to serve a certain service area (i.e. SMF serving area or TAI list) 
 */

#ifndef _OpenAPI_upf_cond_H_
#define _OpenAPI_upf_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_upf_cond_s OpenAPI_upf_cond_t;
typedef enum { OpenAPI_upf_cond_CONDITIONTYPE_NULL = 0, OpenAPI_upf_cond_CONDITIONTYPE_UPF_COND } OpenAPI_upf_cond_condition_type_e;

char* OpenAPI_upf_cond_condition_type_ToString(OpenAPI_upf_cond_condition_type_e condition_type);

OpenAPI_upf_cond_condition_type_e OpenAPI_upf_cond_condition_type_FromString(char* condition_type);
typedef struct OpenAPI_upf_cond_s {
    OpenAPI_upf_cond_condition_type_e condition_type;
    OpenAPI_list_t *smf_serving_area;
    OpenAPI_list_t *tai_list;
} OpenAPI_upf_cond_t;

OpenAPI_upf_cond_t *OpenAPI_upf_cond_create(
    OpenAPI_upf_cond_condition_type_e condition_type,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *tai_list
);
void OpenAPI_upf_cond_free(OpenAPI_upf_cond_t *upf_cond);
OpenAPI_upf_cond_t *OpenAPI_upf_cond_parseFromJSON(cJSON *upf_condJSON);
cJSON *OpenAPI_upf_cond_convertToJSON(OpenAPI_upf_cond_t *upf_cond);
OpenAPI_upf_cond_t *OpenAPI_upf_cond_copy(OpenAPI_upf_cond_t *dst, OpenAPI_upf_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_cond_H_ */

