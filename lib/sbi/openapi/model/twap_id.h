/*
 * twap_id.h
 *
 * Contain the TWAP Identifier as defined in clause 4.2.8.5.3 of 3GPP TS 23.501 or the WLAN location information as defined in clause 4.5.7.2.8 of 3GPP TS 23.402. 
 */

#ifndef _OpenAPI_twap_id_H_
#define _OpenAPI_twap_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_twap_id_s OpenAPI_twap_id_t;
typedef struct OpenAPI_twap_id_s {
    char *ss_id;
    char *bss_id;
    char *civic_address;
} OpenAPI_twap_id_t;

OpenAPI_twap_id_t *OpenAPI_twap_id_create(
    char *ss_id,
    char *bss_id,
    char *civic_address
);
void OpenAPI_twap_id_free(OpenAPI_twap_id_t *twap_id);
OpenAPI_twap_id_t *OpenAPI_twap_id_parseFromJSON(cJSON *twap_idJSON);
cJSON *OpenAPI_twap_id_convertToJSON(OpenAPI_twap_id_t *twap_id);
OpenAPI_twap_id_t *OpenAPI_twap_id_copy(OpenAPI_twap_id_t *dst, OpenAPI_twap_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_twap_id_H_ */

