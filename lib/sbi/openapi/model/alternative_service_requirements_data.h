/*
 * alternative_service_requirements_data.h
 *
 * Contains an alternative QoS related parameter set.
 */

#ifndef _OpenAPI_alternative_service_requirements_data_H_
#define _OpenAPI_alternative_service_requirements_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_alternative_service_requirements_data_s OpenAPI_alternative_service_requirements_data_t;
#include "pdu_set_qos_para_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_alternative_service_requirements_data_s {
    char *alt_qos_param_set_ref;
    char *gbr_ul;
    char *gbr_dl;
    bool is_pdb;
    int pdb;
    char *per;
    bool is_aver_window;
    int aver_window;
    bool is_max_data_burst_vol_null;
    bool is_max_data_burst_vol;
    int max_data_burst_vol;
    bool is_pdu_set_qos_dl_null;
    struct OpenAPI_pdu_set_qos_para_rm_s *pdu_set_qos_dl;
    bool is_pdu_set_qos_ul_null;
    struct OpenAPI_pdu_set_qos_para_rm_s *pdu_set_qos_ul;
};

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_create(
    char *alt_qos_param_set_ref,
    char *gbr_ul,
    char *gbr_dl,
    bool is_pdb,
    int pdb,
    char *per,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol_null,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_pdu_set_qos_dl_null,
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_dl,
    bool is_pdu_set_qos_ul_null,
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_ul
);
void OpenAPI_alternative_service_requirements_data_free(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data);
OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_parseFromJSON(cJSON *alternative_service_requirements_dataJSON);
cJSON *OpenAPI_alternative_service_requirements_data_convertToJSON(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data);
OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_copy(OpenAPI_alternative_service_requirements_data_t *dst, OpenAPI_alternative_service_requirements_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_alternative_service_requirements_data_H_ */

