/*
 * up_interface_type.h
 *
 * Types of User-Plane interfaces of the UPF
 */

#ifndef _OpenAPI_up_interface_type_H_
#define _OpenAPI_up_interface_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_up_interface_type_NULL = 0, OpenAPI_up_interface_type_N3, OpenAPI_up_interface_type_N6, OpenAPI_up_interface_type_N9, OpenAPI_up_interface_type_DATA_FORWARDING, OpenAPI_up_interface_type_N3MB, OpenAPI_up_interface_type_N6MB, OpenAPI_up_interface_type_N19MB, OpenAPI_up_interface_type_NMB9, OpenAPI_up_interface_type_S1U, OpenAPI_up_interface_type_S5U, OpenAPI_up_interface_type_S8U, OpenAPI_up_interface_type_S11U, OpenAPI_up_interface_type_S12, OpenAPI_up_interface_type_S2AU, OpenAPI_up_interface_type_S2BU, OpenAPI_up_interface_type_N3TRUSTEDN3GPP, OpenAPI_up_interface_type_N3UNTRUSTEDN3GPP, OpenAPI_up_interface_type_N9ROAMING, OpenAPI_up_interface_type_SGI, OpenAPI_up_interface_type_N19, OpenAPI_up_interface_type_SXAU, OpenAPI_up_interface_type_SXBU, OpenAPI_up_interface_type_N4U, OpenAPI_up_interface_type_GNU, OpenAPI_up_interface_type_GPU } OpenAPI_up_interface_type_e;

char* OpenAPI_up_interface_type_ToString(OpenAPI_up_interface_type_e up_interface_type);

OpenAPI_up_interface_type_e OpenAPI_up_interface_type_FromString(char* up_interface_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_interface_type_H_ */

