/*
 * cnf.h
 *
 * A conjunctive normal form
 */

#ifndef _OpenAPI_cnf_H_
#define _OpenAPI_cnf_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cnf_unit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cnf_s OpenAPI_cnf_t;
typedef struct OpenAPI_cnf_s {
    OpenAPI_list_t *cnf_units;
} OpenAPI_cnf_t;

OpenAPI_cnf_t *OpenAPI_cnf_create(
    OpenAPI_list_t *cnf_units
);
void OpenAPI_cnf_free(OpenAPI_cnf_t *cnf);
OpenAPI_cnf_t *OpenAPI_cnf_parseFromJSON(cJSON *cnfJSON);
cJSON *OpenAPI_cnf_convertToJSON(OpenAPI_cnf_t *cnf);
OpenAPI_cnf_t *OpenAPI_cnf_copy(OpenAPI_cnf_t *dst, OpenAPI_cnf_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cnf_H_ */

