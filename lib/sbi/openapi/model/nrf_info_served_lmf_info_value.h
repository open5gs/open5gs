/*
 * nrf_info_served_lmf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_lmf_info_value_H_
#define _OpenAPI_nrf_info_served_lmf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "an_node_type.h"
#include "external_client_type.h"
#include "lmf_info.h"
#include "rat_type.h"
#include "supported_gad_shapes.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_lmf_info_value_s OpenAPI_nrf_info_served_lmf_info_value_t;
typedef struct OpenAPI_nrf_info_served_lmf_info_value_s {
    OpenAPI_list_t *serving_client_types;
    char *lmf_id;
    OpenAPI_list_t *serving_access_types;
    OpenAPI_list_t *serving_an_node_types;
    OpenAPI_list_t *serving_rat_types;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t *supported_gad_shapes;
} OpenAPI_nrf_info_served_lmf_info_value_t;

OpenAPI_nrf_info_served_lmf_info_value_t *OpenAPI_nrf_info_served_lmf_info_value_create(
    OpenAPI_list_t *serving_client_types,
    char *lmf_id,
    OpenAPI_list_t *serving_access_types,
    OpenAPI_list_t *serving_an_node_types,
    OpenAPI_list_t *serving_rat_types,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *supported_gad_shapes
);
void OpenAPI_nrf_info_served_lmf_info_value_free(OpenAPI_nrf_info_served_lmf_info_value_t *nrf_info_served_lmf_info_value);
OpenAPI_nrf_info_served_lmf_info_value_t *OpenAPI_nrf_info_served_lmf_info_value_parseFromJSON(cJSON *nrf_info_served_lmf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_lmf_info_value_convertToJSON(OpenAPI_nrf_info_served_lmf_info_value_t *nrf_info_served_lmf_info_value);
OpenAPI_nrf_info_served_lmf_info_value_t *OpenAPI_nrf_info_served_lmf_info_value_copy(OpenAPI_nrf_info_served_lmf_info_value_t *dst, OpenAPI_nrf_info_served_lmf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_lmf_info_value_H_ */

