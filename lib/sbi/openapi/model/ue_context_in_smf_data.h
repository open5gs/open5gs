/*
 * ue_context_in_smf_data.h
 *
 *
 */

#ifndef _OpenAPI_ue_context_in_smf_data_H_
#define _OpenAPI_ue_context_in_smf_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "emergency_info.h"
#include "pdu_session.h"
#include "pgw_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_in_smf_data_s OpenAPI_ue_context_in_smf_data_t;
typedef struct OpenAPI_ue_context_in_smf_data_s {
    OpenAPI_list_t* pdu_sessions;
    OpenAPI_list_t *pgw_info;
    struct OpenAPI_emergency_info_s *emergency_info;
} OpenAPI_ue_context_in_smf_data_t;

OpenAPI_ue_context_in_smf_data_t *OpenAPI_ue_context_in_smf_data_create(
    OpenAPI_list_t* pdu_sessions,
    OpenAPI_list_t *pgw_info,
    OpenAPI_emergency_info_t *emergency_info
    );
void OpenAPI_ue_context_in_smf_data_free(OpenAPI_ue_context_in_smf_data_t *ue_context_in_smf_data);
OpenAPI_ue_context_in_smf_data_t *OpenAPI_ue_context_in_smf_data_parseFromJSON(cJSON *ue_context_in_smf_dataJSON);
cJSON *OpenAPI_ue_context_in_smf_data_convertToJSON(OpenAPI_ue_context_in_smf_data_t *ue_context_in_smf_data);
OpenAPI_ue_context_in_smf_data_t *OpenAPI_ue_context_in_smf_data_copy(OpenAPI_ue_context_in_smf_data_t *dst, OpenAPI_ue_context_in_smf_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_in_smf_data_H_ */

