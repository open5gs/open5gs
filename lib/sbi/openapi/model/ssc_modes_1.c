
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssc_modes_1.h"

OpenAPI_ssc_modes_1_t *OpenAPI_ssc_modes_1_create(
    OpenAPI_ssc_mode_e default_ssc_mode,
    OpenAPI_list_t *allowed_ssc_modes
)
{
    OpenAPI_ssc_modes_1_t *ssc_modes_1_local_var = ogs_malloc(sizeof(OpenAPI_ssc_modes_1_t));
    ogs_assert(ssc_modes_1_local_var);

    ssc_modes_1_local_var->default_ssc_mode = default_ssc_mode;
    ssc_modes_1_local_var->allowed_ssc_modes = allowed_ssc_modes;

    return ssc_modes_1_local_var;
}

void OpenAPI_ssc_modes_1_free(OpenAPI_ssc_modes_1_t *ssc_modes_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ssc_modes_1) {
        return;
    }
    if (ssc_modes_1->allowed_ssc_modes) {
        OpenAPI_list_free(ssc_modes_1->allowed_ssc_modes);
        ssc_modes_1->allowed_ssc_modes = NULL;
    }
    ogs_free(ssc_modes_1);
}

cJSON *OpenAPI_ssc_modes_1_convertToJSON(OpenAPI_ssc_modes_1_t *ssc_modes_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ssc_modes_1 == NULL) {
        ogs_error("OpenAPI_ssc_modes_1_convertToJSON() failed [SscModes_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ssc_modes_1->default_ssc_mode == OpenAPI_ssc_mode_NULL) {
        ogs_error("OpenAPI_ssc_modes_1_convertToJSON() failed [default_ssc_mode]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "defaultSscMode", OpenAPI_ssc_mode_ToString(ssc_modes_1->default_ssc_mode)) == NULL) {
        ogs_error("OpenAPI_ssc_modes_1_convertToJSON() failed [default_ssc_mode]");
        goto end;
    }

    if (ssc_modes_1->allowed_ssc_modes != OpenAPI_ssc_mode_NULL) {
    cJSON *allowed_ssc_modesList = cJSON_AddArrayToObject(item, "allowedSscModes");
    if (allowed_ssc_modesList == NULL) {
        ogs_error("OpenAPI_ssc_modes_1_convertToJSON() failed [allowed_ssc_modes]");
        goto end;
    }
    OpenAPI_list_for_each(ssc_modes_1->allowed_ssc_modes, node) {
        if (cJSON_AddStringToObject(allowed_ssc_modesList, "", OpenAPI_ssc_mode_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ssc_modes_1_convertToJSON() failed [allowed_ssc_modes]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_ssc_modes_1_t *OpenAPI_ssc_modes_1_parseFromJSON(cJSON *ssc_modes_1JSON)
{
    OpenAPI_ssc_modes_1_t *ssc_modes_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *default_ssc_mode = NULL;
    OpenAPI_ssc_mode_e default_ssc_modeVariable = 0;
    cJSON *allowed_ssc_modes = NULL;
    OpenAPI_list_t *allowed_ssc_modesList = NULL;
    default_ssc_mode = cJSON_GetObjectItemCaseSensitive(ssc_modes_1JSON, "defaultSscMode");
    if (!default_ssc_mode) {
        ogs_error("OpenAPI_ssc_modes_1_parseFromJSON() failed [default_ssc_mode]");
        goto end;
    }
    if (!cJSON_IsString(default_ssc_mode)) {
        ogs_error("OpenAPI_ssc_modes_1_parseFromJSON() failed [default_ssc_mode]");
        goto end;
    }
    default_ssc_modeVariable = OpenAPI_ssc_mode_FromString(default_ssc_mode->valuestring);

    allowed_ssc_modes = cJSON_GetObjectItemCaseSensitive(ssc_modes_1JSON, "allowedSscModes");
    if (allowed_ssc_modes) {
        cJSON *allowed_ssc_modes_local = NULL;
        if (!cJSON_IsArray(allowed_ssc_modes)) {
            ogs_error("OpenAPI_ssc_modes_1_parseFromJSON() failed [allowed_ssc_modes]");
            goto end;
        }

        allowed_ssc_modesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_ssc_modes_local, allowed_ssc_modes) {
            OpenAPI_ssc_mode_e localEnum = OpenAPI_ssc_mode_NULL;
            if (!cJSON_IsString(allowed_ssc_modes_local)) {
                ogs_error("OpenAPI_ssc_modes_1_parseFromJSON() failed [allowed_ssc_modes]");
                goto end;
            }
            localEnum = OpenAPI_ssc_mode_FromString(allowed_ssc_modes_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"allowed_ssc_modes\" is not supported. Ignoring it ...",
                         allowed_ssc_modes_local->valuestring);
            } else {
                OpenAPI_list_add(allowed_ssc_modesList, (void *)localEnum);
            }
        }
        if (allowed_ssc_modesList->count == 0) {
            ogs_error("OpenAPI_ssc_modes_1_parseFromJSON() failed: Expected allowed_ssc_modesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    ssc_modes_1_local_var = OpenAPI_ssc_modes_1_create (
        default_ssc_modeVariable,
        allowed_ssc_modes ? allowed_ssc_modesList : NULL
    );

    return ssc_modes_1_local_var;
end:
    if (allowed_ssc_modesList) {
        OpenAPI_list_free(allowed_ssc_modesList);
        allowed_ssc_modesList = NULL;
    }
    return NULL;
}

OpenAPI_ssc_modes_1_t *OpenAPI_ssc_modes_1_copy(OpenAPI_ssc_modes_1_t *dst, OpenAPI_ssc_modes_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ssc_modes_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ssc_modes_1_convertToJSON() failed");
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

    OpenAPI_ssc_modes_1_free(dst);
    dst = OpenAPI_ssc_modes_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

