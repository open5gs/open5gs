/*
 * serving_nf_identity.h
 *
 *
 */

#ifndef _OpenAPI_serving_nf_identity_H_
#define _OpenAPI_serving_nf_identity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "an_gw_address.h"
#include "guami.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_serving_nf_identity_s OpenAPI_serving_nf_identity_t;
typedef struct OpenAPI_serving_nf_identity_s {
    char *serv_nf_inst_id;
    struct OpenAPI_guami_s *guami;
    struct OpenAPI_an_gw_address_s *an_gw_addr;
} OpenAPI_serving_nf_identity_t;

OpenAPI_serving_nf_identity_t *OpenAPI_serving_nf_identity_create(
    char *serv_nf_inst_id,
    OpenAPI_guami_t *guami,
    OpenAPI_an_gw_address_t *an_gw_addr
    );
void OpenAPI_serving_nf_identity_free(OpenAPI_serving_nf_identity_t *serving_nf_identity);
OpenAPI_serving_nf_identity_t *OpenAPI_serving_nf_identity_parseFromJSON(cJSON *serving_nf_identityJSON);
cJSON *OpenAPI_serving_nf_identity_convertToJSON(OpenAPI_serving_nf_identity_t *serving_nf_identity);
OpenAPI_serving_nf_identity_t *OpenAPI_serving_nf_identity_copy(OpenAPI_serving_nf_identity_t *dst, OpenAPI_serving_nf_identity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_serving_nf_identity_H_ */

