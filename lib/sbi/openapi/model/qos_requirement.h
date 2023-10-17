/*
 * qos_requirement.h
 *
 * Represents the QoS requirements.
 */

#ifndef _OpenAPI_qos_requirement_H_
#define _OpenAPI_qos_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_resource_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_requirement_s OpenAPI_qos_requirement_t;
typedef struct OpenAPI_qos_requirement_s {
    bool is__5qi;
    int _5qi;
    char *gfbr_ul;
    char *gfbr_dl;
    OpenAPI_qos_resource_type_e res_type;
    bool is_pdb;
    int pdb;
    char *per;
} OpenAPI_qos_requirement_t;

OpenAPI_qos_requirement_t *OpenAPI_qos_requirement_create(
    bool is__5qi,
    int _5qi,
    char *gfbr_ul,
    char *gfbr_dl,
    OpenAPI_qos_resource_type_e res_type,
    bool is_pdb,
    int pdb,
    char *per
);
void OpenAPI_qos_requirement_free(OpenAPI_qos_requirement_t *qos_requirement);
OpenAPI_qos_requirement_t *OpenAPI_qos_requirement_parseFromJSON(cJSON *qos_requirementJSON);
cJSON *OpenAPI_qos_requirement_convertToJSON(OpenAPI_qos_requirement_t *qos_requirement);
OpenAPI_qos_requirement_t *OpenAPI_qos_requirement_copy(OpenAPI_qos_requirement_t *dst, OpenAPI_qos_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_requirement_H_ */

