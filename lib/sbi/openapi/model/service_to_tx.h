/*
 * service_to_tx.h
 *
 *
 */

#ifndef _OpenAPI_service_to_tx_H_
#define _OpenAPI_service_to_tx_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "v2x_rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_to_tx_s OpenAPI_service_to_tx_t;
typedef struct OpenAPI_service_to_tx_s {
    OpenAPI_list_t *ser_ids;
    char *tx_profile;
    struct OpenAPI_v2x_rat_type_s *rat;
} OpenAPI_service_to_tx_t;

OpenAPI_service_to_tx_t *OpenAPI_service_to_tx_create(
    OpenAPI_list_t *ser_ids,
    char *tx_profile,
    OpenAPI_v2x_rat_type_t *rat
    );
void OpenAPI_service_to_tx_free(OpenAPI_service_to_tx_t *service_to_tx);
OpenAPI_service_to_tx_t *OpenAPI_service_to_tx_parseFromJSON(cJSON *service_to_txJSON);
cJSON *OpenAPI_service_to_tx_convertToJSON(OpenAPI_service_to_tx_t *service_to_tx);
OpenAPI_service_to_tx_t *OpenAPI_service_to_tx_copy(OpenAPI_service_to_tx_t *dst, OpenAPI_service_to_tx_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_to_tx_H_ */

