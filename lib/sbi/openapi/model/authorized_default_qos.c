
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authorized_default_qos.h"

OpenAPI_authorized_default_qos_t *OpenAPI_authorized_default_qos_create(
    bool is__5qi,
    int _5qi,
    OpenAPI_arp_t *arp,
    bool is_priority_level,
    int priority_level,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    char *maxbr_ul,
    char *maxbr_dl,
    char *gbr_ul,
    char *gbr_dl,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol
)
{
    OpenAPI_authorized_default_qos_t *authorized_default_qos_local_var = ogs_malloc(sizeof(OpenAPI_authorized_default_qos_t));
    ogs_assert(authorized_default_qos_local_var);

    authorized_default_qos_local_var->is__5qi = is__5qi;
    authorized_default_qos_local_var->_5qi = _5qi;
    authorized_default_qos_local_var->arp = arp;
    authorized_default_qos_local_var->is_priority_level = is_priority_level;
    authorized_default_qos_local_var->priority_level = priority_level;
    authorized_default_qos_local_var->is_aver_window = is_aver_window;
    authorized_default_qos_local_var->aver_window = aver_window;
    authorized_default_qos_local_var->is_max_data_burst_vol = is_max_data_burst_vol;
    authorized_default_qos_local_var->max_data_burst_vol = max_data_burst_vol;
    authorized_default_qos_local_var->maxbr_ul = maxbr_ul;
    authorized_default_qos_local_var->maxbr_dl = maxbr_dl;
    authorized_default_qos_local_var->gbr_ul = gbr_ul;
    authorized_default_qos_local_var->gbr_dl = gbr_dl;
    authorized_default_qos_local_var->is_ext_max_data_burst_vol = is_ext_max_data_burst_vol;
    authorized_default_qos_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;

    return authorized_default_qos_local_var;
}

void OpenAPI_authorized_default_qos_free(OpenAPI_authorized_default_qos_t *authorized_default_qos)
{
    if (NULL == authorized_default_qos) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_arp_free(authorized_default_qos->arp);
    ogs_free(authorized_default_qos->maxbr_ul);
    ogs_free(authorized_default_qos->maxbr_dl);
    ogs_free(authorized_default_qos->gbr_ul);
    ogs_free(authorized_default_qos->gbr_dl);
    ogs_free(authorized_default_qos);
}

cJSON *OpenAPI_authorized_default_qos_convertToJSON(OpenAPI_authorized_default_qos_t *authorized_default_qos)
{
    cJSON *item = NULL;

    if (authorized_default_qos == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [AuthorizedDefaultQos]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (authorized_default_qos->is__5qi) {
    if (cJSON_AddNumberToObject(item, "5qi", authorized_default_qos->_5qi) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [_5qi]");
        goto end;
    }
    }

    if (authorized_default_qos->arp) {
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(authorized_default_qos->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [arp]");
        goto end;
    }
    }

    if (authorized_default_qos->is_priority_level) {
    if (cJSON_AddNumberToObject(item, "priorityLevel", authorized_default_qos->priority_level) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [priority_level]");
        goto end;
    }
    }

    if (authorized_default_qos->is_aver_window) {
    if (cJSON_AddNumberToObject(item, "averWindow", authorized_default_qos->aver_window) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [aver_window]");
        goto end;
    }
    }

    if (authorized_default_qos->is_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "maxDataBurstVol", authorized_default_qos->max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    if (authorized_default_qos->maxbr_ul) {
    if (cJSON_AddStringToObject(item, "maxbrUl", authorized_default_qos->maxbr_ul) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [maxbr_ul]");
        goto end;
    }
    }

    if (authorized_default_qos->maxbr_dl) {
    if (cJSON_AddStringToObject(item, "maxbrDl", authorized_default_qos->maxbr_dl) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [maxbr_dl]");
        goto end;
    }
    }

    if (authorized_default_qos->gbr_ul) {
    if (cJSON_AddStringToObject(item, "gbrUl", authorized_default_qos->gbr_ul) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [gbr_ul]");
        goto end;
    }
    }

    if (authorized_default_qos->gbr_dl) {
    if (cJSON_AddStringToObject(item, "gbrDl", authorized_default_qos->gbr_dl) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [gbr_dl]");
        goto end;
    }
    }

    if (authorized_default_qos->is_ext_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", authorized_default_qos->ext_max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_authorized_default_qos_t *OpenAPI_authorized_default_qos_parseFromJSON(cJSON *authorized_default_qosJSON)
{
    OpenAPI_authorized_default_qos_t *authorized_default_qos_local_var = NULL;
    cJSON *_5qi = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "5qi");

    if (_5qi) {
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [_5qi]");
        goto end;
    }
    }

    cJSON *arp = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "arp");

    OpenAPI_arp_t *arp_local_nonprim = NULL;
    if (arp) {
    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);
    }

    cJSON *priority_level = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "priorityLevel");

    if (priority_level) {
    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [priority_level]");
        goto end;
    }
    }

    cJSON *aver_window = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "averWindow");

    if (aver_window) {
    if (!cJSON_IsNumber(aver_window)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [aver_window]");
        goto end;
    }
    }

    cJSON *max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "maxDataBurstVol");

    if (max_data_burst_vol) {
    if (!cJSON_IsNumber(max_data_burst_vol)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    cJSON *maxbr_ul = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "maxbrUl");

    if (maxbr_ul) {
    if (!cJSON_IsString(maxbr_ul)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [maxbr_ul]");
        goto end;
    }
    }

    cJSON *maxbr_dl = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "maxbrDl");

    if (maxbr_dl) {
    if (!cJSON_IsString(maxbr_dl)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [maxbr_dl]");
        goto end;
    }
    }

    cJSON *gbr_ul = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "gbrUl");

    if (gbr_ul) {
    if (!cJSON_IsString(gbr_ul)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [gbr_ul]");
        goto end;
    }
    }

    cJSON *gbr_dl = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "gbrDl");

    if (gbr_dl) {
    if (!cJSON_IsString(gbr_dl)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [gbr_dl]");
        goto end;
    }
    }

    cJSON *ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(authorized_default_qosJSON, "extMaxDataBurstVol");

    if (ext_max_data_burst_vol) {
    if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
        ogs_error("OpenAPI_authorized_default_qos_parseFromJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

    authorized_default_qos_local_var = OpenAPI_authorized_default_qos_create (
        _5qi ? true : false,
        _5qi ? _5qi->valuedouble : 0,
        arp ? arp_local_nonprim : NULL,
        priority_level ? true : false,
        priority_level ? priority_level->valuedouble : 0,
        aver_window ? true : false,
        aver_window ? aver_window->valuedouble : 0,
        max_data_burst_vol ? true : false,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        maxbr_ul ? ogs_strdup(maxbr_ul->valuestring) : NULL,
        maxbr_dl ? ogs_strdup(maxbr_dl->valuestring) : NULL,
        gbr_ul ? ogs_strdup(gbr_ul->valuestring) : NULL,
        gbr_dl ? ogs_strdup(gbr_dl->valuestring) : NULL,
        ext_max_data_burst_vol ? true : false,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0
    );

    return authorized_default_qos_local_var;
end:
    return NULL;
}

OpenAPI_authorized_default_qos_t *OpenAPI_authorized_default_qos_copy(OpenAPI_authorized_default_qos_t *dst, OpenAPI_authorized_default_qos_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authorized_default_qos_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authorized_default_qos_convertToJSON() failed");
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

    OpenAPI_authorized_default_qos_free(dst);
    dst = OpenAPI_authorized_default_qos_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

