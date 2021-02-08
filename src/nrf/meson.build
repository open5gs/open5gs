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

libnrf_sources = files('''
    context.c
    event.c
    timer.c

    nnrf-handler.c
    nnrf-build.c
    sbi-path.c

    nf-sm.c
    nrf-sm.c

    init.c
'''.split())

libnrf = static_library('nrf',
    sources : libnrf_sources,
    dependencies : [libapp_dep,
                    libdbi_dep,
                    libsbi_dep],
    install : false)

libnrf_dep = declare_dependency(
    link_with : libnrf,
    dependencies : [libapp_dep,
                    libdbi_dep,
                    libsbi_dep])

nrf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-nrfd',
    sources : nrf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/nrf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libnrf_dep,
    install_rpath : libdir,
    install : true)
