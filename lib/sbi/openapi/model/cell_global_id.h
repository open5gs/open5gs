/*
 * cell_global_id.h
 *
 * Contains a Cell Global Identification as defined in 3GPP TS 23.003, clause 4.3.1.
 */

#ifndef _OpenAPI_cell_global_id_H_
#define _OpenAPI_cell_global_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cell_global_id_s OpenAPI_cell_global_id_t;
typedef struct OpenAPI_cell_global_id_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *lac;
    char *cell_id;
} OpenAPI_cell_global_id_t;

OpenAPI_cell_global_id_t *OpenAPI_cell_global_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *lac,
    char *cell_id
);
void OpenAPI_cell_global_id_free(OpenAPI_cell_global_id_t *cell_global_id);
OpenAPI_cell_global_id_t *OpenAPI_cell_global_id_parseFromJSON(cJSON *cell_global_idJSON);
cJSON *OpenAPI_cell_global_id_convertToJSON(OpenAPI_cell_global_id_t *cell_global_id);
OpenAPI_cell_global_id_t *OpenAPI_cell_global_id_copy(OpenAPI_cell_global_id_t *dst, OpenAPI_cell_global_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cell_global_id_H_ */

