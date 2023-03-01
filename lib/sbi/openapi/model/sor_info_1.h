/*
 * sor_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_sor_info_1_H_
#define _OpenAPI_sor_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "steering_container.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sor_info_1_s OpenAPI_sor_info_1_t;
typedef struct OpenAPI_sor_info_1_s {
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
} OpenAPI_sor_info_1_t;

OpenAPI_sor_info_1_t *OpenAPI_sor_info_1_create(
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
void OpenAPI_sor_info_1_free(OpenAPI_sor_info_1_t *sor_info_1);
OpenAPI_sor_info_1_t *OpenAPI_sor_info_1_parseFromJSON(cJSON *sor_info_1JSON);
cJSON *OpenAPI_sor_info_1_convertToJSON(OpenAPI_sor_info_1_t *sor_info_1);
OpenAPI_sor_info_1_t *OpenAPI_sor_info_1_copy(OpenAPI_sor_info_1_t *dst, OpenAPI_sor_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sor_info_1_H_ */

