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

libapp_sources = files('''
    ogs-app.h

    ogs-yaml.h
    ogs-context.h
    ogs-config.h
    ogs-init.h

    ogs-yaml.c
    ogs-context.c
    ogs-config.c
    ogs-init.c
'''.split())

yaml_dep = dependency('yaml-0.1')

libapp_inc = include_directories('.')

libapp = library('ogsapp',
    sources : libapp_sources,
    version : libogslib_version,
    c_args : '-DOGS_APP_COMPILATION',
    include_directories : [libapp_inc, libinc],
    dependencies : [libcore_dep, yaml_dep],
    install : true)

libapp_dep = declare_dependency(
    link_with : libapp,
    include_directories : [libapp_inc, libinc],
    dependencies : [libcore_dep, yaml_dep],
)
