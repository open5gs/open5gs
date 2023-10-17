
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_information.h"

OpenAPI_pro_se_information_t *OpenAPI_pro_se_information_create(
    OpenAPI_n2_info_content_t *n2_pc5_pro_se_pol
)
{
    OpenAPI_pro_se_information_t *pro_se_information_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_information_t));
    ogs_assert(pro_se_information_local_var);

    pro_se_information_local_var->n2_pc5_pro_se_pol = n2_pc5_pro_se_pol;

    return pro_se_information_local_var;
}

void OpenAPI_pro_se_information_free(OpenAPI_pro_se_information_t *pro_se_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_information) {
        return;
    }
    if (pro_se_information->n2_pc5_pro_se_pol) {
        OpenAPI_n2_info_content_free(pro_se_information->n2_pc5_pro_se_pol);
        pro_se_information->n2_pc5_pro_se_pol = NULL;
    }
    ogs_free(pro_se_information);
}

cJSON *OpenAPI_pro_se_information_convertToJSON(OpenAPI_pro_se_information_t *pro_se_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_information == NULL) {
        ogs_error("OpenAPI_pro_se_information_convertToJSON() failed [ProSeInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pro_se_information->n2_pc5_pro_se_pol) {
    cJSON *n2_pc5_pro_se_pol_local_JSON = OpenAPI_n2_info_content_convertToJSON(pro_se_information->n2_pc5_pro_se_pol);
    if (n2_pc5_pro_se_pol_local_JSON == NULL) {
        ogs_error("OpenAPI_pro_se_information_convertToJSON() failed [n2_pc5_pro_se_pol]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2Pc5ProSePol", n2_pc5_pro_se_pol_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pro_se_information_convertToJSON() failed [n2_pc5_pro_se_pol]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_information_t *OpenAPI_pro_se_information_parseFromJSON(cJSON *pro_se_informationJSON)
{
    OpenAPI_pro_se_information_t *pro_se_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n2_pc5_pro_se_pol = NULL;
    OpenAPI_n2_info_content_t *n2_pc5_pro_se_pol_local_nonprim = NULL;
    n2_pc5_pro_se_pol = cJSON_GetObjectItemCaseSensitive(pro_se_informationJSON, "n2Pc5ProSePol");
    if (n2_pc5_pro_se_pol) {
    n2_pc5_pro_se_pol_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(n2_pc5_pro_se_pol);
    if (!n2_pc5_pro_se_pol_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [n2_pc5_pro_se_pol]");
        goto end;
    }
    }

    pro_se_information_local_var = OpenAPI_pro_se_information_create (
        n2_pc5_pro_se_pol ? n2_pc5_pro_se_pol_local_nonprim : NULL
    );

    return pro_se_information_local_var;
end:
    if (n2_pc5_pro_se_pol_local_nonprim) {
        OpenAPI_n2_info_content_free(n2_pc5_pro_se_pol_local_nonprim);
        n2_pc5_pro_se_pol_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pro_se_information_t *OpenAPI_pro_se_information_copy(OpenAPI_pro_se_information_t *dst, OpenAPI_pro_se_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_information_convertToJSON() failed");
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

    OpenAPI_pro_se_information_free(dst);
    dst = OpenAPI_pro_se_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

