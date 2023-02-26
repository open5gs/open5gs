#ifndef OGS_SBI_LIST_H
#define OGS_SBI_LIST_H

#include "../external/cJSON.h"
#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_list_s OpenAPI_list_t;

typedef struct OpenAPI_lnode_s OpenAPI_lnode_t;

struct OpenAPI_lnode_s {
    OpenAPI_lnode_t *next;
    OpenAPI_lnode_t *prev;
    void *data;
};

typedef struct OpenAPI_list_s {
    OpenAPI_lnode_t *first;
    OpenAPI_lnode_t *last;

    long count;
} OpenAPI_list_t;

#define OpenAPI_list_for_each(list, element) for(element = (list != NULL) ? (list)->first : NULL; element != NULL; element = element->next)

OpenAPI_list_t *OpenAPI_list_create(void);
void OpenAPI_list_free(OpenAPI_list_t *listToFree);

void OpenAPI_list_add(OpenAPI_list_t *list, void *dataToAddInList);
OpenAPI_lnode_t *OpenAPI_list_find(OpenAPI_list_t *list, long indexOfElement);
void OpenAPI_list_remove(
    OpenAPI_list_t *list, OpenAPI_lnode_t *elementToRemove);

void OpenAPI_list_iterate_forward(OpenAPI_list_t *list,
        void (*operationToPerform)(OpenAPI_lnode_t*, void*),
        void *additionalDataNeededForCallbackFunction);
void OpenAPI_list_iterate_backward(OpenAPI_list_t *list,
        void (*operationToPerform)(OpenAPI_lnode_t*, void*),
        void *additionalDataNeededForCallbackFunction);

void OpenAPI_lnode_print(OpenAPI_lnode_t *listEntry, void *additionalData);
void OpenAPI_lnode_free(OpenAPI_lnode_t *listEntry, void *additionalData);

char *OpenAPI_findStrInStrList(OpenAPI_list_t *strList, const char *str);
void OpenAPI_clear_and_free_string_list(OpenAPI_list_t *list);

#ifdef __cplusplus
}
#endif

#endif // OGS_SBI_LIST_H
