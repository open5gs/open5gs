/*
 * subscription_data_subscr_cond.h
 *
 * 
 */

#ifndef _OpenAPI_subscription_data_subscr_cond_H_
#define _OpenAPI_subscription_data_subscr_cond_H_

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

typedef struct OpenAPI_subscription_data_subscr_cond_s OpenAPI_subscription_data_subscr_cond_t;
typedef struct OpenAPI_subscription_data_subscr_cond_s {
    char *nf_instance_id;
    OpenAPI_nf_type_e nf_type;
    char *service_name;
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
} OpenAPI_subscription_data_subscr_cond_t;

OpenAPI_subscription_data_subscr_cond_t *OpenAPI_subscription_data_subscr_cond_create(
    char *nf_instance_id,
    OpenAPI_nf_type_e nf_type,
    char *service_name,
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
void OpenAPI_subscription_data_subscr_cond_free(OpenAPI_subscription_data_subscr_cond_t *subscription_data_subscr_cond);
OpenAPI_subscription_data_subscr_cond_t *OpenAPI_subscription_data_subscr_cond_parseFromJSON(cJSON *subscription_data_subscr_condJSON);
cJSON *OpenAPI_subscription_data_subscr_cond_convertToJSON(OpenAPI_subscription_data_subscr_cond_t *subscription_data_subscr_cond);
OpenAPI_subscription_data_subscr_cond_t *OpenAPI_subscription_data_subscr_cond_copy(OpenAPI_subscription_data_subscr_cond_t *dst, OpenAPI_subscription_data_subscr_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_data_subscr_cond_H_ */

