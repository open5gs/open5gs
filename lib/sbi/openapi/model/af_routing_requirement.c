
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
    OpenAPI_up_path_chg_event_t *up_path_chg_sub,
    bool is_addr_preser_ind,
    int addr_preser_ind
)
{
    OpenAPI_af_routing_requirement_t *af_routing_requirement_local_var = ogs_malloc(sizeof(OpenAPI_af_routing_requirement_t));
    ogs_assert(af_routing_requirement_local_var);

    af_routing_requirement_local_var->is_app_reloc = is_app_reloc;
    af_routing_requirement_local_var->app_reloc = app_reloc;
    af_routing_requirement_local_var->route_to_locs = route_to_locs;
    af_routing_requirement_local_var->sp_val = sp_val;
    af_routing_requirement_local_var->temp_vals = temp_vals;
    af_routing_requirement_local_var->up_path_chg_sub = up_path_chg_sub;
    af_routing_requirement_local_var->is_addr_preser_ind = is_addr_preser_ind;
    af_routing_requirement_local_var->addr_preser_ind = addr_preser_ind;

    return af_routing_requirement_local_var;
}

void OpenAPI_af_routing_requirement_free(OpenAPI_af_routing_requirement_t *af_routing_requirement)
{
    if (NULL == af_routing_requirement) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(af_routing_requirement->route_to_locs, node) {
        OpenAPI_route_to_location_free(node->data);
    }
    OpenAPI_list_free(af_routing_requirement->route_to_locs);
    OpenAPI_spatial_validity_free(af_routing_requirement->sp_val);
    OpenAPI_list_for_each(af_routing_requirement->temp_vals, node) {
        OpenAPI_temporal_validity_free(node->data);
    }
    OpenAPI_list_free(af_routing_requirement->temp_vals);
    OpenAPI_up_path_chg_event_free(af_routing_requirement->up_path_chg_sub);
    ogs_free(af_routing_requirement);
}

cJSON *OpenAPI_af_routing_requirement_convertToJSON(OpenAPI_af_routing_requirement_t *af_routing_requirement)
{
    cJSON *item = NULL;

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

    OpenAPI_lnode_t *route_to_locs_node;
    if (af_routing_requirement->route_to_locs) {
        OpenAPI_list_for_each(af_routing_requirement->route_to_locs, route_to_locs_node) {
            cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(route_to_locs_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [route_to_locs]");
                goto end;
            }
            cJSON_AddItemToArray(route_to_locsList, itemLocal);
        }
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

    OpenAPI_lnode_t *temp_vals_node;
    if (af_routing_requirement->temp_vals) {
        OpenAPI_list_for_each(af_routing_requirement->temp_vals, temp_vals_node) {
            cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(temp_vals_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [temp_vals]");
                goto end;
            }
            cJSON_AddItemToArray(temp_valsList, itemLocal);
        }
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
    }

    if (af_routing_requirement->is_addr_preser_ind) {
    if (cJSON_AddBoolToObject(item, "addrPreserInd", af_routing_requirement->addr_preser_ind) == NULL) {
        ogs_error("OpenAPI_af_routing_requirement_convertToJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_af_routing_requirement_t *OpenAPI_af_routing_requirement_parseFromJSON(cJSON *af_routing_requirementJSON)
{
    OpenAPI_af_routing_requirement_t *af_routing_requirement_local_var = NULL;
    cJSON *app_reloc = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "appReloc");

    if (app_reloc) {
    if (!cJSON_IsBool(app_reloc)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [app_reloc]");
        goto end;
    }
    }

    cJSON *route_to_locs = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "routeToLocs");

    OpenAPI_list_t *route_to_locsList;
    if (route_to_locs) {
    cJSON *route_to_locs_local_nonprimitive;
    if (!cJSON_IsArray(route_to_locs)){
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [route_to_locs]");
        goto end;
    }

    route_to_locsList = OpenAPI_list_create();

    cJSON_ArrayForEach(route_to_locs_local_nonprimitive, route_to_locs ) {
        if (!cJSON_IsObject(route_to_locs_local_nonprimitive)) {
            ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [route_to_locs]");
            goto end;
        }
        OpenAPI_route_to_location_t *route_to_locsItem = OpenAPI_route_to_location_parseFromJSON(route_to_locs_local_nonprimitive);

        if (!route_to_locsItem) {
            ogs_error("No route_to_locsItem");
            OpenAPI_list_free(route_to_locsList);
            goto end;
        }

        OpenAPI_list_add(route_to_locsList, route_to_locsItem);
    }
    }

    cJSON *sp_val = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "spVal");

    OpenAPI_spatial_validity_t *sp_val_local_nonprim = NULL;
    if (sp_val) {
    sp_val_local_nonprim = OpenAPI_spatial_validity_parseFromJSON(sp_val);
    }

    cJSON *temp_vals = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "tempVals");

    OpenAPI_list_t *temp_valsList;
    if (temp_vals) {
    cJSON *temp_vals_local_nonprimitive;
    if (!cJSON_IsArray(temp_vals)){
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [temp_vals]");
        goto end;
    }

    temp_valsList = OpenAPI_list_create();

    cJSON_ArrayForEach(temp_vals_local_nonprimitive, temp_vals ) {
        if (!cJSON_IsObject(temp_vals_local_nonprimitive)) {
            ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [temp_vals]");
            goto end;
        }
        OpenAPI_temporal_validity_t *temp_valsItem = OpenAPI_temporal_validity_parseFromJSON(temp_vals_local_nonprimitive);

        if (!temp_valsItem) {
            ogs_error("No temp_valsItem");
            OpenAPI_list_free(temp_valsList);
            goto end;
        }

        OpenAPI_list_add(temp_valsList, temp_valsItem);
    }
    }

    cJSON *up_path_chg_sub = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "upPathChgSub");

    OpenAPI_up_path_chg_event_t *up_path_chg_sub_local_nonprim = NULL;
    if (up_path_chg_sub) {
    up_path_chg_sub_local_nonprim = OpenAPI_up_path_chg_event_parseFromJSON(up_path_chg_sub);
    }

    cJSON *addr_preser_ind = cJSON_GetObjectItemCaseSensitive(af_routing_requirementJSON, "addrPreserInd");

    if (addr_preser_ind) {
    if (!cJSON_IsBool(addr_preser_ind)) {
        ogs_error("OpenAPI_af_routing_requirement_parseFromJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    af_routing_requirement_local_var = OpenAPI_af_routing_requirement_create (
        app_reloc ? true : false,
        app_reloc ? app_reloc->valueint : 0,
        route_to_locs ? route_to_locsList : NULL,
        sp_val ? sp_val_local_nonprim : NULL,
        temp_vals ? temp_valsList : NULL,
        up_path_chg_sub ? up_path_chg_sub_local_nonprim : NULL,
        addr_preser_ind ? true : false,
        addr_preser_ind ? addr_preser_ind->valueint : 0
    );

    return af_routing_requirement_local_var;
end:
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

