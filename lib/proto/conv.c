/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-proto.h"

void ogs_extract_digit_from_string(char *digit, char *string)
{
    bool extracting = false;
    int i = 0;

    ogs_assert(string);
    ogs_assert(digit);

    while (*string && i < OGS_MAX_IMSI_BCD_LEN) {
        if (*string >= '0' && *string <= '9') {
            *digit++ = *string;
            extracting = true;
        } else if (extracting == true) {
            break;
        }
        string++;
        i++;
    }

    *digit = 0;
}
