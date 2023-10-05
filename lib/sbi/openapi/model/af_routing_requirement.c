
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_routing_requirement.h"

OpenAPI_af_routing_requirement_t *OpenAPI_af_routing_requirement_create(
    bool is_app_reloc,
    int app_reloc,
    OpenAPI_list_t *route_to_locs,
    OpenAPI_spatial_validity_t *sp_val,
    OpenAPI_list_t *temp_vals,
    bool is_up_path_chg_sub_null,
    OpenAPI_up_path_chg_event_t *up_path_chg_sub,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term,
    int sim_conn_term,
    OpenAPI_list_t *eas_ip_replace_infos,
    bool is_eas_redis_ind,
    int eas_redis_ind,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat
)
{
    OpenAPI_af_routing_requirement_t *af_routing_requirement_local_var = ogs_malloc(sizeof(OpenAPI_af_routing_requirement_t));
    ogs_assert(af_routing_requirement_local_var);

    af_routing_requirement_local_var->is_app_reloc = is_app_reloc;
    af_routing_requirement_local_var->app_reloc = app_reloc;
    af_routing_requirement_local_var->route_to_locs = route_to_locs;
    af_routing_requirement_local_var->sp_val = sp_val;
    af_routing_requirement_local_var->temp_vals = temp_vals;
    af_routing_requirement_local_var->is_up_path_chg_sub_null = is_up_path_chg_sub_null;
    af_routing_requirement_local_var->up_path_chg_sub = up_path_chg_sub;
    af_routing_requirement_local_var->is_addr_preser_ind = is_addr_preser_ind;
    af_routing_requirement_local_var->addr_preser_ind = addr_preser_ind;
    af_routing_requirement_local_var->is_sim_conn_ind = is_sim_conn_ind;
    af_routing_requirement_local_var->sim_conn_ind = sim_conn_ind;
    af_routing_requirement_local_var->is_sim_conn_term = is_sim_conn_term;
    af_routing_requirement_local_var->sim_conn_term = sim_conn_term;
    af_routing_requirement_local_var->eas_ip_replace_infos = eas_ip_replace_infos;
    af_routing_requirement_local_var->is_eas_redis_ind = is_eas_redis_ind;
    af_routing_requirement_local_var->eas_redis_ind = eas_redis_ind;
    af_routing_requirement_local_var->is_max_allowed_up_lat = is_max_allowed_up_lat;
    af_routing_requirement_local_var->max_allowed_up_lat = max_allowed_up_lat;

    return af_routing_requirement_local_var;
}

void OpenAPI_af_routing_requirement_free(OpenAPI_af_routing_requirement_t *af_routing_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_routing_requirement) {
        return;
    }
    if (af_routing_requirement->route_to_locs) {
        OpenAPI_list_for_each(af_routing_requirement->route_to_locs, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(af_routing_requirement->route_to_locs);
        af_routing_requirement->route_to_locs = NULL;
    }
    if (af_routing_requirement->sp_val) {
        OpenAPI_spatial_validity_free(af_routing_requirement->sp_val);
        af_routing_requirement->sp_val = NULL;
    }
    if (af_routing_requirement->temp_vals) {
        OpenAPI_list_for_each(af_routing_requirement->temp_vals, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(af_routing_requirement->temp_vals);
        af_routing_requirement->temp_vals = NULL;
    }
    if (af_routing_requirement->up_path_chg_sub) {
        OpenAPI_up_path_chg_event_free(af_routing_requirement->up_path_chg_sub);
        af_routing_requirement->up_path_chg_sub = NULL;
    }
    if (af_routing_requirement->eas_ip_replace_infos) {
        OpenAPI_list_for_each(af_routing_requirement->eas_ip_replace_infos, node) {
            OpenAPI_eas_ip_replacement_info_free(node->data);
        }
        OpenAPI_list_free(af_routing_requirement->eas_ip_replace_infos);
        af_routing_requirement->eas_ip_replace_infos = NULL;
    }
    ogs_free(af_routing_requirement);
}

cJSON *OpenAPI_af_routing_requirement_convertToJSON(OpenAPI_af_routing_requirement_t *af_routing_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_routing_requirement == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [AfRoutingRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_routing_requirement->is_app_reloc) {
    if (cJSON_AddBoolToObject(item, "appReloc", af_routing_requirement->app_reloc) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [app_reloc]");
        goto end;
    }
    }

    if (af_routing_requirement->route_to_locs) {
    cJSON *route_to_locsList = cJSON_AddArrayToObject(item, "routeToLocs");
    if (route_to_locsList == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [route_to_locs]");
        goto end;
    }
    OpenAPI_list_for_each(af_routing_requirement->route_to_locs, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [route_to_locs]");
            goto end;
        }
        cJSON_AddItemToArray(route_to_locsList, itemLocal);
    }
    }

    if (af_routing_requirement->sp_val) {
    cJSON *sp_val_local_JSON = OpenAPI_spatial_validity_convertToJSON(af_routing_requirement->sp_val);
    if (sp_val_local_JSON == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [sp_val]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spVal", sp_val_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [sp_val]");
        goto end;
    }
    }

    if (af_routing_requirement->temp_vals) {
    cJSON *temp_valsList = cJSON_AddArrayToObject(item, "tempVals");
    if (temp_valsList == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [temp_vals]");
        goto end;
    }
    OpenAPI_list_for_each(af_routing_requirement->temp_vals, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [temp_vals]");
            goto end;
        }
        cJSON_AddItemToArray(temp_valsList, itemLocal);
    }
    }

    if (af_routing_requirement->up_path_chg_sub) {
    cJSON *up_path_chg_sub_local_JSON = OpenAPI_up_path_chg_event_convertToJSON(af_routing_requirement->up_path_chg_sub);
    if (up_path_chg_sub_local_JSON == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [up_path_chg_sub]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upPathChgSub", up_path_chg_sub_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [up_path_chg_sub]");
        goto end;
    }
    } else if (af_routing_requirement->is_up_path_chg_sub_null) {
        if (cJSON_AddNullToObject(item, "upPathChgSub") == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [up_path_chg_sub]");
            goto end;
        }
    }

    if (af_routing_requirement->is_addr_preser_ind) {
    if (cJSON_AddBoolToObject(item, "addrPreserInd", af_routing_requirement->addr_preser_ind) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    if (af_routing_requirement->is_sim_conn_ind) {
    if (cJSON_AddBoolToObject(item, "simConnInd", af_routing_requirement->sim_conn_ind) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    if (af_routing_requirement->is_sim_conn_term) {
    if (cJSON_AddNumberToObject(item, "simConnTerm", af_routing_requirement->sim_conn_term) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    if (af_routing_requirement->eas_ip_replace_infos) {
    cJSON *eas_ip_replace_infosList = cJSON_AddArrayToObject(item, "easIpReplaceInfos");
    if (eas_ip_replace_infosList == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [eas_ip_replace_infos]");
        goto end;
    }
    OpenAPI_list_for_each(af_routing_requirement->eas_ip_replace_infos, node) {
        cJSON *itemLocal = OpenAPI_eas_ip_replacement_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [eas_ip_replace_infos]");
            goto end;
        }
        cJSON_AddItemToArray(eas_ip_replace_infosList, itemLocal);
    }
    }

    if (af_routing_requirement->is_eas_redis_ind) {
    if (cJSON_AddBoolToObject(item, "easRedisInd", af_routing_requirement->eas_redis_ind) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [eas_redis_ind]");
        goto end;
    }
    }

    if (af_routing_requirement->is_max_allowed_up_lat) {
    if (cJSON_AddNumberToObject(item, "maxAllowedUpLat", af_routing_requirement->max_allowed_up_lat) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_af_routing_requirement_t *OpenAPI_af_routing_requirement_parseFromJSON(cJSON *af_routing_requirementJSON)
{
    OpenAPI_af_routing_requirement_t *af_routing_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_reloc = NULL;
    cJSON *route_to_locs = NULL;
    OpenAPI_list_t *route_to_locsList = NULL;
    cJSON *sp_val = NULL;
    OpenAPI_spatial_validity_t *sp_val_local_nonprim = NULL;
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
    app_reloc = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "appReloc");
    if (app_reloc) {
    if (!cJSON_IsBool(app_reloc)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [app_reloc]");
        goto end;
    }
    }

    route_to_locs = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "routeToLocs");
    if (route_to_locs) {
        cJSON *route_to_locs_local = NULL;
        if (!cJSON_IsArray(route_to_locs)) {
            ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [route_to_locs]");
            goto end;
        }

        route_to_locsList = OpenAPI_list_create();

        cJSON_ArrayForEach(route_to_locs_local, route_to_locs) {
            if (!cJSON_IsObject(route_to_locs_local)) {
                ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [route_to_locs]");
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

    sp_val = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "spVal");
    if (sp_val) {
    sp_val_local_nonprim = OpenAPI_spatial_validity_parseFromJSON(sp_val);
    if (!sp_val_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_parseFromJSON failed [sp_val]");
        goto end;
    }
    }

    temp_vals = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "tempVals");
    if (temp_vals) {
        cJSON *temp_vals_local = NULL;
        if (!cJSON_IsArray(temp_vals)) {
            ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [temp_vals]");
            goto end;
        }

        temp_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_vals_local, temp_vals) {
            if (!cJSON_IsObject(temp_vals_local)) {
                ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [temp_vals]");
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

    up_path_chg_sub = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "upPathChgSub");
    if (up_path_chg_sub) {
    if (!cJSON_IsNull(up_path_chg_sub)) {
    up_path_chg_sub_local_nonprim = OpenAPI_up_path_chg_event_parseFromJSON(up_path_chg_sub);
    if (!up_path_chg_sub_local_nonprim) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON failed [up_path_chg_sub]");
        goto end;
    }
    }
    }

    addr_preser_ind = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "addrPreserInd");
    if (addr_preser_ind) {
    if (!cJSON_IsBool(addr_preser_ind)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    sim_conn_ind = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "simConnInd");
    if (sim_conn_ind) {
    if (!cJSON_IsBool(sim_conn_ind)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    sim_conn_term = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "simConnTerm");
    if (sim_conn_term) {
    if (!cJSON_IsNumber(sim_conn_term)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    eas_ip_replace_infos = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "easIpReplaceInfos");
    if (eas_ip_replace_infos) {
        cJSON *eas_ip_replace_infos_local = NULL;
        if (!cJSON_IsArray(eas_ip_replace_infos)) {
            ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [eas_ip_replace_infos]");
            goto end;
        }

        eas_ip_replace_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(eas_ip_replace_infos_local, eas_ip_replace_infos) {
            if (!cJSON_IsObject(eas_ip_replace_infos_local)) {
                ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [eas_ip_replace_infos]");
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

    eas_redis_ind = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "easRedisInd");
    if (eas_redis_ind) {
    if (!cJSON_IsBool(eas_redis_ind)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [eas_redis_ind]");
        goto end;
    }
    }

    max_allowed_up_lat = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "maxAllowedUpLat");
    if (max_allowed_up_lat) {
    if (!cJSON_IsNumber(max_allowed_up_lat)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }

    af_routing_requirement_local_var = OpenAPI_af_routing_requirement_create (
        app_reloc ? true : false,
        app_reloc ? app_reloc->valueint : 0,
        route_to_locs ? route_to_locsList : NULL,
        sp_val ? sp_val_local_nonprim : NULL,
        temp_vals ? temp_valsList : NULL,
        up_path_chg_sub && cJSON_IsNull(up_path_chg_sub) ? true : false,
        up_path_chg_sub ? up_path_chg_sub_local_nonprim : NULL,
        addr_preser_ind ? true : false,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        sim_conn_ind ? true : false,
        sim_conn_ind ? sim_conn_ind->valueint : 0,
        sim_conn_term ? true : false,
        sim_conn_term ? sim_conn_term->valuedouble : 0,
        eas_ip_replace_infos ? eas_ip_replace_infosList : NULL,
        eas_redis_ind ? true : false,
        eas_redis_ind ? eas_redis_ind->valueint : 0,
        max_allowed_up_lat ? true : false,
        max_allowed_up_lat ? max_allowed_up_lat->valuedouble : 0
    );

    return af_routing_requirement_local_var;
end:
    if (route_to_locsList) {
        OpenAPI_list_for_each(route_to_locsList, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(route_to_locsList);
        route_to_locsList = NULL;
    }
    if (sp_val_local_nonprim) {
        OpenAPI_spatial_validity_free(sp_val_local_nonprim);
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

OpenAPI_af_routing_requirement_t *OpenAPI_af_routing_requirement_copy(OpenAPI_af_routing_requirement_t *dst, OpenAPI_af_routing_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_routing_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed");
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

    OpenAPI_af_routing_requirement_free(dst);
    dst = OpenAPI_af_routing_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

