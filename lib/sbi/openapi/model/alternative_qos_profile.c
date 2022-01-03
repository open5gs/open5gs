
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alternative_qos_profile.h"

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_create(
    int index,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    bool is_packet_delay_budget,
    int packet_delay_budget,
    char *packet_err_rate
)
{
    OpenAPI_alternative_qos_profile_t *alternative_qos_profile_local_var = ogs_malloc(sizeof(OpenAPI_alternative_qos_profile_t));
    ogs_assert(alternative_qos_profile_local_var);

    alternative_qos_profile_local_var->index = index;
    alternative_qos_profile_local_var->gua_fbr_dl = gua_fbr_dl;
    alternative_qos_profile_local_var->gua_fbr_ul = gua_fbr_ul;
    alternative_qos_profile_local_var->is_packet_delay_budget = is_packet_delay_budget;
    alternative_qos_profile_local_var->packet_delay_budget = packet_delay_budget;
    alternative_qos_profile_local_var->packet_err_rate = packet_err_rate;

    return alternative_qos_profile_local_var;
}

void OpenAPI_alternative_qos_profile_free(OpenAPI_alternative_qos_profile_t *alternative_qos_profile)
{
    if (NULL == alternative_qos_profile) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(alternative_qos_profile->gua_fbr_dl);
    ogs_free(alternative_qos_profile->gua_fbr_ul);
    ogs_free(alternative_qos_profile->packet_err_rate);
    ogs_free(alternative_qos_profile);
}

cJSON *OpenAPI_alternative_qos_profile_convertToJSON(OpenAPI_alternative_qos_profile_t *alternative_qos_profile)
{
    cJSON *item = NULL;

    if (alternative_qos_profile == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [AlternativeQosProfile]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "index", alternative_qos_profile->index) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [index]");
        goto end;
    }

    if (alternative_qos_profile->gua_fbr_dl) {
    if (cJSON_AddStringToObject(item, "guaFbrDl", alternative_qos_profile->gua_fbr_dl) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    if (alternative_qos_profile->gua_fbr_ul) {
    if (cJSON_AddStringToObject(item, "guaFbrUl", alternative_qos_profile->gua_fbr_ul) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

    if (alternative_qos_profile->is_packet_delay_budget) {
    if (cJSON_AddNumberToObject(item, "packetDelayBudget", alternative_qos_profile->packet_delay_budget) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }
    }

    if (alternative_qos_profile->packet_err_rate) {
    if (cJSON_AddStringToObject(item, "packetErrRate", alternative_qos_profile->packet_err_rate) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [packet_err_rate]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_parseFromJSON(cJSON *alternative_qos_profileJSON)
{
    OpenAPI_alternative_qos_profile_t *alternative_qos_profile_local_var = NULL;
    cJSON *index = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "index");
    if (!index) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [index]");
        goto end;
    }

    if (!cJSON_IsNumber(index)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [index]");
        goto end;
    }

    cJSON *gua_fbr_dl = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "guaFbrDl");

    if (gua_fbr_dl) {
    if (!cJSON_IsString(gua_fbr_dl)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    cJSON *gua_fbr_ul = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "guaFbrUl");

    if (gua_fbr_ul) {
    if (!cJSON_IsString(gua_fbr_ul)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

    cJSON *packet_delay_budget = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "packetDelayBudget");

    if (packet_delay_budget) {
    if (!cJSON_IsNumber(packet_delay_budget)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }
    }

    cJSON *packet_err_rate = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "packetErrRate");

    if (packet_err_rate) {
    if (!cJSON_IsString(packet_err_rate)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [packet_err_rate]");
        goto end;
    }
    }

    alternative_qos_profile_local_var = OpenAPI_alternative_qos_profile_create (
        
        index->valuedouble,
        gua_fbr_dl ? ogs_strdup(gua_fbr_dl->valuestring) : NULL,
        gua_fbr_ul ? ogs_strdup(gua_fbr_ul->valuestring) : NULL,
        packet_delay_budget ? true : false,
        packet_delay_budget ? packet_delay_budget->valuedouble : 0,
        packet_err_rate ? ogs_strdup(packet_err_rate->valuestring) : NULL
    );

    return alternative_qos_profile_local_var;
end:
    return NULL;
}

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_copy(OpenAPI_alternative_qos_profile_t *dst, OpenAPI_alternative_qos_profile_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_alternative_qos_profile_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed");
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

    OpenAPI_alternative_qos_profile_free(dst);
    dst = OpenAPI_alternative_qos_profile_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

