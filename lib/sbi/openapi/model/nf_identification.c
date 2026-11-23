
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_identification.h"

OpenAPI_nf_identification_t *OpenAPI_nf_identification_create(
    char *n_f_name,
    char *n_fipv4_address,
    char *n_fipv6_address,
    OpenAPI_plmn_id_t *n_fplmnid,
    OpenAPI_node_functionality_e node_functionality,
    char *n_f_fqdn
)
{
    OpenAPI_nf_identification_t *nf_identification_local_var = ogs_malloc(sizeof(OpenAPI_nf_identification_t));
    ogs_assert(nf_identification_local_var);

    nf_identification_local_var->n_f_name = n_f_name;
    nf_identification_local_var->n_fipv4_address = n_fipv4_address;
    nf_identification_local_var->n_fipv6_address = n_fipv6_address;
    nf_identification_local_var->n_fplmnid = n_fplmnid;
    nf_identification_local_var->node_functionality = node_functionality;
    nf_identification_local_var->n_f_fqdn = n_f_fqdn;

    return nf_identification_local_var;
}

void OpenAPI_nf_identification_free(OpenAPI_nf_identification_t *nf_identification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_identification) {
        return;
    }
    if (nf_identification->n_f_name) {
        ogs_free(nf_identification->n_f_name);
        nf_identification->n_f_name = NULL;
    }
    if (nf_identification->n_fipv4_address) {
        ogs_free(nf_identification->n_fipv4_address);
        nf_identification->n_fipv4_address = NULL;
    }
    if (nf_identification->n_fipv6_address) {
        ogs_free(nf_identification->n_fipv6_address);
        nf_identification->n_fipv6_address = NULL;
    }
    if (nf_identification->n_fplmnid) {
        OpenAPI_plmn_id_free(nf_identification->n_fplmnid);
        nf_identification->n_fplmnid = NULL;
    }
    if (nf_identification->n_f_fqdn) {
        ogs_free(nf_identification->n_f_fqdn);
        nf_identification->n_f_fqdn = NULL;
    }
    ogs_free(nf_identification);
}

cJSON *OpenAPI_nf_identification_convertToJSON(OpenAPI_nf_identification_t *nf_identification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_identification == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [NFIdentification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nf_identification->n_f_name) {
    if (cJSON_AddStringToObject(item, "nFName", nf_identification->n_f_name) == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [n_f_name]");
        goto end;
    }
    }

    if (nf_identification->n_fipv4_address) {
    if (cJSON_AddStringToObject(item, "nFIPv4Address", nf_identification->n_fipv4_address) == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [n_fipv4_address]");
        goto end;
    }
    }

    if (nf_identification->n_fipv6_address) {
    if (cJSON_AddStringToObject(item, "nFIPv6Address", nf_identification->n_fipv6_address) == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [n_fipv6_address]");
        goto end;
    }
    }

    if (nf_identification->n_fplmnid) {
    cJSON *n_fplmnid_local_JSON = OpenAPI_plmn_id_convertToJSON(nf_identification->n_fplmnid);
    if (n_fplmnid_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [n_fplmnid]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nFPLMNID", n_fplmnid_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [n_fplmnid]");
        goto end;
    }
    }

    if (nf_identification->node_functionality == OpenAPI_node_functionality_NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [node_functionality]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nodeFunctionality", OpenAPI_node_functionality_ToString(nf_identification->node_functionality)) == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [node_functionality]");
        goto end;
    }

    if (nf_identification->n_f_fqdn) {
    if (cJSON_AddStringToObject(item, "nFFqdn", nf_identification->n_f_fqdn) == NULL) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed [n_f_fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nf_identification_t *OpenAPI_nf_identification_parseFromJSON(cJSON *nf_identificationJSON)
{
    OpenAPI_nf_identification_t *nf_identification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n_f_name = NULL;
    cJSON *n_fipv4_address = NULL;
    cJSON *n_fipv6_address = NULL;
    cJSON *n_fplmnid = NULL;
    OpenAPI_plmn_id_t *n_fplmnid_local_nonprim = NULL;
    cJSON *node_functionality = NULL;
    OpenAPI_node_functionality_e node_functionalityVariable = 0;
    cJSON *n_f_fqdn = NULL;
    n_f_name = cJSON_GetObjectItemCaseSensitive(nf_identificationJSON, "nFName");
    if (n_f_name) {
    if (!cJSON_IsString(n_f_name) && !cJSON_IsNull(n_f_name)) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON() failed [n_f_name]");
        goto end;
    }
    }

    n_fipv4_address = cJSON_GetObjectItemCaseSensitive(nf_identificationJSON, "nFIPv4Address");
    if (n_fipv4_address) {
    if (!cJSON_IsString(n_fipv4_address) && !cJSON_IsNull(n_fipv4_address)) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON() failed [n_fipv4_address]");
        goto end;
    }
    }

    n_fipv6_address = cJSON_GetObjectItemCaseSensitive(nf_identificationJSON, "nFIPv6Address");
    if (n_fipv6_address) {
    if (!cJSON_IsString(n_fipv6_address) && !cJSON_IsNull(n_fipv6_address)) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON() failed [n_fipv6_address]");
        goto end;
    }
    }

    n_fplmnid = cJSON_GetObjectItemCaseSensitive(nf_identificationJSON, "nFPLMNID");
    if (n_fplmnid) {
    n_fplmnid_local_nonprim = OpenAPI_plmn_id_parseFromJSON(n_fplmnid);
    if (!n_fplmnid_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [n_fplmnid]");
        goto end;
    }
    }

    node_functionality = cJSON_GetObjectItemCaseSensitive(nf_identificationJSON, "nodeFunctionality");
    if (!node_functionality) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON() failed [node_functionality]");
        goto end;
    }
    if (!cJSON_IsString(node_functionality)) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON() failed [node_functionality]");
        goto end;
    }
    node_functionalityVariable = OpenAPI_node_functionality_FromString(node_functionality->valuestring);

    n_f_fqdn = cJSON_GetObjectItemCaseSensitive(nf_identificationJSON, "nFFqdn");
    if (n_f_fqdn) {
    if (!cJSON_IsString(n_f_fqdn) && !cJSON_IsNull(n_f_fqdn)) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON() failed [n_f_fqdn]");
        goto end;
    }
    }

    nf_identification_local_var = OpenAPI_nf_identification_create (
        n_f_name && !cJSON_IsNull(n_f_name) ? ogs_strdup(n_f_name->valuestring) : NULL,
        n_fipv4_address && !cJSON_IsNull(n_fipv4_address) ? ogs_strdup(n_fipv4_address->valuestring) : NULL,
        n_fipv6_address && !cJSON_IsNull(n_fipv6_address) ? ogs_strdup(n_fipv6_address->valuestring) : NULL,
        n_fplmnid ? n_fplmnid_local_nonprim : NULL,
        node_functionalityVariable,
        n_f_fqdn && !cJSON_IsNull(n_f_fqdn) ? ogs_strdup(n_f_fqdn->valuestring) : NULL
    );

    return nf_identification_local_var;
end:
    if (n_fplmnid_local_nonprim) {
        OpenAPI_plmn_id_free(n_fplmnid_local_nonprim);
        n_fplmnid_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nf_identification_t *OpenAPI_nf_identification_copy(OpenAPI_nf_identification_t *dst, OpenAPI_nf_identification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_identification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_identification_convertToJSON() failed");
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

    OpenAPI_nf_identification_free(dst);
    dst = OpenAPI_nf_identification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

