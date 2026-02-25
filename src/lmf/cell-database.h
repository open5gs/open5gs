/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
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

#ifndef LMF_CELL_DATABASE_H
#define LMF_CELL_DATABASE_H

#include "ogs-core.h"
#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Cell database entry */
typedef struct lmf_cell_info_s {
    ogs_lnode_t lnode;

    ogs_nr_cgi_t ncgi;                  /* NR Cell Global Identifier */
    
    /* Geographic position */
    double latitude;                    /* Latitude in degrees */
    double longitude;                   /* Longitude in degrees */
    double altitude;                    /* Altitude in meters (above sea level) */
    
    /* Cell coverage */
    uint32_t cell_radius;               /* Cell radius in meters */
    
    /* Antenna configuration */
    uint32_t antenna_azimuth;           /* Antenna azimuth in degrees (0-360) */
    uint32_t antenna_beamwidth;         /* Antenna beamwidth in degrees */
    int32_t antenna_tilt;               /* Antenna tilt in degrees (negative = down) */
} lmf_cell_info_t;

/* Initialize cell database */
void lmf_cell_database_init(void);
void lmf_cell_database_final(void);

/* Parse cell database from configuration */
int lmf_cell_database_parse_config(void);

/* Cell database operations */
lmf_cell_info_t *lmf_cell_database_add(ogs_nr_cgi_t *ncgi);
void lmf_cell_database_remove(lmf_cell_info_t *cell_info);
void lmf_cell_database_remove_all(void);

/* Lookup functions */
lmf_cell_info_t *lmf_cell_database_find_by_ncgi(ogs_nr_cgi_t *ncgi);
lmf_cell_info_t *lmf_cell_database_find_by_cell_id(uint64_t cell_id);

/* Get cell position (returns OGS_OK if found, OGS_ERROR if not) */
int lmf_cell_get_position(ogs_nr_cgi_t *ncgi,
        double *latitude, double *longitude, double *altitude);

/* Get cell coverage radius */
int lmf_cell_get_radius(ogs_nr_cgi_t *ncgi, uint32_t *radius);

/* Get antenna configuration */
int lmf_cell_get_antenna_config(ogs_nr_cgi_t *ncgi,
        uint32_t *azimuth, uint32_t *beamwidth, int32_t *tilt);

#ifdef __cplusplus
}
#endif

#endif /* LMF_CELL_DATABASE_H */



