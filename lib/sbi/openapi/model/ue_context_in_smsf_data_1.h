/*
 * ue_context_in_smsf_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_in_smsf_data_1_H_
#define _OpenAPI_ue_context_in_smsf_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "smsf_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_in_smsf_data_1_s OpenAPI_ue_context_in_smsf_data_1_t;
typedef struct OpenAPI_ue_context_in_smsf_data_1_s {
    struct OpenAPI_smsf_info_1_s *smsf_info3_gpp_access;
    struct OpenAPI_smsf_info_1_s *smsf_info_non3_gpp_access;
} OpenAPI_ue_context_in_smsf_data_1_t;

OpenAPI_ue_context_in_smsf_data_1_t *OpenAPI_ue_context_in_smsf_data_1_create(
    OpenAPI_smsf_info_1_t *smsf_info3_gpp_access,
    OpenAPI_smsf_info_1_t *smsf_info_non3_gpp_access
);
void OpenAPI_ue_context_in_smsf_data_1_free(OpenAPI_ue_context_in_smsf_data_1_t *ue_context_in_smsf_data_1);
OpenAPI_ue_context_in_smsf_data_1_t *OpenAPI_ue_context_in_smsf_data_1_parseFromJSON(cJSON *ue_context_in_smsf_data_1JSON);
cJSON *OpenAPI_ue_context_in_smsf_data_1_convertToJSON(OpenAPI_ue_context_in_smsf_data_1_t *ue_context_in_smsf_data_1);
OpenAPI_ue_context_in_smsf_data_1_t *OpenAPI_ue_context_in_smsf_data_1_copy(OpenAPI_ue_context_in_smsf_data_1_t *dst, OpenAPI_ue_context_in_smsf_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_in_smsf_data_1_H_ */

