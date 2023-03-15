
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_info_container.h"

OpenAPI_n2_info_container_t *OpenAPI_n2_info_container_create(
    OpenAPI_n2_information_class_e n2_information_class,
    OpenAPI_n2_sm_information_t *sm_info,
    OpenAPI_n2_ran_information_t *ran_info,
    OpenAPI_nrppa_information_t *nrppa_info,
    OpenAPI_pws_information_t *pws_info,
    OpenAPI_v2x_information_t *v2x_info,
    OpenAPI_pro_se_information_t *prose_info
)
{
    OpenAPI_n2_info_container_t *n2_info_container_local_var = ogs_malloc(sizeof(OpenAPI_n2_info_container_t));
    ogs_assert(n2_info_container_local_var);

    n2_info_container_local_var->n2_information_class = n2_information_class;
    n2_info_container_local_var->sm_info = sm_info;
    n2_info_container_local_var->ran_info = ran_info;
    n2_info_container_local_var->nrppa_info = nrppa_info;
    n2_info_container_local_var->pws_info = pws_info;
    n2_info_container_local_var->v2x_info = v2x_info;
    n2_info_container_local_var->prose_info = prose_info;

    return n2_info_container_local_var;
}

void OpenAPI_n2_info_container_free(OpenAPI_n2_info_container_t *n2_info_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_info_container) {
        return;
    }
    if (n2_info_container->sm_info) {
        OpenAPI_n2_sm_information_free(n2_info_container->sm_info);
        n2_info_container->sm_info = NULL;
    }
    if (n2_info_container->ran_info) {
        OpenAPI_n2_ran_information_free(n2_info_container->ran_info);
        n2_info_container->ran_info = NULL;
    }
    if (n2_info_container->nrppa_info) {
        OpenAPI_nrppa_information_free(n2_info_container->nrppa_info);
        n2_info_container->nrppa_info = NULL;
    }
    if (n2_info_container->pws_info) {
        OpenAPI_pws_information_free(n2_info_container->pws_info);
        n2_info_container->pws_info = NULL;
    }
    if (n2_info_container->v2x_info) {
        OpenAPI_v2x_information_free(n2_info_container->v2x_info);
        n2_info_container->v2x_info = NULL;
    }
    if (n2_info_container->prose_info) {
        OpenAPI_pro_se_information_free(n2_info_container->prose_info);
        n2_info_container->prose_info = NULL;
    }
    ogs_free(n2_info_container);
}

cJSON *OpenAPI_n2_info_container_convertToJSON(OpenAPI_n2_info_container_t *n2_info_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_info_container == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [N2InfoContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_info_container->n2_information_class == OpenAPI_n2_information_class_NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [n2_information_class]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n2InformationClass", OpenAPI_n2_information_class_ToString(n2_info_container->n2_information_class)) == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [n2_information_class]");
        goto end;
    }

    if (n2_info_container->sm_info) {
    cJSON *sm_info_local_JSON = OpenAPI_n2_sm_information_convertToJSON(n2_info_container->sm_info);
    if (sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smInfo", sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [sm_info]");
        goto end;
    }
    }

    if (n2_info_container->ran_info) {
    cJSON *ran_info_local_JSON = OpenAPI_n2_ran_information_convertToJSON(n2_info_container->ran_info);
    if (ran_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [ran_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranInfo", ran_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [ran_info]");
        goto end;
    }
    }

    if (n2_info_container->nrppa_info) {
    cJSON *nrppa_info_local_JSON = OpenAPI_nrppa_information_convertToJSON(n2_info_container->nrppa_info);
    if (nrppa_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [nrppa_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nrppaInfo", nrppa_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [nrppa_info]");
        goto end;
    }
    }

    if (n2_info_container->pws_info) {
    cJSON *pws_info_local_JSON = OpenAPI_pws_information_convertToJSON(n2_info_container->pws_info);
    if (pws_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [pws_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pwsInfo", pws_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [pws_info]");
        goto end;
    }
    }

    if (n2_info_container->v2x_info) {
    cJSON *v2x_info_local_JSON = OpenAPI_v2x_information_convertToJSON(n2_info_container->v2x_info);
    if (v2x_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [v2x_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "v2xInfo", v2x_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [v2x_info]");
        goto end;
    }
    }

    if (n2_info_container->prose_info) {
    cJSON *prose_info_local_JSON = OpenAPI_pro_se_information_convertToJSON(n2_info_container->prose_info);
    if (prose_info_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [prose_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseInfo", prose_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [prose_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_info_container_t *OpenAPI_n2_info_container_parseFromJSON(cJSON *n2_info_containerJSON)
{
    OpenAPI_n2_info_container_t *n2_info_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n2_information_class = NULL;
    OpenAPI_n2_information_class_e n2_information_classVariable = 0;
    cJSON *sm_info = NULL;
    OpenAPI_n2_sm_information_t *sm_info_local_nonprim = NULL;
    cJSON *ran_info = NULL;
    OpenAPI_n2_ran_information_t *ran_info_local_nonprim = NULL;
    cJSON *nrppa_info = NULL;
    OpenAPI_nrppa_information_t *nrppa_info_local_nonprim = NULL;
    cJSON *pws_info = NULL;
    OpenAPI_pws_information_t *pws_info_local_nonprim = NULL;
    cJSON *v2x_info = NULL;
    OpenAPI_v2x_information_t *v2x_info_local_nonprim = NULL;
    cJSON *prose_info = NULL;
    OpenAPI_pro_se_information_t *prose_info_local_nonprim = NULL;
    n2_information_class = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "n2InformationClass");
    if (!n2_information_class) {
        ogs_error("OpenAPI_n2_info_container_parseFromJSON() failed [n2_information_class]");
        goto end;
    }
    if (!cJSON_IsString(n2_information_class)) {
        ogs_error("OpenAPI_n2_info_container_parseFromJSON() failed [n2_information_class]");
        goto end;
    }
    n2_information_classVariable = OpenAPI_n2_information_class_FromString(n2_information_class->valuestring);

    sm_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "smInfo");
    if (sm_info) {
    sm_info_local_nonprim = OpenAPI_n2_sm_information_parseFromJSON(sm_info);
    if (!sm_info_local_nonprim) {
        ogs_error("OpenAPI_n2_sm_information_parseFromJSON failed [sm_info]");
        goto end;
    }
    }

    ran_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "ranInfo");
    if (ran_info) {
    ran_info_local_nonprim = OpenAPI_n2_ran_information_parseFromJSON(ran_info);
    if (!ran_info_local_nonprim) {
        ogs_error("OpenAPI_n2_ran_information_parseFromJSON failed [ran_info]");
        goto end;
    }
    }

    nrppa_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "nrppaInfo");
    if (nrppa_info) {
    nrppa_info_local_nonprim = OpenAPI_nrppa_information_parseFromJSON(nrppa_info);
    if (!nrppa_info_local_nonprim) {
        ogs_error("OpenAPI_nrppa_information_parseFromJSON failed [nrppa_info]");
        goto end;
    }
    }

    pws_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "pwsInfo");
    if (pws_info) {
    pws_info_local_nonprim = OpenAPI_pws_information_parseFromJSON(pws_info);
    if (!pws_info_local_nonprim) {
        ogs_error("OpenAPI_pws_information_parseFromJSON failed [pws_info]");
        goto end;
    }
    }

    v2x_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "v2xInfo");
    if (v2x_info) {
    v2x_info_local_nonprim = OpenAPI_v2x_information_parseFromJSON(v2x_info);
    if (!v2x_info_local_nonprim) {
        ogs_error("OpenAPI_v2x_information_parseFromJSON failed [v2x_info]");
        goto end;
    }
    }

    prose_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "proseInfo");
    if (prose_info) {
    prose_info_local_nonprim = OpenAPI_pro_se_information_parseFromJSON(prose_info);
    if (!prose_info_local_nonprim) {
        ogs_error("OpenAPI_pro_se_information_parseFromJSON failed [prose_info]");
        goto end;
    }
    }

    n2_info_container_local_var = OpenAPI_n2_info_container_create (
        n2_information_classVariable,
        sm_info ? sm_info_local_nonprim : NULL,
        ran_info ? ran_info_local_nonprim : NULL,
        nrppa_info ? nrppa_info_local_nonprim : NULL,
        pws_info ? pws_info_local_nonprim : NULL,
        v2x_info ? v2x_info_local_nonprim : NULL,
        prose_info ? prose_info_local_nonprim : NULL
    );

    return n2_info_container_local_var;
end:
    if (sm_info_local_nonprim) {
        OpenAPI_n2_sm_information_free(sm_info_local_nonprim);
        sm_info_local_nonprim = NULL;
    }
    if (ran_info_local_nonprim) {
        OpenAPI_n2_ran_information_free(ran_info_local_nonprim);
        ran_info_local_nonprim = NULL;
    }
    if (nrppa_info_local_nonprim) {
        OpenAPI_nrppa_information_free(nrppa_info_local_nonprim);
        nrppa_info_local_nonprim = NULL;
    }
    if (pws_info_local_nonprim) {
        OpenAPI_pws_information_free(pws_info_local_nonprim);
        pws_info_local_nonprim = NULL;
    }
    if (v2x_info_local_nonprim) {
        OpenAPI_v2x_information_free(v2x_info_local_nonprim);
        v2x_info_local_nonprim = NULL;
    }
    if (prose_info_local_nonprim) {
        OpenAPI_pro_se_information_free(prose_info_local_nonprim);
        prose_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n2_info_container_t *OpenAPI_n2_info_container_copy(OpenAPI_n2_info_container_t *dst, OpenAPI_n2_info_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_info_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed");
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

    OpenAPI_n2_info_container_free(dst);
    dst = OpenAPI_n2_info_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

