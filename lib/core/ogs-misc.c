/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2019-2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
