
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snpn_information.h"

OpenAPI_snpn_information_t *OpenAPI_snpn_information_create(
    OpenAPI_plmn_id_nid_t *s_npnid,
    OpenAPI_access_type_e access_type,
    char *n3_iwf_fqdn
)
{
    OpenAPI_snpn_information_t *snpn_information_local_var = ogs_malloc(sizeof(OpenAPI_snpn_information_t));
    ogs_assert(snpn_information_local_var);

    snpn_information_local_var->s_npnid = s_npnid;
    snpn_information_local_var->access_type = access_type;
    snpn_information_local_var->n3_iwf_fqdn = n3_iwf_fqdn;

    return snpn_information_local_var;
}

void OpenAPI_snpn_information_free(OpenAPI_snpn_information_t *snpn_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == snpn_information) {
        return;
    }
    if (snpn_information->s_npnid) {
        OpenAPI_plmn_id_nid_free(snpn_information->s_npnid);
        snpn_information->s_npnid = NULL;
    }
    if (snpn_information->n3_iwf_fqdn) {
        ogs_free(snpn_information->n3_iwf_fqdn);
        snpn_information->n3_iwf_fqdn = NULL;
    }
    ogs_free(snpn_information);
}

cJSON *OpenAPI_snpn_information_convertToJSON(OpenAPI_snpn_information_t *snpn_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (snpn_information == NULL) {
        ogs_error("OpenAPI_snpn_information_convertToJSON() failed [SNPNInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!snpn_information->s_npnid) {
        ogs_error("OpenAPI_snpn_information_convertToJSON() failed [s_npnid]");
        return NULL;
    }
    cJSON *s_npnid_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(snpn_information->s_npnid);
    if (s_npnid_local_JSON == NULL) {
        ogs_error("OpenAPI_snpn_information_convertToJSON() failed [s_npnid]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNPNID", s_npnid_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snpn_information_convertToJSON() failed [s_npnid]");
        goto end;
    }

    if (snpn_information->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(snpn_information->access_type)) == NULL) {
        ogs_error("OpenAPI_snpn_information_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (snpn_information->n3_iwf_fqdn) {
    if (cJSON_AddStringToObject(item, "n3IwfFqdn", snpn_information->n3_iwf_fqdn) == NULL) {
        ogs_error("OpenAPI_snpn_information_convertToJSON() failed [n3_iwf_fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_snpn_information_t *OpenAPI_snpn_information_parseFromJSON(cJSON *snpn_informationJSON)
{
    OpenAPI_snpn_information_t *snpn_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_npnid = NULL;
    OpenAPI_plmn_id_nid_t *s_npnid_local_nonprim = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *n3_iwf_fqdn = NULL;
    s_npnid = cJSON_GetObjectItemCaseSensitive(snpn_informationJSON, "sNPNID");
    if (!s_npnid) {
        ogs_error("OpenAPI_snpn_information_parseFromJSON() failed [s_npnid]");
        goto end;
    }
    s_npnid_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(s_npnid);
    if (!s_npnid_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [s_npnid]");
        goto end;
    }

    access_type = cJSON_GetObjectItemCaseSensitive(snpn_informationJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_snpn_information_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    n3_iwf_fqdn = cJSON_GetObjectItemCaseSensitive(snpn_informationJSON, "n3IwfFqdn");
    if (n3_iwf_fqdn) {
    if (!cJSON_IsString(n3_iwf_fqdn) && !cJSON_IsNull(n3_iwf_fqdn)) {
        ogs_error("OpenAPI_snpn_information_parseFromJSON() failed [n3_iwf_fqdn]");
        goto end;
    }
    }

    snpn_information_local_var = OpenAPI_snpn_information_create (
        s_npnid_local_nonprim,
        access_type ? access_typeVariable : 0,
        n3_iwf_fqdn && !cJSON_IsNull(n3_iwf_fqdn) ? ogs_strdup(n3_iwf_fqdn->valuestring) : NULL
    );

    return snpn_information_local_var;
end:
    if (s_npnid_local_nonprim) {
        OpenAPI_plmn_id_nid_free(s_npnid_local_nonprim);
        s_npnid_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_snpn_information_t *OpenAPI_snpn_information_copy(OpenAPI_snpn_information_t *dst, OpenAPI_snpn_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snpn_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snpn_information_convertToJSON() failed");
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

    OpenAPI_snpn_information_free(dst);
    dst = OpenAPI_snpn_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

