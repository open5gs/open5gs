/*
 * nudm_amf_info.h
 *
 * 
 */

#ifndef _OpenAPI_nudm_amf_info_H_
#define _OpenAPI_nudm_amf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nudm_amf_info_s OpenAPI_nudm_amf_info_t;
typedef struct OpenAPI_nudm_amf_info_s {
    char *amf_instance_id;
    struct OpenAPI_guami_s *guami;
    OpenAPI_access_type_e access_type;
} OpenAPI_nudm_amf_info_t;

OpenAPI_nudm_amf_info_t *OpenAPI_nudm_amf_info_create(
    char *amf_instance_id,
    OpenAPI_guami_t *guami,
    OpenAPI_access_type_e access_type
);
void OpenAPI_nudm_amf_info_free(OpenAPI_nudm_amf_info_t *nudm_amf_info);
OpenAPI_nudm_amf_info_t *OpenAPI_nudm_amf_info_parseFromJSON(cJSON *nudm_amf_infoJSON);
cJSON *OpenAPI_nudm_amf_info_convertToJSON(OpenAPI_nudm_amf_info_t *nudm_amf_info);
OpenAPI_nudm_amf_info_t *OpenAPI_nudm_amf_info_copy(OpenAPI_nudm_amf_info_t *dst, OpenAPI_nudm_amf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nudm_amf_info_H_ */

