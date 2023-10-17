/*
 * model_5_g_ddnmf_info.h
 *
 * Information of an 5G DDNMF NF Instance
 */

#ifndef _OpenAPI_model_5_g_ddnmf_info_H_
#define _OpenAPI_model_5_g_ddnmf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_model_5_g_ddnmf_info_s OpenAPI_model_5_g_ddnmf_info_t;
typedef struct OpenAPI_model_5_g_ddnmf_info_s {
    struct OpenAPI_plmn_id_s *plmn_id;
} OpenAPI_model_5_g_ddnmf_info_t;

OpenAPI_model_5_g_ddnmf_info_t *OpenAPI_model_5_g_ddnmf_info_create(
    OpenAPI_plmn_id_t *plmn_id
);
void OpenAPI_model_5_g_ddnmf_info_free(OpenAPI_model_5_g_ddnmf_info_t *model_5_g_ddnmf_info);
OpenAPI_model_5_g_ddnmf_info_t *OpenAPI_model_5_g_ddnmf_info_parseFromJSON(cJSON *model_5_g_ddnmf_infoJSON);
cJSON *OpenAPI_model_5_g_ddnmf_info_convertToJSON(OpenAPI_model_5_g_ddnmf_info_t *model_5_g_ddnmf_info);
OpenAPI_model_5_g_ddnmf_info_t *OpenAPI_model_5_g_ddnmf_info_copy(OpenAPI_model_5_g_ddnmf_info_t *dst, OpenAPI_model_5_g_ddnmf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_g_ddnmf_info_H_ */

