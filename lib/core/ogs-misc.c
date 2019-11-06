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

#include "ogs-core.h"

#define PATH_SEPARATOR '/'

/* Remove trailing separators that don't affect the meaning of PATH. */
static void path_canonicalize(char *dir)
{
    /* At some point this could eliminate redundant components.  For
     * now, it just makes sure there is no trailing slash. */
    size_t len = strlen (dir);
    size_t orig_len = len;

    ogs_assert(dir);

    while ((len > 0) && (dir[len - 1] == PATH_SEPARATOR))
        len--;

    if (len != orig_len)
        dir[len] = 0;
}

/* Remove one component off the end of PATH. */
void ogs_path_remove_last_component(char *dir, const char *path)
{
    int i;
    int len = 0;

    ogs_assert(dir);
    ogs_assert(path);

    strcpy(dir, path);
    path_canonicalize (dir);
    for (i = (strlen(dir) - 1); i >= 0; i--) {
        if (path[i] == PATH_SEPARATOR)
            break;
    }

    len = (i < 0) ? 0 : i;
    dir[len] = 0;
}

bool ogs_path_is_absolute(const char *filename)
{
    ogs_assert(filename);

    if (OGS_IS_DIR_SEPARATOR(filename[0]))
        return true;

#if defined(_WIN32)
    if (isalpha(filename[0]) &&
        filename[1] == ':' && OGS_IS_DIR_SEPARATOR(filename[2]))
    return true;
#endif

    return false;
}
