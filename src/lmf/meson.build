# Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
#
# This file is part of Open5GS.
#
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

cc = meson.get_compiler('c')
libm = cc.find_library('m', required : true)

liblmf_sources = files('''
    context.c
    event.c
    timer.c

    nnrf-handler.c
    nlmf-handler.c
    namf-build.c
    namf-handler.c
    namf-path.c

    nrppa-build.c
    nrppa-parse.c

    cell-database.c

    location-determination.c

    sbi-path.c

    lmf-sm.c

    init.c
'''.split())

liblmf = static_library('lmf',
    sources : liblmf_sources,
    dependencies : [libsbi_dep, libm, libasn1c_nrppa_dep],
    install : false)

liblmf_dep = declare_dependency(
    link_with : liblmf,
    dependencies : [libsbi_dep, libm, libasn1c_nrppa_dep])

lmf_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-lmfd',
    sources : lmf_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/lmf.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : liblmf_dep,
    install_rpath : libdir,
    install : true)

