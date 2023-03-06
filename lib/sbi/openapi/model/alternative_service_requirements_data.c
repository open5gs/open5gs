
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alternative_service_requirements_data.h"

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_create(
    char *alt_qos_param_set_ref,
    char *gbr_ul,
    char *gbr_dl,
    bool is_pdb,
    int pdb
)
{
    OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data_local_var = ogs_malloc(sizeof(OpenAPI_alternative_service_requirements_data_t));
    ogs_assert(alternative_service_requirements_data_local_var);

    alternative_service_requirements_data_local_var->alt_qos_param_set_ref = alt_qos_param_set_ref;
    alternative_service_requirements_data_local_var->gbr_ul = gbr_ul;
    alternative_service_requirements_data_local_var->gbr_dl = gbr_dl;
    alternative_service_requirements_data_local_var->is_pdb = is_pdb;
    alternative_service_requirements_data_local_var->pdb = pdb;

    return alternative_service_requirements_data_local_var;
}

void OpenAPI_alternative_service_requirements_data_free(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == alternative_service_requirements_data) {
        return;
    }
    if (alternative_service_requirements_data->alt_qos_param_set_ref) {
        ogs_free(alternative_service_requirements_data->alt_qos_param_set_ref);
        alternative_service_requirements_data->alt_qos_param_set_ref = NULL;
    }
    if (alternative_service_requirements_data->gbr_ul) {
        ogs_free(alternative_service_requirements_data->gbr_ul);
        alternative_service_requirements_data->gbr_ul = NULL;
    }
    if (alternative_service_requirements_data->gbr_dl) {
        ogs_free(alternative_service_requirements_data->gbr_dl);
        alternative_service_requirements_data->gbr_dl = NULL;
    }
    ogs_free(alternative_service_requirements_data);
}

cJSON *OpenAPI_alternative_service_requirements_data_convertToJSON(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (alternative_service_requirements_data == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [AlternativeServiceRequirementsData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!alternative_service_requirements_data->alt_qos_param_set_ref) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [alt_qos_param_set_ref]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "altQosParamSetRef", alternative_service_requirements_data->alt_qos_param_set_ref) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [alt_qos_param_set_ref]");
        goto end;
    }

    if (alternative_service_requirements_data->gbr_ul) {
    if (cJSON_AddStringToObject(item, "gbrUl", alternative_service_requirements_data->gbr_ul) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [gbr_ul]");
        goto end;
    }
    }

    if (alternative_service_requirements_data->gbr_dl) {
    if (cJSON_AddStringToObject(item, "gbrDl", alternative_service_requirements_data->gbr_dl) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [gbr_dl]");
        goto end;
    }
    }

    if (alternative_service_requirements_data->is_pdb) {
    if (cJSON_AddNumberToObject(item, "pdb", alternative_service_requirements_data->pdb) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdb]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_parseFromJSON(cJSON *alternative_service_requirements_dataJSON)
{
    OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *alt_qos_param_set_ref = NULL;
    cJSON *gbr_ul = NULL;
    cJSON *gbr_dl = NULL;
    cJSON *pdb = NULL;
    alt_qos_param_set_ref = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "altQosParamSetRef");
    if (!alt_qos_param_set_ref) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [alt_qos_param_set_ref]");
        goto end;
    }
    if (!cJSON_IsString(alt_qos_param_set_ref)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [alt_qos_param_set_ref]");
        goto end;
    }

    gbr_ul = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "gbrUl");
    if (gbr_ul) {
    if (!cJSON_IsString(gbr_ul) && !cJSON_IsNull(gbr_ul)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [gbr_ul]");
        goto end;
    }
    }

    gbr_dl = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "gbrDl");
    if (gbr_dl) {
    if (!cJSON_IsString(gbr_dl) && !cJSON_IsNull(gbr_dl)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [gbr_dl]");
        goto end;
    }
    }

    pdb = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "pdb");
    if (pdb) {
    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [pdb]");
        goto end;
    }
    }

    alternative_service_requirements_data_local_var = OpenAPI_alternative_service_requirements_data_create (
        ogs_strdup(alt_qos_param_set_ref->valuestring),
        gbr_ul && !cJSON_IsNull(gbr_ul) ? ogs_strdup(gbr_ul->valuestring) : NULL,
        gbr_dl && !cJSON_IsNull(gbr_dl) ? ogs_strdup(gbr_dl->valuestring) : NULL,
        pdb ? true : false,
        pdb ? pdb->valuedouble : 0
    );

    return alternative_service_requirements_data_local_var;
end:
    return NULL;
}

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_copy(OpenAPI_alternative_service_requirements_data_t *dst, OpenAPI_alternative_service_requirements_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_alternative_service_requirements_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed");
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

    OpenAPI_alternative_service_requirements_data_free(dst);
    dst = OpenAPI_alternative_service_requirements_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

