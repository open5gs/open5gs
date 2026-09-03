/*
 * Copyright (C) 2026 by Erol Yağız Aydın <ygzaydns@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "sbi-path.h"

static eir_context_t self;

int __eir_log_domain;

static int context_initialized = 0;

void eir_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize EIR context */
    memset(&self, 0, sizeof(eir_context_t));

    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", ogs_core()->log.level);
    ogs_log_install_domain(&__eir_log_domain, "eir", ogs_core()->log.level);

    context_initialized = 1;
}

void eir_context_final(void)
{
    ogs_assert(context_initialized == 1);

    context_initialized = 0;
}

eir_context_t *eir_self(void)
{
    return &self;
}

static int eir_context_prepare(void)
{
    return OGS_OK;
}

static int eir_context_validation(void)
{
    return OGS_OK;
}

int eir_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = eir_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if ((!strcmp(root_key, "eir")) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t eir_iter;
            ogs_yaml_iter_recurse(&root_iter, &eir_iter);
            while (ogs_yaml_iter_next(&eir_iter)) {
                const char *eir_key = ogs_yaml_iter_key(&eir_iter);
                ogs_assert(eir_key);
                if (!strcmp(eir_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(eir_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(eir_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(eir_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(eir_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(eir_key, "discovery")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", eir_key);
            }
        }
    }

    rv = eir_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}
