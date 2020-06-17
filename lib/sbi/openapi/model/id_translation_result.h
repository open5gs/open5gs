/*
 * id_translation_result.h
 *
 *
 */

#ifndef _OpenAPI_id_translation_result_H_
#define _OpenAPI_id_translation_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_id_translation_result_s OpenAPI_id_translation_result_t;
typedef struct OpenAPI_id_translation_result_s {
    char *supported_features;
    char *supi;
    char *gpsi;
} OpenAPI_id_translation_result_t;

OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_create(
    char *supported_features,
    char *supi,
    char *gpsi
    );
void OpenAPI_id_translation_result_free(OpenAPI_id_translation_result_t *id_translation_result);
OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_parseFromJSON(cJSON *id_translation_resultJSON);
cJSON *OpenAPI_id_translation_result_convertToJSON(OpenAPI_id_translation_result_t *id_translation_result);
OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_copy(OpenAPI_id_translation_result_t *dst, OpenAPI_id_translation_result_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_id_translation_result_H_ */

