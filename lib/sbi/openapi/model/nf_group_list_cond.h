/*
 * nf_group_list_cond.h
 *
 * Subscription to a set of NFs based on their Group Ids
 */

#ifndef _OpenAPI_nf_group_list_cond_H_
#define _OpenAPI_nf_group_list_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_group_list_cond_s OpenAPI_nf_group_list_cond_t;
typedef enum { OpenAPI_nf_group_list_cond_CONDITIONTYPE_NULL = 0, OpenAPI_nf_group_list_cond_CONDITIONTYPE_NF_GROUP_LIST_COND } OpenAPI_nf_group_list_cond_condition_type_e;

char* OpenAPI_nf_group_list_cond_condition_type_ToString(OpenAPI_nf_group_list_cond_condition_type_e condition_type);

OpenAPI_nf_group_list_cond_condition_type_e OpenAPI_nf_group_list_cond_condition_type_FromString(char* condition_type);
typedef enum { OpenAPI_nf_group_list_cond_NFTYPE_NULL = 0, OpenAPI_nf_group_list_cond_NFTYPE_UDM, OpenAPI_nf_group_list_cond_NFTYPE_AUSF, OpenAPI_nf_group_list_cond_NFTYPE_UDR, OpenAPI_nf_group_list_cond_NFTYPE_PCF, OpenAPI_nf_group_list_cond_NFTYPE_CHF, OpenAPI_nf_group_list_cond_NFTYPE_HSS } OpenAPI_nf_group_list_cond_nf_type_e;

char* OpenAPI_nf_group_list_cond_nf_type_ToString(OpenAPI_nf_group_list_cond_nf_type_e nf_type);

OpenAPI_nf_group_list_cond_nf_type_e OpenAPI_nf_group_list_cond_nf_type_FromString(char* nf_type);
typedef struct OpenAPI_nf_group_list_cond_s {
    OpenAPI_nf_group_list_cond_condition_type_e condition_type;
    OpenAPI_nf_group_list_cond_nf_type_e nf_type;
    OpenAPI_list_t *nf_group_id_list;
} OpenAPI_nf_group_list_cond_t;

OpenAPI_nf_group_list_cond_t *OpenAPI_nf_group_list_cond_create(
    OpenAPI_nf_group_list_cond_condition_type_e condition_type,
    OpenAPI_nf_group_list_cond_nf_type_e nf_type,
    OpenAPI_list_t *nf_group_id_list
);
void OpenAPI_nf_group_list_cond_free(OpenAPI_nf_group_list_cond_t *nf_group_list_cond);
OpenAPI_nf_group_list_cond_t *OpenAPI_nf_group_list_cond_parseFromJSON(cJSON *nf_group_list_condJSON);
cJSON *OpenAPI_nf_group_list_cond_convertToJSON(OpenAPI_nf_group_list_cond_t *nf_group_list_cond);
OpenAPI_nf_group_list_cond_t *OpenAPI_nf_group_list_cond_copy(OpenAPI_nf_group_list_cond_t *dst, OpenAPI_nf_group_list_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_group_list_cond_H_ */

