
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_data_scope.h"

OpenAPI_usage_mon_data_scope_t *OpenAPI_usage_mon_data_scope_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnn
    )
{
    OpenAPI_usage_mon_data_scope_t *usage_mon_data_scope_local_var = OpenAPI_malloc(sizeof(OpenAPI_usage_mon_data_scope_t));
    if (!usage_mon_data_scope_local_var) {
        return NULL;
    }
    usage_mon_data_scope_local_var->snssai = snssai;
    usage_mon_data_scope_local_var->dnn = dnn;

    return usage_mon_data_scope_local_var;
}

void OpenAPI_usage_mon_data_scope_free(OpenAPI_usage_mon_data_scope_t *usage_mon_data_scope)
{
    if (NULL == usage_mon_data_scope) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_snssai_free(usage_mon_data_scope->snssai);
    OpenAPI_list_for_each(usage_mon_data_scope->dnn, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(usage_mon_data_scope->dnn);
    ogs_free(usage_mon_data_scope);
}

cJSON *OpenAPI_usage_mon_data_scope_convertToJSON(OpenAPI_usage_mon_data_scope_t *usage_mon_data_scope)
{
    cJSON *item = NULL;

    if (usage_mon_data_scope == NULL) {
        ogs_error("OpenAPI_usage_mon_data_scope_convertToJSON() failed [UsageMonDataScope]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!usage_mon_data_scope->snssai) {
        ogs_error("OpenAPI_usage_mon_data_scope_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(usage_mon_data_scope->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_usage_mon_data_scope_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_usage_mon_data_scope_convertToJSON() failed [snssai]");
        goto end;
    }

    if (usage_mon_data_scope->dnn) {
        cJSON *dnn = cJSON_AddArrayToObject(item, "dnn");
        if (dnn == NULL) {
            ogs_error("OpenAPI_usage_mon_data_scope_convertToJSON() failed [dnn]");
            goto end;
        }

        OpenAPI_lnode_t *dnn_node;
        OpenAPI_list_for_each(usage_mon_data_scope->dnn, dnn_node)  {
            if (cJSON_AddStringToObject(dnn, "", (char*)dnn_node->data) == NULL) {
                ogs_error("OpenAPI_usage_mon_data_scope_convertToJSON() failed [dnn]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_usage_mon_data_scope_t *OpenAPI_usage_mon_data_scope_parseFromJSON(cJSON *usage_mon_data_scopeJSON)
{
    OpenAPI_usage_mon_data_scope_t *usage_mon_data_scope_local_var = NULL;
    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(usage_mon_data_scopeJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_usage_mon_data_scope_parseFromJSON() failed [snssai]");
        goto end;
    }

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;

    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(usage_mon_data_scopeJSON, "dnn");

    OpenAPI_list_t *dnnList;
    if (dnn) {
        cJSON *dnn_local;
        if (!cJSON_IsArray(dnn)) {
            ogs_error("OpenAPI_usage_mon_data_scope_parseFromJSON() failed [dnn]");
            goto end;
        }
        dnnList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_local, dnn) {
            if (!cJSON_IsString(dnn_local)) {
                ogs_error("OpenAPI_usage_mon_data_scope_parseFromJSON() failed [dnn]");
                goto end;
            }
            OpenAPI_list_add(dnnList, ogs_strdup(dnn_local->valuestring));
        }
    }

    usage_mon_data_scope_local_var = OpenAPI_usage_mon_data_scope_create (
        snssai_local_nonprim,
        dnn ? dnnList : NULL
        );

    return usage_mon_data_scope_local_var;
end:
    return NULL;
}

OpenAPI_usage_mon_data_scope_t *OpenAPI_usage_mon_data_scope_copy(OpenAPI_usage_mon_data_scope_t *dst, OpenAPI_usage_mon_data_scope_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_mon_data_scope_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_mon_data_scope_convertToJSON() failed");
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

    OpenAPI_usage_mon_data_scope_free(dst);
    dst = OpenAPI_usage_mon_data_scope_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

