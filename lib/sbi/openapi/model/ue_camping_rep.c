
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_camping_rep.h"

OpenAPI_ue_camping_rep_t *OpenAPI_ue_camping_rep_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_serving_nf_identity_t *serv_nf_id,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_user_location_t *user_location_info,
    char *ue_time_zone,
    OpenAPI_net_loc_access_support_e net_loc_acc_supp
)
{
    OpenAPI_ue_camping_rep_t *ue_camping_rep_local_var = ogs_malloc(sizeof(OpenAPI_ue_camping_rep_t));
    ogs_assert(ue_camping_rep_local_var);

    ue_camping_rep_local_var->access_type = access_type;
    ue_camping_rep_local_var->rat_type = rat_type;
    ue_camping_rep_local_var->serv_nf_id = serv_nf_id;
    ue_camping_rep_local_var->serving_network = serving_network;
    ue_camping_rep_local_var->user_location_info = user_location_info;
    ue_camping_rep_local_var->ue_time_zone = ue_time_zone;
    ue_camping_rep_local_var->net_loc_acc_supp = net_loc_acc_supp;

    return ue_camping_rep_local_var;
}

void OpenAPI_ue_camping_rep_free(OpenAPI_ue_camping_rep_t *ue_camping_rep)
{
    if (NULL == ue_camping_rep) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_serving_nf_identity_free(ue_camping_rep->serv_nf_id);
    OpenAPI_plmn_id_nid_free(ue_camping_rep->serving_network);
    OpenAPI_user_location_free(ue_camping_rep->user_location_info);
    ogs_free(ue_camping_rep->ue_time_zone);
    ogs_free(ue_camping_rep);
}

cJSON *OpenAPI_ue_camping_rep_convertToJSON(OpenAPI_ue_camping_rep_t *ue_camping_rep)
{
    cJSON *item = NULL;

    if (ue_camping_rep == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [UeCampingRep]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_camping_rep->access_type) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(ue_camping_rep->access_type)) == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (ue_camping_rep->rat_type) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(ue_camping_rep->rat_type)) == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (ue_camping_rep->serv_nf_id) {
    cJSON *serv_nf_id_local_JSON = OpenAPI_serving_nf_identity_convertToJSON(ue_camping_rep->serv_nf_id);
    if (serv_nf_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [serv_nf_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servNfId", serv_nf_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [serv_nf_id]");
        goto end;
    }
    }

    if (ue_camping_rep->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(ue_camping_rep->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

    if (ue_camping_rep->user_location_info) {
    cJSON *user_location_info_local_JSON = OpenAPI_user_location_convertToJSON(ue_camping_rep->user_location_info);
    if (user_location_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [user_location_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInfo", user_location_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [user_location_info]");
        goto end;
    }
    }

    if (ue_camping_rep->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", ue_camping_rep->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (ue_camping_rep->net_loc_acc_supp) {
    if (cJSON_AddStringToObject(item, "netLocAccSupp", OpenAPI_net_loc_access_support_ToString(ue_camping_rep->net_loc_acc_supp)) == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [net_loc_acc_supp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_camping_rep_t *OpenAPI_ue_camping_rep_parseFromJSON(cJSON *ue_camping_repJSON)
{
    OpenAPI_ue_camping_rep_t *ue_camping_rep_local_var = NULL;
    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "accessType");

    OpenAPI_access_type_e access_typeVariable;
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *serv_nf_id = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "servNfId");

    OpenAPI_serving_nf_identity_t *serv_nf_id_local_nonprim = NULL;
    if (serv_nf_id) {
    serv_nf_id_local_nonprim = OpenAPI_serving_nf_identity_parseFromJSON(serv_nf_id);
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "servingNetwork");

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    }

    cJSON *user_location_info = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "userLocationInfo");

    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    if (user_location_info) {
    user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "ueTimeZone");

    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    cJSON *net_loc_acc_supp = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "netLocAccSupp");

    OpenAPI_net_loc_access_support_e net_loc_acc_suppVariable;
    if (net_loc_acc_supp) {
    if (!cJSON_IsString(net_loc_acc_supp)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [net_loc_acc_supp]");
        goto end;
    }
    net_loc_acc_suppVariable = OpenAPI_net_loc_access_support_FromString(net_loc_acc_supp->valuestring);
    }

    ue_camping_rep_local_var = OpenAPI_ue_camping_rep_create (
        access_type ? access_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        serv_nf_id ? serv_nf_id_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        net_loc_acc_supp ? net_loc_acc_suppVariable : 0
    );

    return ue_camping_rep_local_var;
end:
    return NULL;
}

OpenAPI_ue_camping_rep_t *OpenAPI_ue_camping_rep_copy(OpenAPI_ue_camping_rep_t *dst, OpenAPI_ue_camping_rep_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_camping_rep_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_ue_camping_rep_free(dst);
    dst = OpenAPI_ue_camping_rep_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

