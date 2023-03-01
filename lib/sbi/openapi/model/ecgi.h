/*
 * ecgi.h
 *
 * Contains the ECGI (E-UTRAN Cell Global Identity), as described in 3GPP 23.003
 */

#ifndef _OpenAPI_ecgi_H_
#define _OpenAPI_ecgi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ecgi_s OpenAPI_ecgi_t;
typedef struct OpenAPI_ecgi_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *eutra_cell_id;
    char *nid;
} OpenAPI_ecgi_t;

OpenAPI_ecgi_t *OpenAPI_ecgi_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *eutra_cell_id,
    char *nid
);
void OpenAPI_ecgi_free(OpenAPI_ecgi_t *ecgi);
OpenAPI_ecgi_t *OpenAPI_ecgi_parseFromJSON(cJSON *ecgiJSON);
cJSON *OpenAPI_ecgi_convertToJSON(OpenAPI_ecgi_t *ecgi);
OpenAPI_ecgi_t *OpenAPI_ecgi_copy(OpenAPI_ecgi_t *dst, OpenAPI_ecgi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecgi_H_ */

