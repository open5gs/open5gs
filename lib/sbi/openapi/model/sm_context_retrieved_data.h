/*
 * sm_context_retrieved_data.h
 *
 * 
 */

#ifndef _OpenAPI_sm_context_retrieved_data_H_
#define _OpenAPI_sm_context_retrieved_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "apn_rate_status.h"
#include "sm_context.h"
#include "small_data_rate_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_retrieved_data_s OpenAPI_sm_context_retrieved_data_t;
typedef struct OpenAPI_sm_context_retrieved_data_s {
    char *ue_eps_pdn_connection;
    struct OpenAPI_sm_context_s *sm_context;
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
    bool is_dl_data_waiting_ind;
    int dl_data_waiting_ind;
} OpenAPI_sm_context_retrieved_data_t;

OpenAPI_sm_context_retrieved_data_t *OpenAPI_sm_context_retrieved_data_create(
    char *ue_eps_pdn_connection,
    OpenAPI_sm_context_t *sm_context,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    bool is_dl_data_waiting_ind,
    int dl_data_waiting_ind
);
void OpenAPI_sm_context_retrieved_data_free(OpenAPI_sm_context_retrieved_data_t *sm_context_retrieved_data);
OpenAPI_sm_context_retrieved_data_t *OpenAPI_sm_context_retrieved_data_parseFromJSON(cJSON *sm_context_retrieved_dataJSON);
cJSON *OpenAPI_sm_context_retrieved_data_convertToJSON(OpenAPI_sm_context_retrieved_data_t *sm_context_retrieved_data);
OpenAPI_sm_context_retrieved_data_t *OpenAPI_sm_context_retrieved_data_copy(OpenAPI_sm_context_retrieved_data_t *dst, OpenAPI_sm_context_retrieved_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_retrieved_data_H_ */

