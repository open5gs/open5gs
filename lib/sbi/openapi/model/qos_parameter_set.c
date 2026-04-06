
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_parameter_set.h"

OpenAPI_qos_parameter_set_t *OpenAPI_qos_parameter_set_create(
    bool is_ext_max_burst_size,
    int ext_max_burst_size,
    char *gfbr_dl,
    char *gfbr_ul,
    char *max_bit_rate_dl,
    char *max_bit_rate_ul,
    bool is_max_burst_size,
    int max_burst_size,
    bool is_pdb,
    int pdb,
    char *per,
    bool is_prior_level,
    int prior_level
)
{
    OpenAPI_qos_parameter_set_t *qos_parameter_set_local_var = ogs_malloc(sizeof(OpenAPI_qos_parameter_set_t));
    ogs_assert(qos_parameter_set_local_var);

    qos_parameter_set_local_var->is_ext_max_burst_size = is_ext_max_burst_size;
    qos_parameter_set_local_var->ext_max_burst_size = ext_max_burst_size;
    qos_parameter_set_local_var->gfbr_dl = gfbr_dl;
    qos_parameter_set_local_var->gfbr_ul = gfbr_ul;
    qos_parameter_set_local_var->max_bit_rate_dl = max_bit_rate_dl;
    qos_parameter_set_local_var->max_bit_rate_ul = max_bit_rate_ul;
    qos_parameter_set_local_var->is_max_burst_size = is_max_burst_size;
    qos_parameter_set_local_var->max_burst_size = max_burst_size;
    qos_parameter_set_local_var->is_pdb = is_pdb;
    qos_parameter_set_local_var->pdb = pdb;
    qos_parameter_set_local_var->per = per;
    qos_parameter_set_local_var->is_prior_level = is_prior_level;
    qos_parameter_set_local_var->prior_level = prior_level;

    return qos_parameter_set_local_var;
}

void OpenAPI_qos_parameter_set_free(OpenAPI_qos_parameter_set_t *qos_parameter_set)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_parameter_set) {
        return;
    }
    if (qos_parameter_set->gfbr_dl) {
        ogs_free(qos_parameter_set->gfbr_dl);
        qos_parameter_set->gfbr_dl = NULL;
    }
    if (qos_parameter_set->gfbr_ul) {
        ogs_free(qos_parameter_set->gfbr_ul);
        qos_parameter_set->gfbr_ul = NULL;
    }
    if (qos_parameter_set->max_bit_rate_dl) {
        ogs_free(qos_parameter_set->max_bit_rate_dl);
        qos_parameter_set->max_bit_rate_dl = NULL;
    }
    if (qos_parameter_set->max_bit_rate_ul) {
        ogs_free(qos_parameter_set->max_bit_rate_ul);
        qos_parameter_set->max_bit_rate_ul = NULL;
    }
    if (qos_parameter_set->per) {
        ogs_free(qos_parameter_set->per);
        qos_parameter_set->per = NULL;
    }
    ogs_free(qos_parameter_set);
}

cJSON *OpenAPI_qos_parameter_set_convertToJSON(OpenAPI_qos_parameter_set_t *qos_parameter_set)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_parameter_set == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [QosParameterSet]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_parameter_set->is_ext_max_burst_size) {
    if (cJSON_AddNumberToObject(item, "extMaxBurstSize", qos_parameter_set->ext_max_burst_size) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [ext_max_burst_size]");
        goto end;
    }
    }

    if (qos_parameter_set->gfbr_dl) {
    if (cJSON_AddStringToObject(item, "gfbrDl", qos_parameter_set->gfbr_dl) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [gfbr_dl]");
        goto end;
    }
    }

    if (qos_parameter_set->gfbr_ul) {
    if (cJSON_AddStringToObject(item, "gfbrUl", qos_parameter_set->gfbr_ul) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [gfbr_ul]");
        goto end;
    }
    }

    if (qos_parameter_set->max_bit_rate_dl) {
    if (cJSON_AddStringToObject(item, "maxBitRateDl", qos_parameter_set->max_bit_rate_dl) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [max_bit_rate_dl]");
        goto end;
    }
    }

    if (qos_parameter_set->max_bit_rate_ul) {
    if (cJSON_AddStringToObject(item, "maxBitRateUl", qos_parameter_set->max_bit_rate_ul) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [max_bit_rate_ul]");
        goto end;
    }
    }

    if (qos_parameter_set->is_max_burst_size) {
    if (cJSON_AddNumberToObject(item, "maxBurstSize", qos_parameter_set->max_burst_size) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [max_burst_size]");
        goto end;
    }
    }

    if (qos_parameter_set->is_pdb) {
    if (cJSON_AddNumberToObject(item, "pdb", qos_parameter_set->pdb) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [pdb]");
        goto end;
    }
    }

    if (qos_parameter_set->per) {
    if (cJSON_AddStringToObject(item, "per", qos_parameter_set->per) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [per]");
        goto end;
    }
    }

    if (qos_parameter_set->is_prior_level) {
    if (cJSON_AddNumberToObject(item, "priorLevel", qos_parameter_set->prior_level) == NULL) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed [prior_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_parameter_set_t *OpenAPI_qos_parameter_set_parseFromJSON(cJSON *qos_parameter_setJSON)
{
    OpenAPI_qos_parameter_set_t *qos_parameter_set_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ext_max_burst_size = NULL;
    cJSON *gfbr_dl = NULL;
    cJSON *gfbr_ul = NULL;
    cJSON *max_bit_rate_dl = NULL;
    cJSON *max_bit_rate_ul = NULL;
    cJSON *max_burst_size = NULL;
    cJSON *pdb = NULL;
    cJSON *per = NULL;
    cJSON *prior_level = NULL;
    ext_max_burst_size = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "extMaxBurstSize");
    if (ext_max_burst_size) {
    if (!cJSON_IsNumber(ext_max_burst_size)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [ext_max_burst_size]");
        goto end;
    }
    }

    gfbr_dl = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "gfbrDl");
    if (gfbr_dl) {
    if (!cJSON_IsString(gfbr_dl) && !cJSON_IsNull(gfbr_dl)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [gfbr_dl]");
        goto end;
    }
    }

    gfbr_ul = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "gfbrUl");
    if (gfbr_ul) {
    if (!cJSON_IsString(gfbr_ul) && !cJSON_IsNull(gfbr_ul)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [gfbr_ul]");
        goto end;
    }
    }

    max_bit_rate_dl = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "maxBitRateDl");
    if (max_bit_rate_dl) {
    if (!cJSON_IsString(max_bit_rate_dl) && !cJSON_IsNull(max_bit_rate_dl)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [max_bit_rate_dl]");
        goto end;
    }
    }

    max_bit_rate_ul = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "maxBitRateUl");
    if (max_bit_rate_ul) {
    if (!cJSON_IsString(max_bit_rate_ul) && !cJSON_IsNull(max_bit_rate_ul)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [max_bit_rate_ul]");
        goto end;
    }
    }

    max_burst_size = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "maxBurstSize");
    if (max_burst_size) {
    if (!cJSON_IsNumber(max_burst_size)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [max_burst_size]");
        goto end;
    }
    }

    pdb = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "pdb");
    if (pdb) {
    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [pdb]");
        goto end;
    }
    }

    per = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "per");
    if (per) {
    if (!cJSON_IsString(per) && !cJSON_IsNull(per)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [per]");
        goto end;
    }
    }

    prior_level = cJSON_GetObjectItemCaseSensitive(qos_parameter_setJSON, "priorLevel");
    if (prior_level) {
    if (!cJSON_IsNumber(prior_level)) {
        ogs_error("OpenAPI_qos_parameter_set_parseFromJSON() failed [prior_level]");
        goto end;
    }
    }

    qos_parameter_set_local_var = OpenAPI_qos_parameter_set_create (
        ext_max_burst_size ? true : false,
        ext_max_burst_size ? ext_max_burst_size->valuedouble : 0,
        gfbr_dl && !cJSON_IsNull(gfbr_dl) ? ogs_strdup(gfbr_dl->valuestring) : NULL,
        gfbr_ul && !cJSON_IsNull(gfbr_ul) ? ogs_strdup(gfbr_ul->valuestring) : NULL,
        max_bit_rate_dl && !cJSON_IsNull(max_bit_rate_dl) ? ogs_strdup(max_bit_rate_dl->valuestring) : NULL,
        max_bit_rate_ul && !cJSON_IsNull(max_bit_rate_ul) ? ogs_strdup(max_bit_rate_ul->valuestring) : NULL,
        max_burst_size ? true : false,
        max_burst_size ? max_burst_size->valuedouble : 0,
        pdb ? true : false,
        pdb ? pdb->valuedouble : 0,
        per && !cJSON_IsNull(per) ? ogs_strdup(per->valuestring) : NULL,
        prior_level ? true : false,
        prior_level ? prior_level->valuedouble : 0
    );

    return qos_parameter_set_local_var;
end:
    return NULL;
}

OpenAPI_qos_parameter_set_t *OpenAPI_qos_parameter_set_copy(OpenAPI_qos_parameter_set_t *dst, OpenAPI_qos_parameter_set_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_parameter_set_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_parameter_set_convertToJSON() failed");
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

    OpenAPI_qos_parameter_set_free(dst);
    dst = OpenAPI_qos_parameter_set_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

