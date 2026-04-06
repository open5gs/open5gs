
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rate_limit_repo.h"

OpenAPI_rate_limit_repo_t *OpenAPI_rate_limit_repo_create(
    OpenAPI_list_t *rate_limit_repo_ul,
    OpenAPI_list_t *rate_limit_repo_dl,
    OpenAPI_list_t *flows
)
{
    OpenAPI_rate_limit_repo_t *rate_limit_repo_local_var = ogs_malloc(sizeof(OpenAPI_rate_limit_repo_t));
    ogs_assert(rate_limit_repo_local_var);

    rate_limit_repo_local_var->rate_limit_repo_ul = rate_limit_repo_ul;
    rate_limit_repo_local_var->rate_limit_repo_dl = rate_limit_repo_dl;
    rate_limit_repo_local_var->flows = flows;

    return rate_limit_repo_local_var;
}

void OpenAPI_rate_limit_repo_free(OpenAPI_rate_limit_repo_t *rate_limit_repo)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rate_limit_repo) {
        return;
    }
    if (rate_limit_repo->rate_limit_repo_ul) {
        OpenAPI_list_for_each(rate_limit_repo->rate_limit_repo_ul, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rate_limit_repo->rate_limit_repo_ul);
        rate_limit_repo->rate_limit_repo_ul = NULL;
    }
    if (rate_limit_repo->rate_limit_repo_dl) {
        OpenAPI_list_for_each(rate_limit_repo->rate_limit_repo_dl, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rate_limit_repo->rate_limit_repo_dl);
        rate_limit_repo->rate_limit_repo_dl = NULL;
    }
    if (rate_limit_repo->flows) {
        OpenAPI_list_for_each(rate_limit_repo->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(rate_limit_repo->flows);
        rate_limit_repo->flows = NULL;
    }
    ogs_free(rate_limit_repo);
}

cJSON *OpenAPI_rate_limit_repo_convertToJSON(OpenAPI_rate_limit_repo_t *rate_limit_repo)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rate_limit_repo == NULL) {
        ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed [RateLimitRepo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rate_limit_repo->rate_limit_repo_ul) {
    cJSON *rate_limit_repo_ulList = cJSON_AddArrayToObject(item, "rateLimitRepoUl");
    if (rate_limit_repo_ulList == NULL) {
        ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed [rate_limit_repo_ul]");
        goto end;
    }
    OpenAPI_list_for_each(rate_limit_repo->rate_limit_repo_ul, node) {
        if (cJSON_AddStringToObject(rate_limit_repo_ulList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed [rate_limit_repo_ul]");
            goto end;
        }
    }
    }

    if (rate_limit_repo->rate_limit_repo_dl) {
    cJSON *rate_limit_repo_dlList = cJSON_AddArrayToObject(item, "rateLimitRepoDl");
    if (rate_limit_repo_dlList == NULL) {
        ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed [rate_limit_repo_dl]");
        goto end;
    }
    OpenAPI_list_for_each(rate_limit_repo->rate_limit_repo_dl, node) {
        if (cJSON_AddStringToObject(rate_limit_repo_dlList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed [rate_limit_repo_dl]");
            goto end;
        }
    }
    }

    if (rate_limit_repo->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(rate_limit_repo->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_rate_limit_repo_t *OpenAPI_rate_limit_repo_parseFromJSON(cJSON *rate_limit_repoJSON)
{
    OpenAPI_rate_limit_repo_t *rate_limit_repo_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rate_limit_repo_ul = NULL;
    OpenAPI_list_t *rate_limit_repo_ulList = NULL;
    cJSON *rate_limit_repo_dl = NULL;
    OpenAPI_list_t *rate_limit_repo_dlList = NULL;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    rate_limit_repo_ul = cJSON_GetObjectItemCaseSensitive(rate_limit_repoJSON, "rateLimitRepoUl");
    if (rate_limit_repo_ul) {
        cJSON *rate_limit_repo_ul_local = NULL;
        if (!cJSON_IsArray(rate_limit_repo_ul)) {
            ogs_error("OpenAPI_rate_limit_repo_parseFromJSON() failed [rate_limit_repo_ul]");
            goto end;
        }

        rate_limit_repo_ulList = OpenAPI_list_create();

        cJSON_ArrayForEach(rate_limit_repo_ul_local, rate_limit_repo_ul) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(rate_limit_repo_ul_local)) {
                ogs_error("OpenAPI_rate_limit_repo_parseFromJSON() failed [rate_limit_repo_ul]");
                goto end;
            }
            OpenAPI_list_add(rate_limit_repo_ulList, ogs_strdup(rate_limit_repo_ul_local->valuestring));
        }
    }

    rate_limit_repo_dl = cJSON_GetObjectItemCaseSensitive(rate_limit_repoJSON, "rateLimitRepoDl");
    if (rate_limit_repo_dl) {
        cJSON *rate_limit_repo_dl_local = NULL;
        if (!cJSON_IsArray(rate_limit_repo_dl)) {
            ogs_error("OpenAPI_rate_limit_repo_parseFromJSON() failed [rate_limit_repo_dl]");
            goto end;
        }

        rate_limit_repo_dlList = OpenAPI_list_create();

        cJSON_ArrayForEach(rate_limit_repo_dl_local, rate_limit_repo_dl) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(rate_limit_repo_dl_local)) {
                ogs_error("OpenAPI_rate_limit_repo_parseFromJSON() failed [rate_limit_repo_dl]");
                goto end;
            }
            OpenAPI_list_add(rate_limit_repo_dlList, ogs_strdup(rate_limit_repo_dl_local->valuestring));
        }
    }

    flows = cJSON_GetObjectItemCaseSensitive(rate_limit_repoJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_rate_limit_repo_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_rate_limit_repo_parseFromJSON() failed [flows]");
                goto end;
            }
            OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local);
            if (!flowsItem) {
                ogs_error("No flowsItem");
                goto end;
            }
            OpenAPI_list_add(flowsList, flowsItem);
        }
    }

    rate_limit_repo_local_var = OpenAPI_rate_limit_repo_create (
        rate_limit_repo_ul ? rate_limit_repo_ulList : NULL,
        rate_limit_repo_dl ? rate_limit_repo_dlList : NULL,
        flows ? flowsList : NULL
    );

    return rate_limit_repo_local_var;
end:
    if (rate_limit_repo_ulList) {
        OpenAPI_list_for_each(rate_limit_repo_ulList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rate_limit_repo_ulList);
        rate_limit_repo_ulList = NULL;
    }
    if (rate_limit_repo_dlList) {
        OpenAPI_list_for_each(rate_limit_repo_dlList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rate_limit_repo_dlList);
        rate_limit_repo_dlList = NULL;
    }
    if (flowsList) {
        OpenAPI_list_for_each(flowsList, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(flowsList);
        flowsList = NULL;
    }
    return NULL;
}

OpenAPI_rate_limit_repo_t *OpenAPI_rate_limit_repo_copy(OpenAPI_rate_limit_repo_t *dst, OpenAPI_rate_limit_repo_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rate_limit_repo_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rate_limit_repo_convertToJSON() failed");
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

    OpenAPI_rate_limit_repo_free(dst);
    dst = OpenAPI_rate_limit_repo_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

