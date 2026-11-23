/*
 * allocate_unit.h
 *
 * 
 */

#ifndef _OpenAPI_allocate_unit_H_
#define _OpenAPI_allocate_unit_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_allocate_unit_s OpenAPI_allocate_unit_t;
#include "allocate_unit_indicator.h"
#include "nsac_container_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_allocate_unit_s {
    OpenAPI_allocate_unit_indicator_e allocate_unit_indicator;
    struct OpenAPI_nsac_container_information_s *n_sac_container_information;
};

OpenAPI_allocate_unit_t *OpenAPI_allocate_unit_create(
    OpenAPI_allocate_unit_indicator_e allocate_unit_indicator,
    OpenAPI_nsac_container_information_t *n_sac_container_information
);
void OpenAPI_allocate_unit_free(OpenAPI_allocate_unit_t *allocate_unit);
OpenAPI_allocate_unit_t *OpenAPI_allocate_unit_parseFromJSON(cJSON *allocate_unitJSON);
cJSON *OpenAPI_allocate_unit_convertToJSON(OpenAPI_allocate_unit_t *allocate_unit);
OpenAPI_allocate_unit_t *OpenAPI_allocate_unit_copy(OpenAPI_allocate_unit_t *dst, OpenAPI_allocate_unit_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allocate_unit_H_ */

