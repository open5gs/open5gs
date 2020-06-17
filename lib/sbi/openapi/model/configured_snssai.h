/*
 * configured_snssai.h
 *
 *
 */

#ifndef _OpenAPI_configured_snssai_H_
#define _OpenAPI_configured_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_configured_snssai_s OpenAPI_configured_snssai_t;
typedef struct OpenAPI_configured_snssai_s {
    struct OpenAPI_snssai_s *configured_snssai;
    struct OpenAPI_snssai_s *mapped_home_snssai;
} OpenAPI_configured_snssai_t;

OpenAPI_configured_snssai_t *OpenAPI_configured_snssai_create(
    OpenAPI_snssai_t *configured_snssai,
    OpenAPI_snssai_t *mapped_home_snssai
    );
void OpenAPI_configured_snssai_free(OpenAPI_configured_snssai_t *configured_snssai);
OpenAPI_configured_snssai_t *OpenAPI_configured_snssai_parseFromJSON(cJSON *configured_snssaiJSON);
cJSON *OpenAPI_configured_snssai_convertToJSON(OpenAPI_configured_snssai_t *configured_snssai);
OpenAPI_configured_snssai_t *OpenAPI_configured_snssai_copy(OpenAPI_configured_snssai_t *dst, OpenAPI_configured_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_configured_snssai_H_ */

