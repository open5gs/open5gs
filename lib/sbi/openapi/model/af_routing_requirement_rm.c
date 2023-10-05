
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_routing_requirement_rm.h"

OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_create(
    bool is_app_reloc,
    int app_reloc,
    bool is_route_to_locs_null,
    OpenAPI_list_t *route_to_locs,
    bool is_sp_val_null,
    OpenAPI_spatial_validity_rm_t *sp_val,
    bool is_temp_vals_null,
    OpenAPI_list_t *temp_vals,
    bool is_up_path_chg_sub_null,
    OpenAPI_up_path_chg_event_t *up_path_chg_sub,
    bool is_addr_preser_ind_null,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_sim_conn_ind_null,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term_null,
    bool is_sim_conn_term,
    int sim_conn_term,
    bool is_eas_ip_replace_infos_null,
    OpenAPI_list_t *eas_ip_replace_infos,
    bool is_eas_redis_ind,
    int eas_redis_ind,
    bool is_max_allowed_up_lat_null,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat
)
{
    OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm_local_var = ogs_malloc(sizeof(OpenAPI_af_routing_requirement_rm_t));
    ogs_assert(af_routing_requirement_rm_local_var);

    af_routing_requirement_rm_local_var->is_app_reloc = is_app_reloc;
    af_routing_requirement_rm_local_var->app_reloc = app_reloc;
    af_routing_requirement_rm_local_var->is_route_to_locs_null = is_route_to_locs_null;
    af_routing_requirement_rm_local_var->route_to_locs = route_to_locs;
    af_routing_requirement_rm_local_var->is_sp_val_null = is_sp_val_null;
    af_routing_requirement_rm_local_var->sp_val = sp_val;
    af_routing_requirement_rm_local_var->is_temp_vals_null = is_temp_vals_null;
    af_routing_requirement_rm_local_var->temp_vals = temp_vals;
    af_routing_requirement_rm_local_var->is_up_path_chg_sub_null = is_up_path_chg_sub_null;
    af_routing_requirement_rm_local_var->up_path_chg_sub = up_path_chg_sub;
    af_routing_requirement_rm_local_var->is_addr_preser_ind_null = is_addr_preser_ind_null;
    af_routing_requirement_rm_local_var->is_addr_preser_ind = is_addr_preser_ind;
    af_routing_requirement_rm_local_var->addr_preser_ind = addr_preser_ind;
    af_routing_requirement_rm_local_var->is_sim_conn_ind_null = is_sim_conn_ind_null;
    af_routing_requirement_rm_local_var->is_sim_conn_ind = is_sim_conn_ind;
    af_routing_requirement_rm_local_var->sim_conn_ind = sim_conn_ind;
    af_routing_requirement_rm_local_var->is_sim_conn_term_null = is_sim_conn_term_null;
    af_routing_requirement_rm_local_var->is_sim_conn_term = is_sim_conn_term;
    af_routing_requirement_rm_local_var->sim_conn_term = sim_conn_term;
    af_routing_requirement_rm_local_var->is_eas_ip_replace_infos_null = is_eas_ip_replace_infos_null;
    af_routing_requirement_rm_local_var->eas_ip_replace_infos = eas_ip_replace_infos;
    af_routing_requirement_rm_local_var->is_eas_redis_ind = is_eas_redis_ind;
    af_routing_requirement_rm_local_var->eas_redis_ind = eas_redis_ind;
    af_routing_requirement_rm_local_var->is_max_allowed_up_lat_null = is_max_allowed_up_lat_null;
    af_routing_requirement_rm_local_var->is_max_allowed_up_lat = is_max_allowed_up_lat;
    af_routing_requirement_rm_local_var->max_allowed_up_lat = max_allowed_up_lat;

    return af_routing_requirement_rm_local_var;
}

void OpenAPI_af_routing_requirement_rm_free(OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_routing_requirement_rm) {
        return;
    }
    if (af_routing_requirement_rm->route_to_locs) {
        OpenAPI_list_for_each(af_routing_requirement_rm->route_to_locs, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(af_routing_requirement_rm->route_to_locs);
        af_routing_requirement_rm->route_to_locs = NULL;
    }
    if (af_routing_requirement_rm->sp_val) {
        OpenAPI_spatial_validity_rm_free(af_routing_requirement_rm->sp_val);
        af_routing_requirement_rm->sp_val = NULL;
    }
    if (af_routing_requirement_rm->temp_vals) {
        OpenAPI_list_for_each(af_routing_requirement_rm->temp_vals, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(af_routing_requirement_rm->temp_vals);
        af_routing_requirement_rm->temp_vals = NULL;
    }
    if (af_routing_requirement_rm->up_path_chg_sub) {
        OpenAPI_up_path_chg_event_free(af_routing_requirement_rm->up_path_chg_sub);
        af_routing_requirement_rm->up_path_chg_sub = NULL;
    }
    if (af_routing_requirement_rm->eas_ip_replace_infos) {
        OpenAPI_list_for_each(af_routing_requirement_rm->eas_ip_replace_infos, node) {
            OpenAPI_eas_ip_replacement_info_free(node->data);
        }
        OpenAPI_list_free(af_routing_requirement_rm->eas_ip_replace_infos);
        af_routing_requirement_rm->eas_ip_replace_infos = NULL;
    }
    ogs_free(af_routing_requirement_rm);
}

cJSON *OpenAPI_af_routing_requirement_rm_convertToJSON(OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_routing_requirement_rm == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [AfRoutingRequirementRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_routing_requirement_rm->is_app_reloc) {
    if (cJSON_AddBoolToObject(item, "appReloc", af_routing_requirement_rm->app_reloc) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [app_reloc]");
        goto end;
    }
    }

    if (af_routing_requirement_rm->route_to_locs) {
    cJSON *route_to_locsList = cJSON_AddArrayToObject(item, "routeToLocs");
    if (route_to_locsList == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [route_to_locs]");
        goto end;
    }
    OpenAPI_list_for_each(af_routing_requirement_rm->route_to_locs, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [route_to_locs]");
            goto end;
        }
        cJSON_AddItemToArray(route_to_locsList, itemLocal);
    }
    } else if (af_routing_requirement_rm->is_route_to_locs_null) {
        if (cJSON_AddNullToObject(item, "routeToLocs") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [route_to_locs]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->sp_val) {
    cJSON *sp_val_local_JSON = OpenAPI_spatial_validity_rm_convertToJSON(af_routing_requirement_rm->sp_val);
    if (sp_val_local_JSON == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [sp_val]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spVal", sp_val_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [sp_val]");
        goto end;
    }
    } else if (af_routing_requirement_rm->is_sp_val_null) {
        if (cJSON_AddNullToObject(item, "spVal") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [sp_val]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->temp_vals) {
    cJSON *temp_valsList = cJSON_AddArrayToObject(item, "tempVals");
    if (temp_valsList == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [temp_vals]");
        goto end;
    }
    OpenAPI_list_for_each(af_routing_requirement_rm->temp_vals, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [temp_vals]");
            goto end;
        }
        cJSON_AddItemToArray(temp_valsList, itemLocal);
    }
    } else if (af_routing_requirement_rm->is_temp_vals_null) {
        if (cJSON_AddNullToObject(item, "tempVals") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [temp_vals]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->up_path_chg_sub) {
    cJSON *up_path_chg_sub_local_JSON = OpenAPI_up_path_chg_event_convertToJSON(af_routing_requirement_rm->up_path_chg_sub);
    if (up_path_chg_sub_local_JSON == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [up_path_chg_sub]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upPathChgSub", up_path_chg_sub_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [up_path_chg_sub]");
        goto end;
    }
    } else if (af_routing_requirement_rm->is_up_path_chg_sub_null) {
        if (cJSON_AddNullToObject(item, "upPathChgSub") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [up_path_chg_sub]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->is_addr_preser_ind) {
    if (cJSON_AddBoolToObject(item, "addrPreserInd", af_routing_requirement_rm->addr_preser_ind) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [addr_preser_ind]");
        goto end;
    }
    } else if (af_routing_requirement_rm->is_addr_preser_ind_null) {
        if (cJSON_AddNullToObject(item, "addrPreserInd") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [addr_preser_ind]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->is_sim_conn_ind) {
    if (cJSON_AddBoolToObject(item, "simConnInd", af_routing_requirement_rm->sim_conn_ind) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [sim_conn_ind]");
        goto end;
    }
    } else if (af_routing_requirement_rm->is_sim_conn_ind_null) {
        if (cJSON_AddNullToObject(item, "simConnInd") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [sim_conn_ind]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->is_sim_conn_term) {
    if (cJSON_AddNumberToObject(item, "simConnTerm", af_routing_requirement_rm->sim_conn_term) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [sim_conn_term]");
        goto end;
    }
    } else if (af_routing_requirement_rm->is_sim_conn_term_null) {
        if (cJSON_AddNullToObject(item, "simConnTerm") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [sim_conn_term]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->eas_ip_replace_infos) {
    cJSON *eas_ip_replace_infosList = cJSON_AddArrayToObject(item, "easIpReplaceInfos");
    if (eas_ip_replace_infosList == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [eas_ip_replace_infos]");
        goto end;
    }
    OpenAPI_list_for_each(af_routing_requirement_rm->eas_ip_replace_infos, node) {
        cJSON *itemLocal = OpenAPI_eas_ip_replacement_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [eas_ip_replace_infos]");
            goto end;
        }
        cJSON_AddItemToArray(eas_ip_replace_infosList, itemLocal);
    }
    } else if (af_routing_requirement_rm->is_eas_ip_replace_infos_null) {
        if (cJSON_AddNullToObject(item, "easIpReplaceInfos") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [eas_ip_replace_infos]");
            goto end;
        }
    }

    if (af_routing_requirement_rm->is_eas_redis_ind) {
    if (cJSON_AddBoolToObject(item, "easRedisInd", af_routing_requirement_rm->eas_redis_ind) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [eas_redis_ind]");
        goto end;
    }
    }

    if (af_routing_requirement_rm->is_max_allowed_up_lat) {
    if (cJSON_AddNumberToObject(item, "maxAllowedUpLat", af_routing_requirement_rm->max_allowed_up_lat) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    } else if (af_routing_requirement_rm->is_max_allowed_up_lat_null) {
        if (cJSON_AddNullToObject(item, "maxAllowedUpLat") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed [max_allowed_up_lat]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_parseFromJSON(cJSON *af_routing_requirement_rmJSON)
{
    OpenAPI_af_routing_requirement_rm_t *af_routing_requirement_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_reloc = NULL;
    cJSON *route_to_locs = NULL;
    OpenAPI_list_t *route_to_locsList = NULL;
    cJSON *sp_val = NULL;
    OpenAPI_spatial_validity_rm_t *sp_val_local_nonprim = NULL;
    cJSON *temp_vals = NULL;
    OpenAPI_list_t *temp_valsList = NULL;
    cJSON *up_path_chg_sub = NULL;
    OpenAPI_up_path_chg_event_t *up_path_chg_sub_local_nonprim = NULL;
    cJSON *addr_preser_ind = NULL;
    cJSON *sim_conn_ind = NULL;
    cJSON *sim_conn_term = NULL;
    cJSON *eas_ip_replace_infos = NULL;
    OpenAPI_list_t *eas_ip_replace_infosList = NULL;
    cJSON *eas_redis_ind = NULL;
    cJSON *max_allowed_up_lat = NULL;
    app_reloc = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "appReloc");
    if (app_reloc) {
    if (!cJSON_IsBool(app_reloc)) {
        ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [app_reloc]");
        goto end;
    }
    }

    route_to_locs = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "routeToLocs");
    if (route_to_locs) {
    if (!cJSON_IsNull(route_to_locs)) {
        cJSON *route_to_locs_local = NULL;
        if (!cJSON_IsArray(route_to_locs)) {
            ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [route_to_locs]");
            goto end;
        }

        route_to_locsList = OpenAPI_list_create();

        cJSON_ArrayForEach(route_to_locs_local, route_to_locs) {
            if (!cJSON_IsObject(route_to_locs_local)) {
                ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [route_to_locs]");
                goto end;
            }
            OpenAPI_route_to_location_t *route_to_locsItem = OpenAPI_route_to_location_parseFromJSON(route_to_locs_local);
            if (!route_to_locsItem) {
                ogs_error("No route_to_locsItem");
                goto end;
            }
            OpenAPI_list_add(route_to_locsList, route_to_locsItem);
        }
    }
    }

    sp_val = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "spVal");
    if (sp_val) {
    if (!cJSON_IsNull(sp_val)) {
    sp_val_local_nonprim = OpenAPI_spatial_validity_rm_parseFromJSON(sp_val);
    if (!sp_val_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_rm_parseFromJSON failed [sp_val]");
        goto end;
    }
    }
    }

    temp_vals = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "tempVals");
    if (temp_vals) {
    if (!cJSON_IsNull(temp_vals)) {
        cJSON *temp_vals_local = NULL;
        if (!cJSON_IsArray(temp_vals)) {
            ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [temp_vals]");
            goto end;
        }

        temp_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_vals_local, temp_vals) {
            if (!cJSON_IsObject(temp_vals_local)) {
                ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [temp_vals]");
                goto end;
            }
            OpenAPI_temporal_validity_t *temp_valsItem = OpenAPI_temporal_validity_parseFromJSON(temp_vals_local);
            if (!temp_valsItem) {
                ogs_error("No temp_valsItem");
                goto end;
            }
            OpenAPI_list_add(temp_valsList, temp_valsItem);
        }
    }
    }

    up_path_chg_sub = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "upPathChgSub");
    if (up_path_chg_sub) {
    if (!cJSON_IsNull(up_path_chg_sub)) {
    up_path_chg_sub_local_nonprim = OpenAPI_up_path_chg_event_parseFromJSON(up_path_chg_sub);
    if (!up_path_chg_sub_local_nonprim) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON failed [up_path_chg_sub]");
        goto end;
    }
    }
    }

    addr_preser_ind = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "addrPreserInd");
    if (addr_preser_ind) {
    if (!cJSON_IsNull(addr_preser_ind)) {
    if (!cJSON_IsBool(addr_preser_ind)) {
        ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [addr_preser_ind]");
        goto end;
    }
    }
    }

    sim_conn_ind = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "simConnInd");
    if (sim_conn_ind) {
    if (!cJSON_IsNull(sim_conn_ind)) {
    if (!cJSON_IsBool(sim_conn_ind)) {
        ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [sim_conn_ind]");
        goto end;
    }
    }
    }

    sim_conn_term = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "simConnTerm");
    if (sim_conn_term) {
    if (!cJSON_IsNull(sim_conn_term)) {
    if (!cJSON_IsNumber(sim_conn_term)) {
        ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [sim_conn_term]");
        goto end;
    }
    }
    }

    eas_ip_replace_infos = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "easIpReplaceInfos");
    if (eas_ip_replace_infos) {
    if (!cJSON_IsNull(eas_ip_replace_infos)) {
        cJSON *eas_ip_replace_infos_local = NULL;
        if (!cJSON_IsArray(eas_ip_replace_infos)) {
            ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [eas_ip_replace_infos]");
            goto end;
        }

        eas_ip_replace_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(eas_ip_replace_infos_local, eas_ip_replace_infos) {
            if (!cJSON_IsObject(eas_ip_replace_infos_local)) {
                ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [eas_ip_replace_infos]");
                goto end;
            }
            OpenAPI_eas_ip_replacement_info_t *eas_ip_replace_infosItem = OpenAPI_eas_ip_replacement_info_parseFromJSON(eas_ip_replace_infos_local);
            if (!eas_ip_replace_infosItem) {
                ogs_error("No eas_ip_replace_infosItem");
                goto end;
            }
            OpenAPI_list_add(eas_ip_replace_infosList, eas_ip_replace_infosItem);
        }
    }
    }

    eas_redis_ind = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "easRedisInd");
    if (eas_redis_ind) {
    if (!cJSON_IsBool(eas_redis_ind)) {
        ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [eas_redis_ind]");
        goto end;
    }
    }

    max_allowed_up_lat = cJSON_GetObjectItemCaseSensitive(af_routing_requirement_rmJSON, "maxAllowedUpLat");
    if (max_allowed_up_lat) {
    if (!cJSON_IsNull(max_allowed_up_lat)) {
    if (!cJSON_IsNumber(max_allowed_up_lat)) {
        ogs_error("OpenAPI_af_routing_requirement_rm_parseFromJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }
    }

    af_routing_requirement_rm_local_var = OpenAPI_af_routing_requirement_rm_create (
        app_reloc ? true : false,
        app_reloc ? app_reloc->valueint : 0,
        route_to_locs && cJSON_IsNull(route_to_locs) ? true : false,
        route_to_locs ? route_to_locsList : NULL,
        sp_val && cJSON_IsNull(sp_val) ? true : false,
        sp_val ? sp_val_local_nonprim : NULL,
        temp_vals && cJSON_IsNull(temp_vals) ? true : false,
        temp_vals ? temp_valsList : NULL,
        up_path_chg_sub && cJSON_IsNull(up_path_chg_sub) ? true : false,
        up_path_chg_sub ? up_path_chg_sub_local_nonprim : NULL,
        addr_preser_ind && cJSON_IsNull(addr_preser_ind) ? true : false,
        addr_preser_ind ? true : false,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        sim_conn_ind && cJSON_IsNull(sim_conn_ind) ? true : false,
        sim_conn_ind ? true : false,
        sim_conn_ind ? sim_conn_ind->valueint : 0,
        sim_conn_term && cJSON_IsNull(sim_conn_term) ? true : false,
        sim_conn_term ? true : false,
        sim_conn_term ? sim_conn_term->valuedouble : 0,
        eas_ip_replace_infos && cJSON_IsNull(eas_ip_replace_infos) ? true : false,
        eas_ip_replace_infos ? eas_ip_replace_infosList : NULL,
        eas_redis_ind ? true : false,
        eas_redis_ind ? eas_redis_ind->valueint : 0,
        max_allowed_up_lat && cJSON_IsNull(max_allowed_up_lat) ? true : false,
        max_allowed_up_lat ? true : false,
        max_allowed_up_lat ? max_allowed_up_lat->valuedouble : 0
    );

    return af_routing_requirement_rm_local_var;
end:
    if (route_to_locsList) {
        OpenAPI_list_for_each(route_to_locsList, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(route_to_locsList);
        route_to_locsList = NULL;
    }
    if (sp_val_local_nonprim) {
        OpenAPI_spatial_validity_rm_free(sp_val_local_nonprim);
        sp_val_local_nonprim = NULL;
    }
    if (temp_valsList) {
        OpenAPI_list_for_each(temp_valsList, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(temp_valsList);
        temp_valsList = NULL;
    }
    if (up_path_chg_sub_local_nonprim) {
        OpenAPI_up_path_chg_event_free(up_path_chg_sub_local_nonprim);
        up_path_chg_sub_local_nonprim = NULL;
    }
    if (eas_ip_replace_infosList) {
        OpenAPI_list_for_each(eas_ip_replace_infosList, node) {
            OpenAPI_eas_ip_replacement_info_free(node->data);
        }
        OpenAPI_list_free(eas_ip_replace_infosList);
        eas_ip_replace_infosList = NULL;
    }
    return NULL;
}

OpenAPI_af_routing_requirement_rm_t *OpenAPI_af_routing_requirement_rm_copy(OpenAPI_af_routing_requirement_rm_t *dst, OpenAPI_af_routing_requirement_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_routing_requirement_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_routing_requirement_rm_convertToJSON() failed");
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

    OpenAPI_af_routing_requirement_rm_free(dst);
    dst = OpenAPI_af_routing_requirement_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

