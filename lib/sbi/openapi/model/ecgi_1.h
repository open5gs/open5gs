/*
 * ecgi_1.h
 *
 * 
 */

#ifndef _OpenAPI_ecgi_1_H_
#define _OpenAPI_ecgi_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ecgi_1_s OpenAPI_ecgi_1_t;
typedef struct OpenAPI_ecgi_1_s {
    struct OpenAPI_plmn_id_1_s *plmn_id;
    char *eutra_cell_id;
    char *nid;
} OpenAPI_ecgi_1_t;

OpenAPI_ecgi_1_t *OpenAPI_ecgi_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *eutra_cell_id,
    char *nid
);
void OpenAPI_ecgi_1_free(OpenAPI_ecgi_1_t *ecgi_1);
OpenAPI_ecgi_1_t *OpenAPI_ecgi_1_parseFromJSON(cJSON *ecgi_1JSON);
cJSON *OpenAPI_ecgi_1_convertToJSON(OpenAPI_ecgi_1_t *ecgi_1);
OpenAPI_ecgi_1_t *OpenAPI_ecgi_1_copy(OpenAPI_ecgi_1_t *dst, OpenAPI_ecgi_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecgi_1_H_ */

