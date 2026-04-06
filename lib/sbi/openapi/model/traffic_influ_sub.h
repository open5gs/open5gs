/*
 * traffic_influ_sub.h
 *
 * Represents traffic influence subscription data.
 */

#ifndef _OpenAPI_traffic_influ_sub_H_
#define _OpenAPI_traffic_influ_sub_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traffic_influ_sub_s OpenAPI_traffic_influ_sub_t;
#include "plmn_id.h"
#include "snssai.h"
#include "traffic_influ_data_notif.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traffic_influ_sub_s {
    OpenAPI_list_t *dnns;
    OpenAPI_list_t *snssais;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t *internal_group_ids_add;
    OpenAPI_list_t *subscriber_cat_list;
    OpenAPI_list_t *supis;
    struct OpenAPI_plmn_id_s *plmn_id;
    char *ipv4_addr;
    char *ipv6_addr;
    char *notification_uri;
    char *notif_id;
    char *expiry;
    char *supported_features;
    OpenAPI_list_t *reset_ids;
    bool is_imm_rep;
    int imm_rep;
    OpenAPI_list_t *imm_reports;
};

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_create(
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *snssais,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t *internal_group_ids_add,
    OpenAPI_list_t *subscriber_cat_list,
    OpenAPI_list_t *supis,
    OpenAPI_plmn_id_t *plmn_id,
    char *ipv4_addr,
    char *ipv6_addr,
    char *notification_uri,
    char *notif_id,
    char *expiry,
    char *supported_features,
    OpenAPI_list_t *reset_ids,
    bool is_imm_rep,
    int imm_rep,
    OpenAPI_list_t *imm_reports
);
void OpenAPI_traffic_influ_sub_free(OpenAPI_traffic_influ_sub_t *traffic_influ_sub);
OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_parseFromJSON(cJSON *traffic_influ_subJSON);
cJSON *OpenAPI_traffic_influ_sub_convertToJSON(OpenAPI_traffic_influ_sub_t *traffic_influ_sub);
OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_copy(OpenAPI_traffic_influ_sub_t *dst, OpenAPI_traffic_influ_sub_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influ_sub_H_ */

