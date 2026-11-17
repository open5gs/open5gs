
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "software_image_info.h"

OpenAPI_software_image_info_t *OpenAPI_software_image_info_create(
    bool is_minimum_disk,
    int minimum_disk,
    bool is_minimum_ram,
    int minimum_ram,
    char *disc_format,
    char *operating_system,
    char *sw_image_ref
)
{
    OpenAPI_software_image_info_t *software_image_info_local_var = ogs_malloc(sizeof(OpenAPI_software_image_info_t));
    ogs_assert(software_image_info_local_var);

    software_image_info_local_var->is_minimum_disk = is_minimum_disk;
    software_image_info_local_var->minimum_disk = minimum_disk;
    software_image_info_local_var->is_minimum_ram = is_minimum_ram;
    software_image_info_local_var->minimum_ram = minimum_ram;
    software_image_info_local_var->disc_format = disc_format;
    software_image_info_local_var->operating_system = operating_system;
    software_image_info_local_var->sw_image_ref = sw_image_ref;

    return software_image_info_local_var;
}

void OpenAPI_software_image_info_free(OpenAPI_software_image_info_t *software_image_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == software_image_info) {
        return;
    }
    if (software_image_info->disc_format) {
        ogs_free(software_image_info->disc_format);
        software_image_info->disc_format = NULL;
    }
    if (software_image_info->operating_system) {
        ogs_free(software_image_info->operating_system);
        software_image_info->operating_system = NULL;
    }
    if (software_image_info->sw_image_ref) {
        ogs_free(software_image_info->sw_image_ref);
        software_image_info->sw_image_ref = NULL;
    }
    ogs_free(software_image_info);
}

cJSON *OpenAPI_software_image_info_convertToJSON(OpenAPI_software_image_info_t *software_image_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (software_image_info == NULL) {
        ogs_error("OpenAPI_software_image_info_convertToJSON() failed [SoftwareImageInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (software_image_info->is_minimum_disk) {
    if (cJSON_AddNumberToObject(item, "minimumDisk", software_image_info->minimum_disk) == NULL) {
        ogs_error("OpenAPI_software_image_info_convertToJSON() failed [minimum_disk]");
        goto end;
    }
    }

    if (software_image_info->is_minimum_ram) {
    if (cJSON_AddNumberToObject(item, "minimumRAM", software_image_info->minimum_ram) == NULL) {
        ogs_error("OpenAPI_software_image_info_convertToJSON() failed [minimum_ram]");
        goto end;
    }
    }

    if (software_image_info->disc_format) {
    if (cJSON_AddStringToObject(item, "discFormat", software_image_info->disc_format) == NULL) {
        ogs_error("OpenAPI_software_image_info_convertToJSON() failed [disc_format]");
        goto end;
    }
    }

    if (software_image_info->operating_system) {
    if (cJSON_AddStringToObject(item, "operatingSystem", software_image_info->operating_system) == NULL) {
        ogs_error("OpenAPI_software_image_info_convertToJSON() failed [operating_system]");
        goto end;
    }
    }

    if (software_image_info->sw_image_ref) {
    if (cJSON_AddStringToObject(item, "swImageRef", software_image_info->sw_image_ref) == NULL) {
        ogs_error("OpenAPI_software_image_info_convertToJSON() failed [sw_image_ref]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_software_image_info_t *OpenAPI_software_image_info_parseFromJSON(cJSON *software_image_infoJSON)
{
    OpenAPI_software_image_info_t *software_image_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *minimum_disk = NULL;
    cJSON *minimum_ram = NULL;
    cJSON *disc_format = NULL;
    cJSON *operating_system = NULL;
    cJSON *sw_image_ref = NULL;
    minimum_disk = cJSON_GetObjectItemCaseSensitive(software_image_infoJSON, "minimumDisk");
    if (minimum_disk) {
    if (!cJSON_IsNumber(minimum_disk)) {
        ogs_error("OpenAPI_software_image_info_parseFromJSON() failed [minimum_disk]");
        goto end;
    }
    }

    minimum_ram = cJSON_GetObjectItemCaseSensitive(software_image_infoJSON, "minimumRAM");
    if (minimum_ram) {
    if (!cJSON_IsNumber(minimum_ram)) {
        ogs_error("OpenAPI_software_image_info_parseFromJSON() failed [minimum_ram]");
        goto end;
    }
    }

    disc_format = cJSON_GetObjectItemCaseSensitive(software_image_infoJSON, "discFormat");
    if (disc_format) {
    if (!cJSON_IsString(disc_format) && !cJSON_IsNull(disc_format)) {
        ogs_error("OpenAPI_software_image_info_parseFromJSON() failed [disc_format]");
        goto end;
    }
    }

    operating_system = cJSON_GetObjectItemCaseSensitive(software_image_infoJSON, "operatingSystem");
    if (operating_system) {
    if (!cJSON_IsString(operating_system) && !cJSON_IsNull(operating_system)) {
        ogs_error("OpenAPI_software_image_info_parseFromJSON() failed [operating_system]");
        goto end;
    }
    }

    sw_image_ref = cJSON_GetObjectItemCaseSensitive(software_image_infoJSON, "swImageRef");
    if (sw_image_ref) {
    if (!cJSON_IsString(sw_image_ref) && !cJSON_IsNull(sw_image_ref)) {
        ogs_error("OpenAPI_software_image_info_parseFromJSON() failed [sw_image_ref]");
        goto end;
    }
    }

    software_image_info_local_var = OpenAPI_software_image_info_create (
        minimum_disk ? true : false,
        minimum_disk ? minimum_disk->valuedouble : 0,
        minimum_ram ? true : false,
        minimum_ram ? minimum_ram->valuedouble : 0,
        disc_format && !cJSON_IsNull(disc_format) ? ogs_strdup(disc_format->valuestring) : NULL,
        operating_system && !cJSON_IsNull(operating_system) ? ogs_strdup(operating_system->valuestring) : NULL,
        sw_image_ref && !cJSON_IsNull(sw_image_ref) ? ogs_strdup(sw_image_ref->valuestring) : NULL
    );

    return software_image_info_local_var;
end:
    return NULL;
}

OpenAPI_software_image_info_t *OpenAPI_software_image_info_copy(OpenAPI_software_image_info_t *dst, OpenAPI_software_image_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_software_image_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_software_image_info_convertToJSON() failed");
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

    OpenAPI_software_image_info_free(dst);
    dst = OpenAPI_software_image_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

