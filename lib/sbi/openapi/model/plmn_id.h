/*
 * plmn_id.h
 *
 * 
 */

#ifndef _OpenAPI_plmn_id_H_
#define _OpenAPI_plmn_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_id_s OpenAPI_plmn_id_t;
typedef struct OpenAPI_plmn_id_s {
    char *mcc;
    char *mnc;
} OpenAPI_plmn_id_t;

OpenAPI_plmn_id_t *OpenAPI_plmn_id_create(
    char *mcc,
    char *mnc
);
void OpenAPI_plmn_id_free(OpenAPI_plmn_id_t *plmn_id);
OpenAPI_plmn_id_t *OpenAPI_plmn_id_parseFromJSON(cJSON *plmn_idJSON);
cJSON *OpenAPI_plmn_id_convertToJSON(OpenAPI_plmn_id_t *plmn_id);
OpenAPI_plmn_id_t *OpenAPI_plmn_id_copy(OpenAPI_plmn_id_t *dst, OpenAPI_plmn_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_id_H_ */

