/*
 * vlan_tag_handling.h
 *
 * VLAN Tag Handling information
 */

#ifndef _OpenAPI_vlan_tag_handling_H_
#define _OpenAPI_vlan_tag_handling_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vlan_tag_handling_s OpenAPI_vlan_tag_handling_t;
#include "vlan_tag_type.h"
#include "vlan_tag_value.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vlan_tag_handling_s {
    struct OpenAPI_vlan_tag_value_s *vlan_tag;
    OpenAPI_vlan_tag_type_e vlan_tag_type;
};

OpenAPI_vlan_tag_handling_t *OpenAPI_vlan_tag_handling_create(
    OpenAPI_vlan_tag_value_t *vlan_tag,
    OpenAPI_vlan_tag_type_e vlan_tag_type
);
void OpenAPI_vlan_tag_handling_free(OpenAPI_vlan_tag_handling_t *vlan_tag_handling);
OpenAPI_vlan_tag_handling_t *OpenAPI_vlan_tag_handling_parseFromJSON(cJSON *vlan_tag_handlingJSON);
cJSON *OpenAPI_vlan_tag_handling_convertToJSON(OpenAPI_vlan_tag_handling_t *vlan_tag_handling);
OpenAPI_vlan_tag_handling_t *OpenAPI_vlan_tag_handling_copy(OpenAPI_vlan_tag_handling_t *dst, OpenAPI_vlan_tag_handling_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vlan_tag_handling_H_ */

