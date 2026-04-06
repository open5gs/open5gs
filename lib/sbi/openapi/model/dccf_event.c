
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dccf_event.h"

OpenAPI_dccf_event_t *OpenAPI_dccf_event_create(
    OpenAPI_nwdaf_event_e nwdaf_event,
    OpenAPI_smf_event_e smf_event,
    OpenAPI_amf_event_type_e amf_event,
    OpenAPI_nef_event_e nef_event,
    OpenAPI_event_type_e udm_event,
    OpenAPI_af_event_e af_event,
    OpenAPI_sac_event_t *sac_event,
    OpenAPI_notification_event_type_e nrf_event,
    OpenAPI_event_notify_data_type_e gmlc_event,
    OpenAPI_event_type_1_e upf_event,
    OpenAPI_pc_event_e pcf_event
)
{
    OpenAPI_dccf_event_t *dccf_event_local_var = ogs_malloc(sizeof(OpenAPI_dccf_event_t));
    ogs_assert(dccf_event_local_var);

    dccf_event_local_var->nwdaf_event = nwdaf_event;
    dccf_event_local_var->smf_event = smf_event;
    dccf_event_local_var->amf_event = amf_event;
    dccf_event_local_var->nef_event = nef_event;
    dccf_event_local_var->udm_event = udm_event;
    dccf_event_local_var->af_event = af_event;
    dccf_event_local_var->sac_event = sac_event;
    dccf_event_local_var->nrf_event = nrf_event;
    dccf_event_local_var->gmlc_event = gmlc_event;
    dccf_event_local_var->upf_event = upf_event;
    dccf_event_local_var->pcf_event = pcf_event;

    return dccf_event_local_var;
}

void OpenAPI_dccf_event_free(OpenAPI_dccf_event_t *dccf_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dccf_event) {
        return;
    }
    if (dccf_event->sac_event) {
        OpenAPI_sac_event_free(dccf_event->sac_event);
        dccf_event->sac_event = NULL;
    }
    ogs_free(dccf_event);
}

cJSON *OpenAPI_dccf_event_convertToJSON(OpenAPI_dccf_event_t *dccf_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dccf_event == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [DccfEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dccf_event->nwdaf_event != OpenAPI_nwdaf_event_NULL) {
    if (cJSON_AddStringToObject(item, "nwdafEvent", OpenAPI_nwdaf_event_ToString(dccf_event->nwdaf_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [nwdaf_event]");
        goto end;
    }
    }

    if (dccf_event->smf_event != OpenAPI_smf_event_NULL) {
    if (cJSON_AddStringToObject(item, "smfEvent", OpenAPI_smf_event_ToString(dccf_event->smf_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [smf_event]");
        goto end;
    }
    }

    if (dccf_event->amf_event != OpenAPI_amf_event_type_NULL) {
    if (cJSON_AddStringToObject(item, "amfEvent", OpenAPI_amf_event_type_ToString(dccf_event->amf_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [amf_event]");
        goto end;
    }
    }

    if (dccf_event->nef_event != OpenAPI_nef_event_NULL) {
    if (cJSON_AddStringToObject(item, "nefEvent", OpenAPI_nef_event_ToString(dccf_event->nef_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [nef_event]");
        goto end;
    }
    }

    if (dccf_event->udm_event != OpenAPI_event_type_NULL) {
    if (cJSON_AddStringToObject(item, "udmEvent", OpenAPI_event_type_ToString(dccf_event->udm_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [udm_event]");
        goto end;
    }
    }

    if (dccf_event->af_event != OpenAPI_af_event_NULL) {
    if (cJSON_AddStringToObject(item, "afEvent", OpenAPI_af_event_ToString(dccf_event->af_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [af_event]");
        goto end;
    }
    }

    if (dccf_event->sac_event) {
    cJSON *sac_event_local_JSON = OpenAPI_sac_event_convertToJSON(dccf_event->sac_event);
    if (sac_event_local_JSON == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [sac_event]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sacEvent", sac_event_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [sac_event]");
        goto end;
    }
    }

    if (dccf_event->nrf_event != OpenAPI_notification_event_type_NULL) {
    if (cJSON_AddStringToObject(item, "nrfEvent", OpenAPI_notification_event_type_ToString(dccf_event->nrf_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [nrf_event]");
        goto end;
    }
    }

    if (dccf_event->gmlc_event != OpenAPI_event_notify_data_type_NULL) {
    if (cJSON_AddStringToObject(item, "gmlcEvent", OpenAPI_event_notify_data_type_ToString(dccf_event->gmlc_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [gmlc_event]");
        goto end;
    }
    }

    if (dccf_event->upf_event != OpenAPI_event_type_1_NULL) {
    if (cJSON_AddStringToObject(item, "upfEvent", OpenAPI_event_type_1_ToString(dccf_event->upf_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [upf_event]");
        goto end;
    }
    }

    if (dccf_event->pcf_event != OpenAPI_pc_event_NULL) {
    if (cJSON_AddStringToObject(item, "pcfEvent", OpenAPI_pc_event_ToString(dccf_event->pcf_event)) == NULL) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed [pcf_event]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dccf_event_t *OpenAPI_dccf_event_parseFromJSON(cJSON *dccf_eventJSON)
{
    OpenAPI_dccf_event_t *dccf_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nwdaf_event = NULL;
    OpenAPI_nwdaf_event_e nwdaf_eventVariable = 0;
    cJSON *smf_event = NULL;
    OpenAPI_smf_event_e smf_eventVariable = 0;
    cJSON *amf_event = NULL;
    OpenAPI_amf_event_type_e amf_eventVariable = 0;
    cJSON *nef_event = NULL;
    OpenAPI_nef_event_e nef_eventVariable = 0;
    cJSON *udm_event = NULL;
    OpenAPI_event_type_e udm_eventVariable = 0;
    cJSON *af_event = NULL;
    OpenAPI_af_event_e af_eventVariable = 0;
    cJSON *sac_event = NULL;
    OpenAPI_sac_event_t *sac_event_local_nonprim = NULL;
    cJSON *nrf_event = NULL;
    OpenAPI_notification_event_type_e nrf_eventVariable = 0;
    cJSON *gmlc_event = NULL;
    OpenAPI_event_notify_data_type_e gmlc_eventVariable = 0;
    cJSON *upf_event = NULL;
    OpenAPI_event_type_1_e upf_eventVariable = 0;
    cJSON *pcf_event = NULL;
    OpenAPI_pc_event_e pcf_eventVariable = 0;
    nwdaf_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "nwdafEvent");
    if (nwdaf_event) {
    if (!cJSON_IsString(nwdaf_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [nwdaf_event]");
        goto end;
    }
    nwdaf_eventVariable = OpenAPI_nwdaf_event_FromString(nwdaf_event->valuestring);
    }

    smf_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "smfEvent");
    if (smf_event) {
    if (!cJSON_IsString(smf_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [smf_event]");
        goto end;
    }
    smf_eventVariable = OpenAPI_smf_event_FromString(smf_event->valuestring);
    }

    amf_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "amfEvent");
    if (amf_event) {
    if (!cJSON_IsString(amf_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [amf_event]");
        goto end;
    }
    amf_eventVariable = OpenAPI_amf_event_type_FromString(amf_event->valuestring);
    }

    nef_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "nefEvent");
    if (nef_event) {
    if (!cJSON_IsString(nef_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [nef_event]");
        goto end;
    }
    nef_eventVariable = OpenAPI_nef_event_FromString(nef_event->valuestring);
    }

    udm_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "udmEvent");
    if (udm_event) {
    if (!cJSON_IsString(udm_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [udm_event]");
        goto end;
    }
    udm_eventVariable = OpenAPI_event_type_FromString(udm_event->valuestring);
    }

    af_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "afEvent");
    if (af_event) {
    if (!cJSON_IsString(af_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [af_event]");
        goto end;
    }
    af_eventVariable = OpenAPI_af_event_FromString(af_event->valuestring);
    }

    sac_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "sacEvent");
    if (sac_event) {
    sac_event_local_nonprim = OpenAPI_sac_event_parseFromJSON(sac_event);
    if (!sac_event_local_nonprim) {
        ogs_error("OpenAPI_sac_event_parseFromJSON failed [sac_event]");
        goto end;
    }
    }

    nrf_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "nrfEvent");
    if (nrf_event) {
    if (!cJSON_IsString(nrf_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [nrf_event]");
        goto end;
    }
    nrf_eventVariable = OpenAPI_notification_event_type_FromString(nrf_event->valuestring);
    }

    gmlc_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "gmlcEvent");
    if (gmlc_event) {
    if (!cJSON_IsString(gmlc_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [gmlc_event]");
        goto end;
    }
    gmlc_eventVariable = OpenAPI_event_notify_data_type_FromString(gmlc_event->valuestring);
    }

    upf_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "upfEvent");
    if (upf_event) {
    if (!cJSON_IsString(upf_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [upf_event]");
        goto end;
    }
    upf_eventVariable = OpenAPI_event_type_1_FromString(upf_event->valuestring);
    }

    pcf_event = cJSON_GetObjectItemCaseSensitive(dccf_eventJSON, "pcfEvent");
    if (pcf_event) {
    if (!cJSON_IsString(pcf_event)) {
        ogs_error("OpenAPI_dccf_event_parseFromJSON() failed [pcf_event]");
        goto end;
    }
    pcf_eventVariable = OpenAPI_pc_event_FromString(pcf_event->valuestring);
    }

    dccf_event_local_var = OpenAPI_dccf_event_create (
        nwdaf_event ? nwdaf_eventVariable : 0,
        smf_event ? smf_eventVariable : 0,
        amf_event ? amf_eventVariable : 0,
        nef_event ? nef_eventVariable : 0,
        udm_event ? udm_eventVariable : 0,
        af_event ? af_eventVariable : 0,
        sac_event ? sac_event_local_nonprim : NULL,
        nrf_event ? nrf_eventVariable : 0,
        gmlc_event ? gmlc_eventVariable : 0,
        upf_event ? upf_eventVariable : 0,
        pcf_event ? pcf_eventVariable : 0
    );

    return dccf_event_local_var;
end:
    if (sac_event_local_nonprim) {
        OpenAPI_sac_event_free(sac_event_local_nonprim);
        sac_event_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_dccf_event_t *OpenAPI_dccf_event_copy(OpenAPI_dccf_event_t *dst, OpenAPI_dccf_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dccf_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dccf_event_convertToJSON() failed");
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

    OpenAPI_dccf_event_free(dst);
    dst = OpenAPI_dccf_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

