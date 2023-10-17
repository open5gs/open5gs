
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "suci_info.h"

OpenAPI_suci_info_t *OpenAPI_suci_info_create(
    OpenAPI_list_t *routing_inds,
    OpenAPI_list_t *h_nw_pub_key_ids
)
{
    OpenAPI_suci_info_t *suci_info_local_var = ogs_malloc(sizeof(OpenAPI_suci_info_t));
    ogs_assert(suci_info_local_var);

    suci_info_local_var->routing_inds = routing_inds;
    suci_info_local_var->h_nw_pub_key_ids = h_nw_pub_key_ids;

    return suci_info_local_var;
}

void OpenAPI_suci_info_free(OpenAPI_suci_info_t *suci_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == suci_info) {
        return;
    }
    if (suci_info->routing_inds) {
        OpenAPI_list_for_each(suci_info->routing_inds, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(suci_info->routing_inds);
        suci_info->routing_inds = NULL;
    }
    if (suci_info->h_nw_pub_key_ids) {
        OpenAPI_list_for_each(suci_info->h_nw_pub_key_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(suci_info->h_nw_pub_key_ids);
        suci_info->h_nw_pub_key_ids = NULL;
    }
    ogs_free(suci_info);
}

cJSON *OpenAPI_suci_info_convertToJSON(OpenAPI_suci_info_t *suci_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (suci_info == NULL) {
        ogs_error("OpenAPI_suci_info_convertToJSON() failed [SuciInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (suci_info->routing_inds) {
    cJSON *routing_indsList = cJSON_AddArrayToObject(item, "routingInds");
    if (routing_indsList == NULL) {
        ogs_error("OpenAPI_suci_info_convertToJSON() failed [routing_inds]");
        goto end;
    }
    OpenAPI_list_for_each(suci_info->routing_inds, node) {
        if (cJSON_AddStringToObject(routing_indsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_suci_info_convertToJSON() failed [routing_inds]");
            goto end;
        }
    }
    }

    if (suci_info->h_nw_pub_key_ids) {
    cJSON *h_nw_pub_key_idsList = cJSON_AddArrayToObject(item, "hNwPubKeyIds");
    if (h_nw_pub_key_idsList == NULL) {
        ogs_error("OpenAPI_suci_info_convertToJSON() failed [h_nw_pub_key_ids]");
        goto end;
    }
    OpenAPI_list_for_each(suci_info->h_nw_pub_key_ids, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_suci_info_convertToJSON() failed [h_nw_pub_key_ids]");
            goto end;
        }
        if (cJSON_AddNumberToObject(h_nw_pub_key_idsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_suci_info_convertToJSON() failed [h_nw_pub_key_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_suci_info_t *OpenAPI_suci_info_parseFromJSON(cJSON *suci_infoJSON)
{
    OpenAPI_suci_info_t *suci_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *routing_inds = NULL;
    OpenAPI_list_t *routing_indsList = NULL;
    cJSON *h_nw_pub_key_ids = NULL;
    OpenAPI_list_t *h_nw_pub_key_idsList = NULL;
    routing_inds = cJSON_GetObjectItemCaseSensitive(suci_infoJSON, "routingInds");
    if (routing_inds) {
        cJSON *routing_inds_local = NULL;
        if (!cJSON_IsArray(routing_inds)) {
            ogs_error("OpenAPI_suci_info_parseFromJSON() failed [routing_inds]");
            goto end;
        }

        routing_indsList = OpenAPI_list_create();

        cJSON_ArrayForEach(routing_inds_local, routing_inds) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(routing_inds_local)) {
                ogs_error("OpenAPI_suci_info_parseFromJSON() failed [routing_inds]");
                goto end;
            }
            OpenAPI_list_add(routing_indsList, ogs_strdup(routing_inds_local->valuestring));
        }
    }

    h_nw_pub_key_ids = cJSON_GetObjectItemCaseSensitive(suci_infoJSON, "hNwPubKeyIds");
    if (h_nw_pub_key_ids) {
        cJSON *h_nw_pub_key_ids_local = NULL;
        if (!cJSON_IsArray(h_nw_pub_key_ids)) {
            ogs_error("OpenAPI_suci_info_parseFromJSON() failed [h_nw_pub_key_ids]");
            goto end;
        }

        h_nw_pub_key_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(h_nw_pub_key_ids_local, h_nw_pub_key_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(h_nw_pub_key_ids_local)) {
                ogs_error("OpenAPI_suci_info_parseFromJSON() failed [h_nw_pub_key_ids]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_suci_info_parseFromJSON() failed [h_nw_pub_key_ids]");
                goto end;
            }
            *localDouble = h_nw_pub_key_ids_local->valuedouble;
            OpenAPI_list_add(h_nw_pub_key_idsList, localDouble);
        }
    }

    suci_info_local_var = OpenAPI_suci_info_create (
        routing_inds ? routing_indsList : NULL,
        h_nw_pub_key_ids ? h_nw_pub_key_idsList : NULL
    );

    return suci_info_local_var;
end:
    if (routing_indsList) {
        OpenAPI_list_for_each(routing_indsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(routing_indsList);
        routing_indsList = NULL;
    }
    if (h_nw_pub_key_idsList) {
        OpenAPI_list_for_each(h_nw_pub_key_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(h_nw_pub_key_idsList);
        h_nw_pub_key_idsList = NULL;
    }
    return NULL;
}

OpenAPI_suci_info_t *OpenAPI_suci_info_copy(OpenAPI_suci_info_t *dst, OpenAPI_suci_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_suci_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_suci_info_convertToJSON() failed");
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

    OpenAPI_suci_info_free(dst);
    dst = OpenAPI_suci_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

