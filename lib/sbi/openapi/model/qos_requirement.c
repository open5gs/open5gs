
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_requirement.h"

OpenAPI_qos_requirement_t *OpenAPI_qos_requirement_create(
    bool is__5qi,
    int _5qi,
    char *gfbr_ul,
    char *gfbr_dl,
    OpenAPI_qos_resource_type_e res_type,
    bool is_pdb,
    int pdb,
    char *per
)
{
    OpenAPI_qos_requirement_t *qos_requirement_local_var = ogs_malloc(sizeof(OpenAPI_qos_requirement_t));
    ogs_assert(qos_requirement_local_var);

    qos_requirement_local_var->is__5qi = is__5qi;
    qos_requirement_local_var->_5qi = _5qi;
    qos_requirement_local_var->gfbr_ul = gfbr_ul;
    qos_requirement_local_var->gfbr_dl = gfbr_dl;
    qos_requirement_local_var->res_type = res_type;
    qos_requirement_local_var->is_pdb = is_pdb;
    qos_requirement_local_var->pdb = pdb;
    qos_requirement_local_var->per = per;

    return qos_requirement_local_var;
}

void OpenAPI_qos_requirement_free(OpenAPI_qos_requirement_t *qos_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_requirement) {
        return;
    }
    if (qos_requirement->gfbr_ul) {
        ogs_free(qos_requirement->gfbr_ul);
        qos_requirement->gfbr_ul = NULL;
    }
    if (qos_requirement->gfbr_dl) {
        ogs_free(qos_requirement->gfbr_dl);
        qos_requirement->gfbr_dl = NULL;
    }
    if (qos_requirement->per) {
        ogs_free(qos_requirement->per);
        qos_requirement->per = NULL;
    }
    ogs_free(qos_requirement);
}

cJSON *OpenAPI_qos_requirement_convertToJSON(OpenAPI_qos_requirement_t *qos_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_requirement == NULL) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed [QosRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_requirement->is__5qi) {
    if (cJSON_AddNumberToObject(item, "5qi", qos_requirement->_5qi) == NULL) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed [_5qi]");
        goto end;
    }
    }

    if (qos_requirement->gfbr_ul) {
    if (cJSON_AddStringToObject(item, "gfbrUl", qos_requirement->gfbr_ul) == NULL) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed [gfbr_ul]");
        goto end;
    }
    }

    if (qos_requirement->gfbr_dl) {
    if (cJSON_AddStringToObject(item, "gfbrDl", qos_requirement->gfbr_dl) == NULL) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed [gfbr_dl]");
        goto end;
    }
    }

    if (qos_requirement->res_type != OpenAPI_qos_resource_type_NULL) {
    if (cJSON_AddStringToObject(item, "resType", OpenAPI_qos_resource_type_ToString(qos_requirement->res_type)) == NULL) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed [res_type]");
        goto end;
    }
    }

    if (qos_requirement->is_pdb) {
    if (cJSON_AddNumberToObject(item, "pdb", qos_requirement->pdb) == NULL) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed [pdb]");
        goto end;
    }
    }

    if (qos_requirement->per) {
    if (cJSON_AddStringToObject(item, "per", qos_requirement->per) == NULL) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed [per]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_requirement_t *OpenAPI_qos_requirement_parseFromJSON(cJSON *qos_requirementJSON)
{
    OpenAPI_qos_requirement_t *qos_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_5qi = NULL;
    cJSON *gfbr_ul = NULL;
    cJSON *gfbr_dl = NULL;
    cJSON *res_type = NULL;
    OpenAPI_qos_resource_type_e res_typeVariable = 0;
    cJSON *pdb = NULL;
    cJSON *per = NULL;
    _5qi = cJSON_GetObjectItemCaseSensitive(qos_requirementJSON, "5qi");
    if (_5qi) {
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_qos_requirement_parseFromJSON() failed [_5qi]");
        goto end;
    }
    }

    gfbr_ul = cJSON_GetObjectItemCaseSensitive(qos_requirementJSON, "gfbrUl");
    if (gfbr_ul) {
    if (!cJSON_IsString(gfbr_ul) && !cJSON_IsNull(gfbr_ul)) {
        ogs_error("OpenAPI_qos_requirement_parseFromJSON() failed [gfbr_ul]");
        goto end;
    }
    }

    gfbr_dl = cJSON_GetObjectItemCaseSensitive(qos_requirementJSON, "gfbrDl");
    if (gfbr_dl) {
    if (!cJSON_IsString(gfbr_dl) && !cJSON_IsNull(gfbr_dl)) {
        ogs_error("OpenAPI_qos_requirement_parseFromJSON() failed [gfbr_dl]");
        goto end;
    }
    }

    res_type = cJSON_GetObjectItemCaseSensitive(qos_requirementJSON, "resType");
    if (res_type) {
    if (!cJSON_IsString(res_type)) {
        ogs_error("OpenAPI_qos_requirement_parseFromJSON() failed [res_type]");
        goto end;
    }
    res_typeVariable = OpenAPI_qos_resource_type_FromString(res_type->valuestring);
    }

    pdb = cJSON_GetObjectItemCaseSensitive(qos_requirementJSON, "pdb");
    if (pdb) {
    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_qos_requirement_parseFromJSON() failed [pdb]");
        goto end;
    }
    }

    per = cJSON_GetObjectItemCaseSensitive(qos_requirementJSON, "per");
    if (per) {
    if (!cJSON_IsString(per) && !cJSON_IsNull(per)) {
        ogs_error("OpenAPI_qos_requirement_parseFromJSON() failed [per]");
        goto end;
    }
    }

    qos_requirement_local_var = OpenAPI_qos_requirement_create (
        _5qi ? true : false,
        _5qi ? _5qi->valuedouble : 0,
        gfbr_ul && !cJSON_IsNull(gfbr_ul) ? ogs_strdup(gfbr_ul->valuestring) : NULL,
        gfbr_dl && !cJSON_IsNull(gfbr_dl) ? ogs_strdup(gfbr_dl->valuestring) : NULL,
        res_type ? res_typeVariable : 0,
        pdb ? true : false,
        pdb ? pdb->valuedouble : 0,
        per && !cJSON_IsNull(per) ? ogs_strdup(per->valuestring) : NULL
    );

    return qos_requirement_local_var;
end:
    return NULL;
}

OpenAPI_qos_requirement_t *OpenAPI_qos_requirement_copy(OpenAPI_qos_requirement_t *dst, OpenAPI_qos_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_requirement_convertToJSON() failed");
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

    OpenAPI_qos_requirement_free(dst);
    dst = OpenAPI_qos_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

