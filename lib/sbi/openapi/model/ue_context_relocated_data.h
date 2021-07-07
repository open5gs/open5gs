/*
 * ue_context_relocated_data.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_relocated_data_H_
#define _OpenAPI_ue_context_relocated_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_relocated_data_s OpenAPI_ue_context_relocated_data_t;
typedef struct OpenAPI_ue_context_relocated_data_s {
    struct OpenAPI_ue_context_s *ue_context;
} OpenAPI_ue_context_relocated_data_t;

OpenAPI_ue_context_relocated_data_t *OpenAPI_ue_context_relocated_data_create(
    OpenAPI_ue_context_t *ue_context
);
void OpenAPI_ue_context_relocated_data_free(OpenAPI_ue_context_relocated_data_t *ue_context_relocated_data);
OpenAPI_ue_context_relocated_data_t *OpenAPI_ue_context_relocated_data_parseFromJSON(cJSON *ue_context_relocated_dataJSON);
cJSON *OpenAPI_ue_context_relocated_data_convertToJSON(OpenAPI_ue_context_relocated_data_t *ue_context_relocated_data);
OpenAPI_ue_context_relocated_data_t *OpenAPI_ue_context_relocated_data_copy(OpenAPI_ue_context_relocated_data_t *dst, OpenAPI_ue_context_relocated_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_relocated_data_H_ */

