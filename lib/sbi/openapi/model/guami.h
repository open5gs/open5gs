/*
 * guami.h
 *
 * 
 */

#ifndef _OpenAPI_guami_H_
#define _OpenAPI_guami_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id_nid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_guami_s OpenAPI_guami_t;
typedef struct OpenAPI_guami_s {
    struct OpenAPI_plmn_id_nid_s *plmn_id;
    char *amf_id;
} OpenAPI_guami_t;

OpenAPI_guami_t *OpenAPI_guami_create(
    OpenAPI_plmn_id_nid_t *plmn_id,
    char *amf_id
);
void OpenAPI_guami_free(OpenAPI_guami_t *guami);
OpenAPI_guami_t *OpenAPI_guami_parseFromJSON(cJSON *guamiJSON);
cJSON *OpenAPI_guami_convertToJSON(OpenAPI_guami_t *guami);
OpenAPI_guami_t *OpenAPI_guami_copy(OpenAPI_guami_t *dst, OpenAPI_guami_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_guami_H_ */

