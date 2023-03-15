
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_parameter_data_patch.h"

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_create(
    char *param_over_pc5,
    char *param_over_uu,
    char *param_for_pro_se_dd,
    char *param_for_pro_se_dc,
    char *param_for_pro_se_u2_n_rel_ue,
    char *param_for_pro_se_rem_ue,
    OpenAPI_list_t *ursp_influence,
    OpenAPI_list_t *delivery_events,
    char *polic_deliv_notif_uri
)
{
    OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_service_parameter_data_patch_t));
    ogs_assert(service_parameter_data_patch_local_var);

    service_parameter_data_patch_local_var->param_over_pc5 = param_over_pc5;
    service_parameter_data_patch_local_var->param_over_uu = param_over_uu;
    service_parameter_data_patch_local_var->param_for_pro_se_dd = param_for_pro_se_dd;
    service_parameter_data_patch_local_var->param_for_pro_se_dc = param_for_pro_se_dc;
    service_parameter_data_patch_local_var->param_for_pro_se_u2_n_rel_ue = param_for_pro_se_u2_n_rel_ue;
    service_parameter_data_patch_local_var->param_for_pro_se_rem_ue = param_for_pro_se_rem_ue;
    service_parameter_data_patch_local_var->ursp_influence = ursp_influence;
    service_parameter_data_patch_local_var->delivery_events = delivery_events;
    service_parameter_data_patch_local_var->polic_deliv_notif_uri = polic_deliv_notif_uri;

    return service_parameter_data_patch_local_var;
}

void OpenAPI_service_parameter_data_patch_free(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_parameter_data_patch) {
        return;
    }
    if (service_parameter_data_patch->param_over_pc5) {
        ogs_free(service_parameter_data_patch->param_over_pc5);
        service_parameter_data_patch->param_over_pc5 = NULL;
    }
    if (service_parameter_data_patch->param_over_uu) {
        ogs_free(service_parameter_data_patch->param_over_uu);
        service_parameter_data_patch->param_over_uu = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_dd) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_dd);
        service_parameter_data_patch->param_for_pro_se_dd = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_dc) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_dc);
        service_parameter_data_patch->param_for_pro_se_dc = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue);
        service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue = NULL;
    }
    if (service_parameter_data_patch->param_for_pro_se_rem_ue) {
        ogs_free(service_parameter_data_patch->param_for_pro_se_rem_ue);
        service_parameter_data_patch->param_for_pro_se_rem_ue = NULL;
    }
    if (service_parameter_data_patch->ursp_influence) {
        OpenAPI_list_for_each(service_parameter_data_patch->ursp_influence, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data_patch->ursp_influence);
        service_parameter_data_patch->ursp_influence = NULL;
    }
    if (service_parameter_data_patch->delivery_events) {
        OpenAPI_list_for_each(service_parameter_data_patch->delivery_events, node) {
            OpenAPI_event_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data_patch->delivery_events);
        service_parameter_data_patch->delivery_events = NULL;
    }
    if (service_parameter_data_patch->polic_deliv_notif_uri) {
        ogs_free(service_parameter_data_patch->polic_deliv_notif_uri);
        service_parameter_data_patch->polic_deliv_notif_uri = NULL;
    }
    ogs_free(service_parameter_data_patch);
}

cJSON *OpenAPI_service_parameter_data_patch_convertToJSON(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_parameter_data_patch == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ServiceParameterDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_parameter_data_patch->param_over_pc5) {
    if (cJSON_AddStringToObject(item, "paramOverPc5", service_parameter_data_patch->param_over_pc5) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_pc5]");
        goto end;
    }
    }

    if (service_parameter_data_patch->param_over_uu) {
    if (cJSON_AddStringToObject(item, "paramOverUu", service_parameter_data_patch->param_over_uu) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_uu]");
        goto end;
    }
    }

    if (service_parameter_data_patch->param_for_pro_se_dd) {
    if (cJSON_AddStringToObject(item, "paramForProSeDd", service_parameter_data_patch->param_for_pro_se_dd) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_dd]");
        goto end;
    }
    }

    if (service_parameter_data_patch->param_for_pro_se_dc) {
    if (cJSON_AddStringToObject(item, "paramForProSeDc", service_parameter_data_patch->param_for_pro_se_dc) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_dc]");
        goto end;
    }
    }

    if (service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeU2NRelUe", service_parameter_data_patch->param_for_pro_se_u2_n_rel_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_u2_n_rel_ue]");
        goto end;
    }
    }

    if (service_parameter_data_patch->param_for_pro_se_rem_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeRemUe", service_parameter_data_patch->param_for_pro_se_rem_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_for_pro_se_rem_ue]");
        goto end;
    }
    }

    if (service_parameter_data_patch->ursp_influence) {
    cJSON *ursp_influenceList = cJSON_AddArrayToObject(item, "urspInfluence");
    if (ursp_influenceList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ursp_influence]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->ursp_influence, node) {
        cJSON *itemLocal = OpenAPI_ursp_rule_request_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ursp_influence]");
            goto end;
        }
        cJSON_AddItemToArray(ursp_influenceList, itemLocal);
    }
    }

    if (service_parameter_data_patch->delivery_events) {
    cJSON *delivery_eventsList = cJSON_AddArrayToObject(item, "deliveryEvents");
    if (delivery_eventsList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [delivery_events]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data_patch->delivery_events, node) {
        cJSON *itemLocal = OpenAPI_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [delivery_events]");
            goto end;
        }
        cJSON_AddItemToArray(delivery_eventsList, itemLocal);
    }
    }

    if (service_parameter_data_patch->polic_deliv_notif_uri) {
    if (cJSON_AddStringToObject(item, "policDelivNotifUri", service_parameter_data_patch->polic_deliv_notif_uri) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [polic_deliv_notif_uri]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_parseFromJSON(cJSON *service_parameter_data_patchJSON)
{
    OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *param_over_pc5 = NULL;
    cJSON *param_over_uu = NULL;
    cJSON *param_for_pro_se_dd = NULL;
    cJSON *param_for_pro_se_dc = NULL;
    cJSON *param_for_pro_se_u2_n_rel_ue = NULL;
    cJSON *param_for_pro_se_rem_ue = NULL;
    cJSON *ursp_influence = NULL;
    OpenAPI_list_t *ursp_influenceList = NULL;
    cJSON *delivery_events = NULL;
    OpenAPI_list_t *delivery_eventsList = NULL;
    cJSON *polic_deliv_notif_uri = NULL;
    param_over_pc5 = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramOverPc5");
    if (param_over_pc5) {
    if (!cJSON_IsString(param_over_pc5) && !cJSON_IsNull(param_over_pc5)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_over_pc5]");
        goto end;
    }
    }

    param_over_uu = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramOverUu");
    if (param_over_uu) {
    if (!cJSON_IsString(param_over_uu) && !cJSON_IsNull(param_over_uu)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_over_uu]");
        goto end;
    }
    }

    param_for_pro_se_dd = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeDd");
    if (param_for_pro_se_dd) {
    if (!cJSON_IsString(param_for_pro_se_dd) && !cJSON_IsNull(param_for_pro_se_dd)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_dd]");
        goto end;
    }
    }

    param_for_pro_se_dc = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeDc");
    if (param_for_pro_se_dc) {
    if (!cJSON_IsString(param_for_pro_se_dc) && !cJSON_IsNull(param_for_pro_se_dc)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_dc]");
        goto end;
    }
    }

    param_for_pro_se_u2_n_rel_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeU2NRelUe");
    if (param_for_pro_se_u2_n_rel_ue) {
    if (!cJSON_IsString(param_for_pro_se_u2_n_rel_ue) && !cJSON_IsNull(param_for_pro_se_u2_n_rel_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_u2_n_rel_ue]");
        goto end;
    }
    }

    param_for_pro_se_rem_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramForProSeRemUe");
    if (param_for_pro_se_rem_ue) {
    if (!cJSON_IsString(param_for_pro_se_rem_ue) && !cJSON_IsNull(param_for_pro_se_rem_ue)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_for_pro_se_rem_ue]");
        goto end;
    }
    }

    ursp_influence = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "urspInfluence");
    if (ursp_influence) {
        cJSON *ursp_influence_local = NULL;
        if (!cJSON_IsArray(ursp_influence)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [ursp_influence]");
            goto end;
        }

        ursp_influenceList = OpenAPI_list_create();

        cJSON_ArrayForEach(ursp_influence_local, ursp_influence) {
            if (!cJSON_IsObject(ursp_influence_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [ursp_influence]");
                goto end;
            }
            OpenAPI_ursp_rule_request_t *ursp_influenceItem = OpenAPI_ursp_rule_request_parseFromJSON(ursp_influence_local);
            if (!ursp_influenceItem) {
                ogs_error("No ursp_influenceItem");
                goto end;
            }
            OpenAPI_list_add(ursp_influenceList, ursp_influenceItem);
        }
    }

    delivery_events = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "deliveryEvents");
    if (delivery_events) {
        cJSON *delivery_events_local = NULL;
        if (!cJSON_IsArray(delivery_events)) {
            ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [delivery_events]");
            goto end;
        }

        delivery_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(delivery_events_local, delivery_events) {
            if (!cJSON_IsObject(delivery_events_local)) {
                ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [delivery_events]");
                goto end;
            }
            OpenAPI_event_t *delivery_eventsItem = OpenAPI_event_parseFromJSON(delivery_events_local);
            if (!delivery_eventsItem) {
                ogs_error("No delivery_eventsItem");
                goto end;
            }
            OpenAPI_list_add(delivery_eventsList, delivery_eventsItem);
        }
    }

    polic_deliv_notif_uri = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "policDelivNotifUri");
    if (polic_deliv_notif_uri) {
    if (!cJSON_IsString(polic_deliv_notif_uri) && !cJSON_IsNull(polic_deliv_notif_uri)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [polic_deliv_notif_uri]");
        goto end;
    }
    }

    service_parameter_data_patch_local_var = OpenAPI_service_parameter_data_patch_create (
        param_over_pc5 && !cJSON_IsNull(param_over_pc5) ? ogs_strdup(param_over_pc5->valuestring) : NULL,
        param_over_uu && !cJSON_IsNull(param_over_uu) ? ogs_strdup(param_over_uu->valuestring) : NULL,
        param_for_pro_se_dd && !cJSON_IsNull(param_for_pro_se_dd) ? ogs_strdup(param_for_pro_se_dd->valuestring) : NULL,
        param_for_pro_se_dc && !cJSON_IsNull(param_for_pro_se_dc) ? ogs_strdup(param_for_pro_se_dc->valuestring) : NULL,
        param_for_pro_se_u2_n_rel_ue && !cJSON_IsNull(param_for_pro_se_u2_n_rel_ue) ? ogs_strdup(param_for_pro_se_u2_n_rel_ue->valuestring) : NULL,
        param_for_pro_se_rem_ue && !cJSON_IsNull(param_for_pro_se_rem_ue) ? ogs_strdup(param_for_pro_se_rem_ue->valuestring) : NULL,
        ursp_influence ? ursp_influenceList : NULL,
        delivery_events ? delivery_eventsList : NULL,
        polic_deliv_notif_uri && !cJSON_IsNull(polic_deliv_notif_uri) ? ogs_strdup(polic_deliv_notif_uri->valuestring) : NULL
    );

    return service_parameter_data_patch_local_var;
end:
    if (ursp_influenceList) {
        OpenAPI_list_for_each(ursp_influenceList, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(ursp_influenceList);
        ursp_influenceList = NULL;
    }
    if (delivery_eventsList) {
        OpenAPI_list_for_each(delivery_eventsList, node) {
            OpenAPI_event_free(node->data);
        }
        OpenAPI_list_free(delivery_eventsList);
        delivery_eventsList = NULL;
    }
    return NULL;
}

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_copy(OpenAPI_service_parameter_data_patch_t *dst, OpenAPI_service_parameter_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_parameter_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed");
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

    OpenAPI_service_parameter_data_patch_free(dst);
    dst = OpenAPI_service_parameter_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

