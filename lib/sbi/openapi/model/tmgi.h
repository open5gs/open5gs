/*
 * tmgi.h
 *
 * Temporary Mobile Group Identity
 */

#ifndef _OpenAPI_tmgi_H_
#define _OpenAPI_tmgi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tmgi_s OpenAPI_tmgi_t;
typedef struct OpenAPI_tmgi_s {
    char *mbs_service_id;
    struct OpenAPI_plmn_id_s *plmn_id;
} OpenAPI_tmgi_t;

OpenAPI_tmgi_t *OpenAPI_tmgi_create(
    char *mbs_service_id,
    OpenAPI_plmn_id_t *plmn_id
);
void OpenAPI_tmgi_free(OpenAPI_tmgi_t *tmgi);
OpenAPI_tmgi_t *OpenAPI_tmgi_parseFromJSON(cJSON *tmgiJSON);
cJSON *OpenAPI_tmgi_convertToJSON(OpenAPI_tmgi_t *tmgi);
OpenAPI_tmgi_t *OpenAPI_tmgi_copy(OpenAPI_tmgi_t *dst, OpenAPI_tmgi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tmgi_H_ */

