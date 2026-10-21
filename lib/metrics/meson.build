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


#metrics_impl_optval = get_option('metrics_impl')

#if metrics_impl_optval == 'prometheus'

libmicrohttpd_dep = dependency('libmicrohttpd', version: '>=0.9.40')

cmake = import('cmake')
# Ubuntu Bionic cannot parse meson's dictionary
# because the meson version is less than v0.47.0.
#
# We will change to using meson's dictionary
# after April 2023 when Ubuntu bionic is deprecated.
#
#    opt_var = cmake.subproject_options()
#    opt_var.add_cmake_defines({'CMAKE_INSTALL_LIBDIR': libdir})
#    prometheus_client_c_proj = cmake.subproject(
#      'prometheus-client-c', options: opt_var)
prometheus_client_c_proj = cmake.subproject('prometheus-client-c')

#
# @acetcom
# We removed -Werror=missing-include-dirs in possible_cc_flags
#
# @pepsin
# generated cmake subproject seems to include
# open5gs/subprojects/prometheus-client-c/__CMake_build as include, which
# doesn't exist and fail:
#
#    missing_include_dir = join_paths(meson.current_source_dir(), '../../subprojects/prometheus-client-c/__CMake_build')
#    run_command('mkdir', '-p', missing_include_dir, check: true)
libprom_dep = prometheus_client_c_proj.dependency('prom')

libmetrics_dependencies = libmetrics_dependencies + [libprom_dep, libmicrohttpd_dep]
libmetrics_file_list = libmetrics_file_list + ' prometheus/context.c'

libmetrics_sources = files(libmetrics_file_list.split())

libmetrics_inc = include_directories('.')

libmetrics = library('ogsmetrics',
    sources : libmetrics_sources,
    version : libogslib_version,
    c_args : '-DOGS_METRICS_COMPILATION',
    include_directories : [libmetrics_inc, libinc],
    dependencies : libmetrics_dependencies,
    install_rpath: libdir,
    install : true)

libmetrics_dep = declare_dependency(
    link_with : libmetrics,
    include_directories : [libmetrics_inc, libinc],
    dependencies : libmetrics_dependencies)
