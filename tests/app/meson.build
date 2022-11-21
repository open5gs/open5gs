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

libtestepc_cc_args = '-DDEFAULT_CONFIG_FILENAME="@0@/configs/sample.yaml"'.format(open5gs_build_dir)

libtestepc_sources = files('''
    epc-init.c
'''.split())

libtestepc = static_library('testepc',
    sources : [libtestepc_sources],
    c_args : libtestepc_cc_args,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : libtestcommon_dep,
    install : false)

libtestepc_dep = declare_dependency(
    link_with : libtestepc,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : libtestcommon_dep)

testepc_sources = files('''
    ../../src/main.c
'''.split())

executable('epc',
    sources : [testepc_sources],
    c_args : libtestepc_cc_args,
    include_directories : srcinc,
    dependencies : libtestepc_dep)

libtest5gc_cc_args = '-DDEFAULT_CONFIG_FILENAME="@0@/configs/sample.yaml"'.format(open5gs_build_dir)

libtest5gc_sources = files('''
    5gc-init.c
'''.split())

libtest5gc = static_library('test5gc',
    sources : [libtest5gc_sources],
    c_args : libtest5gc_cc_args,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : libtestcommon_dep,
    install : false)

libtest5gc_dep = declare_dependency(
    link_with : libtest5gc,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : libtestcommon_dep)

test5gc_sources = files('''
    ../../src/main.c
'''.split())

executable('5gc',
    sources : [test5gc_sources],
    c_args : libtest5gc_cc_args,
    include_directories : srcinc,
    dependencies : libtest5gc_dep)

libtestapp_cc_args = '-DDEFAULT_CONFIG_FILENAME="@0@/configs/sample.yaml"'.format(open5gs_build_dir)

libtestapp_sources = files('''
    app-init.c
'''.split())

libtestapp = static_library('testapp',
    sources : [libtestapp_sources],
    c_args : libtestapp_cc_args,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : libtestcommon_dep,
    install : false)

libtestapp_dep = declare_dependency(
    link_with : libtestapp,
    include_directories : [libtestapp_inc, srcinc],
    dependencies : libtestcommon_dep)

testapp_sources = files('''
    ../../src/main.c
'''.split())

executable('app',
    sources : [testapp_sources],
    c_args : libtestapp_cc_args,
    include_directories : srcinc,
    dependencies : libtestapp_dep)
