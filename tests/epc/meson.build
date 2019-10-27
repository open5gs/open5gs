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
#

testepc_conf = configuration_data()
testepc_conf.set_quoted('MESON_BUILD_ROOT', meson.build_root())
configure_file(output : 'test-config-private.h', configuration : testepc_conf)

testepc_cc_args = '-DDEFAULT_CONFIG_FILENAME="@0@/configs/simple.yaml"'.format(meson.build_root())

libtestepc_sources = files('''
    app-init.c
'''.split())

libtestepc_inc = include_directories('.')

libtestepc = static_library('epc',
    sources : [libtestepc_sources],
    c_args : testepc_cc_args,
    include_directories : [libtestepc_inc, srcinc],
    link_with : [libmme, libhss, libsgw, libpgw, libpcrf],
    dependencies : [libmme_dep,
                    libhss_dep,
                    libsgw_dep,
                    libpgw_dep,
                    libpcrf_dep],
    install : false)

libtestepc_dep = declare_dependency(
    link_with : libtestepc,
    include_directories : [libtestepc_inc, srcinc],
    dependencies : [libmme_dep,
                    libhss_dep,
                    libsgw_dep,
                    libpgw_dep,
                    libpcrf_dep])

testepc_sources = files('''
    ../../src/main.c
'''.split())

executable('epc',
    sources : [testepc_sources],
    c_args : testepc_cc_args,
    include_directories : srcinc,
    dependencies : [libtestepc_dep])
