/*
 * nsi_information.h
 *
 *
 */

#ifndef _OpenAPI_nsi_information_H_
#define _OpenAPI_nsi_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nsi_information_s OpenAPI_nsi_information_t;
typedef struct OpenAPI_nsi_information_s {
    char *nrf_id;
    char *nsi_id;
    char *nrf_nf_mgt_uri;
    char *nrf_access_token_uri;
} OpenAPI_nsi_information_t;

OpenAPI_nsi_information_t *OpenAPI_nsi_information_create(
    char *nrf_id,
    char *nsi_id,
    char *nrf_nf_mgt_uri,
    char *nrf_access_token_uri
    );
void OpenAPI_nsi_information_free(OpenAPI_nsi_information_t *nsi_information);
OpenAPI_nsi_information_t *OpenAPI_nsi_information_parseFromJSON(cJSON *nsi_informationJSON);
cJSON *OpenAPI_nsi_information_convertToJSON(OpenAPI_nsi_information_t *nsi_information);
OpenAPI_nsi_information_t *OpenAPI_nsi_information_copy(OpenAPI_nsi_information_t *dst, OpenAPI_nsi_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsi_information_H_ */

