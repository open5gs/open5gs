# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

subdir('openapi')

libsbi_sources = files('''
    contrib/multipart_parser.c

    custom/any_type.c

    custom/links.c
    custom/ue_authentication_ctx.c
    custom/patch_item.c

    yuarel.c
    conv.c
    message.c

    mhd-server.c
    nghttp2-server.c
    server.c

    client.c
    context.c

    nnrf-build.c
    nnrf-handler.c

    path.c
'''.split())

libsbi_inc = include_directories('.')

sbi_cc_flags = ['-DOGS_SBI_COMPILATION']

libgnutls_dep = cc.find_library('gnutls', required : true)
libnghttp2_dep = dependency('libnghttp2', version: '>=1.18.1')
libmicrohttpd_dep = dependency('libmicrohttpd', version: '>=0.9.40')
libcurl_dep = dependency('libcurl', version: '>=7.52.1')

libsbi = library('ogssbi',
    sources : libsbi_sources,
    version : libogslib_version,
    c_args : sbi_cc_flags,
    include_directories : [libsbi_inc, libinc],
    dependencies : [libcore_dep,
                    libcrypt_dep,
                    libapp_dep,
                    libsbi_openapi_dep,
                    libgnutls_dep,
                    libnghttp2_dep,
                    libmicrohttpd_dep,
                    libcurl_dep],
    install_rpath : libdir,
    install : true)

libsbi_dep = declare_dependency(
    link_with : libsbi,
    include_directories : [libsbi_inc, libinc],
    dependencies : [libcore_dep,
                    libcrypt_dep,
                    libapp_dep,
                    libsbi_openapi_dep,
                    libgnutls_dep,
                    libnghttp2_dep,
                    libmicrohttpd_dep,
                    libcurl_dep])
