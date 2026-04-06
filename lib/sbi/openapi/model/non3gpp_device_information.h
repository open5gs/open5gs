/*
 * non3gpp_device_information.h
 *
 * Represents the Non-3GPP device identifier(s) information.
 */

#ifndef _OpenAPI_non3gpp_device_information_H_
#define _OpenAPI_non3gpp_device_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_non3gpp_device_information_s OpenAPI_non3gpp_device_information_t;
#include "dnn_snssai_information.h"
#include "eth_flow_info.h"
#include "flow_info_1.h"
#include "qos_parameter_set.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_non3gpp_device_information_s {
    char *non3gpp_dev_id;
    struct OpenAPI_dnn_snssai_information_s *dnn_snssai_info;
    OpenAPI_list_t *flow_infos;
    OpenAPI_list_t *eth_flow_infos;
    char *qos_reference;
    struct OpenAPI_qos_parameter_set_s *ind_qos_param_set;
};

OpenAPI_non3gpp_device_information_t *OpenAPI_non3gpp_device_information_create(
    char *non3gpp_dev_id,
    OpenAPI_dnn_snssai_information_t *dnn_snssai_info,
    OpenAPI_list_t *flow_infos,
    OpenAPI_list_t *eth_flow_infos,
    char *qos_reference,
    OpenAPI_qos_parameter_set_t *ind_qos_param_set
);
void OpenAPI_non3gpp_device_information_free(OpenAPI_non3gpp_device_information_t *non3gpp_device_information);
OpenAPI_non3gpp_device_information_t *OpenAPI_non3gpp_device_information_parseFromJSON(cJSON *non3gpp_device_informationJSON);
cJSON *OpenAPI_non3gpp_device_information_convertToJSON(OpenAPI_non3gpp_device_information_t *non3gpp_device_information);
OpenAPI_non3gpp_device_information_t *OpenAPI_non3gpp_device_information_copy(OpenAPI_non3gpp_device_information_t *dst, OpenAPI_non3gpp_device_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non3gpp_device_information_H_ */

