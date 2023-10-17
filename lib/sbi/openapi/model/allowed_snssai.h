/*
 * allowed_snssai.h
 *
 * Contains the authorized S-NSSAI and optional mapped home S-NSSAI and network slice instance information 
 */

#ifndef _OpenAPI_allowed_snssai_H_
#define _OpenAPI_allowed_snssai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nsi_information.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_allowed_snssai_s OpenAPI_allowed_snssai_t;
typedef struct OpenAPI_allowed_snssai_s {
    struct OpenAPI_snssai_s *allowed_snssai;
    OpenAPI_list_t *nsi_information_list;
    struct OpenAPI_snssai_s *mapped_home_snssai;
} OpenAPI_allowed_snssai_t;

OpenAPI_allowed_snssai_t *OpenAPI_allowed_snssai_create(
    OpenAPI_snssai_t *allowed_snssai,
    OpenAPI_list_t *nsi_information_list,
    OpenAPI_snssai_t *mapped_home_snssai
);
void OpenAPI_allowed_snssai_free(OpenAPI_allowed_snssai_t *allowed_snssai);
OpenAPI_allowed_snssai_t *OpenAPI_allowed_snssai_parseFromJSON(cJSON *allowed_snssaiJSON);
cJSON *OpenAPI_allowed_snssai_convertToJSON(OpenAPI_allowed_snssai_t *allowed_snssai);
OpenAPI_allowed_snssai_t *OpenAPI_allowed_snssai_copy(OpenAPI_allowed_snssai_t *dst, OpenAPI_allowed_snssai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allowed_snssai_H_ */

