/*
 * intended_n32_purpose.h
 *
 * Indicates the intended N32 establishment purpose
 */

#ifndef _OpenAPI_intended_n32_purpose_H_
#define _OpenAPI_intended_n32_purpose_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n32_purpose.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_intended_n32_purpose_s OpenAPI_intended_n32_purpose_t;
typedef struct OpenAPI_intended_n32_purpose_s {
    OpenAPI_n32_purpose_e usage_purpose;
    char *additional_info;
    char *cause;
} OpenAPI_intended_n32_purpose_t;

OpenAPI_intended_n32_purpose_t *OpenAPI_intended_n32_purpose_create(
    OpenAPI_n32_purpose_e usage_purpose,
    char *additional_info,
    char *cause
);
void OpenAPI_intended_n32_purpose_free(OpenAPI_intended_n32_purpose_t *intended_n32_purpose);
OpenAPI_intended_n32_purpose_t *OpenAPI_intended_n32_purpose_parseFromJSON(cJSON *intended_n32_purposeJSON);
cJSON *OpenAPI_intended_n32_purpose_convertToJSON(OpenAPI_intended_n32_purpose_t *intended_n32_purpose);
OpenAPI_intended_n32_purpose_t *OpenAPI_intended_n32_purpose_copy(OpenAPI_intended_n32_purpose_t *dst, OpenAPI_intended_n32_purpose_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_intended_n32_purpose_H_ */

