/*
 * model_5_g_vn_group_configuration.h
 *
 *
 */

#ifndef _OpenAPI_model_5_g_vn_group_configuration_H_
#define _OpenAPI_model_5_g_vn_group_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "model_5_g_vn_group_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_model_5_g_vn_group_configuration_s OpenAPI_model_5_g_vn_group_configuration_t;
typedef struct OpenAPI_model_5_g_vn_group_configuration_s {
    struct OpenAPI_model_5_g_vn_group_data_s *_5g_vn_group_data;
    OpenAPI_list_t *members;
    int reference_id;
    char *af_instance_id;
    char *internal_group_identifier;
} OpenAPI_model_5_g_vn_group_configuration_t;

OpenAPI_model_5_g_vn_group_configuration_t *OpenAPI_model_5_g_vn_group_configuration_create(
    OpenAPI_model_5_g_vn_group_data_t *_5g_vn_group_data,
    OpenAPI_list_t *members,
    int reference_id,
    char *af_instance_id,
    char *internal_group_identifier
    );
void OpenAPI_model_5_g_vn_group_configuration_free(OpenAPI_model_5_g_vn_group_configuration_t *model_5_g_vn_group_configuration);
OpenAPI_model_5_g_vn_group_configuration_t *OpenAPI_model_5_g_vn_group_configuration_parseFromJSON(cJSON *model_5_g_vn_group_configurationJSON);
cJSON *OpenAPI_model_5_g_vn_group_configuration_convertToJSON(OpenAPI_model_5_g_vn_group_configuration_t *model_5_g_vn_group_configuration);
OpenAPI_model_5_g_vn_group_configuration_t *OpenAPI_model_5_g_vn_group_configuration_copy(OpenAPI_model_5_g_vn_group_configuration_t *dst, OpenAPI_model_5_g_vn_group_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_g_vn_group_configuration_H_ */

