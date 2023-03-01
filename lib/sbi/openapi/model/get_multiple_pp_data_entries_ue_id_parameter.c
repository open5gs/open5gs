
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "get_multiple_pp_data_entries_ue_id_parameter.h"

OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_create(
)
{
    OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *get_multiple_pp_data_entries_ue_id_parameter_local_var = ogs_malloc(sizeof(OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t));
    ogs_assert(get_multiple_pp_data_entries_ue_id_parameter_local_var);


    return get_multiple_pp_data_entries_ue_id_parameter_local_var;
}

void OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_free(OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *get_multiple_pp_data_entries_ue_id_parameter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == get_multiple_pp_data_entries_ue_id_parameter) {
        return;
    }
    ogs_free(get_multiple_pp_data_entries_ue_id_parameter);
}

cJSON *OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_convertToJSON(OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *get_multiple_pp_data_entries_ue_id_parameter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (get_multiple_pp_data_entries_ue_id_parameter == NULL) {
        ogs_error("OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_convertToJSON() failed [Get_Multiple_PP_Data_Entries_ueId_parameter]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_parseFromJSON(cJSON *get_multiple_pp_data_entries_ue_id_parameterJSON)
{
    OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *get_multiple_pp_data_entries_ue_id_parameter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    get_multiple_pp_data_entries_ue_id_parameter_local_var = OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_create (
    );

    return get_multiple_pp_data_entries_ue_id_parameter_local_var;
end:
    return NULL;
}

OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_copy(OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *dst, OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_convertToJSON() failed");
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

    OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_free(dst);
    dst = OpenAPI_get_multiple_pp_data_entries_ue_id_parameter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

