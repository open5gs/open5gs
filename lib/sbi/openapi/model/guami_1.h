/*
 * guami_1.h
 *
 * Globally Unique AMF Identifier constructed out of PLMN, Network and AMF identity.
 */

#ifndef _OpenAPI_guami_1_H_
#define _OpenAPI_guami_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_guami_1_s OpenAPI_guami_1_t;
#include "plmn_id_nid_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_guami_1_s {
    struct OpenAPI_plmn_id_nid_1_s *plmn_id;
    char *amf_id;
};

OpenAPI_guami_1_t *OpenAPI_guami_1_create(
    OpenAPI_plmn_id_nid_1_t *plmn_id,
    char *amf_id
);
void OpenAPI_guami_1_free(OpenAPI_guami_1_t *guami_1);
OpenAPI_guami_1_t *OpenAPI_guami_1_parseFromJSON(cJSON *guami_1JSON);
cJSON *OpenAPI_guami_1_convertToJSON(OpenAPI_guami_1_t *guami_1);
OpenAPI_guami_1_t *OpenAPI_guami_1_copy(OpenAPI_guami_1_t *dst, OpenAPI_guami_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_guami_1_H_ */

