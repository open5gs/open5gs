/*
 * dnf_unit.h
 *
 *
 */

#ifndef _OpenAPI_dnf_unit_H_
#define _OpenAPI_dnf_unit_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "atom.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnf_unit_s OpenAPI_dnf_unit_t;
typedef struct OpenAPI_dnf_unit_s {
    OpenAPI_list_t *dnf_unit;
} OpenAPI_dnf_unit_t;

OpenAPI_dnf_unit_t *OpenAPI_dnf_unit_create(
    OpenAPI_list_t *dnf_unit
    );
void OpenAPI_dnf_unit_free(OpenAPI_dnf_unit_t *dnf_unit);
OpenAPI_dnf_unit_t *OpenAPI_dnf_unit_parseFromJSON(cJSON *dnf_unitJSON);
cJSON *OpenAPI_dnf_unit_convertToJSON(OpenAPI_dnf_unit_t *dnf_unit);
OpenAPI_dnf_unit_t *OpenAPI_dnf_unit_copy(OpenAPI_dnf_unit_t *dst, OpenAPI_dnf_unit_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnf_unit_H_ */

