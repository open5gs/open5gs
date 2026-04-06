/*
 * pei_update_info_ext.h
 *
 * Additional attributes to the object PeiUpdateInfo
 */

#ifndef _OpenAPI_pei_update_info_ext_H_
#define _OpenAPI_pei_update_info_ext_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pei_update_info_ext_s OpenAPI_pei_update_info_ext_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pei_update_info_ext_s {
    char *last_pei_change_timestamp;
    char *last_imei_change_timestamp;
    char *previous_pei;
    char *previous_pei_timestamp;
};

OpenAPI_pei_update_info_ext_t *OpenAPI_pei_update_info_ext_create(
    char *last_pei_change_timestamp,
    char *last_imei_change_timestamp,
    char *previous_pei,
    char *previous_pei_timestamp
);
void OpenAPI_pei_update_info_ext_free(OpenAPI_pei_update_info_ext_t *pei_update_info_ext);
OpenAPI_pei_update_info_ext_t *OpenAPI_pei_update_info_ext_parseFromJSON(cJSON *pei_update_info_extJSON);
cJSON *OpenAPI_pei_update_info_ext_convertToJSON(OpenAPI_pei_update_info_ext_t *pei_update_info_ext);
OpenAPI_pei_update_info_ext_t *OpenAPI_pei_update_info_ext_copy(OpenAPI_pei_update_info_ext_t *dst, OpenAPI_pei_update_info_ext_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pei_update_info_ext_H_ */

