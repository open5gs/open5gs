
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
    OpenAPI_v2x_information_t *v2x_info
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

    return n2_info_container_local_var;
}

void OpenAPI_n2_info_container_free(OpenAPI_n2_info_container_t *n2_info_container)
{
    if (NULL == n2_info_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_n2_sm_information_free(n2_info_container->sm_info);
    OpenAPI_n2_ran_information_free(n2_info_container->ran_info);
    OpenAPI_nrppa_information_free(n2_info_container->nrppa_info);
    OpenAPI_pws_information_free(n2_info_container->pws_info);
    OpenAPI_v2x_information_free(n2_info_container->v2x_info);
    ogs_free(n2_info_container);
}

cJSON *OpenAPI_n2_info_container_convertToJSON(OpenAPI_n2_info_container_t *n2_info_container)
{
    cJSON *item = NULL;

    if (n2_info_container == NULL) {
        ogs_error("OpenAPI_n2_info_container_convertToJSON() failed [N2InfoContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
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

end:
    return item;
}

OpenAPI_n2_info_container_t *OpenAPI_n2_info_container_parseFromJSON(cJSON *n2_info_containerJSON)
{
    OpenAPI_n2_info_container_t *n2_info_container_local_var = NULL;
    cJSON *n2_information_class = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "n2InformationClass");
    if (!n2_information_class) {
        ogs_error("OpenAPI_n2_info_container_parseFromJSON() failed [n2_information_class]");
        goto end;
    }

    OpenAPI_n2_information_class_e n2_information_classVariable;
    if (!cJSON_IsString(n2_information_class)) {
        ogs_error("OpenAPI_n2_info_container_parseFromJSON() failed [n2_information_class]");
        goto end;
    }
    n2_information_classVariable = OpenAPI_n2_information_class_FromString(n2_information_class->valuestring);

    cJSON *sm_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "smInfo");

    OpenAPI_n2_sm_information_t *sm_info_local_nonprim = NULL;
    if (sm_info) {
    sm_info_local_nonprim = OpenAPI_n2_sm_information_parseFromJSON(sm_info);
    }

    cJSON *ran_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "ranInfo");

    OpenAPI_n2_ran_information_t *ran_info_local_nonprim = NULL;
    if (ran_info) {
    ran_info_local_nonprim = OpenAPI_n2_ran_information_parseFromJSON(ran_info);
    }

    cJSON *nrppa_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "nrppaInfo");

    OpenAPI_nrppa_information_t *nrppa_info_local_nonprim = NULL;
    if (nrppa_info) {
    nrppa_info_local_nonprim = OpenAPI_nrppa_information_parseFromJSON(nrppa_info);
    }

    cJSON *pws_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "pwsInfo");

    OpenAPI_pws_information_t *pws_info_local_nonprim = NULL;
    if (pws_info) {
    pws_info_local_nonprim = OpenAPI_pws_information_parseFromJSON(pws_info);
    }

    cJSON *v2x_info = cJSON_GetObjectItemCaseSensitive(n2_info_containerJSON, "v2xInfo");

    OpenAPI_v2x_information_t *v2x_info_local_nonprim = NULL;
    if (v2x_info) {
    v2x_info_local_nonprim = OpenAPI_v2x_information_parseFromJSON(v2x_info);
    }

    n2_info_container_local_var = OpenAPI_n2_info_container_create (
        n2_information_classVariable,
        sm_info ? sm_info_local_nonprim : NULL,
        ran_info ? ran_info_local_nonprim : NULL,
        nrppa_info ? nrppa_info_local_nonprim : NULL,
        pws_info ? pws_info_local_nonprim : NULL,
        v2x_info ? v2x_info_local_nonprim : NULL
    );

    return n2_info_container_local_var;
end:
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

