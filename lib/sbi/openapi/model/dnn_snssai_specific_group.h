/*
 * dnn_snssai_specific_group.h
 *
 * Contains DNN and S-NSSAI specific Group
 */

#ifndef _OpenAPI_dnn_snssai_specific_group_H_
#define _OpenAPI_dnn_snssai_specific_group_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dnn_snssai_specific_group_s OpenAPI_dnn_snssai_specific_group_t;
#include "af_req_default_qo_s.h"
#include "snssai.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dnn_snssai_specific_group_s {
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    struct OpenAPI_af_req_default_qo_s_s *def_qos;
    OpenAPI_list_t *af_req_serv_area;
};

OpenAPI_dnn_snssai_specific_group_t *OpenAPI_dnn_snssai_specific_group_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_af_req_default_qo_s_t *def_qos,
    OpenAPI_list_t *af_req_serv_area
);
void OpenAPI_dnn_snssai_specific_group_free(OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group);
OpenAPI_dnn_snssai_specific_group_t *OpenAPI_dnn_snssai_specific_group_parseFromJSON(cJSON *dnn_snssai_specific_groupJSON);
cJSON *OpenAPI_dnn_snssai_specific_group_convertToJSON(OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group);
OpenAPI_dnn_snssai_specific_group_t *OpenAPI_dnn_snssai_specific_group_copy(OpenAPI_dnn_snssai_specific_group_t *dst, OpenAPI_dnn_snssai_specific_group_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_snssai_specific_group_H_ */

