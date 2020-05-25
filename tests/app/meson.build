# Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>

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

libtestapp_inc = include_directories('.')

libtestepc_cc_args = '-DDEFAULT_CONFIG_FILENAME="@0@/configs/epc.yaml"'.format(meson.build_root())

libtestepc_sources = files('''
    test-packet.c
    epc-init.c
'''.split())

libtestepc = static_library('testepc',
    sources : [libtestepc_sources],
    c_args : libtestepc_cc_args,
    include_directories : [libtestapp_inc, srcinc],
    link_with : [libmme, libhss, libsgw, libpgw, libpcrf],
    dependencies : [libtestcommon_dep,
                    libmme_dep,
                    libhss_dep,
                    libsgw_dep,
                    libpgw_dep,
                    libpcrf_dep],
    install : false)

libtestepc_dep = declare_dependency(
    link_with : libtestepc,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : [libtestcommon_dep,
                    libmme_dep,
                    libhss_dep,
                    libsgw_dep,
                    libpgw_dep,
                    libpcrf_dep])

testepc_sources = files('''
    ../../src/main.c
'''.split())

executable('epc',
    sources : [testepc_sources],
    c_args : libtestepc_cc_args,
    include_directories : srcinc,
    dependencies : [libtestepc_dep])

libtest5gc_cc_args = '-DDEFAULT_CONFIG_FILENAME="@0@/configs/5gc.yaml"'.format(meson.build_root())

libtest5gc_sources = files('''
    5gc-init.c
'''.split())

libtest5gc = static_library('test5gc',
    sources : [libtest5gc_sources],
    c_args : libtest5gc_cc_args,
    include_directories : [libtestapp_inc, srcinc],
    link_with : libamf,
    dependencies : [libtestcommon_dep,
                    libamf_dep],
    install : false)

libtest5gc_dep = declare_dependency(
    link_with : libtest5gc,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : [libtestcommon_dep,
                    libamf_dep])

test5gc_sources = files('''
    ../../src/main.c
'''.split())

executable('5gc',
    sources : [test5gc_sources],
    c_args : libtest5gc_cc_args,
    include_directories : srcinc,
    dependencies : [libtest5gc_dep])
