/*
 * data_filter.h
 *
 * Identifies a data filter.
 */

#ifndef _OpenAPI_data_filter_H_
#define _OpenAPI_data_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_data_filter_s OpenAPI_data_filter_t;
#include "data_ind.h"
#include "dnn_snssai_information.h"
#include "network_description_1.h"
#include "plmn_id.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_data_filter_s {
    OpenAPI_data_ind_e data_ind;
    OpenAPI_list_t *dnns;
    OpenAPI_list_t *snssais;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *app_ids;
    OpenAPI_list_t *ue_ipv4s;
    OpenAPI_list_t *ue_ipv6s;
    OpenAPI_list_t *ue_macs;
    bool is_any_ue_ind;
    int any_ue_ind;
    OpenAPI_list_t *dnn_snssai_infos;
    OpenAPI_list_t *dnais;
    OpenAPI_list_t *roam_ue_plmn_ids;
    OpenAPI_list_t *roam_ue_net_descs;
};

OpenAPI_data_filter_t *OpenAPI_data_filter_create(
    OpenAPI_data_ind_e data_ind,
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *snssais,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *ue_ipv4s,
    OpenAPI_list_t *ue_ipv6s,
    OpenAPI_list_t *ue_macs,
    bool is_any_ue_ind,
    int any_ue_ind,
    OpenAPI_list_t *dnn_snssai_infos,
    OpenAPI_list_t *dnais,
    OpenAPI_list_t *roam_ue_plmn_ids,
    OpenAPI_list_t *roam_ue_net_descs
);
void OpenAPI_data_filter_free(OpenAPI_data_filter_t *data_filter);
OpenAPI_data_filter_t *OpenAPI_data_filter_parseFromJSON(cJSON *data_filterJSON);
cJSON *OpenAPI_data_filter_convertToJSON(OpenAPI_data_filter_t *data_filter);
OpenAPI_data_filter_t *OpenAPI_data_filter_copy(OpenAPI_data_filter_t *dst, OpenAPI_data_filter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_filter_H_ */

