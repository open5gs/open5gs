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

libudm_sources = files('''
    context.c
    event.c

    nnrf-handler.c
    nudm-handler.c

    nudr-build.c
    nudr-handler.c
    ue-sm.c
    sess-sm.c

    sbi-path.c
    udm-sm.c

    init.c
'''.split())

libudm = static_library('udm',
    sources : libudm_sources,
    dependencies : libsbi_dep,
    install : false)

libudm_dep = declare_dependency(
    link_with : libudm,
    dependencies : libsbi_dep)

udm_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-udmd',
    sources : udm_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/udm.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libudm_dep,
    install_rpath : libdir,
    install : true)
