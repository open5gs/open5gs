
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ps_cell_information.h"

OpenAPI_ps_cell_information_t *OpenAPI_ps_cell_information_create(
    OpenAPI_ncgi_t *nrcgi,
    OpenAPI_ecgi_t *ecgi
)
{
    OpenAPI_ps_cell_information_t *ps_cell_information_local_var = ogs_malloc(sizeof(OpenAPI_ps_cell_information_t));
    ogs_assert(ps_cell_information_local_var);

    ps_cell_information_local_var->nrcgi = nrcgi;
    ps_cell_information_local_var->ecgi = ecgi;

    return ps_cell_information_local_var;
}

void OpenAPI_ps_cell_information_free(OpenAPI_ps_cell_information_t *ps_cell_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ps_cell_information) {
        return;
    }
    if (ps_cell_information->nrcgi) {
        OpenAPI_ncgi_free(ps_cell_information->nrcgi);
        ps_cell_information->nrcgi = NULL;
    }
    if (ps_cell_information->ecgi) {
        OpenAPI_ecgi_free(ps_cell_information->ecgi);
        ps_cell_information->ecgi = NULL;
    }
    ogs_free(ps_cell_information);
}

cJSON *OpenAPI_ps_cell_information_convertToJSON(OpenAPI_ps_cell_information_t *ps_cell_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ps_cell_information == NULL) {
        ogs_error("OpenAPI_ps_cell_information_convertToJSON() failed [PSCellInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ps_cell_information->nrcgi) {
    cJSON *nrcgi_local_JSON = OpenAPI_ncgi_convertToJSON(ps_cell_information->nrcgi);
    if (nrcgi_local_JSON == NULL) {
        ogs_error("OpenAPI_ps_cell_information_convertToJSON() failed [nrcgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nrcgi", nrcgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ps_cell_information_convertToJSON() failed [nrcgi]");
        goto end;
    }
    }

    if (ps_cell_information->ecgi) {
    cJSON *ecgi_local_JSON = OpenAPI_ecgi_convertToJSON(ps_cell_information->ecgi);
    if (ecgi_local_JSON == NULL) {
        ogs_error("OpenAPI_ps_cell_information_convertToJSON() failed [ecgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgi", ecgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ps_cell_information_convertToJSON() failed [ecgi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ps_cell_information_t *OpenAPI_ps_cell_information_parseFromJSON(cJSON *ps_cell_informationJSON)
{
    OpenAPI_ps_cell_information_t *ps_cell_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nrcgi = NULL;
    OpenAPI_ncgi_t *nrcgi_local_nonprim = NULL;
    cJSON *ecgi = NULL;
    OpenAPI_ecgi_t *ecgi_local_nonprim = NULL;
    nrcgi = cJSON_GetObjectItemCaseSensitive(ps_cell_informationJSON, "nrcgi");
    if (nrcgi) {
    nrcgi_local_nonprim = OpenAPI_ncgi_parseFromJSON(nrcgi);
    if (!nrcgi_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [nrcgi]");
        goto end;
    }
    }

    ecgi = cJSON_GetObjectItemCaseSensitive(ps_cell_informationJSON, "ecgi");
    if (ecgi) {
    ecgi_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi);
    if (!ecgi_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi]");
        goto end;
    }
    }

    ps_cell_information_local_var = OpenAPI_ps_cell_information_create (
        nrcgi ? nrcgi_local_nonprim : NULL,
        ecgi ? ecgi_local_nonprim : NULL
    );

    return ps_cell_information_local_var;
end:
    if (nrcgi_local_nonprim) {
        OpenAPI_ncgi_free(nrcgi_local_nonprim);
        nrcgi_local_nonprim = NULL;
    }
    if (ecgi_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_local_nonprim);
        ecgi_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ps_cell_information_t *OpenAPI_ps_cell_information_copy(OpenAPI_ps_cell_information_t *dst, OpenAPI_ps_cell_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ps_cell_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ps_cell_information_convertToJSON() failed");
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

    OpenAPI_ps_cell_information_free(dst);
    dst = OpenAPI_ps_cell_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

