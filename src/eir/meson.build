# Copyright (C) 2026 by Erol Yağız Aydın <ygzaydns@gmail.com>

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

libeir_sources = files('''
    context.c
    event.c

    n5geir-handler.c

    sbi-path.c
    eir-sm.c

    init.c
'''.split())

libeir = static_library('eir',
    sources : libeir_sources,
    dependencies : [libdbi_dep,
                    libsbi_dep],
    install : false)

libeir_dep = declare_dependency(
    link_with : libeir,
    dependencies : [libdbi_dep,
                    libsbi_dep])

eir_sources = files('''
    app.c
    ../main.c
'''.split())

executable('open5gs-eird',
    sources : eir_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/eir.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : libeir_dep,
    install_rpath : libdir,
    install : true)
