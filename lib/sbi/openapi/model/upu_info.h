/*
 * upu_info.h
 *
 * 
 */

#ifndef _OpenAPI_upu_info_H_
#define _OpenAPI_upu_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "upu_data_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_upu_info_s OpenAPI_upu_info_t;
typedef struct OpenAPI_upu_info_s {
    OpenAPI_list_t *upu_data_list;
    int upu_reg_ind;
    int upu_ack_ind;
    char *upu_mac_iausf;
    char *counter_upu;
    char *provisioning_time;
} OpenAPI_upu_info_t;

OpenAPI_upu_info_t *OpenAPI_upu_info_create(
    OpenAPI_list_t *upu_data_list,
    int upu_reg_ind,
    int upu_ack_ind,
    char *upu_mac_iausf,
    char *counter_upu,
    char *provisioning_time
);
void OpenAPI_upu_info_free(OpenAPI_upu_info_t *upu_info);
OpenAPI_upu_info_t *OpenAPI_upu_info_parseFromJSON(cJSON *upu_infoJSON);
cJSON *OpenAPI_upu_info_convertToJSON(OpenAPI_upu_info_t *upu_info);
OpenAPI_upu_info_t *OpenAPI_upu_info_copy(OpenAPI_upu_info_t *dst, OpenAPI_upu_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upu_info_H_ */

