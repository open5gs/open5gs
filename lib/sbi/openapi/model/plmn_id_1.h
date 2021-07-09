/*
 * plmn_id_1.h
 *
 * 
 */

#ifndef _OpenAPI_plmn_id_1_H_
#define _OpenAPI_plmn_id_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_id_1_s OpenAPI_plmn_id_1_t;
typedef struct OpenAPI_plmn_id_1_s {
    char *mcc;
    char *mnc;
} OpenAPI_plmn_id_1_t;

OpenAPI_plmn_id_1_t *OpenAPI_plmn_id_1_create(
    char *mcc,
    char *mnc
);
void OpenAPI_plmn_id_1_free(OpenAPI_plmn_id_1_t *plmn_id_1);
OpenAPI_plmn_id_1_t *OpenAPI_plmn_id_1_parseFromJSON(cJSON *plmn_id_1JSON);
cJSON *OpenAPI_plmn_id_1_convertToJSON(OpenAPI_plmn_id_1_t *plmn_id_1);
OpenAPI_plmn_id_1_t *OpenAPI_plmn_id_1_copy(OpenAPI_plmn_id_1_t *dst, OpenAPI_plmn_id_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_id_1_H_ */

