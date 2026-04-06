
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alt_qos_param_set.h"

OpenAPI_alt_qos_param_set_t *OpenAPI_alt_qos_param_set_create(
    char *gfbr_dl,
    char *gfbr_ul,
    bool is_pdb,
    int pdb,
    char *per
)
{
    OpenAPI_alt_qos_param_set_t *alt_qos_param_set_local_var = ogs_malloc(sizeof(OpenAPI_alt_qos_param_set_t));
    ogs_assert(alt_qos_param_set_local_var);

    alt_qos_param_set_local_var->gfbr_dl = gfbr_dl;
    alt_qos_param_set_local_var->gfbr_ul = gfbr_ul;
    alt_qos_param_set_local_var->is_pdb = is_pdb;
    alt_qos_param_set_local_var->pdb = pdb;
    alt_qos_param_set_local_var->per = per;

    return alt_qos_param_set_local_var;
}

void OpenAPI_alt_qos_param_set_free(OpenAPI_alt_qos_param_set_t *alt_qos_param_set)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == alt_qos_param_set) {
        return;
    }
    if (alt_qos_param_set->gfbr_dl) {
        ogs_free(alt_qos_param_set->gfbr_dl);
        alt_qos_param_set->gfbr_dl = NULL;
    }
    if (alt_qos_param_set->gfbr_ul) {
        ogs_free(alt_qos_param_set->gfbr_ul);
        alt_qos_param_set->gfbr_ul = NULL;
    }
    if (alt_qos_param_set->per) {
        ogs_free(alt_qos_param_set->per);
        alt_qos_param_set->per = NULL;
    }
    ogs_free(alt_qos_param_set);
}

cJSON *OpenAPI_alt_qos_param_set_convertToJSON(OpenAPI_alt_qos_param_set_t *alt_qos_param_set)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (alt_qos_param_set == NULL) {
        ogs_error("OpenAPI_alt_qos_param_set_convertToJSON() failed [AltQosParamSet]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (alt_qos_param_set->gfbr_dl) {
    if (cJSON_AddStringToObject(item, "gfbrDl", alt_qos_param_set->gfbr_dl) == NULL) {
        ogs_error("OpenAPI_alt_qos_param_set_convertToJSON() failed [gfbr_dl]");
        goto end;
    }
    }

    if (alt_qos_param_set->gfbr_ul) {
    if (cJSON_AddStringToObject(item, "gfbrUl", alt_qos_param_set->gfbr_ul) == NULL) {
        ogs_error("OpenAPI_alt_qos_param_set_convertToJSON() failed [gfbr_ul]");
        goto end;
    }
    }

    if (alt_qos_param_set->is_pdb) {
    if (cJSON_AddNumberToObject(item, "pdb", alt_qos_param_set->pdb) == NULL) {
        ogs_error("OpenAPI_alt_qos_param_set_convertToJSON() failed [pdb]");
        goto end;
    }
    }

    if (alt_qos_param_set->per) {
    if (cJSON_AddStringToObject(item, "per", alt_qos_param_set->per) == NULL) {
        ogs_error("OpenAPI_alt_qos_param_set_convertToJSON() failed [per]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_alt_qos_param_set_t *OpenAPI_alt_qos_param_set_parseFromJSON(cJSON *alt_qos_param_setJSON)
{
    OpenAPI_alt_qos_param_set_t *alt_qos_param_set_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *gfbr_dl = NULL;
    cJSON *gfbr_ul = NULL;
    cJSON *pdb = NULL;
    cJSON *per = NULL;
    gfbr_dl = cJSON_GetObjectItemCaseSensitive(alt_qos_param_setJSON, "gfbrDl");
    if (gfbr_dl) {
    if (!cJSON_IsString(gfbr_dl) && !cJSON_IsNull(gfbr_dl)) {
        ogs_error("OpenAPI_alt_qos_param_set_parseFromJSON() failed [gfbr_dl]");
        goto end;
    }
    }

    gfbr_ul = cJSON_GetObjectItemCaseSensitive(alt_qos_param_setJSON, "gfbrUl");
    if (gfbr_ul) {
    if (!cJSON_IsString(gfbr_ul) && !cJSON_IsNull(gfbr_ul)) {
        ogs_error("OpenAPI_alt_qos_param_set_parseFromJSON() failed [gfbr_ul]");
        goto end;
    }
    }

    pdb = cJSON_GetObjectItemCaseSensitive(alt_qos_param_setJSON, "pdb");
    if (pdb) {
    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_alt_qos_param_set_parseFromJSON() failed [pdb]");
        goto end;
    }
    }

    per = cJSON_GetObjectItemCaseSensitive(alt_qos_param_setJSON, "per");
    if (per) {
    if (!cJSON_IsString(per) && !cJSON_IsNull(per)) {
        ogs_error("OpenAPI_alt_qos_param_set_parseFromJSON() failed [per]");
        goto end;
    }
    }

    alt_qos_param_set_local_var = OpenAPI_alt_qos_param_set_create (
        gfbr_dl && !cJSON_IsNull(gfbr_dl) ? ogs_strdup(gfbr_dl->valuestring) : NULL,
        gfbr_ul && !cJSON_IsNull(gfbr_ul) ? ogs_strdup(gfbr_ul->valuestring) : NULL,
        pdb ? true : false,
        pdb ? pdb->valuedouble : 0,
        per && !cJSON_IsNull(per) ? ogs_strdup(per->valuestring) : NULL
    );

    return alt_qos_param_set_local_var;
end:
    return NULL;
}

OpenAPI_alt_qos_param_set_t *OpenAPI_alt_qos_param_set_copy(OpenAPI_alt_qos_param_set_t *dst, OpenAPI_alt_qos_param_set_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_alt_qos_param_set_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_alt_qos_param_set_convertToJSON() failed");
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

    OpenAPI_alt_qos_param_set_free(dst);
    dst = OpenAPI_alt_qos_param_set_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

