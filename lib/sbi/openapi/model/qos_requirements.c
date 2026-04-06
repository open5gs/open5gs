
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_requirements.h"

OpenAPI_qos_requirements_t *OpenAPI_qos_requirements_create(
    char *mar_bw_ul,
    char *mar_bw_dl,
    char *mir_bw_ul,
    char *mir_bw_dl,
    bool is_max_burst_size,
    int max_burst_size,
    bool is_ext_max_burst_size,
    int ext_max_burst_size,
    bool is_pdb,
    int pdb,
    char *per,
    bool is_prior_level,
    int prior_level
)
{
    OpenAPI_qos_requirements_t *qos_requirements_local_var = ogs_malloc(sizeof(OpenAPI_qos_requirements_t));
    ogs_assert(qos_requirements_local_var);

    qos_requirements_local_var->mar_bw_ul = mar_bw_ul;
    qos_requirements_local_var->mar_bw_dl = mar_bw_dl;
    qos_requirements_local_var->mir_bw_ul = mir_bw_ul;
    qos_requirements_local_var->mir_bw_dl = mir_bw_dl;
    qos_requirements_local_var->is_max_burst_size = is_max_burst_size;
    qos_requirements_local_var->max_burst_size = max_burst_size;
    qos_requirements_local_var->is_ext_max_burst_size = is_ext_max_burst_size;
    qos_requirements_local_var->ext_max_burst_size = ext_max_burst_size;
    qos_requirements_local_var->is_pdb = is_pdb;
    qos_requirements_local_var->pdb = pdb;
    qos_requirements_local_var->per = per;
    qos_requirements_local_var->is_prior_level = is_prior_level;
    qos_requirements_local_var->prior_level = prior_level;

    return qos_requirements_local_var;
}

void OpenAPI_qos_requirements_free(OpenAPI_qos_requirements_t *qos_requirements)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_requirements) {
        return;
    }
    if (qos_requirements->mar_bw_ul) {
        ogs_free(qos_requirements->mar_bw_ul);
        qos_requirements->mar_bw_ul = NULL;
    }
    if (qos_requirements->mar_bw_dl) {
        ogs_free(qos_requirements->mar_bw_dl);
        qos_requirements->mar_bw_dl = NULL;
    }
    if (qos_requirements->mir_bw_ul) {
        ogs_free(qos_requirements->mir_bw_ul);
        qos_requirements->mir_bw_ul = NULL;
    }
    if (qos_requirements->mir_bw_dl) {
        ogs_free(qos_requirements->mir_bw_dl);
        qos_requirements->mir_bw_dl = NULL;
    }
    if (qos_requirements->per) {
        ogs_free(qos_requirements->per);
        qos_requirements->per = NULL;
    }
    ogs_free(qos_requirements);
}

cJSON *OpenAPI_qos_requirements_convertToJSON(OpenAPI_qos_requirements_t *qos_requirements)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_requirements == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [QosRequirements]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_requirements->mar_bw_ul) {
    if (cJSON_AddStringToObject(item, "marBwUl", qos_requirements->mar_bw_ul) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    if (qos_requirements->mar_bw_dl) {
    if (cJSON_AddStringToObject(item, "marBwDl", qos_requirements->mar_bw_dl) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    if (qos_requirements->mir_bw_ul) {
    if (cJSON_AddStringToObject(item, "mirBwUl", qos_requirements->mir_bw_ul) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

    if (qos_requirements->mir_bw_dl) {
    if (cJSON_AddStringToObject(item, "mirBwDl", qos_requirements->mir_bw_dl) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    if (qos_requirements->is_max_burst_size) {
    if (cJSON_AddNumberToObject(item, "maxBurstSize", qos_requirements->max_burst_size) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [max_burst_size]");
        goto end;
    }
    }

    if (qos_requirements->is_ext_max_burst_size) {
    if (cJSON_AddNumberToObject(item, "extMaxBurstSize", qos_requirements->ext_max_burst_size) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [ext_max_burst_size]");
        goto end;
    }
    }

    if (qos_requirements->is_pdb) {
    if (cJSON_AddNumberToObject(item, "pdb", qos_requirements->pdb) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [pdb]");
        goto end;
    }
    }

    if (qos_requirements->per) {
    if (cJSON_AddStringToObject(item, "per", qos_requirements->per) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [per]");
        goto end;
    }
    }

    if (qos_requirements->is_prior_level) {
    if (cJSON_AddNumberToObject(item, "priorLevel", qos_requirements->prior_level) == NULL) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed [prior_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_requirements_t *OpenAPI_qos_requirements_parseFromJSON(cJSON *qos_requirementsJSON)
{
    OpenAPI_qos_requirements_t *qos_requirements_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mar_bw_ul = NULL;
    cJSON *mar_bw_dl = NULL;
    cJSON *mir_bw_ul = NULL;
    cJSON *mir_bw_dl = NULL;
    cJSON *max_burst_size = NULL;
    cJSON *ext_max_burst_size = NULL;
    cJSON *pdb = NULL;
    cJSON *per = NULL;
    cJSON *prior_level = NULL;
    mar_bw_ul = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "marBwUl");
    if (mar_bw_ul) {
    if (!cJSON_IsString(mar_bw_ul) && !cJSON_IsNull(mar_bw_ul)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    mar_bw_dl = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "marBwDl");
    if (mar_bw_dl) {
    if (!cJSON_IsString(mar_bw_dl) && !cJSON_IsNull(mar_bw_dl)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    mir_bw_ul = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "mirBwUl");
    if (mir_bw_ul) {
    if (!cJSON_IsString(mir_bw_ul) && !cJSON_IsNull(mir_bw_ul)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

    mir_bw_dl = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "mirBwDl");
    if (mir_bw_dl) {
    if (!cJSON_IsString(mir_bw_dl) && !cJSON_IsNull(mir_bw_dl)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    max_burst_size = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "maxBurstSize");
    if (max_burst_size) {
    if (!cJSON_IsNumber(max_burst_size)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [max_burst_size]");
        goto end;
    }
    }

    ext_max_burst_size = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "extMaxBurstSize");
    if (ext_max_burst_size) {
    if (!cJSON_IsNumber(ext_max_burst_size)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [ext_max_burst_size]");
        goto end;
    }
    }

    pdb = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "pdb");
    if (pdb) {
    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [pdb]");
        goto end;
    }
    }

    per = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "per");
    if (per) {
    if (!cJSON_IsString(per) && !cJSON_IsNull(per)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [per]");
        goto end;
    }
    }

    prior_level = cJSON_GetObjectItemCaseSensitive(qos_requirementsJSON, "priorLevel");
    if (prior_level) {
    if (!cJSON_IsNumber(prior_level)) {
        ogs_error("OpenAPI_qos_requirements_parseFromJSON() failed [prior_level]");
        goto end;
    }
    }

    qos_requirements_local_var = OpenAPI_qos_requirements_create (
        mar_bw_ul && !cJSON_IsNull(mar_bw_ul) ? ogs_strdup(mar_bw_ul->valuestring) : NULL,
        mar_bw_dl && !cJSON_IsNull(mar_bw_dl) ? ogs_strdup(mar_bw_dl->valuestring) : NULL,
        mir_bw_ul && !cJSON_IsNull(mir_bw_ul) ? ogs_strdup(mir_bw_ul->valuestring) : NULL,
        mir_bw_dl && !cJSON_IsNull(mir_bw_dl) ? ogs_strdup(mir_bw_dl->valuestring) : NULL,
        max_burst_size ? true : false,
        max_burst_size ? max_burst_size->valuedouble : 0,
        ext_max_burst_size ? true : false,
        ext_max_burst_size ? ext_max_burst_size->valuedouble : 0,
        pdb ? true : false,
        pdb ? pdb->valuedouble : 0,
        per && !cJSON_IsNull(per) ? ogs_strdup(per->valuestring) : NULL,
        prior_level ? true : false,
        prior_level ? prior_level->valuedouble : 0
    );

    return qos_requirements_local_var;
end:
    return NULL;
}

OpenAPI_qos_requirements_t *OpenAPI_qos_requirements_copy(OpenAPI_qos_requirements_t *dst, OpenAPI_qos_requirements_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_requirements_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_requirements_convertToJSON() failed");
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

    OpenAPI_qos_requirements_free(dst);
    dst = OpenAPI_qos_requirements_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

