
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnai_eas_info.h"

OpenAPI_dnai_eas_info_t *OpenAPI_dnai_eas_info_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnais,
    OpenAPI_list_t *eas_ip_addrs,
    OpenAPI_list_t *fqdns
)
{
    OpenAPI_dnai_eas_info_t *dnai_eas_info_local_var = ogs_malloc(sizeof(OpenAPI_dnai_eas_info_t));
    ogs_assert(dnai_eas_info_local_var);

    dnai_eas_info_local_var->dnn = dnn;
    dnai_eas_info_local_var->snssai = snssai;
    dnai_eas_info_local_var->dnais = dnais;
    dnai_eas_info_local_var->eas_ip_addrs = eas_ip_addrs;
    dnai_eas_info_local_var->fqdns = fqdns;

    return dnai_eas_info_local_var;
}

void OpenAPI_dnai_eas_info_free(OpenAPI_dnai_eas_info_t *dnai_eas_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnai_eas_info) {
        return;
    }
    if (dnai_eas_info->dnn) {
        ogs_free(dnai_eas_info->dnn);
        dnai_eas_info->dnn = NULL;
    }
    if (dnai_eas_info->snssai) {
        OpenAPI_snssai_free(dnai_eas_info->snssai);
        dnai_eas_info->snssai = NULL;
    }
    if (dnai_eas_info->dnais) {
        OpenAPI_list_for_each(dnai_eas_info->dnais, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnai_eas_info->dnais);
        dnai_eas_info->dnais = NULL;
    }
    if (dnai_eas_info->eas_ip_addrs) {
        OpenAPI_list_for_each(dnai_eas_info->eas_ip_addrs, node) {
            OpenAPI_ip_addr_1_free(node->data);
        }
        OpenAPI_list_free(dnai_eas_info->eas_ip_addrs);
        dnai_eas_info->eas_ip_addrs = NULL;
    }
    if (dnai_eas_info->fqdns) {
        OpenAPI_list_for_each(dnai_eas_info->fqdns, node) {
            OpenAPI_fqdn_pattern_matching_rule_free(node->data);
        }
        OpenAPI_list_free(dnai_eas_info->fqdns);
        dnai_eas_info->fqdns = NULL;
    }
    ogs_free(dnai_eas_info);
}

cJSON *OpenAPI_dnai_eas_info_convertToJSON(OpenAPI_dnai_eas_info_t *dnai_eas_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnai_eas_info == NULL) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [DnaiEasInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dnai_eas_info->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", dnai_eas_info->dnn) == NULL) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (dnai_eas_info->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(dnai_eas_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (!dnai_eas_info->dnais) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [dnais]");
        return NULL;
    }
    cJSON *dnaisList = cJSON_AddArrayToObject(item, "dnais");
    if (dnaisList == NULL) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [dnais]");
        goto end;
    }
    OpenAPI_list_for_each(dnai_eas_info->dnais, node) {
        if (cJSON_AddStringToObject(dnaisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [dnais]");
            goto end;
        }
    }

    if (dnai_eas_info->eas_ip_addrs) {
    cJSON *eas_ip_addrsList = cJSON_AddArrayToObject(item, "easIpAddrs");
    if (eas_ip_addrsList == NULL) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [eas_ip_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(dnai_eas_info->eas_ip_addrs, node) {
        cJSON *itemLocal = OpenAPI_ip_addr_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [eas_ip_addrs]");
            goto end;
        }
        cJSON_AddItemToArray(eas_ip_addrsList, itemLocal);
    }
    }

    if (dnai_eas_info->fqdns) {
    cJSON *fqdnsList = cJSON_AddArrayToObject(item, "fqdns");
    if (fqdnsList == NULL) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [fqdns]");
        goto end;
    }
    OpenAPI_list_for_each(dnai_eas_info->fqdns, node) {
        cJSON *itemLocal = OpenAPI_fqdn_pattern_matching_rule_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed [fqdns]");
            goto end;
        }
        cJSON_AddItemToArray(fqdnsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_dnai_eas_info_t *OpenAPI_dnai_eas_info_parseFromJSON(cJSON *dnai_eas_infoJSON)
{
    OpenAPI_dnai_eas_info_t *dnai_eas_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *dnais = NULL;
    OpenAPI_list_t *dnaisList = NULL;
    cJSON *eas_ip_addrs = NULL;
    OpenAPI_list_t *eas_ip_addrsList = NULL;
    cJSON *fqdns = NULL;
    OpenAPI_list_t *fqdnsList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnai_eas_infoJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(dnai_eas_infoJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    dnais = cJSON_GetObjectItemCaseSensitive(dnai_eas_infoJSON, "dnais");
    if (!dnais) {
        ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [dnais]");
        goto end;
    }
        cJSON *dnais_local = NULL;
        if (!cJSON_IsArray(dnais)) {
            ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [dnais]");
            goto end;
        }

        dnaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnais_local, dnais) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnais_local)) {
                ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [dnais]");
                goto end;
            }
            OpenAPI_list_add(dnaisList, ogs_strdup(dnais_local->valuestring));
        }

    eas_ip_addrs = cJSON_GetObjectItemCaseSensitive(dnai_eas_infoJSON, "easIpAddrs");
    if (eas_ip_addrs) {
        cJSON *eas_ip_addrs_local = NULL;
        if (!cJSON_IsArray(eas_ip_addrs)) {
            ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [eas_ip_addrs]");
            goto end;
        }

        eas_ip_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(eas_ip_addrs_local, eas_ip_addrs) {
            if (!cJSON_IsObject(eas_ip_addrs_local)) {
                ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [eas_ip_addrs]");
                goto end;
            }
            OpenAPI_ip_addr_1_t *eas_ip_addrsItem = OpenAPI_ip_addr_1_parseFromJSON(eas_ip_addrs_local);
            if (!eas_ip_addrsItem) {
                ogs_error("No eas_ip_addrsItem");
                goto end;
            }
            OpenAPI_list_add(eas_ip_addrsList, eas_ip_addrsItem);
        }
    }

    fqdns = cJSON_GetObjectItemCaseSensitive(dnai_eas_infoJSON, "fqdns");
    if (fqdns) {
        cJSON *fqdns_local = NULL;
        if (!cJSON_IsArray(fqdns)) {
            ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [fqdns]");
            goto end;
        }

        fqdnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(fqdns_local, fqdns) {
            if (!cJSON_IsObject(fqdns_local)) {
                ogs_error("OpenAPI_dnai_eas_info_parseFromJSON() failed [fqdns]");
                goto end;
            }
            OpenAPI_fqdn_pattern_matching_rule_t *fqdnsItem = OpenAPI_fqdn_pattern_matching_rule_parseFromJSON(fqdns_local);
            if (!fqdnsItem) {
                ogs_error("No fqdnsItem");
                goto end;
            }
            OpenAPI_list_add(fqdnsList, fqdnsItem);
        }
    }

    dnai_eas_info_local_var = OpenAPI_dnai_eas_info_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        dnaisList,
        eas_ip_addrs ? eas_ip_addrsList : NULL,
        fqdns ? fqdnsList : NULL
    );

    return dnai_eas_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (dnaisList) {
        OpenAPI_list_for_each(dnaisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnaisList);
        dnaisList = NULL;
    }
    if (eas_ip_addrsList) {
        OpenAPI_list_for_each(eas_ip_addrsList, node) {
            OpenAPI_ip_addr_1_free(node->data);
        }
        OpenAPI_list_free(eas_ip_addrsList);
        eas_ip_addrsList = NULL;
    }
    if (fqdnsList) {
        OpenAPI_list_for_each(fqdnsList, node) {
            OpenAPI_fqdn_pattern_matching_rule_free(node->data);
        }
        OpenAPI_list_free(fqdnsList);
        fqdnsList = NULL;
    }
    return NULL;
}

OpenAPI_dnai_eas_info_t *OpenAPI_dnai_eas_info_copy(OpenAPI_dnai_eas_info_t *dst, OpenAPI_dnai_eas_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnai_eas_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnai_eas_info_convertToJSON() failed");
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

    OpenAPI_dnai_eas_info_free(dst);
    dst = OpenAPI_dnai_eas_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

