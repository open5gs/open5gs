
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_event_notification.h"

OpenAPI_pdu_session_event_notification_t *OpenAPI_pdu_session_event_notification_create(
    OpenAPI_af_event_notification_t *ev_notif,
    char *supi,
    char *ue_ipv4,
    char *ue_ipv6,
    char *ue_mac,
    OpenAPI_npcf_pdu_session_status_e status,
    OpenAPI_pcf_addressing_info_t *pcf_info,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *gpsi
)
{
    OpenAPI_pdu_session_event_notification_t *pdu_session_event_notification_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_event_notification_t));
    ogs_assert(pdu_session_event_notification_local_var);

    pdu_session_event_notification_local_var->ev_notif = ev_notif;
    pdu_session_event_notification_local_var->supi = supi;
    pdu_session_event_notification_local_var->ue_ipv4 = ue_ipv4;
    pdu_session_event_notification_local_var->ue_ipv6 = ue_ipv6;
    pdu_session_event_notification_local_var->ue_mac = ue_mac;
    pdu_session_event_notification_local_var->status = status;
    pdu_session_event_notification_local_var->pcf_info = pcf_info;
    pdu_session_event_notification_local_var->dnn = dnn;
    pdu_session_event_notification_local_var->snssai = snssai;
    pdu_session_event_notification_local_var->gpsi = gpsi;

    return pdu_session_event_notification_local_var;
}

void OpenAPI_pdu_session_event_notification_free(OpenAPI_pdu_session_event_notification_t *pdu_session_event_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_event_notification) {
        return;
    }
    if (pdu_session_event_notification->ev_notif) {
        OpenAPI_af_event_notification_free(pdu_session_event_notification->ev_notif);
        pdu_session_event_notification->ev_notif = NULL;
    }
    if (pdu_session_event_notification->supi) {
        ogs_free(pdu_session_event_notification->supi);
        pdu_session_event_notification->supi = NULL;
    }
    if (pdu_session_event_notification->ue_ipv4) {
        ogs_free(pdu_session_event_notification->ue_ipv4);
        pdu_session_event_notification->ue_ipv4 = NULL;
    }
    if (pdu_session_event_notification->ue_ipv6) {
        ogs_free(pdu_session_event_notification->ue_ipv6);
        pdu_session_event_notification->ue_ipv6 = NULL;
    }
    if (pdu_session_event_notification->ue_mac) {
        ogs_free(pdu_session_event_notification->ue_mac);
        pdu_session_event_notification->ue_mac = NULL;
    }
    if (pdu_session_event_notification->pcf_info) {
        OpenAPI_pcf_addressing_info_free(pdu_session_event_notification->pcf_info);
        pdu_session_event_notification->pcf_info = NULL;
    }
    if (pdu_session_event_notification->dnn) {
        ogs_free(pdu_session_event_notification->dnn);
        pdu_session_event_notification->dnn = NULL;
    }
    if (pdu_session_event_notification->snssai) {
        OpenAPI_snssai_free(pdu_session_event_notification->snssai);
        pdu_session_event_notification->snssai = NULL;
    }
    if (pdu_session_event_notification->gpsi) {
        ogs_free(pdu_session_event_notification->gpsi);
        pdu_session_event_notification->gpsi = NULL;
    }
    ogs_free(pdu_session_event_notification);
}

cJSON *OpenAPI_pdu_session_event_notification_convertToJSON(OpenAPI_pdu_session_event_notification_t *pdu_session_event_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_event_notification == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [PduSessionEventNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pdu_session_event_notification->ev_notif) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [ev_notif]");
        return NULL;
    }
    cJSON *ev_notif_local_JSON = OpenAPI_af_event_notification_convertToJSON(pdu_session_event_notification->ev_notif);
    if (ev_notif_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [ev_notif]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evNotif", ev_notif_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [ev_notif]");
        goto end;
    }

    if (pdu_session_event_notification->supi) {
    if (cJSON_AddStringToObject(item, "supi", pdu_session_event_notification->supi) == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (pdu_session_event_notification->ue_ipv4) {
    if (cJSON_AddStringToObject(item, "ueIpv4", pdu_session_event_notification->ue_ipv4) == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    if (pdu_session_event_notification->ue_ipv6) {
    if (cJSON_AddStringToObject(item, "ueIpv6", pdu_session_event_notification->ue_ipv6) == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [ue_ipv6]");
        goto end;
    }
    }

    if (pdu_session_event_notification->ue_mac) {
    if (cJSON_AddStringToObject(item, "ueMac", pdu_session_event_notification->ue_mac) == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [ue_mac]");
        goto end;
    }
    }

    if (pdu_session_event_notification->status != OpenAPI_npcf_pdu_session_status_NULL) {
    if (cJSON_AddStringToObject(item, "status", OpenAPI_npcf_pdu_session_status_ToString(pdu_session_event_notification->status)) == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [status]");
        goto end;
    }
    }

    if (pdu_session_event_notification->pcf_info) {
    cJSON *pcf_info_local_JSON = OpenAPI_pcf_addressing_info_convertToJSON(pdu_session_event_notification->pcf_info);
    if (pcf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [pcf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfInfo", pcf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [pcf_info]");
        goto end;
    }
    }

    if (pdu_session_event_notification->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pdu_session_event_notification->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (pdu_session_event_notification->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_event_notification->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (pdu_session_event_notification->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", pdu_session_event_notification->gpsi) == NULL) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_event_notification_t *OpenAPI_pdu_session_event_notification_parseFromJSON(cJSON *pdu_session_event_notificationJSON)
{
    OpenAPI_pdu_session_event_notification_t *pdu_session_event_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ev_notif = NULL;
    OpenAPI_af_event_notification_t *ev_notif_local_nonprim = NULL;
    cJSON *supi = NULL;
    cJSON *ue_ipv4 = NULL;
    cJSON *ue_ipv6 = NULL;
    cJSON *ue_mac = NULL;
    cJSON *status = NULL;
    OpenAPI_npcf_pdu_session_status_e statusVariable = 0;
    cJSON *pcf_info = NULL;
    OpenAPI_pcf_addressing_info_t *pcf_info_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *gpsi = NULL;
    ev_notif = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "evNotif");
    if (!ev_notif) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [ev_notif]");
        goto end;
    }
    ev_notif_local_nonprim = OpenAPI_af_event_notification_parseFromJSON(ev_notif);
    if (!ev_notif_local_nonprim) {
        ogs_error("OpenAPI_af_event_notification_parseFromJSON failed [ev_notif]");
        goto end;
    }

    supi = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    ue_ipv4 = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "ueIpv4");
    if (ue_ipv4) {
    if (!cJSON_IsString(ue_ipv4) && !cJSON_IsNull(ue_ipv4)) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    ue_ipv6 = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "ueIpv6");
    if (ue_ipv6) {
    if (!cJSON_IsString(ue_ipv6) && !cJSON_IsNull(ue_ipv6)) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [ue_ipv6]");
        goto end;
    }
    }

    ue_mac = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "ueMac");
    if (ue_mac) {
    if (!cJSON_IsString(ue_mac) && !cJSON_IsNull(ue_mac)) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [ue_mac]");
        goto end;
    }
    }

    status = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "status");
    if (status) {
    if (!cJSON_IsString(status)) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [status]");
        goto end;
    }
    statusVariable = OpenAPI_npcf_pdu_session_status_FromString(status->valuestring);
    }

    pcf_info = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "pcfInfo");
    if (pcf_info) {
    pcf_info_local_nonprim = OpenAPI_pcf_addressing_info_parseFromJSON(pcf_info);
    if (!pcf_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_addressing_info_parseFromJSON failed [pcf_info]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(pdu_session_event_notificationJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_pdu_session_event_notification_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    pdu_session_event_notification_local_var = OpenAPI_pdu_session_event_notification_create (
        ev_notif_local_nonprim,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        ue_ipv4 && !cJSON_IsNull(ue_ipv4) ? ogs_strdup(ue_ipv4->valuestring) : NULL,
        ue_ipv6 && !cJSON_IsNull(ue_ipv6) ? ogs_strdup(ue_ipv6->valuestring) : NULL,
        ue_mac && !cJSON_IsNull(ue_mac) ? ogs_strdup(ue_mac->valuestring) : NULL,
        status ? statusVariable : 0,
        pcf_info ? pcf_info_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL
    );

    return pdu_session_event_notification_local_var;
end:
    if (ev_notif_local_nonprim) {
        OpenAPI_af_event_notification_free(ev_notif_local_nonprim);
        ev_notif_local_nonprim = NULL;
    }
    if (pcf_info_local_nonprim) {
        OpenAPI_pcf_addressing_info_free(pcf_info_local_nonprim);
        pcf_info_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_event_notification_t *OpenAPI_pdu_session_event_notification_copy(OpenAPI_pdu_session_event_notification_t *dst, OpenAPI_pdu_session_event_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_event_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_event_notification_convertToJSON() failed");
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

    OpenAPI_pdu_session_event_notification_free(dst);
    dst = OpenAPI_pdu_session_event_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

