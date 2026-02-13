
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gnss_positioning_method_and_usage.h"

OpenAPI_gnss_positioning_method_and_usage_t *OpenAPI_gnss_positioning_method_and_usage_create(
    OpenAPI_positioning_mode_t *mode,
    OpenAPI_gnss_id_t *gnss,
    OpenAPI_usage_t *usage
)
{
    OpenAPI_gnss_positioning_method_and_usage_t *gnss_positioning_method_and_usage_local_var = ogs_malloc(sizeof(OpenAPI_gnss_positioning_method_and_usage_t));
    ogs_assert(gnss_positioning_method_and_usage_local_var);

    gnss_positioning_method_and_usage_local_var->mode = mode;
    gnss_positioning_method_and_usage_local_var->gnss = gnss;
    gnss_positioning_method_and_usage_local_var->usage = usage;

    return gnss_positioning_method_and_usage_local_var;
}

void OpenAPI_gnss_positioning_method_and_usage_free(OpenAPI_gnss_positioning_method_and_usage_t *gnss_positioning_method_and_usage)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gnss_positioning_method_and_usage) {
        return;
    }
    if (gnss_positioning_method_and_usage->mode) {
        OpenAPI_positioning_mode_free(gnss_positioning_method_and_usage->mode);
        gnss_positioning_method_and_usage->mode = NULL;
    }
    if (gnss_positioning_method_and_usage->gnss) {
        OpenAPI_gnss_id_free(gnss_positioning_method_and_usage->gnss);
        gnss_positioning_method_and_usage->gnss = NULL;
    }
    if (gnss_positioning_method_and_usage->usage) {
        OpenAPI_usage_free(gnss_positioning_method_and_usage->usage);
        gnss_positioning_method_and_usage->usage = NULL;
    }
    ogs_free(gnss_positioning_method_and_usage);
}

cJSON *OpenAPI_gnss_positioning_method_and_usage_convertToJSON(OpenAPI_gnss_positioning_method_and_usage_t *gnss_positioning_method_and_usage)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gnss_positioning_method_and_usage == NULL) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [GnssPositioningMethodAndUsage]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!gnss_positioning_method_and_usage->mode) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [mode]");
        return NULL;
    }
    cJSON *mode_local_JSON = OpenAPI_positioning_mode_convertToJSON(gnss_positioning_method_and_usage->mode);
    if (mode_local_JSON == NULL) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [mode]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mode", mode_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [mode]");
        goto end;
    }

    if (!gnss_positioning_method_and_usage->gnss) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [gnss]");
        return NULL;
    }
    cJSON *gnss_local_JSON = OpenAPI_gnss_id_convertToJSON(gnss_positioning_method_and_usage->gnss);
    if (gnss_local_JSON == NULL) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [gnss]");
        goto end;
    }
    cJSON_AddItemToObject(item, "gnss", gnss_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [gnss]");
        goto end;
    }

    if (!gnss_positioning_method_and_usage->usage) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [usage]");
        return NULL;
    }
    cJSON *usage_local_JSON = OpenAPI_usage_convertToJSON(gnss_positioning_method_and_usage->usage);
    if (usage_local_JSON == NULL) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [usage]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usage", usage_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed [usage]");
        goto end;
    }

end:
    return item;
}

OpenAPI_gnss_positioning_method_and_usage_t *OpenAPI_gnss_positioning_method_and_usage_parseFromJSON(cJSON *gnss_positioning_method_and_usageJSON)
{
    OpenAPI_gnss_positioning_method_and_usage_t *gnss_positioning_method_and_usage_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mode = NULL;
    OpenAPI_positioning_mode_t *mode_local_nonprim = NULL;
    cJSON *gnss = NULL;
    OpenAPI_gnss_id_t *gnss_local_nonprim = NULL;
    cJSON *usage = NULL;
    OpenAPI_usage_t *usage_local_nonprim = NULL;
    mode = cJSON_GetObjectItemCaseSensitive(gnss_positioning_method_and_usageJSON, "mode");
    if (!mode) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_parseFromJSON() failed [mode]");
        goto end;
    }
    mode_local_nonprim = OpenAPI_positioning_mode_parseFromJSON(mode);
    if (!mode_local_nonprim) {
        ogs_error("OpenAPI_positioning_mode_parseFromJSON failed [mode]");
        goto end;
    }

    gnss = cJSON_GetObjectItemCaseSensitive(gnss_positioning_method_and_usageJSON, "gnss");
    if (!gnss) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_parseFromJSON() failed [gnss]");
        goto end;
    }
    gnss_local_nonprim = OpenAPI_gnss_id_parseFromJSON(gnss);
    if (!gnss_local_nonprim) {
        ogs_error("OpenAPI_gnss_id_parseFromJSON failed [gnss]");
        goto end;
    }

    usage = cJSON_GetObjectItemCaseSensitive(gnss_positioning_method_and_usageJSON, "usage");
    if (!usage) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_parseFromJSON() failed [usage]");
        goto end;
    }
    usage_local_nonprim = OpenAPI_usage_parseFromJSON(usage);
    if (!usage_local_nonprim) {
        ogs_error("OpenAPI_usage_parseFromJSON failed [usage]");
        goto end;
    }

    gnss_positioning_method_and_usage_local_var = OpenAPI_gnss_positioning_method_and_usage_create (
        mode_local_nonprim,
        gnss_local_nonprim,
        usage_local_nonprim
    );

    return gnss_positioning_method_and_usage_local_var;
end:
    if (mode_local_nonprim) {
        OpenAPI_positioning_mode_free(mode_local_nonprim);
        mode_local_nonprim = NULL;
    }
    if (gnss_local_nonprim) {
        OpenAPI_gnss_id_free(gnss_local_nonprim);
        gnss_local_nonprim = NULL;
    }
    if (usage_local_nonprim) {
        OpenAPI_usage_free(usage_local_nonprim);
        usage_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_gnss_positioning_method_and_usage_t *OpenAPI_gnss_positioning_method_and_usage_copy(OpenAPI_gnss_positioning_method_and_usage_t *dst, OpenAPI_gnss_positioning_method_and_usage_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gnss_positioning_method_and_usage_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gnss_positioning_method_and_usage_convertToJSON() failed");
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

    OpenAPI_gnss_positioning_method_and_usage_free(dst);
    dst = OpenAPI_gnss_positioning_method_and_usage_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

