/*
 * vlan_tag_value.h
 *
 * VLAN Tag Value
 */

#ifndef _OpenAPI_vlan_tag_value_H_
#define _OpenAPI_vlan_tag_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vlan_tag_value_s OpenAPI_vlan_tag_value_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vlan_tag_value_s {
    char *vlan_tag_value;
    char *vid;
    char *pcp;
    char *dei;
};

OpenAPI_vlan_tag_value_t *OpenAPI_vlan_tag_value_create(
    char *vlan_tag_value,
    char *vid,
    char *pcp,
    char *dei
);
void OpenAPI_vlan_tag_value_free(OpenAPI_vlan_tag_value_t *vlan_tag_value);
OpenAPI_vlan_tag_value_t *OpenAPI_vlan_tag_value_parseFromJSON(cJSON *vlan_tag_valueJSON);
cJSON *OpenAPI_vlan_tag_value_convertToJSON(OpenAPI_vlan_tag_value_t *vlan_tag_value);
OpenAPI_vlan_tag_value_t *OpenAPI_vlan_tag_value_copy(OpenAPI_vlan_tag_value_t *dst, OpenAPI_vlan_tag_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vlan_tag_value_H_ */

