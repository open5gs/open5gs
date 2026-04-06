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
typedef struct OpenAPI_subscr_cond_s OpenAPI_subscr_cond_t;
#include "af_event.h"
#include "guami.h"
#include "identity_range.h"
#include "ml_analytics_info.h"
#include "nf_type.h"
#include "pfd_data.h"
#include "service_name.h"
#include "snssai.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_subscr_cond_CONDITIONTYPE_NULL = 0, OpenAPI_subscr_cond_CONDITIONTYPE_SERVICE_NAME_LIST_COND } OpenAPI_subscr_cond_condition_type_e;

char* OpenAPI_subscr_cond_condition_type_ToString(OpenAPI_subscr_cond_condition_type_e condition_type);

OpenAPI_subscr_cond_condition_type_e OpenAPI_subscr_cond_condition_type_FromString(char* condition_type);
struct OpenAPI_subscr_cond_s {
    char *nf_instance_id;
    OpenAPI_list_t *nf_instance_id_list;
    OpenAPI_nf_type_e nf_type;
    OpenAPI_service_name_e service_name;
    OpenAPI_subscr_cond_condition_type_e condition_type;
    OpenAPI_list_t *service_name_list;
    char *amf_set_id;
    char *amf_region_id;
    OpenAPI_list_t *guami_list;
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *nsi_list;
    char *nf_group_id;
    OpenAPI_list_t *nf_group_id_list;
    char *nf_set_id;
    char *nf_service_set_id;
    OpenAPI_list_t *smf_serving_area;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *scp_domains;
    OpenAPI_list_t *nf_type_list;
    OpenAPI_list_t *analytics_ids;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *serving_nf_type_list;
    OpenAPI_list_t *serving_nf_set_id_list;
    OpenAPI_list_t *ml_analytics_list;
    OpenAPI_list_t *af_events;
    struct OpenAPI_pfd_data_s *pfd_data;
    OpenAPI_list_t *gpsi_ranges;
    OpenAPI_list_t *external_group_identifiers_ranges;
    OpenAPI_list_t *served_fqdn_list;
};

OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_create(
    char *nf_instance_id,
    OpenAPI_list_t *nf_instance_id_list,
    OpenAPI_nf_type_e nf_type,
    OpenAPI_service_name_e service_name,
    OpenAPI_subscr_cond_condition_type_e condition_type,
    OpenAPI_list_t *service_name_list,
    char *amf_set_id,
    char *amf_region_id,
    OpenAPI_list_t *guami_list,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *nsi_list,
    char *nf_group_id,
    OpenAPI_list_t *nf_group_id_list,
    char *nf_set_id,
    char *nf_service_set_id,
    OpenAPI_list_t *smf_serving_area,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *scp_domains,
    OpenAPI_list_t *nf_type_list,
    OpenAPI_list_t *analytics_ids,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *ml_analytics_list,
    OpenAPI_list_t *af_events,
    OpenAPI_pfd_data_t *pfd_data,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *served_fqdn_list
);
void OpenAPI_subscr_cond_free(OpenAPI_subscr_cond_t *subscr_cond);
OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_parseFromJSON(cJSON *subscr_condJSON);
cJSON *OpenAPI_subscr_cond_convertToJSON(OpenAPI_subscr_cond_t *subscr_cond);
OpenAPI_subscr_cond_t *OpenAPI_subscr_cond_copy(OpenAPI_subscr_cond_t *dst, OpenAPI_subscr_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscr_cond_H_ */

