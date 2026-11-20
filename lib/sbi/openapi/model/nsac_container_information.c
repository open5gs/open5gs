
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsac_container_information.h"

OpenAPI_nsac_container_information_t *OpenAPI_nsac_container_information_create(
    bool is_number_of_ues,
    int number_of_ues,
    bool is_number_of_pdus,
    int number_of_pdus
)
{
    OpenAPI_nsac_container_information_t *nsac_container_information_local_var = ogs_malloc(sizeof(OpenAPI_nsac_container_information_t));
    ogs_assert(nsac_container_information_local_var);

    nsac_container_information_local_var->is_number_of_ues = is_number_of_ues;
    nsac_container_information_local_var->number_of_ues = number_of_ues;
    nsac_container_information_local_var->is_number_of_pdus = is_number_of_pdus;
    nsac_container_information_local_var->number_of_pdus = number_of_pdus;

    return nsac_container_information_local_var;
}

void OpenAPI_nsac_container_information_free(OpenAPI_nsac_container_information_t *nsac_container_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsac_container_information) {
        return;
    }
    ogs_free(nsac_container_information);
}

cJSON *OpenAPI_nsac_container_information_convertToJSON(OpenAPI_nsac_container_information_t *nsac_container_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsac_container_information == NULL) {
        ogs_error("OpenAPI_nsac_container_information_convertToJSON() failed [NSACContainerInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nsac_container_information->is_number_of_ues) {
    if (cJSON_AddNumberToObject(item, "numberOfUEs", nsac_container_information->number_of_ues) == NULL) {
        ogs_error("OpenAPI_nsac_container_information_convertToJSON() failed [number_of_ues]");
        goto end;
    }
    }

    if (nsac_container_information->is_number_of_pdus) {
    if (cJSON_AddNumberToObject(item, "numberOfPDUs", nsac_container_information->number_of_pdus) == NULL) {
        ogs_error("OpenAPI_nsac_container_information_convertToJSON() failed [number_of_pdus]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nsac_container_information_t *OpenAPI_nsac_container_information_parseFromJSON(cJSON *nsac_container_informationJSON)
{
    OpenAPI_nsac_container_information_t *nsac_container_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *number_of_ues = NULL;
    cJSON *number_of_pdus = NULL;
    number_of_ues = cJSON_GetObjectItemCaseSensitive(nsac_container_informationJSON, "numberOfUEs");
    if (number_of_ues) {
    if (!cJSON_IsNumber(number_of_ues)) {
        ogs_error("OpenAPI_nsac_container_information_parseFromJSON() failed [number_of_ues]");
        goto end;
    }
    }

    number_of_pdus = cJSON_GetObjectItemCaseSensitive(nsac_container_informationJSON, "numberOfPDUs");
    if (number_of_pdus) {
    if (!cJSON_IsNumber(number_of_pdus)) {
        ogs_error("OpenAPI_nsac_container_information_parseFromJSON() failed [number_of_pdus]");
        goto end;
    }
    }

    nsac_container_information_local_var = OpenAPI_nsac_container_information_create (
        number_of_ues ? true : false,
        number_of_ues ? number_of_ues->valuedouble : 0,
        number_of_pdus ? true : false,
        number_of_pdus ? number_of_pdus->valuedouble : 0
    );

    return nsac_container_information_local_var;
end:
    return NULL;
}

OpenAPI_nsac_container_information_t *OpenAPI_nsac_container_information_copy(OpenAPI_nsac_container_information_t *dst, OpenAPI_nsac_container_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsac_container_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsac_container_information_convertToJSON() failed");
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

    OpenAPI_nsac_container_information_free(dst);
    dst = OpenAPI_nsac_container_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

