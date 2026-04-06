/*
 * tac_nid.h
 *
 * contains a tracking area code and Network Identity.
 */

#ifndef _OpenAPI_tac_nid_H_
#define _OpenAPI_tac_nid_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tac_nid_s OpenAPI_tac_nid_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tac_nid_s {
    char *tac;
    char *nid;
};

OpenAPI_tac_nid_t *OpenAPI_tac_nid_create(
    char *tac,
    char *nid
);
void OpenAPI_tac_nid_free(OpenAPI_tac_nid_t *tac_nid);
OpenAPI_tac_nid_t *OpenAPI_tac_nid_parseFromJSON(cJSON *tac_nidJSON);
cJSON *OpenAPI_tac_nid_convertToJSON(OpenAPI_tac_nid_t *tac_nid);
OpenAPI_tac_nid_t *OpenAPI_tac_nid_copy(OpenAPI_tac_nid_t *dst, OpenAPI_tac_nid_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tac_nid_H_ */

