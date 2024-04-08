# Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>

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

libhss_sources = files('''
    hss-context.h
    hss-fd-path.h
    hss-s6a-path.h
    hss-event.h
    hss-timer.h
    hss-sm.h
    metrics.h

    hss-init.c
    hss-context.c
    hss-event.c
    hss-timer.c
    hss-sm.c

    hss-s6a-path.c
    hss-cx-path.c
    hss-swx-path.c

    hss-fd-path.c

    metrics.c
'''.split())

libhss = static_library('hss',
    sources : libhss_sources,
    dependencies : [libmetrics_dep,
                    libapp_dep,
                    libcrypt_dep,
                    libdbi_dep,
                    libdiameter_s6a_dep,
                    libdiameter_cx_dep,
                    libdiameter_swx_dep],
    install : false)

libhss_dep = declare_dependency(
    link_with : libhss,
    dependencies : [libmetrics_dep,
                    libapp_dep,
                    libcrypt_dep,
                    libdbi_dep,
                    libdiameter_s6a_dep,
                    libdiameter_cx_dep,
                    libdiameter_swx_dep])

hss_sources = files('''
    app-init.c
    ../main.c
'''.split())

executable('open5gs-hssd',
    sources : hss_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/hss.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libhss_dep,
    install_rpath : libdir,
    install : true)
