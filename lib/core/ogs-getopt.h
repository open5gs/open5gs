/*
 * Copyright (C) 2019-2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * The code is stolen from optparse
 * https://github.com/skeeto/optparse
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GETOPT_H
#define OGS_GETOPT_H

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
