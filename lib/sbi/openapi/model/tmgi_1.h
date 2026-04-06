/*
 * tmgi_1.h
 *
 * Temporary Mobile Group Identity
 */

#ifndef _OpenAPI_tmgi_1_H_
#define _OpenAPI_tmgi_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tmgi_1_s OpenAPI_tmgi_1_t;
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tmgi_1_s {
    char *mbs_service_id;
    struct OpenAPI_plmn_id_s *plmn_id;
};

OpenAPI_tmgi_1_t *OpenAPI_tmgi_1_create(
    char *mbs_service_id,
    OpenAPI_plmn_id_t *plmn_id
);
void OpenAPI_tmgi_1_free(OpenAPI_tmgi_1_t *tmgi_1);
OpenAPI_tmgi_1_t *OpenAPI_tmgi_1_parseFromJSON(cJSON *tmgi_1JSON);
cJSON *OpenAPI_tmgi_1_convertToJSON(OpenAPI_tmgi_1_t *tmgi_1);
OpenAPI_tmgi_1_t *OpenAPI_tmgi_1_copy(OpenAPI_tmgi_1_t *dst, OpenAPI_tmgi_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tmgi_1_H_ */

