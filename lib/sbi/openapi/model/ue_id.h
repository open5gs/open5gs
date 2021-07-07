/*
 * ue_id.h
 *
 * 
 */

#ifndef _OpenAPI_ue_id_H_
#define _OpenAPI_ue_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_id_s OpenAPI_ue_id_t;
typedef struct OpenAPI_ue_id_s {
    char *supi;
    OpenAPI_list_t *gpsi_list;
} OpenAPI_ue_id_t;

OpenAPI_ue_id_t *OpenAPI_ue_id_create(
    char *supi,
    OpenAPI_list_t *gpsi_list
);
void OpenAPI_ue_id_free(OpenAPI_ue_id_t *ue_id);
OpenAPI_ue_id_t *OpenAPI_ue_id_parseFromJSON(cJSON *ue_idJSON);
cJSON *OpenAPI_ue_id_convertToJSON(OpenAPI_ue_id_t *ue_id);
OpenAPI_ue_id_t *OpenAPI_ue_id_copy(OpenAPI_ue_id_t *dst, OpenAPI_ue_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_id_H_ */

