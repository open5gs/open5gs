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

libtestcommon_conf = configuration_data()
libtestcommon_conf.set_quoted('MESON_BUILD_ROOT', meson.build_root())
configure_file(output : 'test-config-private.h',
               configuration : libtestcommon_conf)

libtestcommon_sources = files('''
    sctp.c
    gtpu.c
    context.c
    application.c

    ngap-build.c
'''.split())

libtestcommon_inc = include_directories('.')

libtestcommon = static_library('testcomon',
    sources : libtestcommon_sources,
    c_args : testcore_cc_flags,
    include_directories : [libtestcommon_inc, testinc],
    dependencies : [libcore_dep,
                    libapp_dep,
                    libdbi_dep,
                    libsctp_dep,
                    libngap_dep,
                    libnas_eps_dep,
                    libnas_5gs_dep,
                    libdiameter_common_dep],
    install : false)

libtestcommon_dep = declare_dependency(
    link_with : libtestcommon,
    include_directories : [libtestcommon_inc, testinc],
    dependencies : [libcore_dep,
                    libapp_dep,
                    libdbi_dep,
                    libsctp_dep,
                    libngap_dep,
                    libnas_eps_dep,
                    libnas_5gs_dep,
                    libdiameter_common_dep])
