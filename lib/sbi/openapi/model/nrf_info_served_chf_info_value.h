/*
 * nrf_info_served_chf_info_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_chf_info_value_H_
#define _OpenAPI_nrf_info_served_chf_info_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "chf_info.h"
#include "identity_range.h"
#include "plmn_range.h"
#include "supi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_chf_info_value_s OpenAPI_nrf_info_served_chf_info_value_t;
typedef struct OpenAPI_nrf_info_served_chf_info_value_s {
    OpenAPI_list_t *supi_range_list;
    OpenAPI_list_t *gpsi_range_list;
    OpenAPI_list_t *plmn_range_list;
    char *group_id;
    char *primary_chf_instance;
    char *secondary_chf_instance;
} OpenAPI_nrf_info_served_chf_info_value_t;

OpenAPI_nrf_info_served_chf_info_value_t *OpenAPI_nrf_info_served_chf_info_value_create(
    OpenAPI_list_t *supi_range_list,
    OpenAPI_list_t *gpsi_range_list,
    OpenAPI_list_t *plmn_range_list,
    char *group_id,
    char *primary_chf_instance,
    char *secondary_chf_instance
);
void OpenAPI_nrf_info_served_chf_info_value_free(OpenAPI_nrf_info_served_chf_info_value_t *nrf_info_served_chf_info_value);
OpenAPI_nrf_info_served_chf_info_value_t *OpenAPI_nrf_info_served_chf_info_value_parseFromJSON(cJSON *nrf_info_served_chf_info_valueJSON);
cJSON *OpenAPI_nrf_info_served_chf_info_value_convertToJSON(OpenAPI_nrf_info_served_chf_info_value_t *nrf_info_served_chf_info_value);
OpenAPI_nrf_info_served_chf_info_value_t *OpenAPI_nrf_info_served_chf_info_value_copy(OpenAPI_nrf_info_served_chf_info_value_t *dst, OpenAPI_nrf_info_served_chf_info_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_chf_info_value_H_ */

