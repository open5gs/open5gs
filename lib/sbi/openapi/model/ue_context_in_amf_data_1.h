/*
 * ue_context_in_amf_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_in_amf_data_1_H_
#define _OpenAPI_ue_context_in_amf_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eps_interworking_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_in_amf_data_1_s OpenAPI_ue_context_in_amf_data_1_t;
typedef struct OpenAPI_ue_context_in_amf_data_1_s {
    struct OpenAPI_eps_interworking_info_s *eps_interworking_info;
} OpenAPI_ue_context_in_amf_data_1_t;

OpenAPI_ue_context_in_amf_data_1_t *OpenAPI_ue_context_in_amf_data_1_create(
    OpenAPI_eps_interworking_info_t *eps_interworking_info
);
void OpenAPI_ue_context_in_amf_data_1_free(OpenAPI_ue_context_in_amf_data_1_t *ue_context_in_amf_data_1);
OpenAPI_ue_context_in_amf_data_1_t *OpenAPI_ue_context_in_amf_data_1_parseFromJSON(cJSON *ue_context_in_amf_data_1JSON);
cJSON *OpenAPI_ue_context_in_amf_data_1_convertToJSON(OpenAPI_ue_context_in_amf_data_1_t *ue_context_in_amf_data_1);
OpenAPI_ue_context_in_amf_data_1_t *OpenAPI_ue_context_in_amf_data_1_copy(OpenAPI_ue_context_in_amf_data_1_t *dst, OpenAPI_ue_context_in_amf_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_in_amf_data_1_H_ */

