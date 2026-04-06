/*
 * qmc_config_info.h
 *
 * It contains the configuration information for signaling-based activation of the  Quality of Experience (QoE) Measurements Collection (QMC) functionality.  
 */

#ifndef _OpenAPI_qmc_config_info_H_
#define _OpenAPI_qmc_config_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qmc_config_info_s OpenAPI_qmc_config_info_t;
#include "available_ran_visible_qoe_metric.h"
#include "ip_addr.h"
#include "mbs_service_type.h"
#include "qmc_area_scope.h"
#include "qoe_service_type.h"
#include "qoe_target.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qmc_config_info_s {
    char *qoe_reference;
    OpenAPI_qoe_service_type_e service_type;
    OpenAPI_list_t *slice_scope;
    struct OpenAPI_qmc_area_scope_s *area_scope;
    struct OpenAPI_ip_addr_s *qoe_collection_entity_address;
    struct OpenAPI_qoe_target_s *qoe_target;
    char *mdt_alignment_info;
    OpenAPI_list_t *available_ran_visible_qoe_metrics;
    char *container_for_app_layer_meas_config;
    OpenAPI_mbs_service_type_e mbs_communication_service_type;
};

OpenAPI_qmc_config_info_t *OpenAPI_qmc_config_info_create(
    char *qoe_reference,
    OpenAPI_qoe_service_type_e service_type,
    OpenAPI_list_t *slice_scope,
    OpenAPI_qmc_area_scope_t *area_scope,
    OpenAPI_ip_addr_t *qoe_collection_entity_address,
    OpenAPI_qoe_target_t *qoe_target,
    char *mdt_alignment_info,
    OpenAPI_list_t *available_ran_visible_qoe_metrics,
    char *container_for_app_layer_meas_config,
    OpenAPI_mbs_service_type_e mbs_communication_service_type
);
void OpenAPI_qmc_config_info_free(OpenAPI_qmc_config_info_t *qmc_config_info);
OpenAPI_qmc_config_info_t *OpenAPI_qmc_config_info_parseFromJSON(cJSON *qmc_config_infoJSON);
cJSON *OpenAPI_qmc_config_info_convertToJSON(OpenAPI_qmc_config_info_t *qmc_config_info);
OpenAPI_qmc_config_info_t *OpenAPI_qmc_config_info_copy(OpenAPI_qmc_config_info_t *dst, OpenAPI_qmc_config_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qmc_config_info_H_ */

