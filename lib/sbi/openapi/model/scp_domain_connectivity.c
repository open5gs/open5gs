
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_domain_connectivity.h"

OpenAPI_scp_domain_connectivity_t *OpenAPI_scp_domain_connectivity_create(
    OpenAPI_list_t *connected_scp_domain_list
)
{
    OpenAPI_scp_domain_connectivity_t *scp_domain_connectivity_local_var = ogs_malloc(sizeof(OpenAPI_scp_domain_connectivity_t));
    ogs_assert(scp_domain_connectivity_local_var);

    scp_domain_connectivity_local_var->connected_scp_domain_list = connected_scp_domain_list;

    return scp_domain_connectivity_local_var;
}

void OpenAPI_scp_domain_connectivity_free(OpenAPI_scp_domain_connectivity_t *scp_domain_connectivity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scp_domain_connectivity) {
        return;
    }
    if (scp_domain_connectivity->connected_scp_domain_list) {
        OpenAPI_list_for_each(scp_domain_connectivity->connected_scp_domain_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_domain_connectivity->connected_scp_domain_list);
        scp_domain_connectivity->connected_scp_domain_list = NULL;
    }
    ogs_free(scp_domain_connectivity);
}

cJSON *OpenAPI_scp_domain_connectivity_convertToJSON(OpenAPI_scp_domain_connectivity_t *scp_domain_connectivity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scp_domain_connectivity == NULL) {
        ogs_error("OpenAPI_scp_domain_connectivity_convertToJSON() failed [ScpDomainConnectivity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!scp_domain_connectivity->connected_scp_domain_list) {
        ogs_error("OpenAPI_scp_domain_connectivity_convertToJSON() failed [connected_scp_domain_list]");
        return NULL;
    }
    cJSON *connected_scp_domain_listList = cJSON_AddArrayToObject(item, "connectedScpDomainList");
    if (connected_scp_domain_listList == NULL) {
        ogs_error("OpenAPI_scp_domain_connectivity_convertToJSON() failed [connected_scp_domain_list]");
        goto end;
    }
    OpenAPI_list_for_each(scp_domain_connectivity->connected_scp_domain_list, node) {
        if (cJSON_AddStringToObject(connected_scp_domain_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_scp_domain_connectivity_convertToJSON() failed [connected_scp_domain_list]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_scp_domain_connectivity_t *OpenAPI_scp_domain_connectivity_parseFromJSON(cJSON *scp_domain_connectivityJSON)
{
    OpenAPI_scp_domain_connectivity_t *scp_domain_connectivity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *connected_scp_domain_list = NULL;
    OpenAPI_list_t *connected_scp_domain_listList = NULL;
    connected_scp_domain_list = cJSON_GetObjectItemCaseSensitive(scp_domain_connectivityJSON, "connectedScpDomainList");
    if (!connected_scp_domain_list) {
        ogs_error("OpenAPI_scp_domain_connectivity_parseFromJSON() failed [connected_scp_domain_list]");
        goto end;
    }
        cJSON *connected_scp_domain_list_local = NULL;
        if (!cJSON_IsArray(connected_scp_domain_list)) {
            ogs_error("OpenAPI_scp_domain_connectivity_parseFromJSON() failed [connected_scp_domain_list]");
            goto end;
        }

        connected_scp_domain_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(connected_scp_domain_list_local, connected_scp_domain_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(connected_scp_domain_list_local)) {
                ogs_error("OpenAPI_scp_domain_connectivity_parseFromJSON() failed [connected_scp_domain_list]");
                goto end;
            }
            OpenAPI_list_add(connected_scp_domain_listList, ogs_strdup(connected_scp_domain_list_local->valuestring));
        }

    scp_domain_connectivity_local_var = OpenAPI_scp_domain_connectivity_create (
        connected_scp_domain_listList
    );

    return scp_domain_connectivity_local_var;
end:
    if (connected_scp_domain_listList) {
        OpenAPI_list_for_each(connected_scp_domain_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(connected_scp_domain_listList);
        connected_scp_domain_listList = NULL;
    }
    return NULL;
}

OpenAPI_scp_domain_connectivity_t *OpenAPI_scp_domain_connectivity_copy(OpenAPI_scp_domain_connectivity_t *dst, OpenAPI_scp_domain_connectivity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scp_domain_connectivity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scp_domain_connectivity_convertToJSON() failed");
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

    OpenAPI_scp_domain_connectivity_free(dst);
    dst = OpenAPI_scp_domain_connectivity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

