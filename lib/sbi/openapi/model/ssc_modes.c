
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ssc_modes.h"

OpenAPI_ssc_modes_t *OpenAPI_ssc_modes_create(
    OpenAPI_ssc_mode_t *default_ssc_mode,
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
    OpenAPI_ssc_mode_free(ssc_modes->default_ssc_mode);
    OpenAPI_list_for_each(ssc_modes->allowed_ssc_modes, node) {
        OpenAPI_ssc_mode_free(node->data);
    }
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
    cJSON *default_ssc_mode_local_JSON = OpenAPI_ssc_mode_convertToJSON(ssc_modes->default_ssc_mode);
    if (default_ssc_mode_local_JSON == NULL) {
        ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [default_ssc_mode]");
        goto end;
    }
    cJSON_AddItemToObject(item, "defaultSscMode", default_ssc_mode_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [default_ssc_mode]");
        goto end;
    }

    if (ssc_modes->allowed_ssc_modes) {
        cJSON *allowed_ssc_modesList = cJSON_AddArrayToObject(item, "allowedSscModes");
        if (allowed_ssc_modesList == NULL) {
            ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [allowed_ssc_modes]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_ssc_modes_node;
        if (ssc_modes->allowed_ssc_modes) {
            OpenAPI_list_for_each(ssc_modes->allowed_ssc_modes, allowed_ssc_modes_node) {
                cJSON *itemLocal = OpenAPI_ssc_mode_convertToJSON(allowed_ssc_modes_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ssc_modes_convertToJSON() failed [allowed_ssc_modes]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_ssc_modesList, itemLocal);
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

    OpenAPI_ssc_mode_t *default_ssc_mode_local_nonprim = NULL;

    default_ssc_mode_local_nonprim = OpenAPI_ssc_mode_parseFromJSON(default_ssc_mode);

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
            if (!cJSON_IsObject(allowed_ssc_modes_local_nonprimitive)) {
                ogs_error("OpenAPI_ssc_modes_parseFromJSON() failed [allowed_ssc_modes]");
                goto end;
            }
            OpenAPI_ssc_mode_t *allowed_ssc_modesItem = OpenAPI_ssc_mode_parseFromJSON(allowed_ssc_modes_local_nonprimitive);

            OpenAPI_list_add(allowed_ssc_modesList, allowed_ssc_modesItem);
        }
    }

    ssc_modes_local_var = OpenAPI_ssc_modes_create (
        default_ssc_mode_local_nonprim,
        allowed_ssc_modes ? allowed_ssc_modesList : NULL
        );

    return ssc_modes_local_var;
end:
    return NULL;
}

