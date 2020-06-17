/*
 * nf_info.h
 *
 * Information of a generic NF Instance
 */

#ifndef _OpenAPI_nf_info_H_
#define _OpenAPI_nf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_info_s OpenAPI_nf_info_t;
typedef struct OpenAPI_nf_info_s {
    OpenAPI_nf_type_e nf_type;
} OpenAPI_nf_info_t;

OpenAPI_nf_info_t *OpenAPI_nf_info_create(
    OpenAPI_nf_type_e nf_type
    );
void OpenAPI_nf_info_free(OpenAPI_nf_info_t *nf_info);
OpenAPI_nf_info_t *OpenAPI_nf_info_parseFromJSON(cJSON *nf_infoJSON);
cJSON *OpenAPI_nf_info_convertToJSON(OpenAPI_nf_info_t *nf_info);
OpenAPI_nf_info_t *OpenAPI_nf_info_copy(OpenAPI_nf_info_t *dst, OpenAPI_nf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_info_H_ */

