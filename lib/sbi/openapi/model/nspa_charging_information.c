
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nspa_charging_information.h"

OpenAPI_nspa_charging_information_t *OpenAPI_nspa_charging_information_create(
    OpenAPI_snssai_t *single_nssai
)
{
    OpenAPI_nspa_charging_information_t *nspa_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_nspa_charging_information_t));
    ogs_assert(nspa_charging_information_local_var);

    nspa_charging_information_local_var->single_nssai = single_nssai;

    return nspa_charging_information_local_var;
}

void OpenAPI_nspa_charging_information_free(OpenAPI_nspa_charging_information_t *nspa_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nspa_charging_information) {
        return;
    }
    if (nspa_charging_information->single_nssai) {
        OpenAPI_snssai_free(nspa_charging_information->single_nssai);
        nspa_charging_information->single_nssai = NULL;
    }
    ogs_free(nspa_charging_information);
}

cJSON *OpenAPI_nspa_charging_information_convertToJSON(OpenAPI_nspa_charging_information_t *nspa_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nspa_charging_information == NULL) {
        ogs_error("OpenAPI_nspa_charging_information_convertToJSON() failed [NSPAChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nspa_charging_information->single_nssai) {
        ogs_error("OpenAPI_nspa_charging_information_convertToJSON() failed [single_nssai]");
        return NULL;
    }
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(nspa_charging_information->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nspa_charging_information_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNSSAI", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nspa_charging_information_convertToJSON() failed [single_nssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nspa_charging_information_t *OpenAPI_nspa_charging_information_parseFromJSON(cJSON *nspa_charging_informationJSON)
{
    OpenAPI_nspa_charging_information_t *nspa_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    single_nssai = cJSON_GetObjectItemCaseSensitive(nspa_charging_informationJSON, "singleNSSAI");
    if (!single_nssai) {
        ogs_error("OpenAPI_nspa_charging_information_parseFromJSON() failed [single_nssai]");
        goto end;
    }
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }

    nspa_charging_information_local_var = OpenAPI_nspa_charging_information_create (
        single_nssai_local_nonprim
    );

    return nspa_charging_information_local_var;
end:
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nspa_charging_information_t *OpenAPI_nspa_charging_information_copy(OpenAPI_nspa_charging_information_t *dst, OpenAPI_nspa_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nspa_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nspa_charging_information_convertToJSON() failed");
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

    OpenAPI_nspa_charging_information_free(dst);
    dst = OpenAPI_nspa_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

