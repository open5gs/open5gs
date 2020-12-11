
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_notification_control_info.h"

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_qos_notif_type_t *notif_type,
    int cont_ver,
    char *alt_qos_param_id
    )
{
    OpenAPI_qos_notification_control_info_t *qos_notification_control_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_qos_notification_control_info_t));
    if (!qos_notification_control_info_local_var) {
        return NULL;
    }
    qos_notification_control_info_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;
    qos_notification_control_info_local_var->notif_type = notif_type;
    qos_notification_control_info_local_var->cont_ver = cont_ver;
    qos_notification_control_info_local_var->alt_qos_param_id = alt_qos_param_id;

    return qos_notification_control_info_local_var;
}

void OpenAPI_qos_notification_control_info_free(OpenAPI_qos_notification_control_info_t *qos_notification_control_info)
{
    if (NULL == qos_notification_control_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(qos_notification_control_info->ref_pcc_rule_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(qos_notification_control_info->ref_pcc_rule_ids);
    OpenAPI_qos_notif_type_free(qos_notification_control_info->notif_type);
    ogs_free(qos_notification_control_info->alt_qos_param_id);
    ogs_free(qos_notification_control_info);
}

cJSON *OpenAPI_qos_notification_control_info_convertToJSON(OpenAPI_qos_notification_control_info_t *qos_notification_control_info)
{
    cJSON *item = NULL;

    if (qos_notification_control_info == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [QosNotificationControlInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qos_notification_control_info->ref_pcc_rule_ids) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    cJSON *ref_pcc_rule_ids = cJSON_AddArrayToObject(item, "refPccRuleIds");
    if (ref_pcc_rule_ids == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }

    OpenAPI_lnode_t *ref_pcc_rule_ids_node;
    OpenAPI_list_for_each(qos_notification_control_info->ref_pcc_rule_ids, ref_pcc_rule_ids_node)  {
        if (cJSON_AddStringToObject(ref_pcc_rule_ids, "", (char*)ref_pcc_rule_ids_node->data) == NULL) {
            ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
    }

    if (!qos_notification_control_info->notif_type) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [notif_type]");
        goto end;
    }
    cJSON *notif_type_local_JSON = OpenAPI_qos_notif_type_convertToJSON(qos_notification_control_info->notif_type);
    if (notif_type_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [notif_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "notifType", notif_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [notif_type]");
        goto end;
    }

    if (qos_notification_control_info->cont_ver) {
        if (cJSON_AddNumberToObject(item, "contVer", qos_notification_control_info->cont_ver) == NULL) {
            ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [cont_ver]");
            goto end;
        }
    }

    if (qos_notification_control_info->alt_qos_param_id) {
        if (cJSON_AddStringToObject(item, "altQosParamId", qos_notification_control_info->alt_qos_param_id) == NULL) {
            ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [alt_qos_param_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_parseFromJSON(cJSON *qos_notification_control_infoJSON)
{
    OpenAPI_qos_notification_control_info_t *qos_notification_control_info_local_var = NULL;
    cJSON *ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(qos_notification_control_infoJSON, "refPccRuleIds");
    if (!ref_pcc_rule_ids) {
        ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }

    OpenAPI_list_t *ref_pcc_rule_idsList;

    cJSON *ref_pcc_rule_ids_local;
    if (!cJSON_IsArray(ref_pcc_rule_ids)) {
        ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    ref_pcc_rule_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
        if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
            ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
        OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
    }

    cJSON *notif_type = cJSON_GetObjectItemCaseSensitive(qos_notification_control_infoJSON, "notifType");
    if (!notif_type) {
        ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [notif_type]");
        goto end;
    }

    OpenAPI_qos_notif_type_t *notif_type_local_nonprim = NULL;

    notif_type_local_nonprim = OpenAPI_qos_notif_type_parseFromJSON(notif_type);

    cJSON *cont_ver = cJSON_GetObjectItemCaseSensitive(qos_notification_control_infoJSON, "contVer");

    if (cont_ver) {
        if (!cJSON_IsNumber(cont_ver)) {
            ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [cont_ver]");
            goto end;
        }
    }

    cJSON *alt_qos_param_id = cJSON_GetObjectItemCaseSensitive(qos_notification_control_infoJSON, "altQosParamId");

    if (alt_qos_param_id) {
        if (!cJSON_IsString(alt_qos_param_id)) {
            ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [alt_qos_param_id]");
            goto end;
        }
    }

    qos_notification_control_info_local_var = OpenAPI_qos_notification_control_info_create (
        ref_pcc_rule_idsList,
        notif_type_local_nonprim,
        cont_ver ? cont_ver->valuedouble : 0,
        alt_qos_param_id ? ogs_strdup(alt_qos_param_id->valuestring) : NULL
        );

    return qos_notification_control_info_local_var;
end:
    return NULL;
}

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_copy(OpenAPI_qos_notification_control_info_t *dst, OpenAPI_qos_notification_control_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_notification_control_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed");
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

    OpenAPI_qos_notification_control_info_free(dst);
    dst = OpenAPI_qos_notification_control_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

