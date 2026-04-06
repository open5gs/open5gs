/*
 * vlan_tag_type.h
 *
 * Type of a Vlan Tag.
 */

#ifndef _OpenAPI_vlan_tag_type_H_
#define _OpenAPI_vlan_tag_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_vlan_tag_type_NULL = 0, OpenAPI_vlan_tag_type_C_VLAN_TAG, OpenAPI_vlan_tag_type_S_VLAN_TAG } OpenAPI_vlan_tag_type_e;

char* OpenAPI_vlan_tag_type_ToString(OpenAPI_vlan_tag_type_e vlan_tag_type);

OpenAPI_vlan_tag_type_e OpenAPI_vlan_tag_type_FromString(char* vlan_tag_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vlan_tag_type_H_ */

