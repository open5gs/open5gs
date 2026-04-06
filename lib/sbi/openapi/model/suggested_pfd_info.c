
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "suggested_pfd_info.h"

OpenAPI_suggested_pfd_info_t *OpenAPI_suggested_pfd_info_create(
    char *pfd_id,
    OpenAPI_list_t *ip3_tuple_list,
    OpenAPI_list_t *urls,
    OpenAPI_list_t *domain_names,
    OpenAPI_domain_name_protocol_e dn_protocol,
    bool is_pfd_confidence,
    int pfd_confidence
)
{
    OpenAPI_suggested_pfd_info_t *suggested_pfd_info_local_var = ogs_malloc(sizeof(OpenAPI_suggested_pfd_info_t));
    ogs_assert(suggested_pfd_info_local_var);

    suggested_pfd_info_local_var->pfd_id = pfd_id;
    suggested_pfd_info_local_var->ip3_tuple_list = ip3_tuple_list;
    suggested_pfd_info_local_var->urls = urls;
    suggested_pfd_info_local_var->domain_names = domain_names;
    suggested_pfd_info_local_var->dn_protocol = dn_protocol;
    suggested_pfd_info_local_var->is_pfd_confidence = is_pfd_confidence;
    suggested_pfd_info_local_var->pfd_confidence = pfd_confidence;

    return suggested_pfd_info_local_var;
}

void OpenAPI_suggested_pfd_info_free(OpenAPI_suggested_pfd_info_t *suggested_pfd_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == suggested_pfd_info) {
        return;
    }
    if (suggested_pfd_info->pfd_id) {
        ogs_free(suggested_pfd_info->pfd_id);
        suggested_pfd_info->pfd_id = NULL;
    }
    if (suggested_pfd_info->ip3_tuple_list) {
        OpenAPI_list_for_each(suggested_pfd_info->ip3_tuple_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(suggested_pfd_info->ip3_tuple_list);
        suggested_pfd_info->ip3_tuple_list = NULL;
    }
    if (suggested_pfd_info->urls) {
        OpenAPI_list_for_each(suggested_pfd_info->urls, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(suggested_pfd_info->urls);
        suggested_pfd_info->urls = NULL;
    }
    if (suggested_pfd_info->domain_names) {
        OpenAPI_list_for_each(suggested_pfd_info->domain_names, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(suggested_pfd_info->domain_names);
        suggested_pfd_info->domain_names = NULL;
    }
    ogs_free(suggested_pfd_info);
}

cJSON *OpenAPI_suggested_pfd_info_convertToJSON(OpenAPI_suggested_pfd_info_t *suggested_pfd_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (suggested_pfd_info == NULL) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [SuggestedPfdInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!suggested_pfd_info->pfd_id) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [pfd_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "pfdId", suggested_pfd_info->pfd_id) == NULL) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [pfd_id]");
        goto end;
    }

    if (suggested_pfd_info->ip3_tuple_list) {
    cJSON *ip3_tuple_listList = cJSON_AddArrayToObject(item, "ip3TupleList");
    if (ip3_tuple_listList == NULL) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [ip3_tuple_list]");
        goto end;
    }
    OpenAPI_list_for_each(suggested_pfd_info->ip3_tuple_list, node) {
        if (cJSON_AddStringToObject(ip3_tuple_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [ip3_tuple_list]");
            goto end;
        }
    }
    }

    if (suggested_pfd_info->urls) {
    cJSON *urlsList = cJSON_AddArrayToObject(item, "urls");
    if (urlsList == NULL) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [urls]");
        goto end;
    }
    OpenAPI_list_for_each(suggested_pfd_info->urls, node) {
        if (cJSON_AddStringToObject(urlsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [urls]");
            goto end;
        }
    }
    }

    if (suggested_pfd_info->domain_names) {
    cJSON *domain_namesList = cJSON_AddArrayToObject(item, "domainNames");
    if (domain_namesList == NULL) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [domain_names]");
        goto end;
    }
    OpenAPI_list_for_each(suggested_pfd_info->domain_names, node) {
        if (cJSON_AddStringToObject(domain_namesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [domain_names]");
            goto end;
        }
    }
    }

    if (suggested_pfd_info->dn_protocol != OpenAPI_domain_name_protocol_NULL) {
    if (cJSON_AddStringToObject(item, "dnProtocol", OpenAPI_domain_name_protocol_ToString(suggested_pfd_info->dn_protocol)) == NULL) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [dn_protocol]");
        goto end;
    }
    }

    if (suggested_pfd_info->is_pfd_confidence) {
    if (cJSON_AddNumberToObject(item, "pfdConfidence", suggested_pfd_info->pfd_confidence) == NULL) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed [pfd_confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_suggested_pfd_info_t *OpenAPI_suggested_pfd_info_parseFromJSON(cJSON *suggested_pfd_infoJSON)
{
    OpenAPI_suggested_pfd_info_t *suggested_pfd_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pfd_id = NULL;
    cJSON *ip3_tuple_list = NULL;
    OpenAPI_list_t *ip3_tuple_listList = NULL;
    cJSON *urls = NULL;
    OpenAPI_list_t *urlsList = NULL;
    cJSON *domain_names = NULL;
    OpenAPI_list_t *domain_namesList = NULL;
    cJSON *dn_protocol = NULL;
    OpenAPI_domain_name_protocol_e dn_protocolVariable = 0;
    cJSON *pfd_confidence = NULL;
    pfd_id = cJSON_GetObjectItemCaseSensitive(suggested_pfd_infoJSON, "pfdId");
    if (!pfd_id) {
        ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [pfd_id]");
        goto end;
    }
    if (!cJSON_IsString(pfd_id)) {
        ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [pfd_id]");
        goto end;
    }

    ip3_tuple_list = cJSON_GetObjectItemCaseSensitive(suggested_pfd_infoJSON, "ip3TupleList");
    if (ip3_tuple_list) {
        cJSON *ip3_tuple_list_local = NULL;
        if (!cJSON_IsArray(ip3_tuple_list)) {
            ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [ip3_tuple_list]");
            goto end;
        }

        ip3_tuple_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ip3_tuple_list_local, ip3_tuple_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ip3_tuple_list_local)) {
                ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [ip3_tuple_list]");
                goto end;
            }
            OpenAPI_list_add(ip3_tuple_listList, ogs_strdup(ip3_tuple_list_local->valuestring));
        }
    }

    urls = cJSON_GetObjectItemCaseSensitive(suggested_pfd_infoJSON, "urls");
    if (urls) {
        cJSON *urls_local = NULL;
        if (!cJSON_IsArray(urls)) {
            ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [urls]");
            goto end;
        }

        urlsList = OpenAPI_list_create();

        cJSON_ArrayForEach(urls_local, urls) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(urls_local)) {
                ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [urls]");
                goto end;
            }
            OpenAPI_list_add(urlsList, ogs_strdup(urls_local->valuestring));
        }
    }

    domain_names = cJSON_GetObjectItemCaseSensitive(suggested_pfd_infoJSON, "domainNames");
    if (domain_names) {
        cJSON *domain_names_local = NULL;
        if (!cJSON_IsArray(domain_names)) {
            ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [domain_names]");
            goto end;
        }

        domain_namesList = OpenAPI_list_create();

        cJSON_ArrayForEach(domain_names_local, domain_names) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(domain_names_local)) {
                ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [domain_names]");
                goto end;
            }
            OpenAPI_list_add(domain_namesList, ogs_strdup(domain_names_local->valuestring));
        }
    }

    dn_protocol = cJSON_GetObjectItemCaseSensitive(suggested_pfd_infoJSON, "dnProtocol");
    if (dn_protocol) {
    if (!cJSON_IsString(dn_protocol)) {
        ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [dn_protocol]");
        goto end;
    }
    dn_protocolVariable = OpenAPI_domain_name_protocol_FromString(dn_protocol->valuestring);
    }

    pfd_confidence = cJSON_GetObjectItemCaseSensitive(suggested_pfd_infoJSON, "pfdConfidence");
    if (pfd_confidence) {
    if (!cJSON_IsNumber(pfd_confidence)) {
        ogs_error("OpenAPI_suggested_pfd_info_parseFromJSON() failed [pfd_confidence]");
        goto end;
    }
    }

    suggested_pfd_info_local_var = OpenAPI_suggested_pfd_info_create (
        ogs_strdup(pfd_id->valuestring),
        ip3_tuple_list ? ip3_tuple_listList : NULL,
        urls ? urlsList : NULL,
        domain_names ? domain_namesList : NULL,
        dn_protocol ? dn_protocolVariable : 0,
        pfd_confidence ? true : false,
        pfd_confidence ? pfd_confidence->valuedouble : 0
    );

    return suggested_pfd_info_local_var;
end:
    if (ip3_tuple_listList) {
        OpenAPI_list_for_each(ip3_tuple_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ip3_tuple_listList);
        ip3_tuple_listList = NULL;
    }
    if (urlsList) {
        OpenAPI_list_for_each(urlsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(urlsList);
        urlsList = NULL;
    }
    if (domain_namesList) {
        OpenAPI_list_for_each(domain_namesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(domain_namesList);
        domain_namesList = NULL;
    }
    return NULL;
}

OpenAPI_suggested_pfd_info_t *OpenAPI_suggested_pfd_info_copy(OpenAPI_suggested_pfd_info_t *dst, OpenAPI_suggested_pfd_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_suggested_pfd_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_suggested_pfd_info_convertToJSON() failed");
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

    OpenAPI_suggested_pfd_info_free(dst);
    dst = OpenAPI_suggested_pfd_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

