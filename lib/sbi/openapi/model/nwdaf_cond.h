/*
 * nwdaf_cond.h
 *
 * Subscription to a set of NF Instances (NWDAFs), identified by Analytics ID(s), S-NSSAI(s) or NWDAF Serving Area information, i.e. list of TAIs for which the NWDAF can provide analytics. 
 */

#ifndef _OpenAPI_nwdaf_cond_H_
#define _OpenAPI_nwdaf_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ml_analytics_info.h"
#include "nf_type.h"
#include "snssai.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_cond_s OpenAPI_nwdaf_cond_t;
typedef enum { OpenAPI_nwdaf_cond_CONDITIONTYPE_NULL = 0, OpenAPI_nwdaf_cond_CONDITIONTYPE_NWDAF_COND } OpenAPI_nwdaf_cond_condition_type_e;

char* OpenAPI_nwdaf_cond_condition_type_ToString(OpenAPI_nwdaf_cond_condition_type_e condition_type);

OpenAPI_nwdaf_cond_condition_type_e OpenAPI_nwdaf_cond_condition_type_FromString(char* condition_type);
typedef struct OpenAPI_nwdaf_cond_s {
    OpenAPI_nwdaf_cond_condition_type_e condition_type;
    OpenAPI_list_t *analytics_ids;
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *serving_nf_set_id_list;
    OpenAPI_list_t *ml_analytics_list;
} OpenAPI_nwdaf_cond_t;

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_create(
    OpenAPI_nwdaf_cond_condition_type_e condition_type,
    OpenAPI_list_t *analytics_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *ml_analytics_list
);
void OpenAPI_nwdaf_cond_free(OpenAPI_nwdaf_cond_t *nwdaf_cond);
OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_parseFromJSON(cJSON *nwdaf_condJSON);
cJSON *OpenAPI_nwdaf_cond_convertToJSON(OpenAPI_nwdaf_cond_t *nwdaf_cond);
OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_copy(OpenAPI_nwdaf_cond_t *dst, OpenAPI_nwdaf_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_cond_H_ */

