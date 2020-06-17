/*
 * ue_context_transfer_req_data.h
 *
 *
 */

#ifndef _OpenAPI_ue_context_transfer_req_data_H_
#define _OpenAPI_ue_context_transfer_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "n1_message_container.h"
#include "plmn_id.h"
#include "transfer_reason.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_transfer_req_data_s OpenAPI_ue_context_transfer_req_data_t;
typedef struct OpenAPI_ue_context_transfer_req_data_s {
    OpenAPI_transfer_reason_e reason;
    OpenAPI_access_type_e access_type;
    struct OpenAPI_plmn_id_s *plmn_id;
    struct OpenAPI_n1_message_container_s *reg_request;
    char *supported_features;
} OpenAPI_ue_context_transfer_req_data_t;

OpenAPI_ue_context_transfer_req_data_t *OpenAPI_ue_context_transfer_req_data_create(
    OpenAPI_transfer_reason_e reason,
    OpenAPI_access_type_e access_type,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_n1_message_container_t *reg_request,
    char *supported_features
    );
void OpenAPI_ue_context_transfer_req_data_free(OpenAPI_ue_context_transfer_req_data_t *ue_context_transfer_req_data);
OpenAPI_ue_context_transfer_req_data_t *OpenAPI_ue_context_transfer_req_data_parseFromJSON(cJSON *ue_context_transfer_req_dataJSON);
cJSON *OpenAPI_ue_context_transfer_req_data_convertToJSON(OpenAPI_ue_context_transfer_req_data_t *ue_context_transfer_req_data);
OpenAPI_ue_context_transfer_req_data_t *OpenAPI_ue_context_transfer_req_data_copy(OpenAPI_ue_context_transfer_req_data_t *dst, OpenAPI_ue_context_transfer_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_transfer_req_data_H_ */

