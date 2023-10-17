/*
 * ncgi_1.h
 *
 * Contains the NCGI (NR Cell Global Identity), as described in 3GPP 23.003
 */

#ifndef _OpenAPI_ncgi_1_H_
#define _OpenAPI_ncgi_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ncgi_1_s OpenAPI_ncgi_1_t;
typedef struct OpenAPI_ncgi_1_s {
    struct OpenAPI_plmn_id_1_s *plmn_id;
    char *nr_cell_id;
    char *nid;
} OpenAPI_ncgi_1_t;

OpenAPI_ncgi_1_t *OpenAPI_ncgi_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *nr_cell_id,
    char *nid
);
void OpenAPI_ncgi_1_free(OpenAPI_ncgi_1_t *ncgi_1);
OpenAPI_ncgi_1_t *OpenAPI_ncgi_1_parseFromJSON(cJSON *ncgi_1JSON);
cJSON *OpenAPI_ncgi_1_convertToJSON(OpenAPI_ncgi_1_t *ncgi_1);
OpenAPI_ncgi_1_t *OpenAPI_ncgi_1_copy(OpenAPI_ncgi_1_t *dst, OpenAPI_ncgi_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ncgi_1_H_ */

