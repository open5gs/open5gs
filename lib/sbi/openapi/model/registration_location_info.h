/*
 * registration_location_info.h
 *
 * 
 */

#ifndef _OpenAPI_registration_location_info_H_
#define _OpenAPI_registration_location_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "plmn_id.h"
#include "vgmlc_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_registration_location_info_s OpenAPI_registration_location_info_t;
typedef struct OpenAPI_registration_location_info_s {
    char *amf_instance_id;
    struct OpenAPI_plmn_id_s *plmn_id;
    struct OpenAPI_vgmlc_address_s *vgmlc_address;
    OpenAPI_list_t *access_type_list;
} OpenAPI_registration_location_info_t;

OpenAPI_registration_location_info_t *OpenAPI_registration_location_info_create(
    char *amf_instance_id,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_vgmlc_address_t *vgmlc_address,
    OpenAPI_list_t *access_type_list
);
void OpenAPI_registration_location_info_free(OpenAPI_registration_location_info_t *registration_location_info);
OpenAPI_registration_location_info_t *OpenAPI_registration_location_info_parseFromJSON(cJSON *registration_location_infoJSON);
cJSON *OpenAPI_registration_location_info_convertToJSON(OpenAPI_registration_location_info_t *registration_location_info);
OpenAPI_registration_location_info_t *OpenAPI_registration_location_info_copy(OpenAPI_registration_location_info_t *dst, OpenAPI_registration_location_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_location_info_H_ */

