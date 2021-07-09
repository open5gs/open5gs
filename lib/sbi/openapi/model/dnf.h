/*
 * dnf.h
 *
 * 
 */

#ifndef _OpenAPI_dnf_H_
#define _OpenAPI_dnf_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dnf_unit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnf_s OpenAPI_dnf_t;
typedef struct OpenAPI_dnf_s {
    OpenAPI_list_t *dnf_units;
} OpenAPI_dnf_t;

OpenAPI_dnf_t *OpenAPI_dnf_create(
    OpenAPI_list_t *dnf_units
);
void OpenAPI_dnf_free(OpenAPI_dnf_t *dnf);
OpenAPI_dnf_t *OpenAPI_dnf_parseFromJSON(cJSON *dnfJSON);
cJSON *OpenAPI_dnf_convertToJSON(OpenAPI_dnf_t *dnf);
OpenAPI_dnf_t *OpenAPI_dnf_copy(OpenAPI_dnf_t *dst, OpenAPI_dnf_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnf_H_ */

