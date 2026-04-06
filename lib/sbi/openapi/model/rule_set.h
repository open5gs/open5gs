/*
 * rule_set.h
 *
 * List of rules specifying whether access/scopes are allowed/denied for NF-Consumers. 
 */

#ifndef _OpenAPI_rule_set_H_
#define _OpenAPI_rule_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rule_set_s OpenAPI_rule_set_t;
#include "ext_snssai.h"
#include "nf_type.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "rule_set_action.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rule_set_s {
    int priority;
    OpenAPI_list_t *plmns;
    OpenAPI_list_t *snpns;
    OpenAPI_list_t *nf_types;
    OpenAPI_list_t *nf_domains;
    OpenAPI_list_t *nssais;
    OpenAPI_list_t *nf_instances;
    OpenAPI_list_t *scopes;
    OpenAPI_rule_set_action_e action;
};

OpenAPI_rule_set_t *OpenAPI_rule_set_create(
    int priority,
    OpenAPI_list_t *plmns,
    OpenAPI_list_t *snpns,
    OpenAPI_list_t *nf_types,
    OpenAPI_list_t *nf_domains,
    OpenAPI_list_t *nssais,
    OpenAPI_list_t *nf_instances,
    OpenAPI_list_t *scopes,
    OpenAPI_rule_set_action_e action
);
void OpenAPI_rule_set_free(OpenAPI_rule_set_t *rule_set);
OpenAPI_rule_set_t *OpenAPI_rule_set_parseFromJSON(cJSON *rule_setJSON);
cJSON *OpenAPI_rule_set_convertToJSON(OpenAPI_rule_set_t *rule_set);
OpenAPI_rule_set_t *OpenAPI_rule_set_copy(OpenAPI_rule_set_t *dst, OpenAPI_rule_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rule_set_H_ */

