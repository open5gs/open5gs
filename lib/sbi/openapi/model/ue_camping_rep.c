
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
    OpenAPI_net_loc_access_support_e net_loc_acc_supp,
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category
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
    ue_camping_rep_local_var->sat_backhaul_category = sat_backhaul_category;

    return ue_camping_rep_local_var;
}

void OpenAPI_ue_camping_rep_free(OpenAPI_ue_camping_rep_t *ue_camping_rep)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_camping_rep) {
        return;
    }
    if (ue_camping_rep->serv_nf_id) {
        OpenAPI_serving_nf_identity_free(ue_camping_rep->serv_nf_id);
        ue_camping_rep->serv_nf_id = NULL;
    }
    if (ue_camping_rep->serving_network) {
        OpenAPI_plmn_id_nid_free(ue_camping_rep->serving_network);
        ue_camping_rep->serving_network = NULL;
    }
    if (ue_camping_rep->user_location_info) {
        OpenAPI_user_location_free(ue_camping_rep->user_location_info);
        ue_camping_rep->user_location_info = NULL;
    }
    if (ue_camping_rep->ue_time_zone) {
        ogs_free(ue_camping_rep->ue_time_zone);
        ue_camping_rep->ue_time_zone = NULL;
    }
    ogs_free(ue_camping_rep);
}

cJSON *OpenAPI_ue_camping_rep_convertToJSON(OpenAPI_ue_camping_rep_t *ue_camping_rep)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_camping_rep == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [UeCampingRep]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_camping_rep->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(ue_camping_rep->access_type)) == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (ue_camping_rep->rat_type != OpenAPI_rat_type_NULL) {
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

    if (ue_camping_rep->net_loc_acc_supp != OpenAPI_net_loc_access_support_NULL) {
    if (cJSON_AddStringToObject(item, "netLocAccSupp", OpenAPI_net_loc_access_support_ToString(ue_camping_rep->net_loc_acc_supp)) == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [net_loc_acc_supp]");
        goto end;
    }
    }

    if (ue_camping_rep->sat_backhaul_category != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satBackhaulCategory", OpenAPI_satellite_backhaul_category_ToString(ue_camping_rep->sat_backhaul_category)) == NULL) {
        ogs_error("OpenAPI_ue_camping_rep_convertToJSON() failed [sat_backhaul_category]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_camping_rep_t *OpenAPI_ue_camping_rep_parseFromJSON(cJSON *ue_camping_repJSON)
{
    OpenAPI_ue_camping_rep_t *ue_camping_rep_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *serv_nf_id = NULL;
    OpenAPI_serving_nf_identity_t *serv_nf_id_local_nonprim = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    cJSON *user_location_info = NULL;
    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *net_loc_acc_supp = NULL;
    OpenAPI_net_loc_access_support_e net_loc_acc_suppVariable = 0;
    cJSON *sat_backhaul_category = NULL;
    OpenAPI_satellite_backhaul_category_e sat_backhaul_categoryVariable = 0;
    access_type = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    serv_nf_id = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "servNfId");
    if (serv_nf_id) {
    serv_nf_id_local_nonprim = OpenAPI_serving_nf_identity_parseFromJSON(serv_nf_id);
    if (!serv_nf_id_local_nonprim) {
        ogs_error("OpenAPI_serving_nf_identity_parseFromJSON failed [serv_nf_id]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    user_location_info = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "userLocationInfo");
    if (user_location_info) {
    user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    if (!user_location_info_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [user_location_info]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    net_loc_acc_supp = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "netLocAccSupp");
    if (net_loc_acc_supp) {
    if (!cJSON_IsString(net_loc_acc_supp)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [net_loc_acc_supp]");
        goto end;
    }
    net_loc_acc_suppVariable = OpenAPI_net_loc_access_support_FromString(net_loc_acc_supp->valuestring);
    }

    sat_backhaul_category = cJSON_GetObjectItemCaseSensitive(ue_camping_repJSON, "satBackhaulCategory");
    if (sat_backhaul_category) {
    if (!cJSON_IsString(sat_backhaul_category)) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON() failed [sat_backhaul_category]");
        goto end;
    }
    sat_backhaul_categoryVariable = OpenAPI_satellite_backhaul_category_FromString(sat_backhaul_category->valuestring);
    }

    ue_camping_rep_local_var = OpenAPI_ue_camping_rep_create (
        access_type ? access_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        serv_nf_id ? serv_nf_id_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        net_loc_acc_supp ? net_loc_acc_suppVariable : 0,
        sat_backhaul_category ? sat_backhaul_categoryVariable : 0
    );

    return ue_camping_rep_local_var;
end:
    if (serv_nf_id_local_nonprim) {
        OpenAPI_serving_nf_identity_free(serv_nf_id_local_nonprim);
        serv_nf_id_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (user_location_info_local_nonprim) {
        OpenAPI_user_location_free(user_location_info_local_nonprim);
        user_location_info_local_nonprim = NULL;
    }
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

