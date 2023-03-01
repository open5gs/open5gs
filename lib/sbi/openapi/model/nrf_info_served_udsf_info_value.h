/*
 * nrf_info_served_udsf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_udsf_info_value_H_
#define _OpenAPI_nrf_info_served_udsf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"
#include "supi_range.h"
#include "udsf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_udsf_info_value_s OpenAPI_nrf_info_served_udsf_info_value_t;
typedef struct OpenAPI_nrf_info_served_udsf_info_value_s {
    char *group_id;
    OpenAPI_list_t *supi_ranges;
    OpenAPI_list_t* storage_id_ranges;
} OpenAPI_nrf_info_served_udsf_info_value_t;

OpenAPI_nrf_info_served_udsf_info_value_t *OpenAPI_nrf_info_served_udsf_info_value_create(
    char *group_id,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t* storage_id_ranges
);
void OpenAPI_nrf_info_served_udsf_info_value_free(OpenAPI_nrf_info_served_udsf_info_value_t *nrf_info_served_udsf_info_value);
OpenAPI_nrf_info_served_udsf_info_value_t *OpenAPI_nrf_info_served_udsf_info_value_parseFromJSON(cJSON *nrf_info_served_udsf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_udsf_info_value_convertToJSON(OpenAPI_nrf_info_served_udsf_info_value_t *nrf_info_served_udsf_info_value);
OpenAPI_nrf_info_served_udsf_info_value_t *OpenAPI_nrf_info_served_udsf_info_value_copy(OpenAPI_nrf_info_served_udsf_info_value_t *dst, OpenAPI_nrf_info_served_udsf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_udsf_info_value_H_ */

