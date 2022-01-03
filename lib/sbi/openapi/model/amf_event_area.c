
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_area.h"

OpenAPI_amf_event_area_t *OpenAPI_amf_event_area_create(
    OpenAPI_presence_info_t *presence_info,
    OpenAPI_ladn_info_t *ladn_info,
    OpenAPI_snssai_t *s_nssai,
    char *nsi_id
)
{
    OpenAPI_amf_event_area_t *amf_event_area_local_var = ogs_malloc(sizeof(OpenAPI_amf_event_area_t));
    ogs_assert(amf_event_area_local_var);

    amf_event_area_local_var->presence_info = presence_info;
    amf_event_area_local_var->ladn_info = ladn_info;
    amf_event_area_local_var->s_nssai = s_nssai;
    amf_event_area_local_var->nsi_id = nsi_id;

    return amf_event_area_local_var;
}

void OpenAPI_amf_event_area_free(OpenAPI_amf_event_area_t *amf_event_area)
{
    if (NULL == amf_event_area) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_presence_info_free(amf_event_area->presence_info);
    OpenAPI_ladn_info_free(amf_event_area->ladn_info);
    OpenAPI_snssai_free(amf_event_area->s_nssai);
    ogs_free(amf_event_area->nsi_id);
    ogs_free(amf_event_area);
}

cJSON *OpenAPI_amf_event_area_convertToJSON(OpenAPI_amf_event_area_t *amf_event_area)
{
    cJSON *item = NULL;

    if (amf_event_area == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [AmfEventArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (amf_event_area->presence_info) {
    cJSON *presence_info_local_JSON = OpenAPI_presence_info_convertToJSON(amf_event_area->presence_info);
    if (presence_info_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [presence_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "presenceInfo", presence_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [presence_info]");
        goto end;
    }
    }

    if (amf_event_area->ladn_info) {
    cJSON *ladn_info_local_JSON = OpenAPI_ladn_info_convertToJSON(amf_event_area->ladn_info);
    if (ladn_info_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [ladn_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ladnInfo", ladn_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [ladn_info]");
        goto end;
    }
    }

    if (amf_event_area->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(amf_event_area->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (amf_event_area->nsi_id) {
    if (cJSON_AddStringToObject(item, "nsiId", amf_event_area->nsi_id) == NULL) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed [nsi_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_event_area_t *OpenAPI_amf_event_area_parseFromJSON(cJSON *amf_event_areaJSON)
{
    OpenAPI_amf_event_area_t *amf_event_area_local_var = NULL;
    cJSON *presence_info = cJSON_GetObjectItemCaseSensitive(amf_event_areaJSON, "presenceInfo");

    OpenAPI_presence_info_t *presence_info_local_nonprim = NULL;
    if (presence_info) {
    presence_info_local_nonprim = OpenAPI_presence_info_parseFromJSON(presence_info);
    }

    cJSON *ladn_info = cJSON_GetObjectItemCaseSensitive(amf_event_areaJSON, "ladnInfo");

    OpenAPI_ladn_info_t *ladn_info_local_nonprim = NULL;
    if (ladn_info) {
    ladn_info_local_nonprim = OpenAPI_ladn_info_parseFromJSON(ladn_info);
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(amf_event_areaJSON, "sNssai");

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    }

    cJSON *nsi_id = cJSON_GetObjectItemCaseSensitive(amf_event_areaJSON, "nsiId");

    if (nsi_id) {
    if (!cJSON_IsString(nsi_id)) {
        ogs_error("OpenAPI_amf_event_area_parseFromJSON() failed [nsi_id]");
        goto end;
    }
    }

    amf_event_area_local_var = OpenAPI_amf_event_area_create (
        presence_info ? presence_info_local_nonprim : NULL,
        ladn_info ? ladn_info_local_nonprim : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        nsi_id ? ogs_strdup(nsi_id->valuestring) : NULL
    );

    return amf_event_area_local_var;
end:
    return NULL;
}

OpenAPI_amf_event_area_t *OpenAPI_amf_event_area_copy(OpenAPI_amf_event_area_t *dst, OpenAPI_amf_event_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_area_convertToJSON() failed");
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

    OpenAPI_amf_event_area_free(dst);
    dst = OpenAPI_amf_event_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

