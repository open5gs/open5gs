
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "extended_problem_details_all_of.h"

OpenAPI_extended_problem_details_all_of_t *OpenAPI_extended_problem_details_all_of_create(
    OpenAPI_acceptable_service_info_t *acceptable_serv_info
)
{
    OpenAPI_extended_problem_details_all_of_t *extended_problem_details_all_of_local_var = ogs_malloc(sizeof(OpenAPI_extended_problem_details_all_of_t));
    ogs_assert(extended_problem_details_all_of_local_var);

    extended_problem_details_all_of_local_var->acceptable_serv_info = acceptable_serv_info;

    return extended_problem_details_all_of_local_var;
}

void OpenAPI_extended_problem_details_all_of_free(OpenAPI_extended_problem_details_all_of_t *extended_problem_details_all_of)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == extended_problem_details_all_of) {
        return;
    }
    if (extended_problem_details_all_of->acceptable_serv_info) {
        OpenAPI_acceptable_service_info_free(extended_problem_details_all_of->acceptable_serv_info);
        extended_problem_details_all_of->acceptable_serv_info = NULL;
    }
    ogs_free(extended_problem_details_all_of);
}

cJSON *OpenAPI_extended_problem_details_all_of_convertToJSON(OpenAPI_extended_problem_details_all_of_t *extended_problem_details_all_of)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (extended_problem_details_all_of == NULL) {
        ogs_error("OpenAPI_extended_problem_details_all_of_convertToJSON() failed [ExtendedProblemDetails_allOf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (extended_problem_details_all_of->acceptable_serv_info) {
    cJSON *acceptable_serv_info_local_JSON = OpenAPI_acceptable_service_info_convertToJSON(extended_problem_details_all_of->acceptable_serv_info);
    if (acceptable_serv_info_local_JSON == NULL) {
        ogs_error("OpenAPI_extended_problem_details_all_of_convertToJSON() failed [acceptable_serv_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "acceptableServInfo", acceptable_serv_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_extended_problem_details_all_of_convertToJSON() failed [acceptable_serv_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_extended_problem_details_all_of_t *OpenAPI_extended_problem_details_all_of_parseFromJSON(cJSON *extended_problem_details_all_ofJSON)
{
    OpenAPI_extended_problem_details_all_of_t *extended_problem_details_all_of_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *acceptable_serv_info = NULL;
    OpenAPI_acceptable_service_info_t *acceptable_serv_info_local_nonprim = NULL;
    acceptable_serv_info = cJSON_GetObjectItemCaseSensitive(extended_problem_details_all_ofJSON, "acceptableServInfo");
    if (acceptable_serv_info) {
    acceptable_serv_info_local_nonprim = OpenAPI_acceptable_service_info_parseFromJSON(acceptable_serv_info);
    if (!acceptable_serv_info_local_nonprim) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON failed [acceptable_serv_info]");
        goto end;
    }
    }

    extended_problem_details_all_of_local_var = OpenAPI_extended_problem_details_all_of_create (
        acceptable_serv_info ? acceptable_serv_info_local_nonprim : NULL
    );

    return extended_problem_details_all_of_local_var;
end:
    if (acceptable_serv_info_local_nonprim) {
        OpenAPI_acceptable_service_info_free(acceptable_serv_info_local_nonprim);
        acceptable_serv_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_extended_problem_details_all_of_t *OpenAPI_extended_problem_details_all_of_copy(OpenAPI_extended_problem_details_all_of_t *dst, OpenAPI_extended_problem_details_all_of_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_extended_problem_details_all_of_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_extended_problem_details_all_of_convertToJSON() failed");
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

    OpenAPI_extended_problem_details_all_of_free(dst);
    dst = OpenAPI_extended_problem_details_all_of_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

