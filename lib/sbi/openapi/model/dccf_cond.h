/*
 * dccf_cond.h
 *
 * Subscription to a set of NF Instances (DCCFs), identified by NF types, NF Set Id(s) or DCCF Serving Area information, i.e. list of TAIs served by the DCCF 
 */

#ifndef _OpenAPI_dccf_cond_H_
#define _OpenAPI_dccf_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dccf_cond_s OpenAPI_dccf_cond_t;
typedef enum { OpenAPI_dccf_cond_CONDITIONTYPE_NULL = 0, OpenAPI_dccf_cond_CONDITIONTYPE_DCCF_COND } OpenAPI_dccf_cond_condition_type_e;

char* OpenAPI_dccf_cond_condition_type_ToString(OpenAPI_dccf_cond_condition_type_e condition_type);

OpenAPI_dccf_cond_condition_type_e OpenAPI_dccf_cond_condition_type_FromString(char* condition_type);
typedef struct OpenAPI_dccf_cond_s {
    OpenAPI_dccf_cond_condition_type_e condition_type;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *serving_nf_set_id_list;
} OpenAPI_dccf_cond_t;

OpenAPI_dccf_cond_t *OpenAPI_dccf_cond_create(
    OpenAPI_dccf_cond_condition_type_e condition_type,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list
);
void OpenAPI_dccf_cond_free(OpenAPI_dccf_cond_t *dccf_cond);
OpenAPI_dccf_cond_t *OpenAPI_dccf_cond_parseFromJSON(cJSON *dccf_condJSON);
cJSON *OpenAPI_dccf_cond_convertToJSON(OpenAPI_dccf_cond_t *dccf_cond);
OpenAPI_dccf_cond_t *OpenAPI_dccf_cond_copy(OpenAPI_dccf_cond_t *dst, OpenAPI_dccf_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dccf_cond_H_ */

