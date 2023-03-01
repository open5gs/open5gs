/*
 * aanf_info.h
 *
 * Represents the information relative to an AAnF NF Instance.
 */

#ifndef _OpenAPI_aanf_info_H_
#define _OpenAPI_aanf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_aanf_info_s OpenAPI_aanf_info_t;
typedef struct OpenAPI_aanf_info_s {
    OpenAPI_list_t *routing_indicators;
} OpenAPI_aanf_info_t;

OpenAPI_aanf_info_t *OpenAPI_aanf_info_create(
    OpenAPI_list_t *routing_indicators
);
void OpenAPI_aanf_info_free(OpenAPI_aanf_info_t *aanf_info);
OpenAPI_aanf_info_t *OpenAPI_aanf_info_parseFromJSON(cJSON *aanf_infoJSON);
cJSON *OpenAPI_aanf_info_convertToJSON(OpenAPI_aanf_info_t *aanf_info);
OpenAPI_aanf_info_t *OpenAPI_aanf_info_copy(OpenAPI_aanf_info_t *dst, OpenAPI_aanf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aanf_info_H_ */

