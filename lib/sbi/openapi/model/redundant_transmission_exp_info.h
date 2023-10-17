/*
 * redundant_transmission_exp_info.h
 *
 * The redundant transmission experience related information. When subscribed event is  \&quot;RED_TRANS_EXP\&quot;, the \&quot;redTransInfos\&quot; attribute shall be included. 
 */

#ifndef _OpenAPI_redundant_transmission_exp_info_H_
#define _OpenAPI_redundant_transmission_exp_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_area_info.h"
#include "redundant_transmission_exp_per_ts.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_redundant_transmission_exp_info_s OpenAPI_redundant_transmission_exp_info_t;
typedef struct OpenAPI_redundant_transmission_exp_info_s {
    struct OpenAPI_network_area_info_s *spatial_valid_con;
    char *dnn;
    OpenAPI_list_t *red_trans_exps;
} OpenAPI_redundant_transmission_exp_info_t;

OpenAPI_redundant_transmission_exp_info_t *OpenAPI_redundant_transmission_exp_info_create(
    OpenAPI_network_area_info_t *spatial_valid_con,
    char *dnn,
    OpenAPI_list_t *red_trans_exps
);
void OpenAPI_redundant_transmission_exp_info_free(OpenAPI_redundant_transmission_exp_info_t *redundant_transmission_exp_info);
OpenAPI_redundant_transmission_exp_info_t *OpenAPI_redundant_transmission_exp_info_parseFromJSON(cJSON *redundant_transmission_exp_infoJSON);
cJSON *OpenAPI_redundant_transmission_exp_info_convertToJSON(OpenAPI_redundant_transmission_exp_info_t *redundant_transmission_exp_info);
OpenAPI_redundant_transmission_exp_info_t *OpenAPI_redundant_transmission_exp_info_copy(OpenAPI_redundant_transmission_exp_info_t *dst, OpenAPI_redundant_transmission_exp_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redundant_transmission_exp_info_H_ */

