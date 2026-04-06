
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_location_trends_report_item.h"

OpenAPI_ue_location_trends_report_item_t *OpenAPI_ue_location_trends_report_item_create(
    OpenAPI_tai_t *tai,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_ncgi_t *ncgi,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_n3ga_location_t *n3ga_location,
    int spacing,
    bool is_spacing_var,
    float spacing_var,
    int duration,
    bool is_duration_var,
    float duration_var,
    bool is_occurrences,
    int occurrences,
    char *timestamp
)
{
    OpenAPI_ue_location_trends_report_item_t *ue_location_trends_report_item_local_var = ogs_malloc(sizeof(OpenAPI_ue_location_trends_report_item_t));
    ogs_assert(ue_location_trends_report_item_local_var);

    ue_location_trends_report_item_local_var->tai = tai;
    ue_location_trends_report_item_local_var->ran_node_id = ran_node_id;
    ue_location_trends_report_item_local_var->ncgi = ncgi;
    ue_location_trends_report_item_local_var->ecgi = ecgi;
    ue_location_trends_report_item_local_var->n3ga_location = n3ga_location;
    ue_location_trends_report_item_local_var->spacing = spacing;
    ue_location_trends_report_item_local_var->is_spacing_var = is_spacing_var;
    ue_location_trends_report_item_local_var->spacing_var = spacing_var;
    ue_location_trends_report_item_local_var->duration = duration;
    ue_location_trends_report_item_local_var->is_duration_var = is_duration_var;
    ue_location_trends_report_item_local_var->duration_var = duration_var;
    ue_location_trends_report_item_local_var->is_occurrences = is_occurrences;
    ue_location_trends_report_item_local_var->occurrences = occurrences;
    ue_location_trends_report_item_local_var->timestamp = timestamp;

    return ue_location_trends_report_item_local_var;
}

void OpenAPI_ue_location_trends_report_item_free(OpenAPI_ue_location_trends_report_item_t *ue_location_trends_report_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_location_trends_report_item) {
        return;
    }
    if (ue_location_trends_report_item->tai) {
        OpenAPI_tai_free(ue_location_trends_report_item->tai);
        ue_location_trends_report_item->tai = NULL;
    }
    if (ue_location_trends_report_item->ran_node_id) {
        OpenAPI_global_ran_node_id_free(ue_location_trends_report_item->ran_node_id);
        ue_location_trends_report_item->ran_node_id = NULL;
    }
    if (ue_location_trends_report_item->ncgi) {
        OpenAPI_ncgi_free(ue_location_trends_report_item->ncgi);
        ue_location_trends_report_item->ncgi = NULL;
    }
    if (ue_location_trends_report_item->ecgi) {
        OpenAPI_ecgi_free(ue_location_trends_report_item->ecgi);
        ue_location_trends_report_item->ecgi = NULL;
    }
    if (ue_location_trends_report_item->n3ga_location) {
        OpenAPI_n3ga_location_free(ue_location_trends_report_item->n3ga_location);
        ue_location_trends_report_item->n3ga_location = NULL;
    }
    if (ue_location_trends_report_item->timestamp) {
        ogs_free(ue_location_trends_report_item->timestamp);
        ue_location_trends_report_item->timestamp = NULL;
    }
    ogs_free(ue_location_trends_report_item);
}

cJSON *OpenAPI_ue_location_trends_report_item_convertToJSON(OpenAPI_ue_location_trends_report_item_t *ue_location_trends_report_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_location_trends_report_item == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [UeLocationTrendsReportItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_location_trends_report_item->tai) {
    cJSON *tai_local_JSON = OpenAPI_tai_convertToJSON(ue_location_trends_report_item->tai);
    if (tai_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [tai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tai", tai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [tai]");
        goto end;
    }
    }

    if (ue_location_trends_report_item->ran_node_id) {
    cJSON *ran_node_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(ue_location_trends_report_item->ran_node_id);
    if (ran_node_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranNodeId", ran_node_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    }

    if (ue_location_trends_report_item->ncgi) {
    cJSON *ncgi_local_JSON = OpenAPI_ncgi_convertToJSON(ue_location_trends_report_item->ncgi);
    if (ncgi_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [ncgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ncgi", ncgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [ncgi]");
        goto end;
    }
    }

    if (ue_location_trends_report_item->ecgi) {
    cJSON *ecgi_local_JSON = OpenAPI_ecgi_convertToJSON(ue_location_trends_report_item->ecgi);
    if (ecgi_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [ecgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgi", ecgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [ecgi]");
        goto end;
    }
    }

    if (ue_location_trends_report_item->n3ga_location) {
    cJSON *n3ga_location_local_JSON = OpenAPI_n3ga_location_convertToJSON(ue_location_trends_report_item->n3ga_location);
    if (n3ga_location_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [n3ga_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n3gaLocation", n3ga_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [n3ga_location]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "spacing", ue_location_trends_report_item->spacing) == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [spacing]");
        goto end;
    }

    if (ue_location_trends_report_item->is_spacing_var) {
    if (cJSON_AddNumberToObject(item, "spacingVar", ue_location_trends_report_item->spacing_var) == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [spacing_var]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "duration", ue_location_trends_report_item->duration) == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [duration]");
        goto end;
    }

    if (ue_location_trends_report_item->is_duration_var) {
    if (cJSON_AddNumberToObject(item, "durationVar", ue_location_trends_report_item->duration_var) == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [duration_var]");
        goto end;
    }
    }

    if (ue_location_trends_report_item->is_occurrences) {
    if (cJSON_AddNumberToObject(item, "occurrences", ue_location_trends_report_item->occurrences) == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [occurrences]");
        goto end;
    }
    }

    if (!ue_location_trends_report_item->timestamp) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [timestamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "timestamp", ue_location_trends_report_item->timestamp) == NULL) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed [timestamp]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ue_location_trends_report_item_t *OpenAPI_ue_location_trends_report_item_parseFromJSON(cJSON *ue_location_trends_report_itemJSON)
{
    OpenAPI_ue_location_trends_report_item_t *ue_location_trends_report_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tai = NULL;
    OpenAPI_tai_t *tai_local_nonprim = NULL;
    cJSON *ran_node_id = NULL;
    OpenAPI_global_ran_node_id_t *ran_node_id_local_nonprim = NULL;
    cJSON *ncgi = NULL;
    OpenAPI_ncgi_t *ncgi_local_nonprim = NULL;
    cJSON *ecgi = NULL;
    OpenAPI_ecgi_t *ecgi_local_nonprim = NULL;
    cJSON *n3ga_location = NULL;
    OpenAPI_n3ga_location_t *n3ga_location_local_nonprim = NULL;
    cJSON *spacing = NULL;
    cJSON *spacing_var = NULL;
    cJSON *duration = NULL;
    cJSON *duration_var = NULL;
    cJSON *occurrences = NULL;
    cJSON *timestamp = NULL;
    tai = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "tai");
    if (tai) {
    tai_local_nonprim = OpenAPI_tai_parseFromJSON(tai);
    if (!tai_local_nonprim) {
        ogs_error("OpenAPI_tai_parseFromJSON failed [tai]");
        goto end;
    }
    }

    ran_node_id = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "ranNodeId");
    if (ran_node_id) {
    ran_node_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id);
    if (!ran_node_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [ran_node_id]");
        goto end;
    }
    }

    ncgi = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "ncgi");
    if (ncgi) {
    ncgi_local_nonprim = OpenAPI_ncgi_parseFromJSON(ncgi);
    if (!ncgi_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [ncgi]");
        goto end;
    }
    }

    ecgi = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "ecgi");
    if (ecgi) {
    ecgi_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi);
    if (!ecgi_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi]");
        goto end;
    }
    }

    n3ga_location = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "n3gaLocation");
    if (n3ga_location) {
    n3ga_location_local_nonprim = OpenAPI_n3ga_location_parseFromJSON(n3ga_location);
    if (!n3ga_location_local_nonprim) {
        ogs_error("OpenAPI_n3ga_location_parseFromJSON failed [n3ga_location]");
        goto end;
    }
    }

    spacing = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "spacing");
    if (!spacing) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [spacing]");
        goto end;
    }
    if (!cJSON_IsNumber(spacing)) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [spacing]");
        goto end;
    }

    spacing_var = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "spacingVar");
    if (spacing_var) {
    if (!cJSON_IsNumber(spacing_var)) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [spacing_var]");
        goto end;
    }
    }

    duration = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "duration");
    if (!duration) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [duration]");
        goto end;
    }
    if (!cJSON_IsNumber(duration)) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [duration]");
        goto end;
    }

    duration_var = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "durationVar");
    if (duration_var) {
    if (!cJSON_IsNumber(duration_var)) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [duration_var]");
        goto end;
    }
    }

    occurrences = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "occurrences");
    if (occurrences) {
    if (!cJSON_IsNumber(occurrences)) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [occurrences]");
        goto end;
    }
    }

    timestamp = cJSON_GetObjectItemCaseSensitive(ue_location_trends_report_itemJSON, "timestamp");
    if (!timestamp) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [timestamp]");
        goto end;
    }
    if (!cJSON_IsString(timestamp) && !cJSON_IsNull(timestamp)) {
        ogs_error("OpenAPI_ue_location_trends_report_item_parseFromJSON() failed [timestamp]");
        goto end;
    }

    ue_location_trends_report_item_local_var = OpenAPI_ue_location_trends_report_item_create (
        tai ? tai_local_nonprim : NULL,
        ran_node_id ? ran_node_id_local_nonprim : NULL,
        ncgi ? ncgi_local_nonprim : NULL,
        ecgi ? ecgi_local_nonprim : NULL,
        n3ga_location ? n3ga_location_local_nonprim : NULL,
        
        spacing->valuedouble,
        spacing_var ? true : false,
        spacing_var ? spacing_var->valuedouble : 0,
        
        duration->valuedouble,
        duration_var ? true : false,
        duration_var ? duration_var->valuedouble : 0,
        occurrences ? true : false,
        occurrences ? occurrences->valuedouble : 0,
        ogs_strdup(timestamp->valuestring)
    );

    return ue_location_trends_report_item_local_var;
end:
    if (tai_local_nonprim) {
        OpenAPI_tai_free(tai_local_nonprim);
        tai_local_nonprim = NULL;
    }
    if (ran_node_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(ran_node_id_local_nonprim);
        ran_node_id_local_nonprim = NULL;
    }
    if (ncgi_local_nonprim) {
        OpenAPI_ncgi_free(ncgi_local_nonprim);
        ncgi_local_nonprim = NULL;
    }
    if (ecgi_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_local_nonprim);
        ecgi_local_nonprim = NULL;
    }
    if (n3ga_location_local_nonprim) {
        OpenAPI_n3ga_location_free(n3ga_location_local_nonprim);
        n3ga_location_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_location_trends_report_item_t *OpenAPI_ue_location_trends_report_item_copy(OpenAPI_ue_location_trends_report_item_t *dst, OpenAPI_ue_location_trends_report_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_location_trends_report_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_location_trends_report_item_convertToJSON() failed");
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

    OpenAPI_ue_location_trends_report_item_free(dst);
    dst = OpenAPI_ue_location_trends_report_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

