
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_sfc_requirement.h"

OpenAPI_af_sfc_requirement_t *OpenAPI_af_sfc_requirement_create(
    bool is_sfc_id_dl_null,
    char *sfc_id_dl,
    bool is_sfc_id_ul_null,
    char *sfc_id_ul,
    bool is_sp_val_null,
    OpenAPI_spatial_validity_rm_t *sp_val,
    bool is_metadata_null,
    char *metadata
)
{
    OpenAPI_af_sfc_requirement_t *af_sfc_requirement_local_var = ogs_malloc(sizeof(OpenAPI_af_sfc_requirement_t));
    ogs_assert(af_sfc_requirement_local_var);

    af_sfc_requirement_local_var->is_sfc_id_dl_null = is_sfc_id_dl_null;
    af_sfc_requirement_local_var->sfc_id_dl = sfc_id_dl;
    af_sfc_requirement_local_var->is_sfc_id_ul_null = is_sfc_id_ul_null;
    af_sfc_requirement_local_var->sfc_id_ul = sfc_id_ul;
    af_sfc_requirement_local_var->is_sp_val_null = is_sp_val_null;
    af_sfc_requirement_local_var->sp_val = sp_val;
    af_sfc_requirement_local_var->is_metadata_null = is_metadata_null;
    af_sfc_requirement_local_var->metadata = metadata;

    return af_sfc_requirement_local_var;
}

void OpenAPI_af_sfc_requirement_free(OpenAPI_af_sfc_requirement_t *af_sfc_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_sfc_requirement) {
        return;
    }
    if (af_sfc_requirement->sfc_id_dl) {
        ogs_free(af_sfc_requirement->sfc_id_dl);
        af_sfc_requirement->sfc_id_dl = NULL;
    }
    if (af_sfc_requirement->sfc_id_ul) {
        ogs_free(af_sfc_requirement->sfc_id_ul);
        af_sfc_requirement->sfc_id_ul = NULL;
    }
    if (af_sfc_requirement->sp_val) {
        OpenAPI_spatial_validity_rm_free(af_sfc_requirement->sp_val);
        af_sfc_requirement->sp_val = NULL;
    }
    if (af_sfc_requirement->metadata) {
        ogs_free(af_sfc_requirement->metadata);
        af_sfc_requirement->metadata = NULL;
    }
    ogs_free(af_sfc_requirement);
}

cJSON *OpenAPI_af_sfc_requirement_convertToJSON(OpenAPI_af_sfc_requirement_t *af_sfc_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_sfc_requirement == NULL) {
        ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [AfSfcRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_sfc_requirement->sfc_id_dl) {
    if (cJSON_AddStringToObject(item, "sfcIdDl", af_sfc_requirement->sfc_id_dl) == NULL) {
        ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [sfc_id_dl]");
        goto end;
    }
    } else if (af_sfc_requirement->is_sfc_id_dl_null) {
        if (cJSON_AddNullToObject(item, "sfcIdDl") == NULL) {
            ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [sfc_id_dl]");
            goto end;
        }
    }

    if (af_sfc_requirement->sfc_id_ul) {
    if (cJSON_AddStringToObject(item, "sfcIdUl", af_sfc_requirement->sfc_id_ul) == NULL) {
        ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [sfc_id_ul]");
        goto end;
    }
    } else if (af_sfc_requirement->is_sfc_id_ul_null) {
        if (cJSON_AddNullToObject(item, "sfcIdUl") == NULL) {
            ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [sfc_id_ul]");
            goto end;
        }
    }

    if (af_sfc_requirement->sp_val) {
    cJSON *sp_val_local_JSON = OpenAPI_spatial_validity_rm_convertToJSON(af_sfc_requirement->sp_val);
    if (sp_val_local_JSON == NULL) {
        ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [sp_val]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spVal", sp_val_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [sp_val]");
        goto end;
    }
    } else if (af_sfc_requirement->is_sp_val_null) {
        if (cJSON_AddNullToObject(item, "spVal") == NULL) {
            ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [sp_val]");
            goto end;
        }
    }

    if (af_sfc_requirement->metadata) {
    if (cJSON_AddStringToObject(item, "metadata", af_sfc_requirement->metadata) == NULL) {
        ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [metadata]");
        goto end;
    }
    } else if (af_sfc_requirement->is_metadata_null) {
        if (cJSON_AddNullToObject(item, "metadata") == NULL) {
            ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed [metadata]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_af_sfc_requirement_t *OpenAPI_af_sfc_requirement_parseFromJSON(cJSON *af_sfc_requirementJSON)
{
    OpenAPI_af_sfc_requirement_t *af_sfc_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sfc_id_dl = NULL;
    cJSON *sfc_id_ul = NULL;
    cJSON *sp_val = NULL;
    OpenAPI_spatial_validity_rm_t *sp_val_local_nonprim = NULL;
    cJSON *metadata = NULL;
    sfc_id_dl = cJSON_GetObjectItemCaseSensitive(af_sfc_requirementJSON, "sfcIdDl");
    if (sfc_id_dl) {
    if (!cJSON_IsNull(sfc_id_dl)) {
    if (!cJSON_IsString(sfc_id_dl) && !cJSON_IsNull(sfc_id_dl)) {
        ogs_error("OpenAPI_af_sfc_requirement_parseFromJSON() failed [sfc_id_dl]");
        goto end;
    }
    }
    }

    sfc_id_ul = cJSON_GetObjectItemCaseSensitive(af_sfc_requirementJSON, "sfcIdUl");
    if (sfc_id_ul) {
    if (!cJSON_IsNull(sfc_id_ul)) {
    if (!cJSON_IsString(sfc_id_ul) && !cJSON_IsNull(sfc_id_ul)) {
        ogs_error("OpenAPI_af_sfc_requirement_parseFromJSON() failed [sfc_id_ul]");
        goto end;
    }
    }
    }

    sp_val = cJSON_GetObjectItemCaseSensitive(af_sfc_requirementJSON, "spVal");
    if (sp_val) {
    if (!cJSON_IsNull(sp_val)) {
    sp_val_local_nonprim = OpenAPI_spatial_validity_rm_parseFromJSON(sp_val);
    if (!sp_val_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_rm_parseFromJSON failed [sp_val]");
        goto end;
    }
    }
    }

    metadata = cJSON_GetObjectItemCaseSensitive(af_sfc_requirementJSON, "metadata");
    if (metadata) {
    if (!cJSON_IsNull(metadata)) {
    if (!cJSON_IsString(metadata) && !cJSON_IsNull(metadata)) {
        ogs_error("OpenAPI_af_sfc_requirement_parseFromJSON() failed [metadata]");
        goto end;
    }
    }
    }

    af_sfc_requirement_local_var = OpenAPI_af_sfc_requirement_create (
        sfc_id_dl && cJSON_IsNull(sfc_id_dl) ? true : false,
        sfc_id_dl && !cJSON_IsNull(sfc_id_dl) ? ogs_strdup(sfc_id_dl->valuestring) : NULL,
        sfc_id_ul && cJSON_IsNull(sfc_id_ul) ? true : false,
        sfc_id_ul && !cJSON_IsNull(sfc_id_ul) ? ogs_strdup(sfc_id_ul->valuestring) : NULL,
        sp_val && cJSON_IsNull(sp_val) ? true : false,
        sp_val ? sp_val_local_nonprim : NULL,
        metadata && cJSON_IsNull(metadata) ? true : false,
        metadata && !cJSON_IsNull(metadata) ? ogs_strdup(metadata->valuestring) : NULL
    );

    return af_sfc_requirement_local_var;
end:
    if (sp_val_local_nonprim) {
        OpenAPI_spatial_validity_rm_free(sp_val_local_nonprim);
        sp_val_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_af_sfc_requirement_t *OpenAPI_af_sfc_requirement_copy(OpenAPI_af_sfc_requirement_t *dst, OpenAPI_af_sfc_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_sfc_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_sfc_requirement_convertToJSON() failed");
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

    OpenAPI_af_sfc_requirement_free(dst);
    dst = OpenAPI_af_sfc_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

