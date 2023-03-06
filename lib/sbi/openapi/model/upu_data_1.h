/*
 * upu_data_1.h
 *
 * Contains UE parameters update data set (e.g., the updated Routing ID Data or the Default configured NSSAI).
 */

#ifndef _OpenAPI_upu_data_1_H_
#define _OpenAPI_upu_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_upu_data_1_s OpenAPI_upu_data_1_t;
typedef struct OpenAPI_upu_data_1_s {
    char *sec_packet;
    OpenAPI_list_t *default_conf_nssai;
    char *routing_id;
} OpenAPI_upu_data_1_t;

OpenAPI_upu_data_1_t *OpenAPI_upu_data_1_create(
    char *sec_packet,
    OpenAPI_list_t *default_conf_nssai,
    char *routing_id
);
void OpenAPI_upu_data_1_free(OpenAPI_upu_data_1_t *upu_data_1);
OpenAPI_upu_data_1_t *OpenAPI_upu_data_1_parseFromJSON(cJSON *upu_data_1JSON);
cJSON *OpenAPI_upu_data_1_convertToJSON(OpenAPI_upu_data_1_t *upu_data_1);
OpenAPI_upu_data_1_t *OpenAPI_upu_data_1_copy(OpenAPI_upu_data_1_t *dst, OpenAPI_upu_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upu_data_1_H_ */

