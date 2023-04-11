
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ec_restriction_1.h"

OpenAPI_ec_restriction_1_t *OpenAPI_ec_restriction_1_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t *plmn_ec_infos,
    char *mtc_provider_information
)
{
    OpenAPI_ec_restriction_1_t *ec_restriction_1_local_var = ogs_malloc(sizeof(OpenAPI_ec_restriction_1_t));
    ogs_assert(ec_restriction_1_local_var);

    ec_restriction_1_local_var->af_instance_id = af_instance_id;
    ec_restriction_1_local_var->reference_id = reference_id;
    ec_restriction_1_local_var->plmn_ec_infos = plmn_ec_infos;
    ec_restriction_1_local_var->mtc_provider_information = mtc_provider_information;

    return ec_restriction_1_local_var;
}

void OpenAPI_ec_restriction_1_free(OpenAPI_ec_restriction_1_t *ec_restriction_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ec_restriction_1) {
        return;
    }
    if (ec_restriction_1->af_instance_id) {
        ogs_free(ec_restriction_1->af_instance_id);
        ec_restriction_1->af_instance_id = NULL;
    }
    if (ec_restriction_1->plmn_ec_infos) {
        OpenAPI_list_for_each(ec_restriction_1->plmn_ec_infos, node) {
            OpenAPI_plmn_ec_info_1_free(node->data);
        }
        OpenAPI_list_free(ec_restriction_1->plmn_ec_infos);
        ec_restriction_1->plmn_ec_infos = NULL;
    }
    if (ec_restriction_1->mtc_provider_information) {
        ogs_free(ec_restriction_1->mtc_provider_information);
        ec_restriction_1->mtc_provider_information = NULL;
    }
    ogs_free(ec_restriction_1);
}

cJSON *OpenAPI_ec_restriction_1_convertToJSON(OpenAPI_ec_restriction_1_t *ec_restriction_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ec_restriction_1 == NULL) {
        ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed [EcRestriction_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ec_restriction_1->af_instance_id) {
        ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed [af_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", ec_restriction_1->af_instance_id) == NULL) {
        ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "referenceId", ec_restriction_1->reference_id) == NULL) {
        ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (ec_restriction_1->plmn_ec_infos) {
    cJSON *plmn_ec_infosList = cJSON_AddArrayToObject(item, "plmnEcInfos");
    if (plmn_ec_infosList == NULL) {
        ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed [plmn_ec_infos]");
        goto end;
    }
    OpenAPI_list_for_each(ec_restriction_1->plmn_ec_infos, node) {
        cJSON *itemLocal = OpenAPI_plmn_ec_info_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed [plmn_ec_infos]");
            goto end;
        }
        cJSON_AddItemToArray(plmn_ec_infosList, itemLocal);
    }
    }

    if (ec_restriction_1->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", ec_restriction_1->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ec_restriction_1_t *OpenAPI_ec_restriction_1_parseFromJSON(cJSON *ec_restriction_1JSON)
{
    OpenAPI_ec_restriction_1_t *ec_restriction_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *plmn_ec_infos = NULL;
    OpenAPI_list_t *plmn_ec_infosList = NULL;
    cJSON *mtc_provider_information = NULL;
    af_instance_id = cJSON_GetObjectItemCaseSensitive(ec_restriction_1JSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_ec_restriction_1_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_ec_restriction_1_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(ec_restriction_1JSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_ec_restriction_1_parseFromJSON() failed [reference_id]");
        goto end;
    }
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_ec_restriction_1_parseFromJSON() failed [reference_id]");
        goto end;
    }

    plmn_ec_infos = cJSON_GetObjectItemCaseSensitive(ec_restriction_1JSON, "plmnEcInfos");
    if (plmn_ec_infos) {
        cJSON *plmn_ec_infos_local = NULL;
        if (!cJSON_IsArray(plmn_ec_infos)) {
            ogs_error("OpenAPI_ec_restriction_1_parseFromJSON() failed [plmn_ec_infos]");
            goto end;
        }

        plmn_ec_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_ec_infos_local, plmn_ec_infos) {
            if (!cJSON_IsObject(plmn_ec_infos_local)) {
                ogs_error("OpenAPI_ec_restriction_1_parseFromJSON() failed [plmn_ec_infos]");
                goto end;
            }
            OpenAPI_plmn_ec_info_1_t *plmn_ec_infosItem = OpenAPI_plmn_ec_info_1_parseFromJSON(plmn_ec_infos_local);
            if (!plmn_ec_infosItem) {
                ogs_error("No plmn_ec_infosItem");
                goto end;
            }
            OpenAPI_list_add(plmn_ec_infosList, plmn_ec_infosItem);
        }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(ec_restriction_1JSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_ec_restriction_1_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    ec_restriction_1_local_var = OpenAPI_ec_restriction_1_create (
        ogs_strdup(af_instance_id->valuestring),
        
        reference_id->valuedouble,
        plmn_ec_infos ? plmn_ec_infosList : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL
    );

    return ec_restriction_1_local_var;
end:
    if (plmn_ec_infosList) {
        OpenAPI_list_for_each(plmn_ec_infosList, node) {
            OpenAPI_plmn_ec_info_1_free(node->data);
        }
        OpenAPI_list_free(plmn_ec_infosList);
        plmn_ec_infosList = NULL;
    }
    return NULL;
}

OpenAPI_ec_restriction_1_t *OpenAPI_ec_restriction_1_copy(OpenAPI_ec_restriction_1_t *dst, OpenAPI_ec_restriction_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ec_restriction_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ec_restriction_1_convertToJSON() failed");
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

    OpenAPI_ec_restriction_1_free(dst);
    dst = OpenAPI_ec_restriction_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

