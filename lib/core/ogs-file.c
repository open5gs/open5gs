/*
 * Copyright (C) 2022 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-core.h"

int ogs_file_read_full(
        const char *filename, void *buf, size_t nbytes, size_t *bytes_read)
{
    FILE *fp = NULL;
    size_t total_read = 0, amt;

    ogs_assert(filename);
    ogs_assert(buf);
    ogs_assert(nbytes);

    fp = fopen(filename, "rb");
    if (!fp) {
        ogs_error("Cannot find file [%s]", filename);
        return OGS_ERROR;
    }

    amt = fread(buf, 1, nbytes, fp);

    if (bytes_read != NULL)
        *bytes_read = amt;

    fclose(fp);

    if (amt > 0)
        return OGS_OK;

    ogs_error("Cannot read file [name:%s,amt:%d,total_read:%d]",
            filename, (int)amt, (int)total_read);
    return OGS_ERROR;
}
