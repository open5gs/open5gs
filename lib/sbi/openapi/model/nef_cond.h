/*
 * nef_cond.h
 *
 * Subscription to a set of NF Instances (NEFs), identified by Event ID(s) provided by AF, S-NSSAI(s), AF Instance ID, Application Identifier, External Identifier, External Group Identifier, or domain name. 
 */

#ifndef _OpenAPI_nef_cond_H_
#define _OpenAPI_nef_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event.h"
#include "identity_range.h"
#include "pfd_data.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nef_cond_s OpenAPI_nef_cond_t;
typedef enum { OpenAPI_nef_cond_CONDITIONTYPE_NULL = 0, OpenAPI_nef_cond_CONDITIONTYPE_NEF_COND } OpenAPI_nef_cond_condition_type_e;

char* OpenAPI_nef_cond_condition_type_ToString(OpenAPI_nef_cond_condition_type_e condition_type);

OpenAPI_nef_cond_condition_type_e OpenAPI_nef_cond_condition_type_FromString(char* condition_type);
typedef struct OpenAPI_nef_cond_s {
    OpenAPI_nef_cond_condition_type_e condition_type;
    OpenAPI_list_t *af_events;
    OpenAPI_list_t *snssai_list;
    struct OpenAPI_pfd_data_s *pfd_data;
    OpenAPI_list_t *gpsi_ranges;
    OpenAPI_list_t *external_group_identifiers_ranges;
    OpenAPI_list_t *served_fqdn_list;
} OpenAPI_nef_cond_t;

OpenAPI_nef_cond_t *OpenAPI_nef_cond_create(
    OpenAPI_nef_cond_condition_type_e condition_type,
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *snssai_list,
    OpenAPI_pfd_data_t *pfd_data,
    OpenAPI_list_t *gpsi_ranges,
    OpenAPI_list_t *external_group_identifiers_ranges,
    OpenAPI_list_t *served_fqdn_list
);
void OpenAPI_nef_cond_free(OpenAPI_nef_cond_t *nef_cond);
OpenAPI_nef_cond_t *OpenAPI_nef_cond_parseFromJSON(cJSON *nef_condJSON);
cJSON *OpenAPI_nef_cond_convertToJSON(OpenAPI_nef_cond_t *nef_cond);
OpenAPI_nef_cond_t *OpenAPI_nef_cond_copy(OpenAPI_nef_cond_t *dst, OpenAPI_nef_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nef_cond_H_ */

