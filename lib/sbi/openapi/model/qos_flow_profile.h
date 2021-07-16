/*
 * qos_flow_profile.h
 *
 * 
 */

#ifndef _OpenAPI_qos_flow_profile_H_
#define _OpenAPI_qos_flow_profile_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "additional_qos_flow_info.h"
#include "arp.h"
#include "dynamic5_qi.h"
#include "gbr_qos_flow_information.h"
#include "non_dynamic5_qi.h"
#include "qos_monitoring_req.h"
#include "reflective_qo_s_attribute.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_profile_s OpenAPI_qos_flow_profile_t;
typedef struct OpenAPI_qos_flow_profile_s {
    int _5qi;
    struct OpenAPI_non_dynamic5_qi_s *non_dynamic5_qi;
    struct OpenAPI_dynamic5_qi_s *dynamic5_qi;
    struct OpenAPI_arp_s *arp;
    struct OpenAPI_gbr_qos_flow_information_s *gbr_qos_flow_info;
    OpenAPI_reflective_qo_s_attribute_e rqa;
    OpenAPI_additional_qos_flow_info_e additional_qos_flow_info;
    OpenAPI_qos_monitoring_req_e qos_monitoring_req;
    bool is_qos_rep_period;
    int qos_rep_period;
} OpenAPI_qos_flow_profile_t;

OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_create(
    int _5qi,
    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi,
    OpenAPI_dynamic5_qi_t *dynamic5_qi,
    OpenAPI_arp_t *arp,
    OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_info,
    OpenAPI_reflective_qo_s_attribute_e rqa,
    OpenAPI_additional_qos_flow_info_e additional_qos_flow_info,
    OpenAPI_qos_monitoring_req_e qos_monitoring_req,
    bool is_qos_rep_period,
    int qos_rep_period
);
void OpenAPI_qos_flow_profile_free(OpenAPI_qos_flow_profile_t *qos_flow_profile);
OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_parseFromJSON(cJSON *qos_flow_profileJSON);
cJSON *OpenAPI_qos_flow_profile_convertToJSON(OpenAPI_qos_flow_profile_t *qos_flow_profile);
OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_copy(OpenAPI_qos_flow_profile_t *dst, OpenAPI_qos_flow_profile_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_profile_H_ */

