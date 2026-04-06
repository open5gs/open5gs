/*
 * qos_flow_setup_item.h
 *
 * Individual QoS flow to setup
 */

#ifndef _OpenAPI_qos_flow_setup_item_H_
#define _OpenAPI_qos_flow_setup_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_flow_setup_item_s OpenAPI_qos_flow_setup_item_t;
#include "ecn_marking_congestion_info_req.h"
#include "qos_flow_access_type.h"
#include "qos_flow_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_flow_setup_item_s {
    int qfi;
    char *qos_rules;
    char *protoc_desc;
    bool is_ebi;
    int ebi;
    char *qos_flow_description;
    struct OpenAPI_qos_flow_profile_s *qos_flow_profile;
    OpenAPI_qos_flow_access_type_e associated_an_type;
    bool is_default_qos_rule_ind;
    int default_qos_rule_ind;
    struct OpenAPI_ecn_marking_congestion_info_req_s *ecn_marking_congest_info_req;
    bool is_transp_level_mark_ind;
    int transp_level_mark_ind;
};

OpenAPI_qos_flow_setup_item_t *OpenAPI_qos_flow_setup_item_create(
    int qfi,
    char *qos_rules,
    char *protoc_desc,
    bool is_ebi,
    int ebi,
    char *qos_flow_description,
    OpenAPI_qos_flow_profile_t *qos_flow_profile,
    OpenAPI_qos_flow_access_type_e associated_an_type,
    bool is_default_qos_rule_ind,
    int default_qos_rule_ind,
    OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congest_info_req,
    bool is_transp_level_mark_ind,
    int transp_level_mark_ind
);
void OpenAPI_qos_flow_setup_item_free(OpenAPI_qos_flow_setup_item_t *qos_flow_setup_item);
OpenAPI_qos_flow_setup_item_t *OpenAPI_qos_flow_setup_item_parseFromJSON(cJSON *qos_flow_setup_itemJSON);
cJSON *OpenAPI_qos_flow_setup_item_convertToJSON(OpenAPI_qos_flow_setup_item_t *qos_flow_setup_item);
OpenAPI_qos_flow_setup_item_t *OpenAPI_qos_flow_setup_item_copy(OpenAPI_qos_flow_setup_item_t *dst, OpenAPI_qos_flow_setup_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_setup_item_H_ */

