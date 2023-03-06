/*
 * subscr_cond.h
 *
 * Condition to determine the set of NFs to monitor under a certain subscription in NRF 
 */

#ifndef _OpenAPI_subscr_cond_H_
#define _OpenAPI_subscr_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "guami.h"
#include "nf_type.h"
#include "snssai.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscr_cond_s OpenAPI_subscr_cond_t;
typedef struct OpenAPI_subscr_cond_s {
    char *nf_instance_id;
    OpenAPI_list_t *nf_instance_list;
    OpenAPI_nf_type_e nf_type;
    char *service_name;
    OpenAPI_list_t *service_name_list;
    char *amf_set_id;
    char *amf_region_id;
    OpenAPI_list_t *guami_list;
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *nsi_list;
    char *nf_group_id;
    char *nf_set_id;
    char *nf_service_set_id;
    OpenAPI_list_t *smf_serving_area;
    OpenAPI_list_t *tai_list;
} OpenAPI_subscr_cond_t;

OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_create(
    char *nf_instance_id,
    OpenAPI_list_t *nf_instance_list,
    OpenAPI_nf_type_e nf_type,
    char *service_name,
    OpenAPI_list_t *service_name_list,
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *nsi_list,
    char *nf_group_id,
    char *nf_set_id,
    char *nf_service_set_id,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *tai_list
);
void OpenAPI_subscr_cond_free(OpenAPI_subscr_cond_t *subscr_cond);
OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_parseFromJSON(cJSON *subscr_condJSON);
cJSON *OpenAPI_subscr_cond_convertToJSON(OpenAPI_subscr_cond_t *subscr_cond);
OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_copy(OpenAPI_subscr_cond_t *dst, OpenAPI_subscr_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscr_cond_H_ */

