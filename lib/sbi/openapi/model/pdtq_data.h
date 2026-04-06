/*
 * pdtq_data.h
 *
 * Contains the planned data transfer data with QoS requirements.
 */

#ifndef _OpenAPI_pdtq_data_H_
#define _OpenAPI_pdtq_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdtq_data_s OpenAPI_pdtq_data_t;
#include "alt_qos_param_set.h"
#include "network_area_info_2.h"
#include "pdtq_policy.h"
#include "qos_parameter_set.h"
#include "snssai.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdtq_data_s {
    char *asp_id;
    struct OpenAPI_pdtq_policy_s *pdtq_policy;
    char *app_id;
    char *pdtq_ref_id;
    struct OpenAPI_network_area_info_2_s *nw_area_info;
    bool is_num_of_ues;
    int num_of_ues;
    OpenAPI_list_t *des_time_ints;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *alt_qos_param_sets;
    OpenAPI_list_t *alt_qos_refs;
    struct OpenAPI_qos_parameter_set_s *qos_param_set;
    char *qos_reference;
    char *notif_uri;
    bool is_warn_notif_enabled;
    int warn_notif_enabled;
    char *supp_feat;
    OpenAPI_list_t *reset_ids;
};

OpenAPI_pdtq_data_t *OpenAPI_pdtq_data_create(
    char *asp_id,
    OpenAPI_pdtq_policy_t *pdtq_policy,
    char *app_id,
    char *pdtq_ref_id,
    OpenAPI_network_area_info_2_t *nw_area_info,
    bool is_num_of_ues,
    int num_of_ues,
    OpenAPI_list_t *des_time_ints,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *alt_qos_param_sets,
    OpenAPI_list_t *alt_qos_refs,
    OpenAPI_qos_parameter_set_t *qos_param_set,
    char *qos_reference,
    char *notif_uri,
    bool is_warn_notif_enabled,
    int warn_notif_enabled,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_pdtq_data_free(OpenAPI_pdtq_data_t *pdtq_data);
OpenAPI_pdtq_data_t *OpenAPI_pdtq_data_parseFromJSON(cJSON *pdtq_dataJSON);
cJSON *OpenAPI_pdtq_data_convertToJSON(OpenAPI_pdtq_data_t *pdtq_data);
OpenAPI_pdtq_data_t *OpenAPI_pdtq_data_copy(OpenAPI_pdtq_data_t *dst, OpenAPI_pdtq_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdtq_data_H_ */

