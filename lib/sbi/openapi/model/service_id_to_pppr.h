/*
 * service_id_to_pppr.h
 *
 *
 */

#ifndef _OpenAPI_service_id_to_pppr_H_
#define _OpenAPI_service_id_to_pppr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_id_to_pppr_s OpenAPI_service_id_to_pppr_t;
typedef struct OpenAPI_service_id_to_pppr_s {
    OpenAPI_list_t *ser_ids;
    int pppr;
} OpenAPI_service_id_to_pppr_t;

OpenAPI_service_id_to_pppr_t *OpenAPI_service_id_to_pppr_create(
    OpenAPI_list_t *ser_ids,
    int pppr
    );
void OpenAPI_service_id_to_pppr_free(OpenAPI_service_id_to_pppr_t *service_id_to_pppr);
OpenAPI_service_id_to_pppr_t *OpenAPI_service_id_to_pppr_parseFromJSON(cJSON *service_id_to_ppprJSON);
cJSON *OpenAPI_service_id_to_pppr_convertToJSON(OpenAPI_service_id_to_pppr_t *service_id_to_pppr);
OpenAPI_service_id_to_pppr_t *OpenAPI_service_id_to_pppr_copy(OpenAPI_service_id_to_pppr_t *dst, OpenAPI_service_id_to_pppr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_id_to_pppr_H_ */

