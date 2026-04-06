/*
 * partially_allowed_snssai.h
 *
 * Indicates a S-NSSAI that is partially allowed in the Registration Area. 
 */

#ifndef _OpenAPI_partially_allowed_snssai_H_
#define _OpenAPI_partially_allowed_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_partially_allowed_snssai_s OpenAPI_partially_allowed_snssai_t;
#include "snssai.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_partially_allowed_snssai_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *allowed_tai_list;
};

OpenAPI_partially_allowed_snssai_t *OpenAPI_partially_allowed_snssai_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *allowed_tai_list
);
void OpenAPI_partially_allowed_snssai_free(OpenAPI_partially_allowed_snssai_t *partially_allowed_snssai);
OpenAPI_partially_allowed_snssai_t *OpenAPI_partially_allowed_snssai_parseFromJSON(cJSON *partially_allowed_snssaiJSON);
cJSON *OpenAPI_partially_allowed_snssai_convertToJSON(OpenAPI_partially_allowed_snssai_t *partially_allowed_snssai);
OpenAPI_partially_allowed_snssai_t *OpenAPI_partially_allowed_snssai_copy(OpenAPI_partially_allowed_snssai_t *dst, OpenAPI_partially_allowed_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_partially_allowed_snssai_H_ */

