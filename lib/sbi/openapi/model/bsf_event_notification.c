
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsf_event_notification.h"

OpenAPI_bsf_event_notification_t *OpenAPI_bsf_event_notification_create(
    OpenAPI_bsf_event_e event,
    OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info,
    OpenAPI_list_t *pcf_for_pdu_sess_infos,
    OpenAPI_list_t *match_snssai_dnns
)
{
    OpenAPI_bsf_event_notification_t *bsf_event_notification_local_var = ogs_malloc(sizeof(OpenAPI_bsf_event_notification_t));
    ogs_assert(bsf_event_notification_local_var);

    bsf_event_notification_local_var->event = event;
    bsf_event_notification_local_var->pcf_for_ue_info = pcf_for_ue_info;
    bsf_event_notification_local_var->pcf_for_pdu_sess_infos = pcf_for_pdu_sess_infos;
    bsf_event_notification_local_var->match_snssai_dnns = match_snssai_dnns;

    return bsf_event_notification_local_var;
}

void OpenAPI_bsf_event_notification_free(OpenAPI_bsf_event_notification_t *bsf_event_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bsf_event_notification) {
        return;
    }
    if (bsf_event_notification->pcf_for_ue_info) {
        OpenAPI_pcf_for_ue_info_free(bsf_event_notification->pcf_for_ue_info);
        bsf_event_notification->pcf_for_ue_info = NULL;
    }
    if (bsf_event_notification->pcf_for_pdu_sess_infos) {
        OpenAPI_list_for_each(bsf_event_notification->pcf_for_pdu_sess_infos, node) {
            OpenAPI_pcf_for_pdu_session_info_free(node->data);
        }
        OpenAPI_list_free(bsf_event_notification->pcf_for_pdu_sess_infos);
        bsf_event_notification->pcf_for_pdu_sess_infos = NULL;
    }
    if (bsf_event_notification->match_snssai_dnns) {
        OpenAPI_list_for_each(bsf_event_notification->match_snssai_dnns, node) {
            OpenAPI_snssai_dnn_pair_free(node->data);
        }
        OpenAPI_list_free(bsf_event_notification->match_snssai_dnns);
        bsf_event_notification->match_snssai_dnns = NULL;
    }
    ogs_free(bsf_event_notification);
}

cJSON *OpenAPI_bsf_event_notification_convertToJSON(OpenAPI_bsf_event_notification_t *bsf_event_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bsf_event_notification == NULL) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [BsfEventNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bsf_event_notification->event == OpenAPI_bsf_event_NULL) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [event]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "event", OpenAPI_bsf_event_ToString(bsf_event_notification->event)) == NULL) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [event]");
        goto end;
    }

    if (bsf_event_notification->pcf_for_ue_info) {
    cJSON *pcf_for_ue_info_local_JSON = OpenAPI_pcf_for_ue_info_convertToJSON(bsf_event_notification->pcf_for_ue_info);
    if (pcf_for_ue_info_local_JSON == NULL) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [pcf_for_ue_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfForUeInfo", pcf_for_ue_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [pcf_for_ue_info]");
        goto end;
    }
    }

    if (bsf_event_notification->pcf_for_pdu_sess_infos) {
    cJSON *pcf_for_pdu_sess_infosList = cJSON_AddArrayToObject(item, "pcfForPduSessInfos");
    if (pcf_for_pdu_sess_infosList == NULL) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [pcf_for_pdu_sess_infos]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_event_notification->pcf_for_pdu_sess_infos, node) {
        cJSON *itemLocal = OpenAPI_pcf_for_pdu_session_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [pcf_for_pdu_sess_infos]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_for_pdu_sess_infosList, itemLocal);
    }
    }

    if (bsf_event_notification->match_snssai_dnns) {
    cJSON *match_snssai_dnnsList = cJSON_AddArrayToObject(item, "matchSnssaiDnns");
    if (match_snssai_dnnsList == NULL) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [match_snssai_dnns]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_event_notification->match_snssai_dnns, node) {
        cJSON *itemLocal = OpenAPI_snssai_dnn_pair_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed [match_snssai_dnns]");
            goto end;
        }
        cJSON_AddItemToArray(match_snssai_dnnsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_bsf_event_notification_t *OpenAPI_bsf_event_notification_parseFromJSON(cJSON *bsf_event_notificationJSON)
{
    OpenAPI_bsf_event_notification_t *bsf_event_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event = NULL;
    OpenAPI_bsf_event_e eventVariable = 0;
    cJSON *pcf_for_ue_info = NULL;
    OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info_local_nonprim = NULL;
    cJSON *pcf_for_pdu_sess_infos = NULL;
    OpenAPI_list_t *pcf_for_pdu_sess_infosList = NULL;
    cJSON *match_snssai_dnns = NULL;
    OpenAPI_list_t *match_snssai_dnnsList = NULL;
    event = cJSON_GetObjectItemCaseSensitive(bsf_event_notificationJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_bsf_event_notification_parseFromJSON() failed [event]");
        goto end;
    }
    if (!cJSON_IsString(event)) {
        ogs_error("OpenAPI_bsf_event_notification_parseFromJSON() failed [event]");
        goto end;
    }
    eventVariable = OpenAPI_bsf_event_FromString(event->valuestring);

    pcf_for_ue_info = cJSON_GetObjectItemCaseSensitive(bsf_event_notificationJSON, "pcfForUeInfo");
    if (pcf_for_ue_info) {
    pcf_for_ue_info_local_nonprim = OpenAPI_pcf_for_ue_info_parseFromJSON(pcf_for_ue_info);
    if (!pcf_for_ue_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_for_ue_info_parseFromJSON failed [pcf_for_ue_info]");
        goto end;
    }
    }

    pcf_for_pdu_sess_infos = cJSON_GetObjectItemCaseSensitive(bsf_event_notificationJSON, "pcfForPduSessInfos");
    if (pcf_for_pdu_sess_infos) {
        cJSON *pcf_for_pdu_sess_infos_local = NULL;
        if (!cJSON_IsArray(pcf_for_pdu_sess_infos)) {
            ogs_error("OpenAPI_bsf_event_notification_parseFromJSON() failed [pcf_for_pdu_sess_infos]");
            goto end;
        }

        pcf_for_pdu_sess_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_for_pdu_sess_infos_local, pcf_for_pdu_sess_infos) {
            if (!cJSON_IsObject(pcf_for_pdu_sess_infos_local)) {
                ogs_error("OpenAPI_bsf_event_notification_parseFromJSON() failed [pcf_for_pdu_sess_infos]");
                goto end;
            }
            OpenAPI_pcf_for_pdu_session_info_t *pcf_for_pdu_sess_infosItem = OpenAPI_pcf_for_pdu_session_info_parseFromJSON(pcf_for_pdu_sess_infos_local);
            if (!pcf_for_pdu_sess_infosItem) {
                ogs_error("No pcf_for_pdu_sess_infosItem");
                goto end;
            }
            OpenAPI_list_add(pcf_for_pdu_sess_infosList, pcf_for_pdu_sess_infosItem);
        }
    }

    match_snssai_dnns = cJSON_GetObjectItemCaseSensitive(bsf_event_notificationJSON, "matchSnssaiDnns");
    if (match_snssai_dnns) {
        cJSON *match_snssai_dnns_local = NULL;
        if (!cJSON_IsArray(match_snssai_dnns)) {
            ogs_error("OpenAPI_bsf_event_notification_parseFromJSON() failed [match_snssai_dnns]");
            goto end;
        }

        match_snssai_dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(match_snssai_dnns_local, match_snssai_dnns) {
            if (!cJSON_IsObject(match_snssai_dnns_local)) {
                ogs_error("OpenAPI_bsf_event_notification_parseFromJSON() failed [match_snssai_dnns]");
                goto end;
            }
            OpenAPI_snssai_dnn_pair_t *match_snssai_dnnsItem = OpenAPI_snssai_dnn_pair_parseFromJSON(match_snssai_dnns_local);
            if (!match_snssai_dnnsItem) {
                ogs_error("No match_snssai_dnnsItem");
                goto end;
            }
            OpenAPI_list_add(match_snssai_dnnsList, match_snssai_dnnsItem);
        }
    }

    bsf_event_notification_local_var = OpenAPI_bsf_event_notification_create (
        eventVariable,
        pcf_for_ue_info ? pcf_for_ue_info_local_nonprim : NULL,
        pcf_for_pdu_sess_infos ? pcf_for_pdu_sess_infosList : NULL,
        match_snssai_dnns ? match_snssai_dnnsList : NULL
    );

    return bsf_event_notification_local_var;
end:
    if (pcf_for_ue_info_local_nonprim) {
        OpenAPI_pcf_for_ue_info_free(pcf_for_ue_info_local_nonprim);
        pcf_for_ue_info_local_nonprim = NULL;
    }
    if (pcf_for_pdu_sess_infosList) {
        OpenAPI_list_for_each(pcf_for_pdu_sess_infosList, node) {
            OpenAPI_pcf_for_pdu_session_info_free(node->data);
        }
        OpenAPI_list_free(pcf_for_pdu_sess_infosList);
        pcf_for_pdu_sess_infosList = NULL;
    }
    if (match_snssai_dnnsList) {
        OpenAPI_list_for_each(match_snssai_dnnsList, node) {
            OpenAPI_snssai_dnn_pair_free(node->data);
        }
        OpenAPI_list_free(match_snssai_dnnsList);
        match_snssai_dnnsList = NULL;
    }
    return NULL;
}

OpenAPI_bsf_event_notification_t *OpenAPI_bsf_event_notification_copy(OpenAPI_bsf_event_notification_t *dst, OpenAPI_bsf_event_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bsf_event_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bsf_event_notification_convertToJSON() failed");
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

    OpenAPI_bsf_event_notification_free(dst);
    dst = OpenAPI_bsf_event_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

