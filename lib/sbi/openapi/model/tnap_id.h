/*
 * tnap_id.h
 *
 * Contain the TNAP Identifier see clause5.6.2 of 3GPP TS 23.501.
 */

#ifndef _OpenAPI_tnap_id_H_
#define _OpenAPI_tnap_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tnap_id_s OpenAPI_tnap_id_t;
typedef struct OpenAPI_tnap_id_s {
    char *ss_id;
    char *bss_id;
    char *civic_address;
} OpenAPI_tnap_id_t;

OpenAPI_tnap_id_t *OpenAPI_tnap_id_create(
    char *ss_id,
    char *bss_id,
    char *civic_address
);
void OpenAPI_tnap_id_free(OpenAPI_tnap_id_t *tnap_id);
OpenAPI_tnap_id_t *OpenAPI_tnap_id_parseFromJSON(cJSON *tnap_idJSON);
cJSON *OpenAPI_tnap_id_convertToJSON(OpenAPI_tnap_id_t *tnap_id);
OpenAPI_tnap_id_t *OpenAPI_tnap_id_copy(OpenAPI_tnap_id_t *dst, OpenAPI_tnap_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tnap_id_H_ */

