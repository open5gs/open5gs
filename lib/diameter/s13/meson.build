# Copyright (C) 2023 by Ryan Dimsey <ryan@omnitouch.com.au>

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

libdiameter_s13_sources = files('''
    ogs-diameter-s13.h

    message.h

    dict.c
    message.c
'''.split())

libdiameter_s13_inc = include_directories('.')

libdiameter_s13 = library('ogsdiameter-s13',
    sources : libdiameter_s13_sources,
    version : libogslib_version,
    c_args : libdiameter_common_cc_flags,
    include_directories : [libdiameter_s13_inc, libcrypt_inc],
    dependencies : libdiameter_common_dep,
    install : true)

libdiameter_s13_dep = declare_dependency(
    link_with : libdiameter_s13,
    include_directories : [libdiameter_s13_inc, libcrypt_inc],
    dependencies : libdiameter_common_dep)
