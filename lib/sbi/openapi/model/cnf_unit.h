/*
 * cnf_unit.h
 *
 * During the processing of cnfUnits attribute, all the members in the array shall be  interpreted as logically concatenated with logical \&quot;AND\&quot;. 
 */

#ifndef _OpenAPI_cnf_unit_H_
#define _OpenAPI_cnf_unit_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "atom.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cnf_unit_s OpenAPI_cnf_unit_t;
typedef struct OpenAPI_cnf_unit_s {
    OpenAPI_list_t *cnf_unit;
} OpenAPI_cnf_unit_t;

OpenAPI_cnf_unit_t *OpenAPI_cnf_unit_create(
    OpenAPI_list_t *cnf_unit
);
void OpenAPI_cnf_unit_free(OpenAPI_cnf_unit_t *cnf_unit);
OpenAPI_cnf_unit_t *OpenAPI_cnf_unit_parseFromJSON(cJSON *cnf_unitJSON);
cJSON *OpenAPI_cnf_unit_convertToJSON(OpenAPI_cnf_unit_t *cnf_unit);
OpenAPI_cnf_unit_t *OpenAPI_cnf_unit_copy(OpenAPI_cnf_unit_t *dst, OpenAPI_cnf_unit_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cnf_unit_H_ */

