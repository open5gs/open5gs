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

#if !defined(OGS_CRYPT_INSIDE) && !defined(OGS_CRYPT_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_BASE64_H
#define OGS_BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

int ogs_base64_decode_len(const char *coded_src);
int ogs_base64_decode(char *plain_dst, const char *coded_src);
int ogs_base64_decode_binary(
        unsigned char *plain_dst, const char *coded_src);

int ogs_base64_encode_len(int len);
int ogs_base64_encode(
        char *coded_dst, const char *plain_src, int len_plain_src);
int ogs_base64_encode_binary(
        char *coded_dst, const unsigned char *plain_src, int len_plain_src);

#ifdef __cplusplus
}
#endif

#endif /* OGS_BASE64_H */
