
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnai_information.h"

OpenAPI_dnai_information_t *OpenAPI_dnai_information_create(
    char *dnai,
    int no_dnai_change_ind,
    int no_local_psa_change_ind
    )
{
    OpenAPI_dnai_information_t *dnai_information_local_var = OpenAPI_malloc(sizeof(OpenAPI_dnai_information_t));
    if (!dnai_information_local_var) {
        return NULL;
    }
    dnai_information_local_var->dnai = dnai;
    dnai_information_local_var->no_dnai_change_ind = no_dnai_change_ind;
    dnai_information_local_var->no_local_psa_change_ind = no_local_psa_change_ind;

    return dnai_information_local_var;
}

void OpenAPI_dnai_information_free(OpenAPI_dnai_information_t *dnai_information)
{
    if (NULL == dnai_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dnai_information->dnai);
    ogs_free(dnai_information);
}

cJSON *OpenAPI_dnai_information_convertToJSON(OpenAPI_dnai_information_t *dnai_information)
{
    cJSON *item = NULL;

    if (dnai_information == NULL) {
        ogs_error("OpenAPI_dnai_information_convertToJSON() failed [DnaiInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnai_information->dnai) {
        ogs_error("OpenAPI_dnai_information_convertToJSON() failed [dnai]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnai", dnai_information->dnai) == NULL) {
        ogs_error("OpenAPI_dnai_information_convertToJSON() failed [dnai]");
        goto end;
    }

    if (dnai_information->no_dnai_change_ind) {
        if (cJSON_AddBoolToObject(item, "noDnaiChangeInd", dnai_information->no_dnai_change_ind) == NULL) {
            ogs_error("OpenAPI_dnai_information_convertToJSON() failed [no_dnai_change_ind]");
            goto end;
        }
    }

    if (dnai_information->no_local_psa_change_ind) {
        if (cJSON_AddBoolToObject(item, "noLocalPsaChangeInd", dnai_information->no_local_psa_change_ind) == NULL) {
            ogs_error("OpenAPI_dnai_information_convertToJSON() failed [no_local_psa_change_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_dnai_information_t *OpenAPI_dnai_information_parseFromJSON(cJSON *dnai_informationJSON)
{
    OpenAPI_dnai_information_t *dnai_information_local_var = NULL;
    cJSON *dnai = cJSON_GetObjectItemCaseSensitive(dnai_informationJSON, "dnai");
    if (!dnai) {
        ogs_error("OpenAPI_dnai_information_parseFromJSON() failed [dnai]");
        goto end;
    }


    if (!cJSON_IsString(dnai)) {
        ogs_error("OpenAPI_dnai_information_parseFromJSON() failed [dnai]");
        goto end;
    }

    cJSON *no_dnai_change_ind = cJSON_GetObjectItemCaseSensitive(dnai_informationJSON, "noDnaiChangeInd");

    if (no_dnai_change_ind) {
        if (!cJSON_IsBool(no_dnai_change_ind)) {
            ogs_error("OpenAPI_dnai_information_parseFromJSON() failed [no_dnai_change_ind]");
            goto end;
        }
    }

    cJSON *no_local_psa_change_ind = cJSON_GetObjectItemCaseSensitive(dnai_informationJSON, "noLocalPsaChangeInd");

    if (no_local_psa_change_ind) {
        if (!cJSON_IsBool(no_local_psa_change_ind)) {
            ogs_error("OpenAPI_dnai_information_parseFromJSON() failed [no_local_psa_change_ind]");
            goto end;
        }
    }

    dnai_information_local_var = OpenAPI_dnai_information_create (
        ogs_strdup(dnai->valuestring),
        no_dnai_change_ind ? no_dnai_change_ind->valueint : 0,
        no_local_psa_change_ind ? no_local_psa_change_ind->valueint : 0
        );

    return dnai_information_local_var;
end:
    return NULL;
}

OpenAPI_dnai_information_t *OpenAPI_dnai_information_copy(OpenAPI_dnai_information_t *dst, OpenAPI_dnai_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnai_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnai_information_convertToJSON() failed");
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

    OpenAPI_dnai_information_free(dst);
    dst = OpenAPI_dnai_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

