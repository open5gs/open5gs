/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_TEST_INSIDE) && !defined(OGS_TEST_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef TEST_COMMON_APPLICATION_H
#define TEST_COMMON_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

void test_app_run(int argc, const char *const argv[],
        const char *name, void (*init)(const char * const argv[]));
void test_child_terminate(void);
ogs_thread_t *test_child_create(const char *name, const char *const argv[]);

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_APPLICATION_H */
