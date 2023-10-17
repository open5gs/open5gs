/*
 * sm_context_retrieve_data.h
 *
 * Data within Retrieve SM Context Request
 */

#ifndef _OpenAPI_sm_context_retrieve_data_H_
#define _OpenAPI_sm_context_retrieve_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mme_capabilities.h"
#include "plmn_id.h"
#include "sm_context_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_retrieve_data_s OpenAPI_sm_context_retrieve_data_t;
typedef struct OpenAPI_sm_context_retrieve_data_s {
    struct OpenAPI_mme_capabilities_s *target_mme_cap;
    OpenAPI_sm_context_type_e sm_context_type;
    struct OpenAPI_plmn_id_s *serving_network;
    OpenAPI_list_t *not_to_transfer_ebi_list;
    bool is_ran_unchanged_ind;
    int ran_unchanged_ind;
} OpenAPI_sm_context_retrieve_data_t;

OpenAPI_sm_context_retrieve_data_t *OpenAPI_sm_context_retrieve_data_create(
    OpenAPI_mme_capabilities_t *target_mme_cap,
    OpenAPI_sm_context_type_e sm_context_type,
    OpenAPI_plmn_id_t *serving_network,
    OpenAPI_list_t *not_to_transfer_ebi_list,
    bool is_ran_unchanged_ind,
    int ran_unchanged_ind
);
void OpenAPI_sm_context_retrieve_data_free(OpenAPI_sm_context_retrieve_data_t *sm_context_retrieve_data);
OpenAPI_sm_context_retrieve_data_t *OpenAPI_sm_context_retrieve_data_parseFromJSON(cJSON *sm_context_retrieve_dataJSON);
cJSON *OpenAPI_sm_context_retrieve_data_convertToJSON(OpenAPI_sm_context_retrieve_data_t *sm_context_retrieve_data);
OpenAPI_sm_context_retrieve_data_t *OpenAPI_sm_context_retrieve_data_copy(OpenAPI_sm_context_retrieve_data_t *dst, OpenAPI_sm_context_retrieve_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_retrieve_data_H_ */

