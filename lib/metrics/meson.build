# Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>

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

libmetrics_file_list = '''
    ogs-metrics.h
    context.h
    context.c
'''
libmetrics_dependencies = [libapp_dep]


metrics_impl_optval = get_option('metrics_impl')

if metrics_impl_optval == 'prometheus'
    # Note: This requires meson >= 0.51.0:
    # 0.47.0: {'check arg in run_command'}
    # 0.50.0: {'CMake Module'}
    # 0.51.0: {'subproject'}

    libmicrohttpd_dep = dependency('libmicrohttpd', version: '>=0.9.40')

    cmake = import('cmake')
    prometheus_client_c_proj = cmake.subproject('prometheus-client-c')
    # generated cmake subproject seems to include
    # open5gs/subprojects/prometheus-client-c/__CMake_build as include, which
    # doesn't exist and fail:
    missing_include_dir = join_paths(meson.current_source_dir(), '../../subprojects/prometheus-client-c/__CMake_build')
    run_command('mkdir', '-p', missing_include_dir, check: true)
    libprom_dep = prometheus_client_c_proj.dependency('prom')

    libmetrics_dependencies = libmetrics_dependencies + [libprom_dep, libmicrohttpd_dep]
    libmetrics_file_list = libmetrics_file_list + ' prometheus/context.c'
else
   libmetrics_file_list = libmetrics_file_list + ' void/context.c'
endif

libmetrics_sources = files(libmetrics_file_list.split())

libmetrics_inc = include_directories('.')

libmetrics = library('ogsmetrics',
    sources : libmetrics_sources,
    version : libogslib_version,
    c_args : '-DOGS_METRICS_COMPILATION',
    include_directories : [libmetrics_inc, libinc],
    dependencies : libmetrics_dependencies,
    install : true)

libmetrics_dep = declare_dependency(
    link_with : libmetrics,
    include_directories : [libmetrics_inc, libinc],
    dependencies : libmetrics_dependencies)
