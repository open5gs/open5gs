
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_content.h"

OpenAPI_pfd_content_t *OpenAPI_pfd_content_create(
    char *pfd_id,
    OpenAPI_list_t *flow_descriptions,
    OpenAPI_list_t *urls,
    OpenAPI_list_t *domain_names,
    OpenAPI_domain_name_protocol_t *dn_protocol
    )
{
    OpenAPI_pfd_content_t *pfd_content_local_var = OpenAPI_malloc(sizeof(OpenAPI_pfd_content_t));
    if (!pfd_content_local_var) {
        return NULL;
    }
    pfd_content_local_var->pfd_id = pfd_id;
    pfd_content_local_var->flow_descriptions = flow_descriptions;
    pfd_content_local_var->urls = urls;
    pfd_content_local_var->domain_names = domain_names;
    pfd_content_local_var->dn_protocol = dn_protocol;

    return pfd_content_local_var;
}

void OpenAPI_pfd_content_free(OpenAPI_pfd_content_t *pfd_content)
{
    if (NULL == pfd_content) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pfd_content->pfd_id);
    OpenAPI_list_for_each(pfd_content->flow_descriptions, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pfd_content->flow_descriptions);
    OpenAPI_list_for_each(pfd_content->urls, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pfd_content->urls);
    OpenAPI_list_for_each(pfd_content->domain_names, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pfd_content->domain_names);
    OpenAPI_domain_name_protocol_free(pfd_content->dn_protocol);
    ogs_free(pfd_content);
}

cJSON *OpenAPI_pfd_content_convertToJSON(OpenAPI_pfd_content_t *pfd_content)
{
    cJSON *item = NULL;

    if (pfd_content == NULL) {
        ogs_error("OpenAPI_pfd_content_convertToJSON() failed [PfdContent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pfd_content->pfd_id) {
        if (cJSON_AddStringToObject(item, "pfdId", pfd_content->pfd_id) == NULL) {
            ogs_error("OpenAPI_pfd_content_convertToJSON() failed [pfd_id]");
            goto end;
        }
    }

    if (pfd_content->flow_descriptions) {
        cJSON *flow_descriptions = cJSON_AddArrayToObject(item, "flowDescriptions");
        if (flow_descriptions == NULL) {
            ogs_error("OpenAPI_pfd_content_convertToJSON() failed [flow_descriptions]");
            goto end;
        }

        OpenAPI_lnode_t *flow_descriptions_node;
        OpenAPI_list_for_each(pfd_content->flow_descriptions, flow_descriptions_node)  {
            if (cJSON_AddStringToObject(flow_descriptions, "", (char*)flow_descriptions_node->data) == NULL) {
                ogs_error("OpenAPI_pfd_content_convertToJSON() failed [flow_descriptions]");
                goto end;
            }
        }
    }

    if (pfd_content->urls) {
        cJSON *urls = cJSON_AddArrayToObject(item, "urls");
        if (urls == NULL) {
            ogs_error("OpenAPI_pfd_content_convertToJSON() failed [urls]");
            goto end;
        }

        OpenAPI_lnode_t *urls_node;
        OpenAPI_list_for_each(pfd_content->urls, urls_node)  {
            if (cJSON_AddStringToObject(urls, "", (char*)urls_node->data) == NULL) {
                ogs_error("OpenAPI_pfd_content_convertToJSON() failed [urls]");
                goto end;
            }
        }
    }

    if (pfd_content->domain_names) {
        cJSON *domain_names = cJSON_AddArrayToObject(item, "domainNames");
        if (domain_names == NULL) {
            ogs_error("OpenAPI_pfd_content_convertToJSON() failed [domain_names]");
            goto end;
        }

        OpenAPI_lnode_t *domain_names_node;
        OpenAPI_list_for_each(pfd_content->domain_names, domain_names_node)  {
            if (cJSON_AddStringToObject(domain_names, "", (char*)domain_names_node->data) == NULL) {
                ogs_error("OpenAPI_pfd_content_convertToJSON() failed [domain_names]");
                goto end;
            }
        }
    }

    if (pfd_content->dn_protocol) {
        cJSON *dn_protocol_local_JSON = OpenAPI_domain_name_protocol_convertToJSON(pfd_content->dn_protocol);
        if (dn_protocol_local_JSON == NULL) {
            ogs_error("OpenAPI_pfd_content_convertToJSON() failed [dn_protocol]");
            goto end;
        }
        cJSON_AddItemToObject(item, "dnProtocol", dn_protocol_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_pfd_content_convertToJSON() failed [dn_protocol]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pfd_content_t *OpenAPI_pfd_content_parseFromJSON(cJSON *pfd_contentJSON)
{
    OpenAPI_pfd_content_t *pfd_content_local_var = NULL;
    cJSON *pfd_id = cJSON_GetObjectItemCaseSensitive(pfd_contentJSON, "pfdId");

    if (pfd_id) {
        if (!cJSON_IsString(pfd_id)) {
            ogs_error("OpenAPI_pfd_content_parseFromJSON() failed [pfd_id]");
            goto end;
        }
    }

    cJSON *flow_descriptions = cJSON_GetObjectItemCaseSensitive(pfd_contentJSON, "flowDescriptions");

    OpenAPI_list_t *flow_descriptionsList;
    if (flow_descriptions) {
        cJSON *flow_descriptions_local;
        if (!cJSON_IsArray(flow_descriptions)) {
            ogs_error("OpenAPI_pfd_content_parseFromJSON() failed [flow_descriptions]");
            goto end;
        }
        flow_descriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_descriptions_local, flow_descriptions) {
            if (!cJSON_IsString(flow_descriptions_local)) {
                ogs_error("OpenAPI_pfd_content_parseFromJSON() failed [flow_descriptions]");
                goto end;
            }
            OpenAPI_list_add(flow_descriptionsList, ogs_strdup(flow_descriptions_local->valuestring));
        }
    }

    cJSON *urls = cJSON_GetObjectItemCaseSensitive(pfd_contentJSON, "urls");

    OpenAPI_list_t *urlsList;
    if (urls) {
        cJSON *urls_local;
        if (!cJSON_IsArray(urls)) {
            ogs_error("OpenAPI_pfd_content_parseFromJSON() failed [urls]");
            goto end;
        }
        urlsList = OpenAPI_list_create();

        cJSON_ArrayForEach(urls_local, urls) {
            if (!cJSON_IsString(urls_local)) {
                ogs_error("OpenAPI_pfd_content_parseFromJSON() failed [urls]");
                goto end;
            }
            OpenAPI_list_add(urlsList, ogs_strdup(urls_local->valuestring));
        }
    }

    cJSON *domain_names = cJSON_GetObjectItemCaseSensitive(pfd_contentJSON, "domainNames");

    OpenAPI_list_t *domain_namesList;
    if (domain_names) {
        cJSON *domain_names_local;
        if (!cJSON_IsArray(domain_names)) {
            ogs_error("OpenAPI_pfd_content_parseFromJSON() failed [domain_names]");
            goto end;
        }
        domain_namesList = OpenAPI_list_create();

        cJSON_ArrayForEach(domain_names_local, domain_names) {
            if (!cJSON_IsString(domain_names_local)) {
                ogs_error("OpenAPI_pfd_content_parseFromJSON() failed [domain_names]");
                goto end;
            }
            OpenAPI_list_add(domain_namesList, ogs_strdup(domain_names_local->valuestring));
        }
    }

    cJSON *dn_protocol = cJSON_GetObjectItemCaseSensitive(pfd_contentJSON, "dnProtocol");

    OpenAPI_domain_name_protocol_t *dn_protocol_local_nonprim = NULL;
    if (dn_protocol) {
        dn_protocol_local_nonprim = OpenAPI_domain_name_protocol_parseFromJSON(dn_protocol);
    }

    pfd_content_local_var = OpenAPI_pfd_content_create (
        pfd_id ? ogs_strdup(pfd_id->valuestring) : NULL,
        flow_descriptions ? flow_descriptionsList : NULL,
        urls ? urlsList : NULL,
        domain_names ? domain_namesList : NULL,
        dn_protocol ? dn_protocol_local_nonprim : NULL
        );

    return pfd_content_local_var;
end:
    return NULL;
}

OpenAPI_pfd_content_t *OpenAPI_pfd_content_copy(OpenAPI_pfd_content_t *dst, OpenAPI_pfd_content_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfd_content_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfd_content_convertToJSON() failed");
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

    OpenAPI_pfd_content_free(dst);
    dst = OpenAPI_pfd_content_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

