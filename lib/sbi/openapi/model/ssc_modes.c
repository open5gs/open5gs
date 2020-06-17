
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssc_modes.h"

OpenAPI_ssc_modes_t *OpenAPI_ssc_modes_create(
    OpenAPI_ssc_mode_e default_ssc_mode,
    OpenAPI_list_t *allowed_ssc_modes
    )
{
    OpenAPI_ssc_modes_t *ssc_modes_local_var = OpenAPI_malloc(sizeof(OpenAPI_ssc_modes_t));
    if (!ssc_modes_local_var) {
        return NULL;
    }
    ssc_modes_local_var->default_ssc_mode = default_ssc_mode;
    ssc_modes_local_var->allowed_ssc_modes = allowed_ssc_modes;

    return ssc_modes_local_var;
}

void OpenAPI_ssc_modes_free(OpenAPI_ssc_modes_t *ssc_modes)
{
    if (NULL == ssc_modes) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(ssc_modes->allowed_ssc_modes);
    ogs_free(ssc_modes);
}

cJSON *OpenAPI_ssc_modes_convertToJSON(OpenAPI_ssc_modes_t *ssc_modes)
{
    cJSON *item = NULL;

    if (ssc_modes == NULL) {
        ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [SscModes]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ssc_modes->default_ssc_mode) {
        ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [default_ssc_mode]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "defaultSscMode", OpenAPI_ssc_mode_ToString(ssc_modes->default_ssc_mode)) == NULL) {
        ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [default_ssc_mode]");
        goto end;
    }

    if (ssc_modes->allowed_ssc_modes) {
        cJSON *allowed_ssc_modes = cJSON_AddArrayToObject(item, "allowedSscModes");
        if (allowed_ssc_modes == NULL) {
            ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [allowed_ssc_modes]");
            goto end;
        }
        OpenAPI_lnode_t *allowed_ssc_modes_node;
        OpenAPI_list_for_each(ssc_modes->allowed_ssc_modes, allowed_ssc_modes_node) {
            if (cJSON_AddStringToObject(allowed_ssc_modes, "", OpenAPI_ssc_mode_ToString((OpenAPI_ssc_mode_e)allowed_ssc_modes_node->data)) == NULL) {
                ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [allowed_ssc_modes]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_ssc_modes_t *OpenAPI_ssc_modes_parseFromJSON(cJSON *ssc_modesJSON)
{
    OpenAPI_ssc_modes_t *ssc_modes_local_var = NULL;
    cJSON *default_ssc_mode = cJSON_GetObjectItemCaseSensitive(ssc_modesJSON, "defaultSscMode");
    if (!default_ssc_mode) {
        ogs_error("OpenAPI_ssc_modes_parseFromJSON() failed [default_ssc_mode]");
        goto end;
    }

    OpenAPI_ssc_mode_e default_ssc_modeVariable;

    if (!cJSON_IsString(default_ssc_mode)) {
        ogs_error("OpenAPI_ssc_modes_parseFromJSON() failed [default_ssc_mode]");
        goto end;
    }
    default_ssc_modeVariable = OpenAPI_ssc_mode_FromString(default_ssc_mode->valuestring);

    cJSON *allowed_ssc_modes = cJSON_GetObjectItemCaseSensitive(ssc_modesJSON, "allowedSscModes");

    OpenAPI_list_t *allowed_ssc_modesList;
    if (allowed_ssc_modes) {
        cJSON *allowed_ssc_modes_local_nonprimitive;
        if (!cJSON_IsArray(allowed_ssc_modes)) {
            ogs_error("OpenAPI_ssc_modes_parseFromJSON() failed [allowed_ssc_modes]");
            goto end;
        }

        allowed_ssc_modesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_ssc_modes_local_nonprimitive, allowed_ssc_modes ) {
            if (!cJSON_IsString(allowed_ssc_modes_local_nonprimitive)) {
                ogs_error("OpenAPI_ssc_modes_parseFromJSON() failed [allowed_ssc_modes]");
                goto end;
            }

            OpenAPI_list_add(allowed_ssc_modesList, (void *)OpenAPI_ssc_mode_FromString(allowed_ssc_modes_local_nonprimitive->valuestring));
        }
    }

    ssc_modes_local_var = OpenAPI_ssc_modes_create (
        default_ssc_modeVariable,
        allowed_ssc_modes ? allowed_ssc_modesList : NULL
        );

    return ssc_modes_local_var;
end:
    return NULL;
}

OpenAPI_ssc_modes_t *OpenAPI_ssc_modes_copy(OpenAPI_ssc_modes_t *dst, OpenAPI_ssc_modes_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ssc_modes_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ssc_modes_convertToJSON() failed");
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

    OpenAPI_ssc_modes_free(dst);
    dst = OpenAPI_ssc_modes_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

