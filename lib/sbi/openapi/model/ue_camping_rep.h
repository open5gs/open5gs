/*
 * ue_camping_rep.h
 *
 * 
 */

#ifndef _OpenAPI_ue_camping_rep_H_
#define _OpenAPI_ue_camping_rep_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "net_loc_access_support.h"
#include "plmn_id_nid.h"
#include "rat_type.h"
#include "serving_nf_identity.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_camping_rep_s OpenAPI_ue_camping_rep_t;
typedef struct OpenAPI_ue_camping_rep_s {
    OpenAPI_access_type_e access_type;
    OpenAPI_rat_type_e rat_type;
    struct OpenAPI_serving_nf_identity_s *serv_nf_id;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    struct OpenAPI_user_location_s *user_location_info;
    char *ue_time_zone;
    OpenAPI_net_loc_access_support_e net_loc_acc_supp;
} OpenAPI_ue_camping_rep_t;

OpenAPI_ue_camping_rep_t *OpenAPI_ue_camping_rep_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_serving_nf_identity_t *serv_nf_id,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_user_location_t *user_location_info,
    char *ue_time_zone,
    OpenAPI_net_loc_access_support_e net_loc_acc_supp
);
void OpenAPI_ue_camping_rep_free(OpenAPI_ue_camping_rep_t *ue_camping_rep);
OpenAPI_ue_camping_rep_t *OpenAPI_ue_camping_rep_parseFromJSON(cJSON *ue_camping_repJSON);
cJSON *OpenAPI_ue_camping_rep_convertToJSON(OpenAPI_ue_camping_rep_t *ue_camping_rep);
OpenAPI_ue_camping_rep_t *OpenAPI_ue_camping_rep_copy(OpenAPI_ue_camping_rep_t *dst, OpenAPI_ue_camping_rep_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_camping_rep_H_ */

