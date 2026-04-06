
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rslp_information.h"

OpenAPI_rslp_information_t *OpenAPI_rslp_information_create(
    OpenAPI_n2_info_content_t *n2_pc5_rslp_pol
)
{
    OpenAPI_rslp_information_t *rslp_information_local_var = ogs_malloc(sizeof(OpenAPI_rslp_information_t));
    ogs_assert(rslp_information_local_var);

    rslp_information_local_var->n2_pc5_rslp_pol = n2_pc5_rslp_pol;

    return rslp_information_local_var;
}

void OpenAPI_rslp_information_free(OpenAPI_rslp_information_t *rslp_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rslp_information) {
        return;
    }
    if (rslp_information->n2_pc5_rslp_pol) {
        OpenAPI_n2_info_content_free(rslp_information->n2_pc5_rslp_pol);
        rslp_information->n2_pc5_rslp_pol = NULL;
    }
    ogs_free(rslp_information);
}

cJSON *OpenAPI_rslp_information_convertToJSON(OpenAPI_rslp_information_t *rslp_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rslp_information == NULL) {
        ogs_error("OpenAPI_rslp_information_convertToJSON() failed [RslpInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rslp_information->n2_pc5_rslp_pol) {
    cJSON *n2_pc5_rslp_pol_local_JSON = OpenAPI_n2_info_content_convertToJSON(rslp_information->n2_pc5_rslp_pol);
    if (n2_pc5_rslp_pol_local_JSON == NULL) {
        ogs_error("OpenAPI_rslp_information_convertToJSON() failed [n2_pc5_rslp_pol]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2Pc5RslpPol", n2_pc5_rslp_pol_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_rslp_information_convertToJSON() failed [n2_pc5_rslp_pol]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_rslp_information_t *OpenAPI_rslp_information_parseFromJSON(cJSON *rslp_informationJSON)
{
    OpenAPI_rslp_information_t *rslp_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n2_pc5_rslp_pol = NULL;
    OpenAPI_n2_info_content_t *n2_pc5_rslp_pol_local_nonprim = NULL;
    n2_pc5_rslp_pol = cJSON_GetObjectItemCaseSensitive(rslp_informationJSON, "n2Pc5RslpPol");
    if (n2_pc5_rslp_pol) {
    n2_pc5_rslp_pol_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(n2_pc5_rslp_pol);
    if (!n2_pc5_rslp_pol_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [n2_pc5_rslp_pol]");
        goto end;
    }
    }

    rslp_information_local_var = OpenAPI_rslp_information_create (
        n2_pc5_rslp_pol ? n2_pc5_rslp_pol_local_nonprim : NULL
    );

    return rslp_information_local_var;
end:
    if (n2_pc5_rslp_pol_local_nonprim) {
        OpenAPI_n2_info_content_free(n2_pc5_rslp_pol_local_nonprim);
        n2_pc5_rslp_pol_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_rslp_information_t *OpenAPI_rslp_information_copy(OpenAPI_rslp_information_t *dst, OpenAPI_rslp_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rslp_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rslp_information_convertToJSON() failed");
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

    OpenAPI_rslp_information_free(dst);
    dst = OpenAPI_rslp_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

