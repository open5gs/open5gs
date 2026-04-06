
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai_replace_info.h"

OpenAPI_snssai_replace_info_t *OpenAPI_snssai_replace_info_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_status_e status,
    OpenAPI_snssai_t *alt_snssai,
    OpenAPI_termination_indication_e ns_repl_termin_ind,
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_mitigation_info_t *mitigation_info
)
{
    OpenAPI_snssai_replace_info_t *snssai_replace_info_local_var = ogs_malloc(sizeof(OpenAPI_snssai_replace_info_t));
    ogs_assert(snssai_replace_info_local_var);

    snssai_replace_info_local_var->snssai = snssai;
    snssai_replace_info_local_var->status = status;
    snssai_replace_info_local_var->alt_snssai = alt_snssai;
    snssai_replace_info_local_var->ns_repl_termin_ind = ns_repl_termin_ind;
    snssai_replace_info_local_var->plmn_id = plmn_id;
    snssai_replace_info_local_var->mitigation_info = mitigation_info;

    return snssai_replace_info_local_var;
}

void OpenAPI_snssai_replace_info_free(OpenAPI_snssai_replace_info_t *snssai_replace_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == snssai_replace_info) {
        return;
    }
    if (snssai_replace_info->snssai) {
        OpenAPI_snssai_free(snssai_replace_info->snssai);
        snssai_replace_info->snssai = NULL;
    }
    if (snssai_replace_info->alt_snssai) {
        OpenAPI_snssai_free(snssai_replace_info->alt_snssai);
        snssai_replace_info->alt_snssai = NULL;
    }
    if (snssai_replace_info->plmn_id) {
        OpenAPI_plmn_id_free(snssai_replace_info->plmn_id);
        snssai_replace_info->plmn_id = NULL;
    }
    if (snssai_replace_info->mitigation_info) {
        OpenAPI_mitigation_info_free(snssai_replace_info->mitigation_info);
        snssai_replace_info->mitigation_info = NULL;
    }
    ogs_free(snssai_replace_info);
}

cJSON *OpenAPI_snssai_replace_info_convertToJSON(OpenAPI_snssai_replace_info_t *snssai_replace_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (snssai_replace_info == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [SnssaiReplaceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!snssai_replace_info->snssai) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(snssai_replace_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (snssai_replace_info->status != OpenAPI_snssai_status_NULL) {
    if (cJSON_AddStringToObject(item, "status", OpenAPI_snssai_status_ToString(snssai_replace_info->status)) == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [status]");
        goto end;
    }
    }

    if (snssai_replace_info->alt_snssai) {
    cJSON *alt_snssai_local_JSON = OpenAPI_snssai_convertToJSON(snssai_replace_info->alt_snssai);
    if (alt_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [alt_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "altSnssai", alt_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [alt_snssai]");
        goto end;
    }
    }

    if (snssai_replace_info->ns_repl_termin_ind != OpenAPI_termination_indication_NULL) {
    if (cJSON_AddStringToObject(item, "nsReplTerminInd", OpenAPI_termination_indication_ToString(snssai_replace_info->ns_repl_termin_ind)) == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [ns_repl_termin_ind]");
        goto end;
    }
    }

    if (snssai_replace_info->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(snssai_replace_info->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (snssai_replace_info->mitigation_info) {
    cJSON *mitigation_info_local_JSON = OpenAPI_mitigation_info_convertToJSON(snssai_replace_info->mitigation_info);
    if (mitigation_info_local_JSON == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [mitigation_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mitigationInfo", mitigation_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed [mitigation_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_snssai_replace_info_t *OpenAPI_snssai_replace_info_parseFromJSON(cJSON *snssai_replace_infoJSON)
{
    OpenAPI_snssai_replace_info_t *snssai_replace_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *status = NULL;
    OpenAPI_snssai_status_e statusVariable = 0;
    cJSON *alt_snssai = NULL;
    OpenAPI_snssai_t *alt_snssai_local_nonprim = NULL;
    cJSON *ns_repl_termin_ind = NULL;
    OpenAPI_termination_indication_e ns_repl_termin_indVariable = 0;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *mitigation_info = NULL;
    OpenAPI_mitigation_info_t *mitigation_info_local_nonprim = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(snssai_replace_infoJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_snssai_replace_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    status = cJSON_GetObjectItemCaseSensitive(snssai_replace_infoJSON, "status");
    if (status) {
    if (!cJSON_IsString(status)) {
        ogs_error("OpenAPI_snssai_replace_info_parseFromJSON() failed [status]");
        goto end;
    }
    statusVariable = OpenAPI_snssai_status_FromString(status->valuestring);
    }

    alt_snssai = cJSON_GetObjectItemCaseSensitive(snssai_replace_infoJSON, "altSnssai");
    if (alt_snssai) {
    alt_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(alt_snssai);
    if (!alt_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [alt_snssai]");
        goto end;
    }
    }

    ns_repl_termin_ind = cJSON_GetObjectItemCaseSensitive(snssai_replace_infoJSON, "nsReplTerminInd");
    if (ns_repl_termin_ind) {
    if (!cJSON_IsString(ns_repl_termin_ind)) {
        ogs_error("OpenAPI_snssai_replace_info_parseFromJSON() failed [ns_repl_termin_ind]");
        goto end;
    }
    ns_repl_termin_indVariable = OpenAPI_termination_indication_FromString(ns_repl_termin_ind->valuestring);
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(snssai_replace_infoJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    mitigation_info = cJSON_GetObjectItemCaseSensitive(snssai_replace_infoJSON, "mitigationInfo");
    if (mitigation_info) {
    mitigation_info_local_nonprim = OpenAPI_mitigation_info_parseFromJSON(mitigation_info);
    if (!mitigation_info_local_nonprim) {
        ogs_error("OpenAPI_mitigation_info_parseFromJSON failed [mitigation_info]");
        goto end;
    }
    }

    snssai_replace_info_local_var = OpenAPI_snssai_replace_info_create (
        snssai_local_nonprim,
        status ? statusVariable : 0,
        alt_snssai ? alt_snssai_local_nonprim : NULL,
        ns_repl_termin_ind ? ns_repl_termin_indVariable : 0,
        plmn_id ? plmn_id_local_nonprim : NULL,
        mitigation_info ? mitigation_info_local_nonprim : NULL
    );

    return snssai_replace_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (alt_snssai_local_nonprim) {
        OpenAPI_snssai_free(alt_snssai_local_nonprim);
        alt_snssai_local_nonprim = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (mitigation_info_local_nonprim) {
        OpenAPI_mitigation_info_free(mitigation_info_local_nonprim);
        mitigation_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_snssai_replace_info_t *OpenAPI_snssai_replace_info_copy(OpenAPI_snssai_replace_info_t *dst, OpenAPI_snssai_replace_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snssai_replace_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snssai_replace_info_convertToJSON() failed");
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

    OpenAPI_snssai_replace_info_free(dst);
    dst = OpenAPI_snssai_replace_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

