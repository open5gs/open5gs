
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rule_set.h"

OpenAPI_rule_set_t *OpenAPI_rule_set_create(
    int priority,
    OpenAPI_list_t *plmns,
    OpenAPI_list_t *snpns,
    OpenAPI_list_t *nf_types,
    OpenAPI_list_t *nf_domains,
    OpenAPI_list_t *nssais,
    OpenAPI_list_t *nf_instances,
    OpenAPI_list_t *scopes,
    OpenAPI_rule_set_action_e action
)
{
    OpenAPI_rule_set_t *rule_set_local_var = ogs_malloc(sizeof(OpenAPI_rule_set_t));
    ogs_assert(rule_set_local_var);

    rule_set_local_var->priority = priority;
    rule_set_local_var->plmns = plmns;
    rule_set_local_var->snpns = snpns;
    rule_set_local_var->nf_types = nf_types;
    rule_set_local_var->nf_domains = nf_domains;
    rule_set_local_var->nssais = nssais;
    rule_set_local_var->nf_instances = nf_instances;
    rule_set_local_var->scopes = scopes;
    rule_set_local_var->action = action;

    return rule_set_local_var;
}

void OpenAPI_rule_set_free(OpenAPI_rule_set_t *rule_set)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rule_set) {
        return;
    }
    if (rule_set->plmns) {
        OpenAPI_list_for_each(rule_set->plmns, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(rule_set->plmns);
        rule_set->plmns = NULL;
    }
    if (rule_set->snpns) {
        OpenAPI_list_for_each(rule_set->snpns, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(rule_set->snpns);
        rule_set->snpns = NULL;
    }
    if (rule_set->nf_types) {
        OpenAPI_list_free(rule_set->nf_types);
        rule_set->nf_types = NULL;
    }
    if (rule_set->nf_domains) {
        OpenAPI_list_for_each(rule_set->nf_domains, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rule_set->nf_domains);
        rule_set->nf_domains = NULL;
    }
    if (rule_set->nssais) {
        OpenAPI_list_for_each(rule_set->nssais, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(rule_set->nssais);
        rule_set->nssais = NULL;
    }
    if (rule_set->nf_instances) {
        OpenAPI_list_for_each(rule_set->nf_instances, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rule_set->nf_instances);
        rule_set->nf_instances = NULL;
    }
    if (rule_set->scopes) {
        OpenAPI_list_for_each(rule_set->scopes, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rule_set->scopes);
        rule_set->scopes = NULL;
    }
    ogs_free(rule_set);
}

cJSON *OpenAPI_rule_set_convertToJSON(OpenAPI_rule_set_t *rule_set)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rule_set == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [RuleSet]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "priority", rule_set->priority) == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [priority]");
        goto end;
    }

    if (rule_set->plmns) {
    cJSON *plmnsList = cJSON_AddArrayToObject(item, "plmns");
    if (plmnsList == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [plmns]");
        goto end;
    }
    OpenAPI_list_for_each(rule_set->plmns, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_rule_set_convertToJSON() failed [plmns]");
            goto end;
        }
        cJSON_AddItemToArray(plmnsList, itemLocal);
    }
    }

    if (rule_set->snpns) {
    cJSON *snpnsList = cJSON_AddArrayToObject(item, "snpns");
    if (snpnsList == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [snpns]");
        goto end;
    }
    OpenAPI_list_for_each(rule_set->snpns, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_rule_set_convertToJSON() failed [snpns]");
            goto end;
        }
        cJSON_AddItemToArray(snpnsList, itemLocal);
    }
    }

    if (rule_set->nf_types != OpenAPI_nf_type_NULL) {
    cJSON *nf_typesList = cJSON_AddArrayToObject(item, "nfTypes");
    if (nf_typesList == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [nf_types]");
        goto end;
    }
    OpenAPI_list_for_each(rule_set->nf_types, node) {
        if (cJSON_AddStringToObject(nf_typesList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_rule_set_convertToJSON() failed [nf_types]");
            goto end;
        }
    }
    }

    if (rule_set->nf_domains) {
    cJSON *nf_domainsList = cJSON_AddArrayToObject(item, "nfDomains");
    if (nf_domainsList == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [nf_domains]");
        goto end;
    }
    OpenAPI_list_for_each(rule_set->nf_domains, node) {
        if (cJSON_AddStringToObject(nf_domainsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rule_set_convertToJSON() failed [nf_domains]");
            goto end;
        }
    }
    }

    if (rule_set->nssais) {
    cJSON *nssaisList = cJSON_AddArrayToObject(item, "nssais");
    if (nssaisList == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [nssais]");
        goto end;
    }
    OpenAPI_list_for_each(rule_set->nssais, node) {
        cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_rule_set_convertToJSON() failed [nssais]");
            goto end;
        }
        cJSON_AddItemToArray(nssaisList, itemLocal);
    }
    }

    if (rule_set->nf_instances) {
    cJSON *nf_instancesList = cJSON_AddArrayToObject(item, "nfInstances");
    if (nf_instancesList == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [nf_instances]");
        goto end;
    }
    OpenAPI_list_for_each(rule_set->nf_instances, node) {
        if (cJSON_AddStringToObject(nf_instancesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rule_set_convertToJSON() failed [nf_instances]");
            goto end;
        }
    }
    }

    if (rule_set->scopes) {
    cJSON *scopesList = cJSON_AddArrayToObject(item, "scopes");
    if (scopesList == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [scopes]");
        goto end;
    }
    OpenAPI_list_for_each(rule_set->scopes, node) {
        if (cJSON_AddStringToObject(scopesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rule_set_convertToJSON() failed [scopes]");
            goto end;
        }
    }
    }

    if (rule_set->action == OpenAPI_rule_set_action_NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [action]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "action", OpenAPI_rule_set_action_ToString(rule_set->action)) == NULL) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed [action]");
        goto end;
    }

end:
    return item;
}

OpenAPI_rule_set_t *OpenAPI_rule_set_parseFromJSON(cJSON *rule_setJSON)
{
    OpenAPI_rule_set_t *rule_set_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *priority = NULL;
    cJSON *plmns = NULL;
    OpenAPI_list_t *plmnsList = NULL;
    cJSON *snpns = NULL;
    OpenAPI_list_t *snpnsList = NULL;
    cJSON *nf_types = NULL;
    OpenAPI_list_t *nf_typesList = NULL;
    cJSON *nf_domains = NULL;
    OpenAPI_list_t *nf_domainsList = NULL;
    cJSON *nssais = NULL;
    OpenAPI_list_t *nssaisList = NULL;
    cJSON *nf_instances = NULL;
    OpenAPI_list_t *nf_instancesList = NULL;
    cJSON *scopes = NULL;
    OpenAPI_list_t *scopesList = NULL;
    cJSON *action = NULL;
    OpenAPI_rule_set_action_e actionVariable = 0;
    priority = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "priority");
    if (!priority) {
        ogs_error("OpenAPI_rule_set_parseFromJSON() failed [priority]");
        goto end;
    }
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_rule_set_parseFromJSON() failed [priority]");
        goto end;
    }

    plmns = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "plmns");
    if (plmns) {
        cJSON *plmns_local = NULL;
        if (!cJSON_IsArray(plmns)) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed [plmns]");
            goto end;
        }

        plmnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmns_local, plmns) {
            if (!cJSON_IsObject(plmns_local)) {
                ogs_error("OpenAPI_rule_set_parseFromJSON() failed [plmns]");
                goto end;
            }
            OpenAPI_plmn_id_t *plmnsItem = OpenAPI_plmn_id_parseFromJSON(plmns_local);
            if (!plmnsItem) {
                ogs_error("No plmnsItem");
                goto end;
            }
            OpenAPI_list_add(plmnsList, plmnsItem);
        }
    }

    snpns = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "snpns");
    if (snpns) {
        cJSON *snpns_local = NULL;
        if (!cJSON_IsArray(snpns)) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed [snpns]");
            goto end;
        }

        snpnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(snpns_local, snpns) {
            if (!cJSON_IsObject(snpns_local)) {
                ogs_error("OpenAPI_rule_set_parseFromJSON() failed [snpns]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *snpnsItem = OpenAPI_plmn_id_nid_parseFromJSON(snpns_local);
            if (!snpnsItem) {
                ogs_error("No snpnsItem");
                goto end;
            }
            OpenAPI_list_add(snpnsList, snpnsItem);
        }
    }

    nf_types = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "nfTypes");
    if (nf_types) {
        cJSON *nf_types_local = NULL;
        if (!cJSON_IsArray(nf_types)) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nf_types]");
            goto end;
        }

        nf_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_types_local, nf_types) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(nf_types_local)) {
                ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nf_types]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(nf_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"nf_types\" is not supported. Ignoring it ...",
                         nf_types_local->valuestring);
            } else {
                OpenAPI_list_add(nf_typesList, (void *)localEnum);
            }
        }
        if (nf_typesList->count == 0) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed: Expected nf_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    nf_domains = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "nfDomains");
    if (nf_domains) {
        cJSON *nf_domains_local = NULL;
        if (!cJSON_IsArray(nf_domains)) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nf_domains]");
            goto end;
        }

        nf_domainsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_domains_local, nf_domains) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_domains_local)) {
                ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nf_domains]");
                goto end;
            }
            OpenAPI_list_add(nf_domainsList, ogs_strdup(nf_domains_local->valuestring));
        }
    }

    nssais = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "nssais");
    if (nssais) {
        cJSON *nssais_local = NULL;
        if (!cJSON_IsArray(nssais)) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nssais]");
            goto end;
        }

        nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(nssais_local, nssais) {
            if (!cJSON_IsObject(nssais_local)) {
                ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nssais]");
                goto end;
            }
            OpenAPI_ext_snssai_t *nssaisItem = OpenAPI_ext_snssai_parseFromJSON(nssais_local);
            if (!nssaisItem) {
                ogs_error("No nssaisItem");
                goto end;
            }
            OpenAPI_list_add(nssaisList, nssaisItem);
        }
    }

    nf_instances = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "nfInstances");
    if (nf_instances) {
        cJSON *nf_instances_local = NULL;
        if (!cJSON_IsArray(nf_instances)) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nf_instances]");
            goto end;
        }

        nf_instancesList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_instances_local, nf_instances) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_instances_local)) {
                ogs_error("OpenAPI_rule_set_parseFromJSON() failed [nf_instances]");
                goto end;
            }
            OpenAPI_list_add(nf_instancesList, ogs_strdup(nf_instances_local->valuestring));
        }
    }

    scopes = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "scopes");
    if (scopes) {
        cJSON *scopes_local = NULL;
        if (!cJSON_IsArray(scopes)) {
            ogs_error("OpenAPI_rule_set_parseFromJSON() failed [scopes]");
            goto end;
        }

        scopesList = OpenAPI_list_create();

        cJSON_ArrayForEach(scopes_local, scopes) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(scopes_local)) {
                ogs_error("OpenAPI_rule_set_parseFromJSON() failed [scopes]");
                goto end;
            }
            OpenAPI_list_add(scopesList, ogs_strdup(scopes_local->valuestring));
        }
    }

    action = cJSON_GetObjectItemCaseSensitive(rule_setJSON, "action");
    if (!action) {
        ogs_error("OpenAPI_rule_set_parseFromJSON() failed [action]");
        goto end;
    }
    if (!cJSON_IsString(action)) {
        ogs_error("OpenAPI_rule_set_parseFromJSON() failed [action]");
        goto end;
    }
    actionVariable = OpenAPI_rule_set_action_FromString(action->valuestring);

    rule_set_local_var = OpenAPI_rule_set_create (
        
        priority->valuedouble,
        plmns ? plmnsList : NULL,
        snpns ? snpnsList : NULL,
        nf_types ? nf_typesList : NULL,
        nf_domains ? nf_domainsList : NULL,
        nssais ? nssaisList : NULL,
        nf_instances ? nf_instancesList : NULL,
        scopes ? scopesList : NULL,
        actionVariable
    );

    return rule_set_local_var;
end:
    if (plmnsList) {
        OpenAPI_list_for_each(plmnsList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(plmnsList);
        plmnsList = NULL;
    }
    if (snpnsList) {
        OpenAPI_list_for_each(snpnsList, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(snpnsList);
        snpnsList = NULL;
    }
    if (nf_typesList) {
        OpenAPI_list_free(nf_typesList);
        nf_typesList = NULL;
    }
    if (nf_domainsList) {
        OpenAPI_list_for_each(nf_domainsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_domainsList);
        nf_domainsList = NULL;
    }
    if (nssaisList) {
        OpenAPI_list_for_each(nssaisList, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(nssaisList);
        nssaisList = NULL;
    }
    if (nf_instancesList) {
        OpenAPI_list_for_each(nf_instancesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_instancesList);
        nf_instancesList = NULL;
    }
    if (scopesList) {
        OpenAPI_list_for_each(scopesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(scopesList);
        scopesList = NULL;
    }
    return NULL;
}

OpenAPI_rule_set_t *OpenAPI_rule_set_copy(OpenAPI_rule_set_t *dst, OpenAPI_rule_set_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rule_set_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rule_set_convertToJSON() failed");
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

    OpenAPI_rule_set_free(dst);
    dst = OpenAPI_rule_set_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

