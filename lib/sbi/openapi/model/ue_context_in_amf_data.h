/*
 * ue_context_in_amf_data.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_in_amf_data_H_
#define _OpenAPI_ue_context_in_amf_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eps_interworking_info.h"
#include "nudm_amf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_in_amf_data_s OpenAPI_ue_context_in_amf_data_t;
typedef struct OpenAPI_ue_context_in_amf_data_s {
    struct OpenAPI_eps_interworking_info_s *eps_interworking_info;
    OpenAPI_list_t *amf_info;
} OpenAPI_ue_context_in_amf_data_t;

OpenAPI_ue_context_in_amf_data_t *OpenAPI_ue_context_in_amf_data_create(
    OpenAPI_eps_interworking_info_t *eps_interworking_info,
    OpenAPI_list_t *amf_info
);
void OpenAPI_ue_context_in_amf_data_free(OpenAPI_ue_context_in_amf_data_t *ue_context_in_amf_data);
OpenAPI_ue_context_in_amf_data_t *OpenAPI_ue_context_in_amf_data_parseFromJSON(cJSON *ue_context_in_amf_dataJSON);
cJSON *OpenAPI_ue_context_in_amf_data_convertToJSON(OpenAPI_ue_context_in_amf_data_t *ue_context_in_amf_data);
OpenAPI_ue_context_in_amf_data_t *OpenAPI_ue_context_in_amf_data_copy(OpenAPI_ue_context_in_amf_data_t *dst, OpenAPI_ue_context_in_amf_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_in_amf_data_H_ */

