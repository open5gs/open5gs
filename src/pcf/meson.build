# Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>

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

libpcf_sources = files('''
    context.c
    metrics.c
    event.c

    nnrf-handler.c
    npcf-handler.c

    nudr-build.c
    nudr-handler.c

    nbsf-build.c
    nbsf-handler.c

    namf-build.c
    nsmf-build.c
    naf-build.c

    am-sm.c
    sm-sm.c

    sbi-path.c
    pcf-sm.c

    init.c
'''.split())

libpcf = static_library('pcf',
    sources : libpcf_sources,
    dependencies : [libdbi_dep,
                    libmetrics_dep,
                    libsbi_dep],
    install : false)

libpcf_dep = declare_dependency(
    link_with : libpcf,
    dependencies : [libdbi_dep,
                    libmetrics_dep,
                    libsbi_dep])

pcf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-pcfd',
    sources : pcf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/pcf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libpcf_dep,
    install_rpath : libdir,
    install : true)
