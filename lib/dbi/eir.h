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

#if !defined(OGS_DBI_INSIDE) && !defined(OGS_DBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DBI_EIR_H
#define OGS_DBI_EIR_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_EIR_PEI_STRING    "pei"
#define OGS_EIR_SUPI_STRING   "supi"
#define OGS_EIR_STATUS_STRING "status"

/*
 * lib/dbi does not depend on lib/sbi, so the record uses its own status
 * enum rather than OpenAPI_equipment_status_e. The N5g-eir handler in
 * src/eir maps this to the OpenAPI type when building the SBI response.
 */
typedef enum ogs_dbi_eir_status_e {
    OGS_DBI_EIR_STATUS_UNKNOWN = 0,
    OGS_DBI_EIR_STATUS_WHITELISTED,
    OGS_DBI_EIR_STATUS_BLACKLISTED,
    OGS_DBI_EIR_STATUS_GREYLISTED,
} ogs_dbi_eir_status_t;

typedef struct ogs_dbi_eir_record_s {
    char *pei;
    char *supi;
    ogs_dbi_eir_status_t status;
} ogs_dbi_eir_record_t;

int ogs_dbi_eir_check_equipment(
        const char *supi, const char *pei, ogs_dbi_eir_record_t *record);
void ogs_dbi_eir_record_free(ogs_dbi_eir_record_t *record);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DBI_EIR_H */
