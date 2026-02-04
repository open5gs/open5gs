/*
 * occurrence_info.h
 *
 * Specifies occurrence of event reporting.
 */

#ifndef _OpenAPI_occurrence_info_H_
#define _OpenAPI_occurrence_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "occurrence_info_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_occurrence_info_s OpenAPI_occurrence_info_t;
typedef struct OpenAPI_occurrence_info_s {
} OpenAPI_occurrence_info_t;

OpenAPI_occurrence_info_t *OpenAPI_occurrence_info_create(void);
void OpenAPI_occurrence_info_free(OpenAPI_occurrence_info_t *occurrence_info);
OpenAPI_occurrence_info_t *OpenAPI_occurrence_info_parseFromJSON(cJSON *occurrence_infoJSON);
cJSON *OpenAPI_occurrence_info_convertToJSON(OpenAPI_occurrence_info_t *occurrence_info);
OpenAPI_occurrence_info_t *OpenAPI_occurrence_info_copy(OpenAPI_occurrence_info_t *dst, OpenAPI_occurrence_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_occurrence_info_H_ */

