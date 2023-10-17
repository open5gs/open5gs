/*
 * atom.h
 *
 * contains a search parameter and its positive or negative content.
 */

#ifndef _OpenAPI_atom_H_
#define _OpenAPI_atom_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "any_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_atom_s OpenAPI_atom_t;
typedef struct OpenAPI_atom_s {
    char *attr;
    bool is_value_null;
    OpenAPI_any_type_t *value;
    bool is_negative;
    int negative;
} OpenAPI_atom_t;

OpenAPI_atom_t *OpenAPI_atom_create(
    char *attr,
    bool is_value_null,
    OpenAPI_any_type_t *value,
    bool is_negative,
    int negative
);
void OpenAPI_atom_free(OpenAPI_atom_t *atom);
OpenAPI_atom_t *OpenAPI_atom_parseFromJSON(cJSON *atomJSON);
cJSON *OpenAPI_atom_convertToJSON(OpenAPI_atom_t *atom);
OpenAPI_atom_t *OpenAPI_atom_copy(OpenAPI_atom_t *dst, OpenAPI_atom_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_atom_H_ */

