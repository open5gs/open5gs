/*
 * upu_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_upu_info_1_H_
#define _OpenAPI_upu_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "upu_data_2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_upu_info_1_s OpenAPI_upu_info_1_t;
typedef struct OpenAPI_upu_info_1_s {
    OpenAPI_list_t *upu_data_list;
    bool is_upu_reg_ind;
    int upu_reg_ind;
    bool is_upu_ack_ind;
    int upu_ack_ind;
    char *upu_mac_iausf;
    char *counter_upu;
    char *provisioning_time;
    char *upu_transparent_container;
} OpenAPI_upu_info_1_t;

OpenAPI_upu_info_1_t *OpenAPI_upu_info_1_create(
    OpenAPI_list_t *upu_data_list,
    bool is_upu_reg_ind,
    int upu_reg_ind,
    bool is_upu_ack_ind,
    int upu_ack_ind,
    char *upu_mac_iausf,
    char *counter_upu,
    char *provisioning_time,
    char *upu_transparent_container
);
void OpenAPI_upu_info_1_free(OpenAPI_upu_info_1_t *upu_info_1);
OpenAPI_upu_info_1_t *OpenAPI_upu_info_1_parseFromJSON(cJSON *upu_info_1JSON);
cJSON *OpenAPI_upu_info_1_convertToJSON(OpenAPI_upu_info_1_t *upu_info_1);
OpenAPI_upu_info_1_t *OpenAPI_upu_info_1_copy(OpenAPI_upu_info_1_t *dst, OpenAPI_upu_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upu_info_1_H_ */

