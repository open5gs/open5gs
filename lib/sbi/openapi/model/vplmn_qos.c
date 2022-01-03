
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vplmn_qos.h"

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_create(
    bool is__5qi,
    int _5qi,
    OpenAPI_arp_t *arp,
    OpenAPI_ambr_t *session_ambr,
    char *max_fbr_dl,
    char *max_fbr_ul,
    char *gua_fbr_dl,
    char *gua_fbr_ul
)
{
    OpenAPI_vplmn_qos_t *vplmn_qos_local_var = ogs_malloc(sizeof(OpenAPI_vplmn_qos_t));
    ogs_assert(vplmn_qos_local_var);

    vplmn_qos_local_var->is__5qi = is__5qi;
    vplmn_qos_local_var->_5qi = _5qi;
    vplmn_qos_local_var->arp = arp;
    vplmn_qos_local_var->session_ambr = session_ambr;
    vplmn_qos_local_var->max_fbr_dl = max_fbr_dl;
    vplmn_qos_local_var->max_fbr_ul = max_fbr_ul;
    vplmn_qos_local_var->gua_fbr_dl = gua_fbr_dl;
    vplmn_qos_local_var->gua_fbr_ul = gua_fbr_ul;

    return vplmn_qos_local_var;
}

void OpenAPI_vplmn_qos_free(OpenAPI_vplmn_qos_t *vplmn_qos)
{
    if (NULL == vplmn_qos) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_arp_free(vplmn_qos->arp);
    OpenAPI_ambr_free(vplmn_qos->session_ambr);
    ogs_free(vplmn_qos->max_fbr_dl);
    ogs_free(vplmn_qos->max_fbr_ul);
    ogs_free(vplmn_qos->gua_fbr_dl);
    ogs_free(vplmn_qos->gua_fbr_ul);
    ogs_free(vplmn_qos);
}

cJSON *OpenAPI_vplmn_qos_convertToJSON(OpenAPI_vplmn_qos_t *vplmn_qos)
{
    cJSON *item = NULL;

    if (vplmn_qos == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [VplmnQos]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vplmn_qos->is__5qi) {
    if (cJSON_AddNumberToObject(item, "5qi", vplmn_qos->_5qi) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [_5qi]");
        goto end;
    }
    }

    if (vplmn_qos->arp) {
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(vplmn_qos->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [arp]");
        goto end;
    }
    }

    if (vplmn_qos->session_ambr) {
    cJSON *session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(vplmn_qos->session_ambr);
    if (session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessionAmbr", session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [session_ambr]");
        goto end;
    }
    }

    if (vplmn_qos->max_fbr_dl) {
    if (cJSON_AddStringToObject(item, "maxFbrDl", vplmn_qos->max_fbr_dl) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [max_fbr_dl]");
        goto end;
    }
    }

    if (vplmn_qos->max_fbr_ul) {
    if (cJSON_AddStringToObject(item, "maxFbrUl", vplmn_qos->max_fbr_ul) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [max_fbr_ul]");
        goto end;
    }
    }

    if (vplmn_qos->gua_fbr_dl) {
    if (cJSON_AddStringToObject(item, "guaFbrDl", vplmn_qos->gua_fbr_dl) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    if (vplmn_qos->gua_fbr_ul) {
    if (cJSON_AddStringToObject(item, "guaFbrUl", vplmn_qos->gua_fbr_ul) == NULL) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_parseFromJSON(cJSON *vplmn_qosJSON)
{
    OpenAPI_vplmn_qos_t *vplmn_qos_local_var = NULL;
    cJSON *_5qi = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "5qi");

    if (_5qi) {
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [_5qi]");
        goto end;
    }
    }

    cJSON *arp = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "arp");

    OpenAPI_arp_t *arp_local_nonprim = NULL;
    if (arp) {
    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);
    }

    cJSON *session_ambr = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "sessionAmbr");

    OpenAPI_ambr_t *session_ambr_local_nonprim = NULL;
    if (session_ambr) {
    session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(session_ambr);
    }

    cJSON *max_fbr_dl = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "maxFbrDl");

    if (max_fbr_dl) {
    if (!cJSON_IsString(max_fbr_dl)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [max_fbr_dl]");
        goto end;
    }
    }

    cJSON *max_fbr_ul = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "maxFbrUl");

    if (max_fbr_ul) {
    if (!cJSON_IsString(max_fbr_ul)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [max_fbr_ul]");
        goto end;
    }
    }

    cJSON *gua_fbr_dl = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "guaFbrDl");

    if (gua_fbr_dl) {
    if (!cJSON_IsString(gua_fbr_dl)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    cJSON *gua_fbr_ul = cJSON_GetObjectItemCaseSensitive(vplmn_qosJSON, "guaFbrUl");

    if (gua_fbr_ul) {
    if (!cJSON_IsString(gua_fbr_ul)) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

    vplmn_qos_local_var = OpenAPI_vplmn_qos_create (
        _5qi ? true : false,
        _5qi ? _5qi->valuedouble : 0,
        arp ? arp_local_nonprim : NULL,
        session_ambr ? session_ambr_local_nonprim : NULL,
        max_fbr_dl ? ogs_strdup(max_fbr_dl->valuestring) : NULL,
        max_fbr_ul ? ogs_strdup(max_fbr_ul->valuestring) : NULL,
        gua_fbr_dl ? ogs_strdup(gua_fbr_dl->valuestring) : NULL,
        gua_fbr_ul ? ogs_strdup(gua_fbr_ul->valuestring) : NULL
    );

    return vplmn_qos_local_var;
end:
    return NULL;
}

OpenAPI_vplmn_qos_t *OpenAPI_vplmn_qos_copy(OpenAPI_vplmn_qos_t *dst, OpenAPI_vplmn_qos_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vplmn_qos_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vplmn_qos_convertToJSON() failed");
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

    OpenAPI_vplmn_qos_free(dst);
    dst = OpenAPI_vplmn_qos_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

