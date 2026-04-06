
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "periodicity_range.h"

OpenAPI_periodicity_range_t *OpenAPI_periodicity_range_create(
    bool is_lower_bound,
    int lower_bound,
    bool is_upper_bound,
    int upper_bound,
    OpenAPI_list_t *periodic_vals,
    OpenAPI_list_t *add_periodic_vals
)
{
    OpenAPI_periodicity_range_t *periodicity_range_local_var = ogs_malloc(sizeof(OpenAPI_periodicity_range_t));
    ogs_assert(periodicity_range_local_var);

    periodicity_range_local_var->is_lower_bound = is_lower_bound;
    periodicity_range_local_var->lower_bound = lower_bound;
    periodicity_range_local_var->is_upper_bound = is_upper_bound;
    periodicity_range_local_var->upper_bound = upper_bound;
    periodicity_range_local_var->periodic_vals = periodic_vals;
    periodicity_range_local_var->add_periodic_vals = add_periodic_vals;

    return periodicity_range_local_var;
}

void OpenAPI_periodicity_range_free(OpenAPI_periodicity_range_t *periodicity_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == periodicity_range) {
        return;
    }
    if (periodicity_range->periodic_vals) {
        OpenAPI_list_for_each(periodicity_range->periodic_vals, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(periodicity_range->periodic_vals);
        periodicity_range->periodic_vals = NULL;
    }
    if (periodicity_range->add_periodic_vals) {
        OpenAPI_list_for_each(periodicity_range->add_periodic_vals, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(periodicity_range->add_periodic_vals);
        periodicity_range->add_periodic_vals = NULL;
    }
    ogs_free(periodicity_range);
}

cJSON *OpenAPI_periodicity_range_convertToJSON(OpenAPI_periodicity_range_t *periodicity_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (periodicity_range == NULL) {
        ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [PeriodicityRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (periodicity_range->is_lower_bound) {
    if (cJSON_AddNumberToObject(item, "lowerBound", periodicity_range->lower_bound) == NULL) {
        ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [lower_bound]");
        goto end;
    }
    }

    if (periodicity_range->is_upper_bound) {
    if (cJSON_AddNumberToObject(item, "upperBound", periodicity_range->upper_bound) == NULL) {
        ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [upper_bound]");
        goto end;
    }
    }

    if (periodicity_range->periodic_vals) {
    cJSON *periodic_valsList = cJSON_AddArrayToObject(item, "periodicVals");
    if (periodic_valsList == NULL) {
        ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [periodic_vals]");
        goto end;
    }
    OpenAPI_list_for_each(periodicity_range->periodic_vals, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [periodic_vals]");
            goto end;
        }
        if (cJSON_AddNumberToObject(periodic_valsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [periodic_vals]");
            goto end;
        }
    }
    }

    if (periodicity_range->add_periodic_vals) {
    cJSON *add_periodic_valsList = cJSON_AddArrayToObject(item, "addPeriodicVals");
    if (add_periodic_valsList == NULL) {
        ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [add_periodic_vals]");
        goto end;
    }
    OpenAPI_list_for_each(periodicity_range->add_periodic_vals, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [add_periodic_vals]");
            goto end;
        }
        if (cJSON_AddNumberToObject(add_periodic_valsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_periodicity_range_convertToJSON() failed [add_periodic_vals]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_periodicity_range_t *OpenAPI_periodicity_range_parseFromJSON(cJSON *periodicity_rangeJSON)
{
    OpenAPI_periodicity_range_t *periodicity_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *lower_bound = NULL;
    cJSON *upper_bound = NULL;
    cJSON *periodic_vals = NULL;
    OpenAPI_list_t *periodic_valsList = NULL;
    cJSON *add_periodic_vals = NULL;
    OpenAPI_list_t *add_periodic_valsList = NULL;
    lower_bound = cJSON_GetObjectItemCaseSensitive(periodicity_rangeJSON, "lowerBound");
    if (lower_bound) {
    if (!cJSON_IsNumber(lower_bound)) {
        ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [lower_bound]");
        goto end;
    }
    }

    upper_bound = cJSON_GetObjectItemCaseSensitive(periodicity_rangeJSON, "upperBound");
    if (upper_bound) {
    if (!cJSON_IsNumber(upper_bound)) {
        ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [upper_bound]");
        goto end;
    }
    }

    periodic_vals = cJSON_GetObjectItemCaseSensitive(periodicity_rangeJSON, "periodicVals");
    if (periodic_vals) {
        cJSON *periodic_vals_local = NULL;
        if (!cJSON_IsArray(periodic_vals)) {
            ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [periodic_vals]");
            goto end;
        }

        periodic_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(periodic_vals_local, periodic_vals) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(periodic_vals_local)) {
                ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [periodic_vals]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [periodic_vals]");
                goto end;
            }
            *localDouble = periodic_vals_local->valuedouble;
            OpenAPI_list_add(periodic_valsList, localDouble);
        }
    }

    add_periodic_vals = cJSON_GetObjectItemCaseSensitive(periodicity_rangeJSON, "addPeriodicVals");
    if (add_periodic_vals) {
        cJSON *add_periodic_vals_local = NULL;
        if (!cJSON_IsArray(add_periodic_vals)) {
            ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [add_periodic_vals]");
            goto end;
        }

        add_periodic_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_periodic_vals_local, add_periodic_vals) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(add_periodic_vals_local)) {
                ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [add_periodic_vals]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_periodicity_range_parseFromJSON() failed [add_periodic_vals]");
                goto end;
            }
            *localDouble = add_periodic_vals_local->valuedouble;
            OpenAPI_list_add(add_periodic_valsList, localDouble);
        }
    }

    periodicity_range_local_var = OpenAPI_periodicity_range_create (
        lower_bound ? true : false,
        lower_bound ? lower_bound->valuedouble : 0,
        upper_bound ? true : false,
        upper_bound ? upper_bound->valuedouble : 0,
        periodic_vals ? periodic_valsList : NULL,
        add_periodic_vals ? add_periodic_valsList : NULL
    );

    return periodicity_range_local_var;
end:
    if (periodic_valsList) {
        OpenAPI_list_for_each(periodic_valsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(periodic_valsList);
        periodic_valsList = NULL;
    }
    if (add_periodic_valsList) {
        OpenAPI_list_for_each(add_periodic_valsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(add_periodic_valsList);
        add_periodic_valsList = NULL;
    }
    return NULL;
}

OpenAPI_periodicity_range_t *OpenAPI_periodicity_range_copy(OpenAPI_periodicity_range_t *dst, OpenAPI_periodicity_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_periodicity_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_periodicity_range_convertToJSON() failed");
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

    OpenAPI_periodicity_range_free(dst);
    dst = OpenAPI_periodicity_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

