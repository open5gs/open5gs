/*
 * pp_data_entry.h
 *
 * 
 */

#ifndef _OpenAPI_pp_data_entry_H_
#define _OpenAPI_pp_data_entry_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "communication_characteristics_af.h"
#include "ec_restriction_1.h"
#include "ecs_addr_config_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pp_data_entry_s OpenAPI_pp_data_entry_t;
typedef struct OpenAPI_pp_data_entry_s {
    bool is_communication_characteristics_null;
    struct OpenAPI_communication_characteristics_af_s *communication_characteristics;
    bool is_reference_id;
    int reference_id;
    char *validity_time;
    char *mtc_provider_information;
    char *supported_features;
    bool is_ecs_addr_config_info_null;
    struct OpenAPI_ecs_addr_config_info_1_s *ecs_addr_config_info;
    OpenAPI_list_t *additional_ecs_addr_config_infos;
    bool is_ec_restriction_null;
    struct OpenAPI_ec_restriction_1_s *ec_restriction;
} OpenAPI_pp_data_entry_t;

OpenAPI_pp_data_entry_t *OpenAPI_pp_data_entry_create(
    bool is_communication_characteristics_null,
    OpenAPI_communication_characteristics_af_t *communication_characteristics,
    bool is_reference_id,
    int reference_id,
    char *validity_time,
    char *mtc_provider_information,
    char *supported_features,
    bool is_ecs_addr_config_info_null,
    OpenAPI_ecs_addr_config_info_1_t *ecs_addr_config_info,
    OpenAPI_list_t *additional_ecs_addr_config_infos,
    bool is_ec_restriction_null,
    OpenAPI_ec_restriction_1_t *ec_restriction
);
void OpenAPI_pp_data_entry_free(OpenAPI_pp_data_entry_t *pp_data_entry);
OpenAPI_pp_data_entry_t *OpenAPI_pp_data_entry_parseFromJSON(cJSON *pp_data_entryJSON);
cJSON *OpenAPI_pp_data_entry_convertToJSON(OpenAPI_pp_data_entry_t *pp_data_entry);
OpenAPI_pp_data_entry_t *OpenAPI_pp_data_entry_copy(OpenAPI_pp_data_entry_t *dst, OpenAPI_pp_data_entry_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pp_data_entry_H_ */

