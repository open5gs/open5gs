/*
 * emergency_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_emergency_info_1_H_
#define _OpenAPI_emergency_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_address_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_emergency_info_1_s OpenAPI_emergency_info_1_t;
typedef struct OpenAPI_emergency_info_1_s {
    char *pgw_fqdn;
    struct OpenAPI_ip_address_1_s *pgw_ip_address;
    char *smf_instance_id;
    bool is_epdg_ind;
    int epdg_ind;
} OpenAPI_emergency_info_1_t;

OpenAPI_emergency_info_1_t *OpenAPI_emergency_info_1_create(
    char *pgw_fqdn,
    OpenAPI_ip_address_1_t *pgw_ip_address,
    char *smf_instance_id,
    bool is_epdg_ind,
    int epdg_ind
);
void OpenAPI_emergency_info_1_free(OpenAPI_emergency_info_1_t *emergency_info_1);
OpenAPI_emergency_info_1_t *OpenAPI_emergency_info_1_parseFromJSON(cJSON *emergency_info_1JSON);
cJSON *OpenAPI_emergency_info_1_convertToJSON(OpenAPI_emergency_info_1_t *emergency_info_1);
OpenAPI_emergency_info_1_t *OpenAPI_emergency_info_1_copy(OpenAPI_emergency_info_1_t *dst, OpenAPI_emergency_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_emergency_info_1_H_ */

