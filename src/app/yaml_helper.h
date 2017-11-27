#ifndef __YAML_HELPER_H__
#define __YAML_HELPER_H__

#include "core_debug.h"

#include <yaml.h>

/* YAML helper functions */
CORE_DECLARE(const char *)  YAML_MAPPING_KEY(
        yaml_document_t *document, yaml_node_pair_t *node_pair);
CORE_DECLARE(const char *)  YAML_MAPPING_VALUE(
        yaml_document_t *document, yaml_node_pair_t *node_pair);
CORE_DECLARE(int)           YAML_MAPPING_BOOL(
        yaml_document_t *document, yaml_node_pair_t *node_pair);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __YAML_HELPER_H__ */
