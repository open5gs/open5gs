/*
 * amf_info_1.h
 *
 * Contains AMF Information
 */

#ifndef _OpenAPI_amf_info_1_H_
#define _OpenAPI_amf_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_amf_info_1_s OpenAPI_amf_info_1_t;
#include "access_type.h"
#include "guami_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_amf_info_1_s {
    char *amf_instance_id;
    struct OpenAPI_guami_1_s *guami;
    OpenAPI_access_type_e access_type;
};

OpenAPI_amf_info_1_t *OpenAPI_amf_info_1_create(
    char *amf_instance_id,
    OpenAPI_guami_1_t *guami,
    OpenAPI_access_type_e access_type
);
void OpenAPI_amf_info_1_free(OpenAPI_amf_info_1_t *amf_info_1);
OpenAPI_amf_info_1_t *OpenAPI_amf_info_1_parseFromJSON(cJSON *amf_info_1JSON);
cJSON *OpenAPI_amf_info_1_convertToJSON(OpenAPI_amf_info_1_t *amf_info_1);
OpenAPI_amf_info_1_t *OpenAPI_amf_info_1_copy(OpenAPI_amf_info_1_t *dst, OpenAPI_amf_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_info_1_H_ */

