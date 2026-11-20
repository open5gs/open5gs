/*
 * charging_plmn_id.h
 *
 * This datatype is used for writable attribute
 */

#ifndef _OpenAPI_charging_plmn_id_H_
#define _OpenAPI_charging_plmn_id_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_plmn_id_s OpenAPI_charging_plmn_id_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_plmn_id_s {
    char *mcc;
    char *mnc;
};

OpenAPI_charging_plmn_id_t *OpenAPI_charging_plmn_id_create(
    char *mcc,
    char *mnc
);
void OpenAPI_charging_plmn_id_free(OpenAPI_charging_plmn_id_t *charging_plmn_id);
OpenAPI_charging_plmn_id_t *OpenAPI_charging_plmn_id_parseFromJSON(cJSON *charging_plmn_idJSON);
cJSON *OpenAPI_charging_plmn_id_convertToJSON(OpenAPI_charging_plmn_id_t *charging_plmn_id);
OpenAPI_charging_plmn_id_t *OpenAPI_charging_plmn_id_copy(OpenAPI_charging_plmn_id_t *dst, OpenAPI_charging_plmn_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_plmn_id_H_ */

