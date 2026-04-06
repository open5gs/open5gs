
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_header_handling_control_info_1.h"

OpenAPI_af_header_handling_control_info_1_t *OpenAPI_af_header_handling_control_info_1_create(
    char *h_detection_reference,
    bool is_h_detection_supp_info_null,
    char *h_detection_supp_info,
    bool is_notif_uri_null,
    char *notif_uri,
    bool is_notif_id_null,
    char *notif_id,
    bool is_sp_val_null,
    OpenAPI_spatial_validity_rm_1_t *sp_val,
    bool is_temp_vals_null,
    OpenAPI_list_t *temp_vals,
    bool is_h_hndlg_ul_null,
    OpenAPI_list_t *h_hndlg_ul,
    bool is_h_hndlg_dl_null,
    OpenAPI_list_t *h_hndlg_dl
)
{
    OpenAPI_af_header_handling_control_info_1_t *af_header_handling_control_info_1_local_var = ogs_malloc(sizeof(OpenAPI_af_header_handling_control_info_1_t));
    ogs_assert(af_header_handling_control_info_1_local_var);

    af_header_handling_control_info_1_local_var->h_detection_reference = h_detection_reference;
    af_header_handling_control_info_1_local_var->is_h_detection_supp_info_null = is_h_detection_supp_info_null;
    af_header_handling_control_info_1_local_var->h_detection_supp_info = h_detection_supp_info;
    af_header_handling_control_info_1_local_var->is_notif_uri_null = is_notif_uri_null;
    af_header_handling_control_info_1_local_var->notif_uri = notif_uri;
    af_header_handling_control_info_1_local_var->is_notif_id_null = is_notif_id_null;
    af_header_handling_control_info_1_local_var->notif_id = notif_id;
    af_header_handling_control_info_1_local_var->is_sp_val_null = is_sp_val_null;
    af_header_handling_control_info_1_local_var->sp_val = sp_val;
    af_header_handling_control_info_1_local_var->is_temp_vals_null = is_temp_vals_null;
    af_header_handling_control_info_1_local_var->temp_vals = temp_vals;
    af_header_handling_control_info_1_local_var->is_h_hndlg_ul_null = is_h_hndlg_ul_null;
    af_header_handling_control_info_1_local_var->h_hndlg_ul = h_hndlg_ul;
    af_header_handling_control_info_1_local_var->is_h_hndlg_dl_null = is_h_hndlg_dl_null;
    af_header_handling_control_info_1_local_var->h_hndlg_dl = h_hndlg_dl;

    return af_header_handling_control_info_1_local_var;
}

void OpenAPI_af_header_handling_control_info_1_free(OpenAPI_af_header_handling_control_info_1_t *af_header_handling_control_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_header_handling_control_info_1) {
        return;
    }
    if (af_header_handling_control_info_1->h_detection_reference) {
        ogs_free(af_header_handling_control_info_1->h_detection_reference);
        af_header_handling_control_info_1->h_detection_reference = NULL;
    }
    if (af_header_handling_control_info_1->h_detection_supp_info) {
        ogs_free(af_header_handling_control_info_1->h_detection_supp_info);
        af_header_handling_control_info_1->h_detection_supp_info = NULL;
    }
    if (af_header_handling_control_info_1->notif_uri) {
        ogs_free(af_header_handling_control_info_1->notif_uri);
        af_header_handling_control_info_1->notif_uri = NULL;
    }
    if (af_header_handling_control_info_1->notif_id) {
        ogs_free(af_header_handling_control_info_1->notif_id);
        af_header_handling_control_info_1->notif_id = NULL;
    }
    if (af_header_handling_control_info_1->sp_val) {
        OpenAPI_spatial_validity_rm_1_free(af_header_handling_control_info_1->sp_val);
        af_header_handling_control_info_1->sp_val = NULL;
    }
    if (af_header_handling_control_info_1->temp_vals) {
        OpenAPI_list_for_each(af_header_handling_control_info_1->temp_vals, node) {
            OpenAPI_temporal_validity_1_free(node->data);
        }
        OpenAPI_list_free(af_header_handling_control_info_1->temp_vals);
        af_header_handling_control_info_1->temp_vals = NULL;
    }
    if (af_header_handling_control_info_1->h_hndlg_ul) {
        OpenAPI_list_for_each(af_header_handling_control_info_1->h_hndlg_ul, node) {
            OpenAPI_header_handling_action_request_1_free(node->data);
        }
        OpenAPI_list_free(af_header_handling_control_info_1->h_hndlg_ul);
        af_header_handling_control_info_1->h_hndlg_ul = NULL;
    }
    if (af_header_handling_control_info_1->h_hndlg_dl) {
        OpenAPI_list_for_each(af_header_handling_control_info_1->h_hndlg_dl, node) {
            OpenAPI_header_handling_action_request_1_free(node->data);
        }
        OpenAPI_list_free(af_header_handling_control_info_1->h_hndlg_dl);
        af_header_handling_control_info_1->h_hndlg_dl = NULL;
    }
    ogs_free(af_header_handling_control_info_1);
}

cJSON *OpenAPI_af_header_handling_control_info_1_convertToJSON(OpenAPI_af_header_handling_control_info_1_t *af_header_handling_control_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_header_handling_control_info_1 == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [AfHeaderHandlingControlInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!af_header_handling_control_info_1->h_detection_reference) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_detection_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "hDetectionReference", af_header_handling_control_info_1->h_detection_reference) == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_detection_reference]");
        goto end;
    }

    if (af_header_handling_control_info_1->h_detection_supp_info) {
    if (cJSON_AddStringToObject(item, "hDetectionSuppInfo", af_header_handling_control_info_1->h_detection_supp_info) == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_detection_supp_info]");
        goto end;
    }
    } else if (af_header_handling_control_info_1->is_h_detection_supp_info_null) {
        if (cJSON_AddNullToObject(item, "hDetectionSuppInfo") == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_detection_supp_info]");
            goto end;
        }
    }

    if (af_header_handling_control_info_1->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", af_header_handling_control_info_1->notif_uri) == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [notif_uri]");
        goto end;
    }
    } else if (af_header_handling_control_info_1->is_notif_uri_null) {
        if (cJSON_AddNullToObject(item, "notifUri") == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [notif_uri]");
            goto end;
        }
    }

    if (af_header_handling_control_info_1->notif_id) {
    if (cJSON_AddStringToObject(item, "notifId", af_header_handling_control_info_1->notif_id) == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [notif_id]");
        goto end;
    }
    } else if (af_header_handling_control_info_1->is_notif_id_null) {
        if (cJSON_AddNullToObject(item, "notifId") == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [notif_id]");
            goto end;
        }
    }

    if (af_header_handling_control_info_1->sp_val) {
    cJSON *sp_val_local_JSON = OpenAPI_spatial_validity_rm_1_convertToJSON(af_header_handling_control_info_1->sp_val);
    if (sp_val_local_JSON == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [sp_val]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spVal", sp_val_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [sp_val]");
        goto end;
    }
    } else if (af_header_handling_control_info_1->is_sp_val_null) {
        if (cJSON_AddNullToObject(item, "spVal") == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [sp_val]");
            goto end;
        }
    }

    if (af_header_handling_control_info_1->temp_vals) {
    cJSON *temp_valsList = cJSON_AddArrayToObject(item, "tempVals");
    if (temp_valsList == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [temp_vals]");
        goto end;
    }
    OpenAPI_list_for_each(af_header_handling_control_info_1->temp_vals, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [temp_vals]");
            goto end;
        }
        cJSON_AddItemToArray(temp_valsList, itemLocal);
    }
    } else if (af_header_handling_control_info_1->is_temp_vals_null) {
        if (cJSON_AddNullToObject(item, "tempVals") == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [temp_vals]");
            goto end;
        }
    }

    if (af_header_handling_control_info_1->h_hndlg_ul) {
    cJSON *h_hndlg_ulList = cJSON_AddArrayToObject(item, "hHndlgUl");
    if (h_hndlg_ulList == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_hndlg_ul]");
        goto end;
    }
    OpenAPI_list_for_each(af_header_handling_control_info_1->h_hndlg_ul, node) {
        cJSON *itemLocal = OpenAPI_header_handling_action_request_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_hndlg_ul]");
            goto end;
        }
        cJSON_AddItemToArray(h_hndlg_ulList, itemLocal);
    }
    } else if (af_header_handling_control_info_1->is_h_hndlg_ul_null) {
        if (cJSON_AddNullToObject(item, "hHndlgUl") == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_hndlg_ul]");
            goto end;
        }
    }

    if (af_header_handling_control_info_1->h_hndlg_dl) {
    cJSON *h_hndlg_dlList = cJSON_AddArrayToObject(item, "hHndlgDl");
    if (h_hndlg_dlList == NULL) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_hndlg_dl]");
        goto end;
    }
    OpenAPI_list_for_each(af_header_handling_control_info_1->h_hndlg_dl, node) {
        cJSON *itemLocal = OpenAPI_header_handling_action_request_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_hndlg_dl]");
            goto end;
        }
        cJSON_AddItemToArray(h_hndlg_dlList, itemLocal);
    }
    } else if (af_header_handling_control_info_1->is_h_hndlg_dl_null) {
        if (cJSON_AddNullToObject(item, "hHndlgDl") == NULL) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed [h_hndlg_dl]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_af_header_handling_control_info_1_t *OpenAPI_af_header_handling_control_info_1_parseFromJSON(cJSON *af_header_handling_control_info_1JSON)
{
    OpenAPI_af_header_handling_control_info_1_t *af_header_handling_control_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_detection_reference = NULL;
    cJSON *h_detection_supp_info = NULL;
    cJSON *notif_uri = NULL;
    cJSON *notif_id = NULL;
    cJSON *sp_val = NULL;
    OpenAPI_spatial_validity_rm_1_t *sp_val_local_nonprim = NULL;
    cJSON *temp_vals = NULL;
    OpenAPI_list_t *temp_valsList = NULL;
    cJSON *h_hndlg_ul = NULL;
    OpenAPI_list_t *h_hndlg_ulList = NULL;
    cJSON *h_hndlg_dl = NULL;
    OpenAPI_list_t *h_hndlg_dlList = NULL;
    h_detection_reference = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "hDetectionReference");
    if (!h_detection_reference) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [h_detection_reference]");
        goto end;
    }
    if (!cJSON_IsString(h_detection_reference)) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [h_detection_reference]");
        goto end;
    }

    h_detection_supp_info = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "hDetectionSuppInfo");
    if (h_detection_supp_info) {
    if (!cJSON_IsNull(h_detection_supp_info)) {
    if (!cJSON_IsString(h_detection_supp_info) && !cJSON_IsNull(h_detection_supp_info)) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [h_detection_supp_info]");
        goto end;
    }
    }
    }

    notif_uri = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsNull(notif_uri)) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }
    }

    notif_id = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "notifId");
    if (notif_id) {
    if (!cJSON_IsNull(notif_id)) {
    if (!cJSON_IsString(notif_id) && !cJSON_IsNull(notif_id)) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [notif_id]");
        goto end;
    }
    }
    }

    sp_val = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "spVal");
    if (sp_val) {
    if (!cJSON_IsNull(sp_val)) {
    sp_val_local_nonprim = OpenAPI_spatial_validity_rm_1_parseFromJSON(sp_val);
    if (!sp_val_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_rm_1_parseFromJSON failed [sp_val]");
        goto end;
    }
    }
    }

    temp_vals = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "tempVals");
    if (temp_vals) {
    if (!cJSON_IsNull(temp_vals)) {
        cJSON *temp_vals_local = NULL;
        if (!cJSON_IsArray(temp_vals)) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [temp_vals]");
            goto end;
        }

        temp_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_vals_local, temp_vals) {
            if (!cJSON_IsObject(temp_vals_local)) {
                ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [temp_vals]");
                goto end;
            }
            OpenAPI_temporal_validity_1_t *temp_valsItem = OpenAPI_temporal_validity_1_parseFromJSON(temp_vals_local);
            if (!temp_valsItem) {
                ogs_error("No temp_valsItem");
                goto end;
            }
            OpenAPI_list_add(temp_valsList, temp_valsItem);
        }
    }
    }

    h_hndlg_ul = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "hHndlgUl");
    if (h_hndlg_ul) {
    if (!cJSON_IsNull(h_hndlg_ul)) {
        cJSON *h_hndlg_ul_local = NULL;
        if (!cJSON_IsArray(h_hndlg_ul)) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [h_hndlg_ul]");
            goto end;
        }

        h_hndlg_ulList = OpenAPI_list_create();

        cJSON_ArrayForEach(h_hndlg_ul_local, h_hndlg_ul) {
            if (!cJSON_IsObject(h_hndlg_ul_local)) {
                ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [h_hndlg_ul]");
                goto end;
            }
            OpenAPI_header_handling_action_request_1_t *h_hndlg_ulItem = OpenAPI_header_handling_action_request_1_parseFromJSON(h_hndlg_ul_local);
            if (!h_hndlg_ulItem) {
                ogs_error("No h_hndlg_ulItem");
                goto end;
            }
            OpenAPI_list_add(h_hndlg_ulList, h_hndlg_ulItem);
        }
    }
    }

    h_hndlg_dl = cJSON_GetObjectItemCaseSensitive(af_header_handling_control_info_1JSON, "hHndlgDl");
    if (h_hndlg_dl) {
    if (!cJSON_IsNull(h_hndlg_dl)) {
        cJSON *h_hndlg_dl_local = NULL;
        if (!cJSON_IsArray(h_hndlg_dl)) {
            ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [h_hndlg_dl]");
            goto end;
        }

        h_hndlg_dlList = OpenAPI_list_create();

        cJSON_ArrayForEach(h_hndlg_dl_local, h_hndlg_dl) {
            if (!cJSON_IsObject(h_hndlg_dl_local)) {
                ogs_error("OpenAPI_af_header_handling_control_info_1_parseFromJSON() failed [h_hndlg_dl]");
                goto end;
            }
            OpenAPI_header_handling_action_request_1_t *h_hndlg_dlItem = OpenAPI_header_handling_action_request_1_parseFromJSON(h_hndlg_dl_local);
            if (!h_hndlg_dlItem) {
                ogs_error("No h_hndlg_dlItem");
                goto end;
            }
            OpenAPI_list_add(h_hndlg_dlList, h_hndlg_dlItem);
        }
    }
    }

    af_header_handling_control_info_1_local_var = OpenAPI_af_header_handling_control_info_1_create (
        ogs_strdup(h_detection_reference->valuestring),
        h_detection_supp_info && cJSON_IsNull(h_detection_supp_info) ? true : false,
        h_detection_supp_info && !cJSON_IsNull(h_detection_supp_info) ? ogs_strdup(h_detection_supp_info->valuestring) : NULL,
        notif_uri && cJSON_IsNull(notif_uri) ? true : false,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        notif_id && cJSON_IsNull(notif_id) ? true : false,
        notif_id && !cJSON_IsNull(notif_id) ? ogs_strdup(notif_id->valuestring) : NULL,
        sp_val && cJSON_IsNull(sp_val) ? true : false,
        sp_val ? sp_val_local_nonprim : NULL,
        temp_vals && cJSON_IsNull(temp_vals) ? true : false,
        temp_vals ? temp_valsList : NULL,
        h_hndlg_ul && cJSON_IsNull(h_hndlg_ul) ? true : false,
        h_hndlg_ul ? h_hndlg_ulList : NULL,
        h_hndlg_dl && cJSON_IsNull(h_hndlg_dl) ? true : false,
        h_hndlg_dl ? h_hndlg_dlList : NULL
    );

    return af_header_handling_control_info_1_local_var;
end:
    if (sp_val_local_nonprim) {
        OpenAPI_spatial_validity_rm_1_free(sp_val_local_nonprim);
        sp_val_local_nonprim = NULL;
    }
    if (temp_valsList) {
        OpenAPI_list_for_each(temp_valsList, node) {
            OpenAPI_temporal_validity_1_free(node->data);
        }
        OpenAPI_list_free(temp_valsList);
        temp_valsList = NULL;
    }
    if (h_hndlg_ulList) {
        OpenAPI_list_for_each(h_hndlg_ulList, node) {
            OpenAPI_header_handling_action_request_1_free(node->data);
        }
        OpenAPI_list_free(h_hndlg_ulList);
        h_hndlg_ulList = NULL;
    }
    if (h_hndlg_dlList) {
        OpenAPI_list_for_each(h_hndlg_dlList, node) {
            OpenAPI_header_handling_action_request_1_free(node->data);
        }
        OpenAPI_list_free(h_hndlg_dlList);
        h_hndlg_dlList = NULL;
    }
    return NULL;
}

OpenAPI_af_header_handling_control_info_1_t *OpenAPI_af_header_handling_control_info_1_copy(OpenAPI_af_header_handling_control_info_1_t *dst, OpenAPI_af_header_handling_control_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_header_handling_control_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_header_handling_control_info_1_convertToJSON() failed");
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

    OpenAPI_af_header_handling_control_info_1_free(dst);
    dst = OpenAPI_af_header_handling_control_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

