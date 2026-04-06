/*
 * cell_id_nid.h
 *
 * contains a NR Cell Identity and Network Identity.
 */

#ifndef _OpenAPI_cell_id_nid_H_
#define _OpenAPI_cell_id_nid_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_cell_id_nid_s OpenAPI_cell_id_nid_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_cell_id_nid_s {
    char *cell_id;
    char *nid;
};

OpenAPI_cell_id_nid_t *OpenAPI_cell_id_nid_create(
    char *cell_id,
    char *nid
);
void OpenAPI_cell_id_nid_free(OpenAPI_cell_id_nid_t *cell_id_nid);
OpenAPI_cell_id_nid_t *OpenAPI_cell_id_nid_parseFromJSON(cJSON *cell_id_nidJSON);
cJSON *OpenAPI_cell_id_nid_convertToJSON(OpenAPI_cell_id_nid_t *cell_id_nid);
OpenAPI_cell_id_nid_t *OpenAPI_cell_id_nid_copy(OpenAPI_cell_id_nid_t *dst, OpenAPI_cell_id_nid_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cell_id_nid_H_ */

