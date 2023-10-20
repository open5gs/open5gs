
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_domain_cond.h"

OpenAPI_scp_domain_cond_t *OpenAPI_scp_domain_cond_create(
    OpenAPI_list_t *scp_domains,
    OpenAPI_list_t *nf_type_list
)
{
    OpenAPI_scp_domain_cond_t *scp_domain_cond_local_var = ogs_malloc(sizeof(OpenAPI_scp_domain_cond_t));
    ogs_assert(scp_domain_cond_local_var);

    scp_domain_cond_local_var->scp_domains = scp_domains;
    scp_domain_cond_local_var->nf_type_list = nf_type_list;

    return scp_domain_cond_local_var;
}

void OpenAPI_scp_domain_cond_free(OpenAPI_scp_domain_cond_t *scp_domain_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scp_domain_cond) {
        return;
    }
    if (scp_domain_cond->scp_domains) {
        OpenAPI_list_for_each(scp_domain_cond->scp_domains, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_domain_cond->scp_domains);
        scp_domain_cond->scp_domains = NULL;
    }
    if (scp_domain_cond->nf_type_list) {
        OpenAPI_list_free(scp_domain_cond->nf_type_list);
        scp_domain_cond->nf_type_list = NULL;
    }
    ogs_free(scp_domain_cond);
}

cJSON *OpenAPI_scp_domain_cond_convertToJSON(OpenAPI_scp_domain_cond_t *scp_domain_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scp_domain_cond == NULL) {
        ogs_error("OpenAPI_scp_domain_cond_convertToJSON() failed [ScpDomainCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!scp_domain_cond->scp_domains) {
        ogs_error("OpenAPI_scp_domain_cond_convertToJSON() failed [scp_domains]");
        return NULL;
    }
    cJSON *scp_domainsList = cJSON_AddArrayToObject(item, "scpDomains");
    if (scp_domainsList == NULL) {
        ogs_error("OpenAPI_scp_domain_cond_convertToJSON() failed [scp_domains]");
        goto end;
    }
    OpenAPI_list_for_each(scp_domain_cond->scp_domains, node) {
        if (cJSON_AddStringToObject(scp_domainsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_scp_domain_cond_convertToJSON() failed [scp_domains]");
            goto end;
        }
    }

    if (scp_domain_cond->nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *nf_type_listList = cJSON_AddArrayToObject(item, "nfTypeList");
    if (nf_type_listList == NULL) {
        ogs_error("OpenAPI_scp_domain_cond_convertToJSON() failed [nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(scp_domain_cond->nf_type_list, node) {
        if (cJSON_AddStringToObject(nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_scp_domain_cond_convertToJSON() failed [nf_type_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_scp_domain_cond_t *OpenAPI_scp_domain_cond_parseFromJSON(cJSON *scp_domain_condJSON)
{
    OpenAPI_scp_domain_cond_t *scp_domain_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *scp_domains = NULL;
    OpenAPI_list_t *scp_domainsList = NULL;
    cJSON *nf_type_list = NULL;
    OpenAPI_list_t *nf_type_listList = NULL;
    scp_domains = cJSON_GetObjectItemCaseSensitive(scp_domain_condJSON, "scpDomains");
    if (!scp_domains) {
        ogs_error("OpenAPI_scp_domain_cond_parseFromJSON() failed [scp_domains]");
        goto end;
    }
        cJSON *scp_domains_local = NULL;
        if (!cJSON_IsArray(scp_domains)) {
            ogs_error("OpenAPI_scp_domain_cond_parseFromJSON() failed [scp_domains]");
            goto end;
        }

        scp_domainsList = OpenAPI_list_create();

        cJSON_ArrayForEach(scp_domains_local, scp_domains) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(scp_domains_local)) {
                ogs_error("OpenAPI_scp_domain_cond_parseFromJSON() failed [scp_domains]");
                goto end;
            }
            OpenAPI_list_add(scp_domainsList, ogs_strdup(scp_domains_local->valuestring));
        }

    nf_type_list = cJSON_GetObjectItemCaseSensitive(scp_domain_condJSON, "nfTypeList");
    if (nf_type_list) {
        cJSON *nf_type_list_local = NULL;
        if (!cJSON_IsArray(nf_type_list)) {
            ogs_error("OpenAPI_scp_domain_cond_parseFromJSON() failed [nf_type_list]");
            goto end;
        }

        nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_type_list_local, nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(nf_type_list_local)) {
                ogs_error("OpenAPI_scp_domain_cond_parseFromJSON() failed [nf_type_list]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(nf_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"nf_type_list\" is not supported. Ignoring it ...",
                         nf_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(nf_type_listList, (void *)localEnum);
            }
        }
        if (nf_type_listList->count == 0) {
            ogs_error("OpenAPI_scp_domain_cond_parseFromJSON() failed: Expected nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    scp_domain_cond_local_var = OpenAPI_scp_domain_cond_create (
        scp_domainsList,
        nf_type_list ? nf_type_listList : NULL
    );

    return scp_domain_cond_local_var;
end:
    if (scp_domainsList) {
        OpenAPI_list_for_each(scp_domainsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scp_domainsList);
        scp_domainsList = NULL;
    }
    if (nf_type_listList) {
        OpenAPI_list_free(nf_type_listList);
        nf_type_listList = NULL;
    }
    return NULL;
}

OpenAPI_scp_domain_cond_t *OpenAPI_scp_domain_cond_copy(OpenAPI_scp_domain_cond_t *dst, OpenAPI_scp_domain_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scp_domain_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scp_domain_cond_convertToJSON() failed");
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

    OpenAPI_scp_domain_cond_free(dst);
    dst = OpenAPI_scp_domain_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

