
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_data_patch.h"

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_create(
    OpenAPI_transfer_policy_t *trans_policy,
    OpenAPI_bdt_policy_status_t *bdtp_status
)
{
    OpenAPI_bdt_data_patch_t *bdt_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_bdt_data_patch_t));
    ogs_assert(bdt_data_patch_local_var);

    bdt_data_patch_local_var->trans_policy = trans_policy;
    bdt_data_patch_local_var->bdtp_status = bdtp_status;

    return bdt_data_patch_local_var;
}

void OpenAPI_bdt_data_patch_free(OpenAPI_bdt_data_patch_t *bdt_data_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bdt_data_patch) {
        return;
    }
    if (bdt_data_patch->trans_policy) {
        OpenAPI_transfer_policy_free(bdt_data_patch->trans_policy);
        bdt_data_patch->trans_policy = NULL;
    }
    if (bdt_data_patch->bdtp_status) {
        OpenAPI_bdt_policy_status_free(bdt_data_patch->bdtp_status);
        bdt_data_patch->bdtp_status = NULL;
    }
    ogs_free(bdt_data_patch);
}

cJSON *OpenAPI_bdt_data_patch_convertToJSON(OpenAPI_bdt_data_patch_t *bdt_data_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bdt_data_patch == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [BdtDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bdt_data_patch->trans_policy) {
    cJSON *trans_policy_local_JSON = OpenAPI_transfer_policy_convertToJSON(bdt_data_patch->trans_policy);
    if (trans_policy_local_JSON == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [trans_policy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "transPolicy", trans_policy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [trans_policy]");
        goto end;
    }
    }

    if (bdt_data_patch->bdtp_status) {
    cJSON *bdtp_status_local_JSON = OpenAPI_bdt_policy_status_convertToJSON(bdt_data_patch->bdtp_status);
    if (bdtp_status_local_JSON == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [bdtp_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "bdtpStatus", bdtp_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [bdtp_status]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_parseFromJSON(cJSON *bdt_data_patchJSON)
{
    OpenAPI_bdt_data_patch_t *bdt_data_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *trans_policy = NULL;
    OpenAPI_transfer_policy_t *trans_policy_local_nonprim = NULL;
    cJSON *bdtp_status = NULL;
    OpenAPI_bdt_policy_status_t *bdtp_status_local_nonprim = NULL;
    trans_policy = cJSON_GetObjectItemCaseSensitive(bdt_data_patchJSON, "transPolicy");
    if (trans_policy) {
    trans_policy_local_nonprim = OpenAPI_transfer_policy_parseFromJSON(trans_policy);
    if (!trans_policy_local_nonprim) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON failed [trans_policy]");
        goto end;
    }
    }

    bdtp_status = cJSON_GetObjectItemCaseSensitive(bdt_data_patchJSON, "bdtpStatus");
    if (bdtp_status) {
    bdtp_status_local_nonprim = OpenAPI_bdt_policy_status_parseFromJSON(bdtp_status);
    if (!bdtp_status_local_nonprim) {
        ogs_error("OpenAPI_bdt_policy_status_parseFromJSON failed [bdtp_status]");
        goto end;
    }
    }

    bdt_data_patch_local_var = OpenAPI_bdt_data_patch_create (
        trans_policy ? trans_policy_local_nonprim : NULL,
        bdtp_status ? bdtp_status_local_nonprim : NULL
    );

    return bdt_data_patch_local_var;
end:
    if (trans_policy_local_nonprim) {
        OpenAPI_transfer_policy_free(trans_policy_local_nonprim);
        trans_policy_local_nonprim = NULL;
    }
    if (bdtp_status_local_nonprim) {
        OpenAPI_bdt_policy_status_free(bdtp_status_local_nonprim);
        bdtp_status_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_copy(OpenAPI_bdt_data_patch_t *dst, OpenAPI_bdt_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bdt_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed");
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

    OpenAPI_bdt_data_patch_free(dst);
    dst = OpenAPI_bdt_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

