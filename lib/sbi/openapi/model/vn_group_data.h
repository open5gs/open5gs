/*
 * vn_group_data.h
 *
 * 
 */

#ifndef _OpenAPI_vn_group_data_H_
#define _OpenAPI_vn_group_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "app_descriptor.h"
#include "pdu_session_types_1.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_vn_group_data_s OpenAPI_vn_group_data_t;
typedef struct OpenAPI_vn_group_data_s {
    struct OpenAPI_pdu_session_types_1_s *pdu_session_types;
    char *dnn;
    struct OpenAPI_snssai_s *single_nssai;
    OpenAPI_list_t *app_descriptors;
} OpenAPI_vn_group_data_t;

OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_create(
    OpenAPI_pdu_session_types_1_t *pdu_session_types,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t *app_descriptors
);
void OpenAPI_vn_group_data_free(OpenAPI_vn_group_data_t *vn_group_data);
OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_parseFromJSON(cJSON *vn_group_dataJSON);
cJSON *OpenAPI_vn_group_data_convertToJSON(OpenAPI_vn_group_data_t *vn_group_data);
OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_copy(OpenAPI_vn_group_data_t *dst, OpenAPI_vn_group_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vn_group_data_H_ */

