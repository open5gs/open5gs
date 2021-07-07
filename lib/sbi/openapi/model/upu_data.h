/*
 * upu_data.h
 *
 * 
 */

#ifndef _OpenAPI_upu_data_H_
#define _OpenAPI_upu_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_update_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_upu_data_s OpenAPI_upu_data_t;
typedef struct OpenAPI_upu_data_s {
    char *provisioning_time;
    OpenAPI_ue_update_status_e ue_update_status;
    char *upu_xmac_iue;
    char *upu_mac_iue;
} OpenAPI_upu_data_t;

OpenAPI_upu_data_t *OpenAPI_upu_data_create(
    char *provisioning_time,
    OpenAPI_ue_update_status_e ue_update_status,
    char *upu_xmac_iue,
    char *upu_mac_iue
);
void OpenAPI_upu_data_free(OpenAPI_upu_data_t *upu_data);
OpenAPI_upu_data_t *OpenAPI_upu_data_parseFromJSON(cJSON *upu_dataJSON);
cJSON *OpenAPI_upu_data_convertToJSON(OpenAPI_upu_data_t *upu_data);
OpenAPI_upu_data_t *OpenAPI_upu_data_copy(OpenAPI_upu_data_t *dst, OpenAPI_upu_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upu_data_H_ */

