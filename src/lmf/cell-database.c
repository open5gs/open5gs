/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "cell-database.h"
#include "context.h"
#include "ogs-app.h"
#include <math.h>

static ogs_list_t cell_database_list;
static int cell_database_initialized = 0;

#define MAX_NUM_OF_CELLS 1024
static OGS_POOL(cell_info_pool, lmf_cell_info_t);
static int max_num_of_cells = 0;

void lmf_cell_database_init(void)
{
    ogs_assert(cell_database_initialized == 0);

    ogs_list_init(&cell_database_list);

    max_num_of_cells = MAX_NUM_OF_CELLS;
    ogs_pool_init(&cell_info_pool, max_num_of_cells);

    cell_database_initialized = 1;
}

void lmf_cell_database_final(void)
{
    ogs_assert(cell_database_initialized == 1);

    lmf_cell_database_remove_all();

    ogs_pool_final(&cell_info_pool);

    cell_database_initialized = 0;
}

int lmf_cell_database_parse_config(void)
{
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter, lmf_iter, cell_db_iter, cell_iter;
    const char *root_key;

    document = ogs_app()->document;
    ogs_assert(document);

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);

        if (!strcmp(root_key, "lmf")) {
            ogs_yaml_iter_recurse(&root_iter, &lmf_iter);
            while (ogs_yaml_iter_next(&lmf_iter)) {
                const char *lmf_key = ogs_yaml_iter_key(&lmf_iter);
                ogs_assert(lmf_key);

                if (!strcmp(lmf_key, "cell_database")) {
                    ogs_yaml_iter_recurse(&lmf_iter, &cell_db_iter);
                    while (ogs_yaml_iter_next(&cell_db_iter)) {
                        const char *cell_key = ogs_yaml_iter_key(&cell_db_iter);
                        ogs_assert(cell_key);

                        if (!strcmp(cell_key, "cells")) {
                            /* Parse cells array */
                            ogs_yaml_iter_recurse(&cell_db_iter, &cell_iter);
                            while (ogs_yaml_iter_next(&cell_iter)) {
                                ogs_yaml_iter_t cell_item_iter;
                                ogs_nr_cgi_t ncgi;
                                lmf_cell_info_t *cell_info = NULL;
                                uint64_t ncgi_value = 0;
                                double latitude = 0.0, longitude = 0.0, altitude = 0.0;
                                uint32_t cell_radius = 500;  /* Default 500m */
                                uint32_t antenna_azimuth = 0;
                                uint32_t antenna_beamwidth = 65;  /* Default 65 degrees */
                                int32_t antenna_tilt = 0;
                                ogs_plmn_id_t plmn_id;
                                uint16_t mcc = 0, mnc = 0;
                                uint16_t mnc_len = 2;  /* Default 2-digit MNC */

                                memset(&ncgi, 0, sizeof(ncgi));
                                memset(&plmn_id, 0, sizeof(plmn_id));

                                ogs_yaml_iter_recurse(&cell_iter, &cell_item_iter);
                                while (ogs_yaml_iter_next(&cell_item_iter)) {
                                    const char *item_key = ogs_yaml_iter_key(&cell_item_iter);
                                    ogs_assert(item_key);

                                    if (!strcmp(item_key, "ncgi")) {
                                        const char *ncgi_str = ogs_yaml_iter_value(&cell_item_iter);
                                        if (ncgi_str) {
                                            ncgi_value = strtoull(ncgi_str, NULL, 0);
                                        }
                                    } else if (!strcmp(item_key, "plmn_id")) {
                                        ogs_yaml_iter_t plmn_iter;
                                        ogs_yaml_iter_recurse(&cell_item_iter, &plmn_iter);
                                        while (ogs_yaml_iter_next(&plmn_iter)) {
                                            const char *plmn_key = ogs_yaml_iter_key(&plmn_iter);
                                            ogs_assert(plmn_key);
                                            const char *plmn_val = ogs_yaml_iter_value(&plmn_iter);

                                            if (!strcmp(plmn_key, "mcc")) {
                                                mcc = atoi(plmn_val);
                                            } else if (!strcmp(plmn_key, "mnc")) {
                                                mnc = atoi(plmn_val);
                                                mnc_len = (mnc >= 100) ? 3 : 2;
                                            }
                                        }
                                    } else if (!strcmp(item_key, "latitude")) {
                                        const char *lat_str = ogs_yaml_iter_value(&cell_item_iter);
                                        if (lat_str) {
                                            latitude = atof(lat_str);
                                        }
                                    } else if (!strcmp(item_key, "longitude")) {
                                        const char *lon_str = ogs_yaml_iter_value(&cell_item_iter);
                                        if (lon_str) {
                                            longitude = atof(lon_str);
                                        }
                                    } else if (!strcmp(item_key, "altitude")) {
                                        const char *alt_str = ogs_yaml_iter_value(&cell_item_iter);
                                        if (alt_str) {
                                            altitude = atof(alt_str);
                                        }
                                    } else if (!strcmp(item_key, "cell_radius")) {
                                        const char *radius_str = ogs_yaml_iter_value(&cell_item_iter);
                                        if (radius_str) {
                                            cell_radius = atoi(radius_str);
                                        }
                                    } else if (!strcmp(item_key, "antenna")) {
                                        ogs_yaml_iter_t antenna_iter;
                                        ogs_yaml_iter_recurse(&cell_item_iter, &antenna_iter);
                                        while (ogs_yaml_iter_next(&antenna_iter)) {
                                            const char *antenna_key = ogs_yaml_iter_key(&antenna_iter);
                                            ogs_assert(antenna_key);
                                            const char *antenna_val = ogs_yaml_iter_value(&antenna_iter);

                                            if (!strcmp(antenna_key, "azimuth")) {
                                                antenna_azimuth = atoi(antenna_val);
                                            } else if (!strcmp(antenna_key, "beamwidth")) {
                                                antenna_beamwidth = atoi(antenna_val);
                                            } else if (!strcmp(antenna_key, "tilt")) {
                                                antenna_tilt = atoi(antenna_val);
                                            }
                                        }
                                    }
                                }

                                /* Create cell info entry */
                                /* Check if coordinates are set (not zero) */
                                if (ncgi_value > 0 && (fabs(latitude) > 0.0001 || fabs(longitude) > 0.0001)) {
                                    /* Build NCGI from PLMN ID and cell ID */
                                    ncgi.cell_id = ncgi_value;
                                    if (mcc > 0 && mnc > 0) {
                                        ogs_plmn_id_build(&ncgi.plmn_id, mcc, mnc, mnc_len);
                                    } else {
                                        /* Default PLMN if not specified */
                                        ogs_plmn_id_build(&ncgi.plmn_id, 999, 70, 2);
                                    }

                                    cell_info = lmf_cell_database_add(&ncgi);
                                    if (cell_info) {
                                        cell_info->latitude = latitude;
                                        cell_info->longitude = longitude;
                                        cell_info->altitude = altitude;
                                        cell_info->cell_radius = cell_radius;
                                        cell_info->antenna_azimuth = antenna_azimuth;
                                        cell_info->antenna_beamwidth = antenna_beamwidth;
                                        cell_info->antenna_tilt = antenna_tilt;

                                        ogs_info("Cell database: Added cell NCGI=0x%llx "
                                                "[PLMN:%06x,CELL:0x%llx] "
                                                "lat=%.6f,lon=%.6f,alt=%.1f,radius=%um",
                                                (unsigned long long)ncgi_value,
                                                ogs_plmn_id_hexdump(&ncgi.plmn_id),
                                                (unsigned long long)ncgi.cell_id,
                                                latitude, longitude, altitude, cell_radius);
                                    }
                                } else {
                                    ogs_warn("Cell database: Skipping cell (missing ncgi or position)");
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return OGS_OK;
}

lmf_cell_info_t *lmf_cell_database_add(ogs_nr_cgi_t *ncgi)
{
    lmf_cell_info_t *cell_info = NULL;

    ogs_assert(ncgi);

    /* Check if cell already exists */
    cell_info = lmf_cell_database_find_by_ncgi(ncgi);
    if (cell_info) {
        ogs_warn("Cell database: Cell already exists [PLMN:%06x,CELL:0x%llx]",
                ogs_plmn_id_hexdump(&ncgi->plmn_id),
                (unsigned long long)ncgi->cell_id);
        return cell_info;
    }

    ogs_pool_alloc(&cell_info_pool, &cell_info);
    ogs_assert(cell_info);
    memset(cell_info, 0, sizeof(*cell_info));

    memcpy(&cell_info->ncgi, ncgi, sizeof(ogs_nr_cgi_t));

    /* Default values */
    cell_info->cell_radius = 500;      /* 500 meters */
    cell_info->antenna_beamwidth = 65; /* 65 degrees */
    cell_info->antenna_tilt = 0;

    ogs_list_add(&cell_database_list, cell_info);

    return cell_info;
}

void lmf_cell_database_remove(lmf_cell_info_t *cell_info)
{
    ogs_assert(cell_info);

    ogs_list_remove(&cell_database_list, cell_info);
    ogs_pool_free(&cell_info_pool, cell_info);
}

void lmf_cell_database_remove_all(void)
{
    lmf_cell_info_t *cell_info = NULL, *next_cell_info = NULL;

    ogs_list_for_each_safe(&cell_database_list, next_cell_info, cell_info)
        lmf_cell_database_remove(cell_info);
}

lmf_cell_info_t *lmf_cell_database_find_by_ncgi(ogs_nr_cgi_t *ncgi)
{
    lmf_cell_info_t *cell_info = NULL;

    ogs_assert(ncgi);

    ogs_list_for_each(&cell_database_list, cell_info) {
        if (memcmp(&cell_info->ncgi.plmn_id, &ncgi->plmn_id, sizeof(ogs_plmn_id_t)) == 0 &&
            cell_info->ncgi.cell_id == ncgi->cell_id) {
            return cell_info;
        }
    }

    return NULL;
}

lmf_cell_info_t *lmf_cell_database_find_by_cell_id(uint64_t cell_id)
{
    lmf_cell_info_t *cell_info = NULL;

    ogs_list_for_each(&cell_database_list, cell_info) {
        if (cell_info->ncgi.cell_id == cell_id) {
            return cell_info;
        }
    }

    return NULL;
}

int lmf_cell_get_position(ogs_nr_cgi_t *ncgi,
        double *latitude, double *longitude, double *altitude)
{
    lmf_cell_info_t *cell_info = NULL;

    ogs_assert(ncgi);
    ogs_assert(latitude);
    ogs_assert(longitude);
    ogs_assert(altitude);

    cell_info = lmf_cell_database_find_by_ncgi(ncgi);
    if (!cell_info) {
        return OGS_ERROR;
    }

    *latitude = cell_info->latitude;
    *longitude = cell_info->longitude;
    *altitude = cell_info->altitude;

    return OGS_OK;
}

int lmf_cell_get_radius(ogs_nr_cgi_t *ncgi, uint32_t *radius)
{
    lmf_cell_info_t *cell_info = NULL;

    ogs_assert(ncgi);
    ogs_assert(radius);

    cell_info = lmf_cell_database_find_by_ncgi(ncgi);
    if (!cell_info) {
        return OGS_ERROR;
    }

    *radius = cell_info->cell_radius;

    return OGS_OK;
}

int lmf_cell_get_antenna_config(ogs_nr_cgi_t *ncgi,
        uint32_t *azimuth, uint32_t *beamwidth, int32_t *tilt)
{
    lmf_cell_info_t *cell_info = NULL;

    ogs_assert(ncgi);
    ogs_assert(azimuth);
    ogs_assert(beamwidth);
    ogs_assert(tilt);

    cell_info = lmf_cell_database_find_by_ncgi(ncgi);
    if (!cell_info) {
        return OGS_ERROR;
    }

    *azimuth = cell_info->antenna_azimuth;
    *beamwidth = cell_info->antenna_beamwidth;
    *tilt = cell_info->antenna_tilt;

    return OGS_OK;
}

