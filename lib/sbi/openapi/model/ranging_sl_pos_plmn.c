
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_pos_plmn.h"

OpenAPI_ranging_sl_pos_plmn_t *OpenAPI_ranging_sl_pos_plmn_create(
    OpenAPI_plmn_id_t *ranging_sl_pos_plmn,
    OpenAPI_list_t *ranging_sl_pos_allowed
)
{
    OpenAPI_ranging_sl_pos_plmn_t *ranging_sl_pos_plmn_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_pos_plmn_t));
    ogs_assert(ranging_sl_pos_plmn_local_var);

    ranging_sl_pos_plmn_local_var->ranging_sl_pos_plmn = ranging_sl_pos_plmn;
    ranging_sl_pos_plmn_local_var->ranging_sl_pos_allowed = ranging_sl_pos_allowed;

    return ranging_sl_pos_plmn_local_var;
}

void OpenAPI_ranging_sl_pos_plmn_free(OpenAPI_ranging_sl_pos_plmn_t *ranging_sl_pos_plmn)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_pos_plmn) {
        return;
    }
    if (ranging_sl_pos_plmn->ranging_sl_pos_plmn) {
        OpenAPI_plmn_id_free(ranging_sl_pos_plmn->ranging_sl_pos_plmn);
        ranging_sl_pos_plmn->ranging_sl_pos_plmn = NULL;
    }
    if (ranging_sl_pos_plmn->ranging_sl_pos_allowed) {
        OpenAPI_list_free(ranging_sl_pos_plmn->ranging_sl_pos_allowed);
        ranging_sl_pos_plmn->ranging_sl_pos_allowed = NULL;
    }
    ogs_free(ranging_sl_pos_plmn);
}

cJSON *OpenAPI_ranging_sl_pos_plmn_convertToJSON(OpenAPI_ranging_sl_pos_plmn_t *ranging_sl_pos_plmn)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_pos_plmn == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_plmn_convertToJSON() failed [RangingSlPosPlmn]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ranging_sl_pos_plmn->ranging_sl_pos_plmn) {
        ogs_error("OpenAPI_ranging_sl_pos_plmn_convertToJSON() failed [ranging_sl_pos_plmn]");
        return NULL;
    }
    cJSON *ranging_sl_pos_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(ranging_sl_pos_plmn->ranging_sl_pos_plmn);
    if (ranging_sl_pos_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_plmn_convertToJSON() failed [ranging_sl_pos_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlPosPlmn", ranging_sl_pos_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_plmn_convertToJSON() failed [ranging_sl_pos_plmn]");
        goto end;
    }

    if (ranging_sl_pos_plmn->ranging_sl_pos_allowed != OpenAPI_ranging_sl_pos_allowed_NULL) {
    cJSON *ranging_sl_pos_allowedList = cJSON_AddArrayToObject(item, "rangingSlPosAllowed");
    if (ranging_sl_pos_allowedList == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_plmn_convertToJSON() failed [ranging_sl_pos_allowed]");
        goto end;
    }
    OpenAPI_list_for_each(ranging_sl_pos_plmn->ranging_sl_pos_allowed, node) {
        if (cJSON_AddStringToObject(ranging_sl_pos_allowedList, "", OpenAPI_ranging_sl_pos_allowed_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ranging_sl_pos_plmn_convertToJSON() failed [ranging_sl_pos_allowed]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_pos_plmn_t *OpenAPI_ranging_sl_pos_plmn_parseFromJSON(cJSON *ranging_sl_pos_plmnJSON)
{
    OpenAPI_ranging_sl_pos_plmn_t *ranging_sl_pos_plmn_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ranging_sl_pos_plmn = NULL;
    OpenAPI_plmn_id_t *ranging_sl_pos_plmn_local_nonprim = NULL;
    cJSON *ranging_sl_pos_allowed = NULL;
    OpenAPI_list_t *ranging_sl_pos_allowedList = NULL;
    ranging_sl_pos_plmn = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_plmnJSON, "rangingSlPosPlmn");
    if (!ranging_sl_pos_plmn) {
        ogs_error("OpenAPI_ranging_sl_pos_plmn_parseFromJSON() failed [ranging_sl_pos_plmn]");
        goto end;
    }
    ranging_sl_pos_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(ranging_sl_pos_plmn);
    if (!ranging_sl_pos_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [ranging_sl_pos_plmn]");
        goto end;
    }

    ranging_sl_pos_allowed = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_plmnJSON, "rangingSlPosAllowed");
    if (ranging_sl_pos_allowed) {
        cJSON *ranging_sl_pos_allowed_local = NULL;
        if (!cJSON_IsArray(ranging_sl_pos_allowed)) {
            ogs_error("OpenAPI_ranging_sl_pos_plmn_parseFromJSON() failed [ranging_sl_pos_allowed]");
            goto end;
        }

        ranging_sl_pos_allowedList = OpenAPI_list_create();

        cJSON_ArrayForEach(ranging_sl_pos_allowed_local, ranging_sl_pos_allowed) {
            OpenAPI_ranging_sl_pos_allowed_e localEnum = OpenAPI_ranging_sl_pos_allowed_NULL;
            if (!cJSON_IsString(ranging_sl_pos_allowed_local)) {
                ogs_error("OpenAPI_ranging_sl_pos_plmn_parseFromJSON() failed [ranging_sl_pos_allowed]");
                goto end;
            }
            localEnum = OpenAPI_ranging_sl_pos_allowed_FromString(ranging_sl_pos_allowed_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ranging_sl_pos_allowed\" is not supported. Ignoring it ...",
                         ranging_sl_pos_allowed_local->valuestring);
            } else {
                OpenAPI_list_add(ranging_sl_pos_allowedList, (void *)localEnum);
            }
        }
        if (ranging_sl_pos_allowedList->count == 0) {
            ogs_error("OpenAPI_ranging_sl_pos_plmn_parseFromJSON() failed: Expected ranging_sl_pos_allowedList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    ranging_sl_pos_plmn_local_var = OpenAPI_ranging_sl_pos_plmn_create (
        ranging_sl_pos_plmn_local_nonprim,
        ranging_sl_pos_allowed ? ranging_sl_pos_allowedList : NULL
    );

    return ranging_sl_pos_plmn_local_var;
end:
    if (ranging_sl_pos_plmn_local_nonprim) {
        OpenAPI_plmn_id_free(ranging_sl_pos_plmn_local_nonprim);
        ranging_sl_pos_plmn_local_nonprim = NULL;
    }
    if (ranging_sl_pos_allowedList) {
        OpenAPI_list_free(ranging_sl_pos_allowedList);
        ranging_sl_pos_allowedList = NULL;
    }
    return NULL;
}

OpenAPI_ranging_sl_pos_plmn_t *OpenAPI_ranging_sl_pos_plmn_copy(OpenAPI_ranging_sl_pos_plmn_t *dst, OpenAPI_ranging_sl_pos_plmn_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_pos_plmn_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_pos_plmn_convertToJSON() failed");
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

    OpenAPI_ranging_sl_pos_plmn_free(dst);
    dst = OpenAPI_ranging_sl_pos_plmn_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

