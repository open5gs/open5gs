/*
 * Copyright (C) 2026 by DNL
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

#include "core/abts.h"

abts_suite *test_cdr_encode(abts_suite *suite);
abts_suite *test_cdr_file(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    { test_cdr_encode },
    { test_cdr_file },
    { NULL },
};

int main(int argc, const char *const argv[])
{
    int i;
    int rv;
    const char *argv_out[argc+3]; /* '-e error' is always added */
    abts_suite *suite = NULL;

    rv = abts_main(argc, argv, argv_out);
    if (rv != 0) return rv;

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
