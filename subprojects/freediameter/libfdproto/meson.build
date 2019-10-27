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

fdproto_sources = files('''
    fdproto-internal.h
    dictionary.c
    dictionary_functions.c
    dispatch.c
    fifo.c
    init.c
    lists.c
    log.c
    messages.c
    ostr.c
    portability.c
    rt_data.c
    sessions.c
    utils.c
    version.c
'''.split())

libfdproto = library('fdproto',
    sources : [fdproto_sources],
    version : freeDiameter_version,
    soversion : freeDiameter_soversion,
    include_directories : freeDiameter_includes,
    dependencies : [threads, librt, libidn ],
    install : true)

libfdproto_dep = declare_dependency(
    link_with : libfdproto,
    include_directories : freeDiameter_includes,
    dependencies : [threads, librt, libidn ],
)
