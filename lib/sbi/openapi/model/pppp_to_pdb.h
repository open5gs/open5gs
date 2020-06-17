/*
 * pppp_to_pdb.h
 *
 *
 */

#ifndef _OpenAPI_pppp_to_pdb_H_
#define _OpenAPI_pppp_to_pdb_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pppp_to_pdb_s OpenAPI_pppp_to_pdb_t;
typedef struct OpenAPI_pppp_to_pdb_s {
    int pppp;
    int pdb;
} OpenAPI_pppp_to_pdb_t;

OpenAPI_pppp_to_pdb_t *OpenAPI_pppp_to_pdb_create(
    int pppp,
    int pdb
    );
void OpenAPI_pppp_to_pdb_free(OpenAPI_pppp_to_pdb_t *pppp_to_pdb);
OpenAPI_pppp_to_pdb_t *OpenAPI_pppp_to_pdb_parseFromJSON(cJSON *pppp_to_pdbJSON);
cJSON *OpenAPI_pppp_to_pdb_convertToJSON(OpenAPI_pppp_to_pdb_t *pppp_to_pdb);
OpenAPI_pppp_to_pdb_t *OpenAPI_pppp_to_pdb_copy(OpenAPI_pppp_to_pdb_t *dst, OpenAPI_pppp_to_pdb_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pppp_to_pdb_H_ */

