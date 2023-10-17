
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_privacy.h"

OpenAPI_lcs_privacy_t *OpenAPI_lcs_privacy_create(
    char *af_instance_id,
    bool is_reference_id,
    int reference_id,
    OpenAPI_lpi_t *lpi,
    char *mtc_provider_information
)
{
    OpenAPI_lcs_privacy_t *lcs_privacy_local_var = ogs_malloc(sizeof(OpenAPI_lcs_privacy_t));
    ogs_assert(lcs_privacy_local_var);

    lcs_privacy_local_var->af_instance_id = af_instance_id;
    lcs_privacy_local_var->is_reference_id = is_reference_id;
    lcs_privacy_local_var->reference_id = reference_id;
    lcs_privacy_local_var->lpi = lpi;
    lcs_privacy_local_var->mtc_provider_information = mtc_provider_information;

    return lcs_privacy_local_var;
}

void OpenAPI_lcs_privacy_free(OpenAPI_lcs_privacy_t *lcs_privacy)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_privacy) {
        return;
    }
    if (lcs_privacy->af_instance_id) {
        ogs_free(lcs_privacy->af_instance_id);
        lcs_privacy->af_instance_id = NULL;
    }
    if (lcs_privacy->lpi) {
        OpenAPI_lpi_free(lcs_privacy->lpi);
        lcs_privacy->lpi = NULL;
    }
    if (lcs_privacy->mtc_provider_information) {
        ogs_free(lcs_privacy->mtc_provider_information);
        lcs_privacy->mtc_provider_information = NULL;
    }
    ogs_free(lcs_privacy);
}

cJSON *OpenAPI_lcs_privacy_convertToJSON(OpenAPI_lcs_privacy_t *lcs_privacy)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_privacy == NULL) {
        ogs_error("OpenAPI_lcs_privacy_convertToJSON() failed [LcsPrivacy]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_privacy->af_instance_id) {
    if (cJSON_AddStringToObject(item, "afInstanceId", lcs_privacy->af_instance_id) == NULL) {
        ogs_error("OpenAPI_lcs_privacy_convertToJSON() failed [af_instance_id]");
        goto end;
    }
    }

    if (lcs_privacy->is_reference_id) {
    if (cJSON_AddNumberToObject(item, "referenceId", lcs_privacy->reference_id) == NULL) {
        ogs_error("OpenAPI_lcs_privacy_convertToJSON() failed [reference_id]");
        goto end;
    }
    }

    if (lcs_privacy->lpi) {
    cJSON *lpi_local_JSON = OpenAPI_lpi_convertToJSON(lcs_privacy->lpi);
    if (lpi_local_JSON == NULL) {
        ogs_error("OpenAPI_lcs_privacy_convertToJSON() failed [lpi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lpi", lpi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lcs_privacy_convertToJSON() failed [lpi]");
        goto end;
    }
    }

    if (lcs_privacy->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", lcs_privacy->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_lcs_privacy_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lcs_privacy_t *OpenAPI_lcs_privacy_parseFromJSON(cJSON *lcs_privacyJSON)
{
    OpenAPI_lcs_privacy_t *lcs_privacy_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *lpi = NULL;
    OpenAPI_lpi_t *lpi_local_nonprim = NULL;
    cJSON *mtc_provider_information = NULL;
    af_instance_id = cJSON_GetObjectItemCaseSensitive(lcs_privacyJSON, "afInstanceId");
    if (af_instance_id) {
    if (!cJSON_IsString(af_instance_id) && !cJSON_IsNull(af_instance_id)) {
        ogs_error("OpenAPI_lcs_privacy_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(lcs_privacyJSON, "referenceId");
    if (reference_id) {
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_lcs_privacy_parseFromJSON() failed [reference_id]");
        goto end;
    }
    }

    lpi = cJSON_GetObjectItemCaseSensitive(lcs_privacyJSON, "lpi");
    if (lpi) {
    lpi_local_nonprim = OpenAPI_lpi_parseFromJSON(lpi);
    if (!lpi_local_nonprim) {
        ogs_error("OpenAPI_lpi_parseFromJSON failed [lpi]");
        goto end;
    }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(lcs_privacyJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_lcs_privacy_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    lcs_privacy_local_var = OpenAPI_lcs_privacy_create (
        af_instance_id && !cJSON_IsNull(af_instance_id) ? ogs_strdup(af_instance_id->valuestring) : NULL,
        reference_id ? true : false,
        reference_id ? reference_id->valuedouble : 0,
        lpi ? lpi_local_nonprim : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL
    );

    return lcs_privacy_local_var;
end:
    if (lpi_local_nonprim) {
        OpenAPI_lpi_free(lpi_local_nonprim);
        lpi_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_lcs_privacy_t *OpenAPI_lcs_privacy_copy(OpenAPI_lcs_privacy_t *dst, OpenAPI_lcs_privacy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_privacy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_privacy_convertToJSON() failed");
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

    OpenAPI_lcs_privacy_free(dst);
    dst = OpenAPI_lcs_privacy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

