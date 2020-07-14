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

libpcrf_sources = files('''
    pcrf-context.h
    pcrf-fd-path.h

    pcrf-init.c
    pcrf-context.c
    pcrf-fd-path.c
    pcrf-gx-path.c
    pcrf-rx-path.c
'''.split())

libpcrf = static_library('pcrf',
    sources : libpcrf_sources,
    dependencies : [libapp_dep,
                    libdbi_dep,
                    libdiameter_rx_dep,
                    libdiameter_gx_dep],
    install : false)

libpcrf_dep = declare_dependency(
    link_with : libpcrf,
    dependencies : [libapp_dep,
                    libdbi_dep,
                    libdiameter_rx_dep,
                    libdiameter_gx_dep])

pcrf_sources = files('''
    app-init.c
    ../main.c
'''.split())

executable('open5gs-pcrfd',
    sources : pcrf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/pcrf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libpcrf_dep,
    install_rpath : libdir,
    install : true)
