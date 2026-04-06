/*
 * adm_info.h
 *
 * ADM information 
 */

#ifndef _OpenAPI_adm_info_H_
#define _OpenAPI_adm_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_adm_info_s OpenAPI_adm_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_adm_info_s {
    OpenAPI_list_t *device_id_list;
    char *dev_id_reg_ex;
    OpenAPI_list_t *af_id_list;
};

OpenAPI_adm_info_t *OpenAPI_adm_info_create(
    OpenAPI_list_t *device_id_list,
    char *dev_id_reg_ex,
    OpenAPI_list_t *af_id_list
);
void OpenAPI_adm_info_free(OpenAPI_adm_info_t *adm_info);
OpenAPI_adm_info_t *OpenAPI_adm_info_parseFromJSON(cJSON *adm_infoJSON);
cJSON *OpenAPI_adm_info_convertToJSON(OpenAPI_adm_info_t *adm_info);
OpenAPI_adm_info_t *OpenAPI_adm_info_copy(OpenAPI_adm_info_t *dst, OpenAPI_adm_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_adm_info_H_ */

