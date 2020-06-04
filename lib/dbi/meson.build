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

libdbi_sources = files('''
    ogs-dbi.h

    ogs-mongoc.h

    ogs-mongoc.c
    subscription.c
'''.split())

libmongoc_dep = dependency('libmongoc-1.0')

libdbi_inc = include_directories('.')

libdbi = library('ogsdbi',
    sources : libdbi_sources,
    version : libogslib_version,
    c_args : '-DOGS_DBI_COMPILATION',
    include_directories : [libdbi_inc, libinc],
    dependencies : [libcrypt_dep, libmongoc_dep],
    install : true)

libdbi_dep = declare_dependency(
    link_with : libdbi,
    include_directories : [libdbi_inc, libinc],
    dependencies : [libcrypt_dep, libmongoc_dep])
