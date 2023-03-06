/*
 * ue_context_cancel_relocate_data.h
 *
 * Data structure used for cancellation of UE Context Relocation
 */

#ifndef _OpenAPI_ue_context_cancel_relocate_data_H_
#define _OpenAPI_ue_context_cancel_relocate_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_cancel_relocate_data_s OpenAPI_ue_context_cancel_relocate_data_t;
typedef struct OpenAPI_ue_context_cancel_relocate_data_s {
    char *supi;
    struct OpenAPI_ref_to_binary_data_s *relocation_cancel_request;
} OpenAPI_ue_context_cancel_relocate_data_t;

OpenAPI_ue_context_cancel_relocate_data_t *OpenAPI_ue_context_cancel_relocate_data_create(
    char *supi,
    OpenAPI_ref_to_binary_data_t *relocation_cancel_request
);
void OpenAPI_ue_context_cancel_relocate_data_free(OpenAPI_ue_context_cancel_relocate_data_t *ue_context_cancel_relocate_data);
OpenAPI_ue_context_cancel_relocate_data_t *OpenAPI_ue_context_cancel_relocate_data_parseFromJSON(cJSON *ue_context_cancel_relocate_dataJSON);
cJSON *OpenAPI_ue_context_cancel_relocate_data_convertToJSON(OpenAPI_ue_context_cancel_relocate_data_t *ue_context_cancel_relocate_data);
OpenAPI_ue_context_cancel_relocate_data_t *OpenAPI_ue_context_cancel_relocate_data_copy(OpenAPI_ue_context_cancel_relocate_data_t *dst, OpenAPI_ue_context_cancel_relocate_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_cancel_relocate_data_H_ */

