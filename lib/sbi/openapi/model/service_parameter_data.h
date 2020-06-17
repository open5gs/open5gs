/*
 * service_parameter_data.h
 *
 *
 */

#ifndef _OpenAPI_service_parameter_data_H_
#define _OpenAPI_service_parameter_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "parameter_over_pc5.h"
#include "parameter_over_uu.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_parameter_data_s OpenAPI_service_parameter_data_t;
typedef struct OpenAPI_service_parameter_data_s {
    char *app_id;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *inter_group_id;
    char *supi;
    char *ipv4_addr;
    char *ipv6_addr;
    char *mac_addr;
    struct OpenAPI_parameter_over_pc5_s *param_over_pc5;
    struct OpenAPI_parameter_over_uu_s *param_over_uu;
    char *supp_feat;
} OpenAPI_service_parameter_data_t;

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_create(
    char *app_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    char *supi,
    char *ipv4_addr,
    char *ipv6_addr,
    char *mac_addr,
    OpenAPI_parameter_over_pc5_t *param_over_pc5,
    OpenAPI_parameter_over_uu_t *param_over_uu,
    char *supp_feat
    );
void OpenAPI_service_parameter_data_free(OpenAPI_service_parameter_data_t *service_parameter_data);
OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_parseFromJSON(cJSON *service_parameter_dataJSON);
cJSON *OpenAPI_service_parameter_data_convertToJSON(OpenAPI_service_parameter_data_t *service_parameter_data);
OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_copy(OpenAPI_service_parameter_data_t *dst, OpenAPI_service_parameter_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_parameter_data_H_ */

