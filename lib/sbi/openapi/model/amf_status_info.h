/*
 * amf_status_info.h
 *
 * 
 */

#ifndef _OpenAPI_amf_status_info_H_
#define _OpenAPI_amf_status_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "guami.h"
#include "status_change.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_status_info_s OpenAPI_amf_status_info_t;
typedef struct OpenAPI_amf_status_info_s {
    OpenAPI_list_t *guami_list;
    OpenAPI_status_change_e status_change;
    char *target_amf_removal;
    char *target_amf_failure;
} OpenAPI_amf_status_info_t;

OpenAPI_amf_status_info_t *OpenAPI_amf_status_info_create(
    OpenAPI_list_t *guami_list,
    OpenAPI_status_change_e status_change,
    char *target_amf_removal,
    char *target_amf_failure
);
void OpenAPI_amf_status_info_free(OpenAPI_amf_status_info_t *amf_status_info);
OpenAPI_amf_status_info_t *OpenAPI_amf_status_info_parseFromJSON(cJSON *amf_status_infoJSON);
cJSON *OpenAPI_amf_status_info_convertToJSON(OpenAPI_amf_status_info_t *amf_status_info);
OpenAPI_amf_status_info_t *OpenAPI_amf_status_info_copy(OpenAPI_amf_status_info_t *dst, OpenAPI_amf_status_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_status_info_H_ */

