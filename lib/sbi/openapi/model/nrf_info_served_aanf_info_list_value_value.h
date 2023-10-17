/*
 * nrf_info_served_aanf_info_list_value_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_aanf_info_list_value_value_H_
#define _OpenAPI_nrf_info_served_aanf_info_list_value_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "aanf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_aanf_info_list_value_value_s OpenAPI_nrf_info_served_aanf_info_list_value_value_t;
typedef struct OpenAPI_nrf_info_served_aanf_info_list_value_value_s {
    OpenAPI_list_t *routing_indicators;
} OpenAPI_nrf_info_served_aanf_info_list_value_value_t;

OpenAPI_nrf_info_served_aanf_info_list_value_value_t *OpenAPI_nrf_info_served_aanf_info_list_value_value_create(
    OpenAPI_list_t *routing_indicators
);
void OpenAPI_nrf_info_served_aanf_info_list_value_value_free(OpenAPI_nrf_info_served_aanf_info_list_value_value_t *nrf_info_served_aanf_info_list_value_value);
OpenAPI_nrf_info_served_aanf_info_list_value_value_t *OpenAPI_nrf_info_served_aanf_info_list_value_value_parseFromJSON(cJSON *nrf_info_served_aanf_info_list_value_valueJSON);
cJSON *OpenAPI_nrf_info_served_aanf_info_list_value_value_convertToJSON(OpenAPI_nrf_info_served_aanf_info_list_value_value_t *nrf_info_served_aanf_info_list_value_value);
OpenAPI_nrf_info_served_aanf_info_list_value_value_t *OpenAPI_nrf_info_served_aanf_info_list_value_value_copy(OpenAPI_nrf_info_served_aanf_info_list_value_value_t *dst, OpenAPI_nrf_info_served_aanf_info_list_value_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_aanf_info_list_value_value_H_ */

