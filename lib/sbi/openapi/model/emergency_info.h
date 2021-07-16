/*
 * emergency_info.h
 *
 * 
 */

#ifndef _OpenAPI_emergency_info_H_
#define _OpenAPI_emergency_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_emergency_info_s OpenAPI_emergency_info_t;
typedef struct OpenAPI_emergency_info_s {
    char *pgw_fqdn;
    struct OpenAPI_ip_address_s *pgw_ip_address;
    char *smf_instance_id;
    bool is_epdg_ind;
    int epdg_ind;
} OpenAPI_emergency_info_t;

OpenAPI_emergency_info_t *OpenAPI_emergency_info_create(
    char *pgw_fqdn,
    OpenAPI_ip_address_t *pgw_ip_address,
    char *smf_instance_id,
    bool is_epdg_ind,
    int epdg_ind
);
void OpenAPI_emergency_info_free(OpenAPI_emergency_info_t *emergency_info);
OpenAPI_emergency_info_t *OpenAPI_emergency_info_parseFromJSON(cJSON *emergency_infoJSON);
cJSON *OpenAPI_emergency_info_convertToJSON(OpenAPI_emergency_info_t *emergency_info);
OpenAPI_emergency_info_t *OpenAPI_emergency_info_copy(OpenAPI_emergency_info_t *dst, OpenAPI_emergency_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_emergency_info_H_ */

