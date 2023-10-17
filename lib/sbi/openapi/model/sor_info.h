/*
 * sor_info.h
 *
 * 
 */

#ifndef _OpenAPI_sor_info_H_
#define _OpenAPI_sor_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "steering_container.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sor_info_s OpenAPI_sor_info_t;
typedef struct OpenAPI_sor_info_s {
    struct OpenAPI_steering_container_s *steering_container;
    int ack_ind;
    char *sor_mac_iausf;
    char *countersor;
    char *provisioning_time;
    char *sor_transparent_container;
    char *sor_cmci;
    bool is_store_sor_cmci_in_me;
    int store_sor_cmci_in_me;
    bool is_usim_support_of_sor_cmci;
    int usim_support_of_sor_cmci;
} OpenAPI_sor_info_t;

OpenAPI_sor_info_t *OpenAPI_sor_info_create(
    OpenAPI_steering_container_t *steering_container,
    int ack_ind,
    char *sor_mac_iausf,
    char *countersor,
    char *provisioning_time,
    char *sor_transparent_container,
    char *sor_cmci,
    bool is_store_sor_cmci_in_me,
    int store_sor_cmci_in_me,
    bool is_usim_support_of_sor_cmci,
    int usim_support_of_sor_cmci
);
void OpenAPI_sor_info_free(OpenAPI_sor_info_t *sor_info);
OpenAPI_sor_info_t *OpenAPI_sor_info_parseFromJSON(cJSON *sor_infoJSON);
cJSON *OpenAPI_sor_info_convertToJSON(OpenAPI_sor_info_t *sor_info);
OpenAPI_sor_info_t *OpenAPI_sor_info_copy(OpenAPI_sor_info_t *dst, OpenAPI_sor_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sor_info_H_ */

