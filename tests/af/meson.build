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

libaf_sources = files('''
    context.c
    event.c
    timer.c
    local.c

    nnrf-build.c
    nnrf-handler.c
    nf-sm.c

    nbsf-build.c
    nbsf-handler.c

    npcf-build.c
    npcf-handler.c

    sbi-path.c
    af-sm.c

    init.c
'''.split())

libaf = static_library('af',
    sources : libaf_sources,
    dependencies : [libapp_dep,
                    libcrypt_dep,
                    libsbi_dep],
    install : false)

libaf_dep = declare_dependency(
    link_with : libaf,
    dependencies : [libapp_dep,
                    libcrypt_dep,
                    libsbi_dep])
