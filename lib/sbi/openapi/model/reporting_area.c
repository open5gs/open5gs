
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_area.h"

OpenAPI_reporting_area_t *OpenAPI_reporting_area_create(
    OpenAPI_reporting_area_type_t *area_type,
    OpenAPI_tai_t *tai,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_ncgi_t *ncgi
)
{
    OpenAPI_reporting_area_t *reporting_area_local_var = ogs_malloc(sizeof(OpenAPI_reporting_area_t));
    ogs_assert(reporting_area_local_var);

    reporting_area_local_var->area_type = area_type;
    reporting_area_local_var->tai = tai;
    reporting_area_local_var->ecgi = ecgi;
    reporting_area_local_var->ncgi = ncgi;

    return reporting_area_local_var;
}

void OpenAPI_reporting_area_free(OpenAPI_reporting_area_t *reporting_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reporting_area) {
        return;
    }
    if (reporting_area->area_type) {
        OpenAPI_reporting_area_type_free(reporting_area->area_type);
        reporting_area->area_type = NULL;
    }
    if (reporting_area->tai) {
        OpenAPI_tai_free(reporting_area->tai);
        reporting_area->tai = NULL;
    }
    if (reporting_area->ecgi) {
        OpenAPI_ecgi_free(reporting_area->ecgi);
        reporting_area->ecgi = NULL;
    }
    if (reporting_area->ncgi) {
        OpenAPI_ncgi_free(reporting_area->ncgi);
        reporting_area->ncgi = NULL;
    }
    ogs_free(reporting_area);
}

cJSON *OpenAPI_reporting_area_convertToJSON(OpenAPI_reporting_area_t *reporting_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reporting_area == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [ReportingArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!reporting_area->area_type) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [area_type]");
        return NULL;
    }
    cJSON *area_type_local_JSON = OpenAPI_reporting_area_type_convertToJSON(reporting_area->area_type);
    if (area_type_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [area_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "areaType", area_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [area_type]");
        goto end;
    }

    if (reporting_area->tai) {
    cJSON *tai_local_JSON = OpenAPI_tai_convertToJSON(reporting_area->tai);
    if (tai_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [tai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tai", tai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [tai]");
        goto end;
    }
    }

    if (reporting_area->ecgi) {
    cJSON *ecgi_local_JSON = OpenAPI_ecgi_convertToJSON(reporting_area->ecgi);
    if (ecgi_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [ecgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgi", ecgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [ecgi]");
        goto end;
    }
    }

    if (reporting_area->ncgi) {
    cJSON *ncgi_local_JSON = OpenAPI_ncgi_convertToJSON(reporting_area->ncgi);
    if (ncgi_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [ncgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ncgi", ncgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed [ncgi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_reporting_area_t *OpenAPI_reporting_area_parseFromJSON(cJSON *reporting_areaJSON)
{
    OpenAPI_reporting_area_t *reporting_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *area_type = NULL;
    OpenAPI_reporting_area_type_t *area_type_local_nonprim = NULL;
    cJSON *tai = NULL;
    OpenAPI_tai_t *tai_local_nonprim = NULL;
    cJSON *ecgi = NULL;
    OpenAPI_ecgi_t *ecgi_local_nonprim = NULL;
    cJSON *ncgi = NULL;
    OpenAPI_ncgi_t *ncgi_local_nonprim = NULL;
    area_type = cJSON_GetObjectItemCaseSensitive(reporting_areaJSON, "areaType");
    if (!area_type) {
        ogs_error("OpenAPI_reporting_area_parseFromJSON() failed [area_type]");
        goto end;
    }
    area_type_local_nonprim = OpenAPI_reporting_area_type_parseFromJSON(area_type);
    if (!area_type_local_nonprim) {
        ogs_error("OpenAPI_reporting_area_type_parseFromJSON failed [area_type]");
        goto end;
    }

    tai = cJSON_GetObjectItemCaseSensitive(reporting_areaJSON, "tai");
    if (tai) {
    tai_local_nonprim = OpenAPI_tai_parseFromJSON(tai);
    if (!tai_local_nonprim) {
        ogs_error("OpenAPI_tai_parseFromJSON failed [tai]");
        goto end;
    }
    }

    ecgi = cJSON_GetObjectItemCaseSensitive(reporting_areaJSON, "ecgi");
    if (ecgi) {
    ecgi_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi);
    if (!ecgi_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi]");
        goto end;
    }
    }

    ncgi = cJSON_GetObjectItemCaseSensitive(reporting_areaJSON, "ncgi");
    if (ncgi) {
    ncgi_local_nonprim = OpenAPI_ncgi_parseFromJSON(ncgi);
    if (!ncgi_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [ncgi]");
        goto end;
    }
    }

    reporting_area_local_var = OpenAPI_reporting_area_create (
        area_type_local_nonprim,
        tai ? tai_local_nonprim : NULL,
        ecgi ? ecgi_local_nonprim : NULL,
        ncgi ? ncgi_local_nonprim : NULL
    );

    return reporting_area_local_var;
end:
    if (area_type_local_nonprim) {
        OpenAPI_reporting_area_type_free(area_type_local_nonprim);
        area_type_local_nonprim = NULL;
    }
    if (tai_local_nonprim) {
        OpenAPI_tai_free(tai_local_nonprim);
        tai_local_nonprim = NULL;
    }
    if (ecgi_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_local_nonprim);
        ecgi_local_nonprim = NULL;
    }
    if (ncgi_local_nonprim) {
        OpenAPI_ncgi_free(ncgi_local_nonprim);
        ncgi_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_reporting_area_t *OpenAPI_reporting_area_copy(OpenAPI_reporting_area_t *dst, OpenAPI_reporting_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reporting_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reporting_area_convertToJSON() failed");
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

    OpenAPI_reporting_area_free(dst);
    dst = OpenAPI_reporting_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

