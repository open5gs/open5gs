/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GETOPT_H
#define OGS_GETOPT_H

/*
 * The following code is stolen from optparse
 * https://github.com/skeeto/optparse
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_getopt_s {
    char **argv;
    int permute;
    int optind;
    int optopt;
    char *optarg;
    char errmsg[64];
    int subopt;
} ogs_getopt_t;

typedef enum {
    OGS_GETOPT_NONE,
    OGS_GETOPT_REQUIRED,
    OGS_GETOPT_OPTIONAL
} ogs_getopt_argtype_e;

typedef struct ogs_getopt_long_s {
    const char *longname;
    int shortname;
    ogs_getopt_argtype_e argtype;
} ogs_getopt_long_t;

void ogs_getopt_init(ogs_getopt_t *options, char **argv);
int ogs_getopt(ogs_getopt_t *options, const char *optstring);
int ogs_getopt_long(ogs_getopt_t *options,
                  const ogs_getopt_long_t *longopts,
                  int *longindex);
char *ogs_getopt_arg(ogs_getopt_t *options);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GETOPT_H */
