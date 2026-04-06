
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mitigation_info.h"

OpenAPI_mitigation_info_t *OpenAPI_mitigation_info_create(
    bool is_perc_value_num_ues,
    int perc_value_num_ues,
    bool is_new_ues_ind,
    int new_ues_ind
)
{
    OpenAPI_mitigation_info_t *mitigation_info_local_var = ogs_malloc(sizeof(OpenAPI_mitigation_info_t));
    ogs_assert(mitigation_info_local_var);

    mitigation_info_local_var->is_perc_value_num_ues = is_perc_value_num_ues;
    mitigation_info_local_var->perc_value_num_ues = perc_value_num_ues;
    mitigation_info_local_var->is_new_ues_ind = is_new_ues_ind;
    mitigation_info_local_var->new_ues_ind = new_ues_ind;

    return mitigation_info_local_var;
}

void OpenAPI_mitigation_info_free(OpenAPI_mitigation_info_t *mitigation_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mitigation_info) {
        return;
    }
    ogs_free(mitigation_info);
}

cJSON *OpenAPI_mitigation_info_convertToJSON(OpenAPI_mitigation_info_t *mitigation_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mitigation_info == NULL) {
        ogs_error("OpenAPI_mitigation_info_convertToJSON() failed [MitigationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mitigation_info->is_perc_value_num_ues) {
    if (cJSON_AddNumberToObject(item, "percValueNumUes", mitigation_info->perc_value_num_ues) == NULL) {
        ogs_error("OpenAPI_mitigation_info_convertToJSON() failed [perc_value_num_ues]");
        goto end;
    }
    }

    if (mitigation_info->is_new_ues_ind) {
    if (cJSON_AddBoolToObject(item, "newUesInd", mitigation_info->new_ues_ind) == NULL) {
        ogs_error("OpenAPI_mitigation_info_convertToJSON() failed [new_ues_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mitigation_info_t *OpenAPI_mitigation_info_parseFromJSON(cJSON *mitigation_infoJSON)
{
    OpenAPI_mitigation_info_t *mitigation_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *perc_value_num_ues = NULL;
    cJSON *new_ues_ind = NULL;
    perc_value_num_ues = cJSON_GetObjectItemCaseSensitive(mitigation_infoJSON, "percValueNumUes");
    if (perc_value_num_ues) {
    if (!cJSON_IsNumber(perc_value_num_ues)) {
        ogs_error("OpenAPI_mitigation_info_parseFromJSON() failed [perc_value_num_ues]");
        goto end;
    }
    }

    new_ues_ind = cJSON_GetObjectItemCaseSensitive(mitigation_infoJSON, "newUesInd");
    if (new_ues_ind) {
    if (!cJSON_IsBool(new_ues_ind)) {
        ogs_error("OpenAPI_mitigation_info_parseFromJSON() failed [new_ues_ind]");
        goto end;
    }
    }

    mitigation_info_local_var = OpenAPI_mitigation_info_create (
        perc_value_num_ues ? true : false,
        perc_value_num_ues ? perc_value_num_ues->valuedouble : 0,
        new_ues_ind ? true : false,
        new_ues_ind ? new_ues_ind->valueint : 0
    );

    return mitigation_info_local_var;
end:
    return NULL;
}

OpenAPI_mitigation_info_t *OpenAPI_mitigation_info_copy(OpenAPI_mitigation_info_t *dst, OpenAPI_mitigation_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mitigation_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mitigation_info_convertToJSON() failed");
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

    OpenAPI_mitigation_info_free(dst);
    dst = OpenAPI_mitigation_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

