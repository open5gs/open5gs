/*
 * ue_context_in_smsf_data.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_in_smsf_data_H_
#define _OpenAPI_ue_context_in_smsf_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "smsf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_in_smsf_data_s OpenAPI_ue_context_in_smsf_data_t;
typedef struct OpenAPI_ue_context_in_smsf_data_s {
    struct OpenAPI_smsf_info_s *smsf_info3_gpp_access;
    struct OpenAPI_smsf_info_s *smsf_info_non3_gpp_access;
} OpenAPI_ue_context_in_smsf_data_t;

OpenAPI_ue_context_in_smsf_data_t *OpenAPI_ue_context_in_smsf_data_create(
    OpenAPI_smsf_info_t *smsf_info3_gpp_access,
    OpenAPI_smsf_info_t *smsf_info_non3_gpp_access
);
void OpenAPI_ue_context_in_smsf_data_free(OpenAPI_ue_context_in_smsf_data_t *ue_context_in_smsf_data);
OpenAPI_ue_context_in_smsf_data_t *OpenAPI_ue_context_in_smsf_data_parseFromJSON(cJSON *ue_context_in_smsf_dataJSON);
cJSON *OpenAPI_ue_context_in_smsf_data_convertToJSON(OpenAPI_ue_context_in_smsf_data_t *ue_context_in_smsf_data);
OpenAPI_ue_context_in_smsf_data_t *OpenAPI_ue_context_in_smsf_data_copy(OpenAPI_ue_context_in_smsf_data_t *dst, OpenAPI_ue_context_in_smsf_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_in_smsf_data_H_ */

