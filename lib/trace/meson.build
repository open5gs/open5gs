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

libtrace_dependencies = [ libapp_dep ]
libtrace_inc = include_directories('.')

if get_option('trace')
    libtrace_sources = files('''
        opentelemetry-cpp/ogs-trace.cpp
    '''.split())

    cmake = import('cmake')

    opt_var = cmake.subproject_options()
    opt_var.add_cmake_defines({'BUILD_TESTING': 'OFF'})
    opt_var.add_cmake_defines({'WITH_BENCHMARK': 'OFF'})
    opt_var.add_cmake_defines({'WITH_STL': 'ON'})
    opt_var.add_cmake_defines({'WITH_OTLP_HTTP': 'ON'})
    opt_var.add_cmake_defines({'WITH_EXAMPLES': 'OFF'})
    opt_var.add_cmake_defines({'WITH_FUNC_TESTS': 'OFF'})

    # Compile & install only static (PIC) libraries, then link libogstrace.so
    # with static libraries. Linker will automatically remove unused code
    # from libopentelemetry-cpp.
    opt_var.add_cmake_defines({'CMAKE_INSTALL_LIBDIR': libdir})
    #opt_var.add_cmake_defines({'BUILD_SHARED_LIBS': 'ON'})
    opt_var.add_cmake_defines({'OTELCPP_VERSIONED_LIBS': 'OFF'})
    opt_var.add_cmake_defines({'CMAKE_POSITION_INDEPENDENT_CODE': 'ON'})

    libopentelemetrycpp = cmake.subproject('opentelemetry-cpp', options: opt_var)
    message('CMake targets:\n - ' + '\n - '.join(libopentelemetrycpp.target_list()))

    libot_common_dep = libopentelemetrycpp.dependency('opentelemetry_common')
    libot_proto_dep = libopentelemetrycpp.dependency('opentelemetry_proto')
    libot_resources_dep = libopentelemetrycpp.dependency('opentelemetry_resources')
    libot_trace_dep = libopentelemetrycpp.dependency('opentelemetry_trace')
    libot_exp_ostream_span_dep = libopentelemetrycpp.dependency('opentelemetry_exporter_ostream_span')
    libot_exp_otlp_http_dep = libopentelemetrycpp.dependency('opentelemetry_exporter_otlp_http')
    libot_exp_otlp_http_client_dep = libopentelemetrycpp.dependency('opentelemetry_exporter_otlp_http_client')
    libot_otlp_recordable_dep = libopentelemetrycpp.dependency('opentelemetry_otlp_recordable')
    libot_http_client_curl_dep = libopentelemetrycpp.dependency('opentelemetry_http_client_curl')

    libtrace_dependencies += [
        libot_common_dep,
        libot_proto_dep,
        libot_resources_dep,
        libot_trace_dep,
        libot_exp_ostream_span_dep,
        libot_exp_otlp_http_dep,
        libot_exp_otlp_http_client_dep,
        libot_http_client_curl_dep,
        libot_otlp_recordable_dep
    ]

else
    libtrace_sources = files('''
        void/ogs-trace.cpp
    '''.split())

endif



libtrace = library('ogstrace',
    sources : libtrace_sources,
    version : libogslib_version,
    c_args : '-DOGS_TRACE_COMPILATION',
    cpp_args: '-DOGS_TRACE_COMPILATION',
    include_directories : [libtrace_inc, libinc],
    dependencies : libtrace_dependencies,
    install_rpath : libdir,
    install : true
)

libtrace_dep = declare_dependency(
    link_with : libtrace,
    include_directories : [libtrace_inc, libinc],
    dependencies : libtrace_dependencies,
)
