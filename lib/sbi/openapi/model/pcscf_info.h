/*
 * pcscf_info.h
 *
 * Information of a P-CSCF NF Instance
 */

#ifndef _OpenAPI_pcscf_info_H_
#define _OpenAPI_pcscf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pcscf_info_s OpenAPI_pcscf_info_t;
typedef struct OpenAPI_pcscf_info_s {
    OpenAPI_list_t *access_type;
    OpenAPI_list_t *dnn_list;
} OpenAPI_pcscf_info_t;

OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_create(
    OpenAPI_list_t *access_type,
    OpenAPI_list_t *dnn_list
    );
void OpenAPI_pcscf_info_free(OpenAPI_pcscf_info_t *pcscf_info);
OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_parseFromJSON(cJSON *pcscf_infoJSON);
cJSON *OpenAPI_pcscf_info_convertToJSON(OpenAPI_pcscf_info_t *pcscf_info);
OpenAPI_pcscf_info_t *OpenAPI_pcscf_info_copy(OpenAPI_pcscf_info_t *dst, OpenAPI_pcscf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pcscf_info_H_ */

