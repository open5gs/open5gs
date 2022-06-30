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

project('open5gs', 'c', 'cpp',
    version : '2.4.8',
    license : 'AGPL-3.0-or-later',
    meson_version : '>= 0.43.0',
    default_options : [
        'c_std=gnu89',
    ],
)

libogslib_version = '2.4.8'

prefix = get_option('prefix')
bindir = join_paths(prefix, get_option('bindir'))
libdir = join_paths(prefix, get_option('libdir'))
sysconfdir = join_paths(prefix, get_option('sysconfdir'))
localstatedir = join_paths(prefix, get_option('localstatedir'))

cc = meson.get_compiler('c')
host_system = host_machine.system()

git = find_program('git', required: false)
#python = import('python')
#python3 = python.find_installation('python3')
python3 = find_program('python3', 'python')
python3_exe = join_paths(python3.path())
mkdir_p = 'import os; os.makedirs("@0@", exist_ok=True) if not os.environ.get("DESTDIR") else False;'
install_conf = 'import os; import shutil; shutil.copy("@0@", "@1@") if not os.environ.get("DESTDIR") and not os.path.isfile(os.path.join("@1@", os.path.split("@0@")[1])) else False;'
cppcheck = find_program('cppcheck', required: false)
clangtidy = find_program('run-clang-tidy', required: false) # requires clang-tools package
if clangtidy.found() != true
    clangtidy = find_program('clang-tidy', required: false)
endif


meson.add_install_script(python3_exe, '-c',
        mkdir_p.format(join_paths(localstatedir, 'log', 'open5gs')))

# Compiler flags
if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
    possible_cc_flags = [
        '-Wextra',
        '-Wlogical-op',
        '-Werror=missing-include-dirs',
        '-Werror=pointer-arith',
        '-Werror=init-self',
        '-Wfloat-equal',
        '-Wsuggest-attribute=noreturn',
        '-Werror=missing-prototypes',
        '-Werror=missing-declarations',
        '-Werror=implicit-function-declaration',
        '-Werror=return-type',
        '-Werror=incompatible-pointer-types',
        '-Werror=format=2',
        '-Wstrict-prototypes',
        '-Wredundant-decls',
        '-Wimplicit-fallthrough=5',
        '-Wendif-labels',
        '-Wstrict-aliasing=3',
        '-Wwrite-strings',
        '-Werror=overflow',
        '-Werror=shift-count-overflow',
        '-Werror=shift-overflow=2',
        '-Wdate-time',
        '-Wnested-externs',
        '-Wunused',
        '-Wduplicated-branches',
        '-Wmisleading-indentation',

        '-Wno-sign-compare',
        '-Wno-unused-parameter',

        '-ffast-math',
        '-fdiagnostics-show-option',
        ## TODO : '-fvisibility=hidden',
        '-fstack-protector',
        '-fstack-protector-strong',
        '--param=ssp-buffer-size=4',
    ]

    if cc.get_id() == 'clang'
        possible_cc_flags += [
            '-Wno-typedef-redefinition',
            '-Wno-gnu-variable-sized-type-not-at-end',
        ]
    endif

    if get_option('buildtype') != 'debug'
        possible_cc_flags += [
            '-ffunction-sections',
            '-fdata-sections',
        ]
    endif
else
    possible_cc_flags = []
endif

add_project_arguments(
    cc.get_supported_arguments(possible_cc_flags),
    language : 'c')

subdir('configs')
subdir('lib')
subdir('src')
subdir('misc')
# Don't build the tests unless we can run them (either natively or in an exe wrapper)
build_tests = not meson.is_cross_build() or (meson.is_cross_build() and meson.has_exe_wrapper())
if build_tests
    subdir('tests')
endif

message('\n'.join([
  '',
  '        prefix:                       ' + prefix,
  '        libdir:                       ' + libdir,
  '        bindir:                       ' + bindir,
  '        sysconfdir:                   ' + sysconfdir,
  '        localstatedir:                ' + localstatedir,
  '        source code location:         ' + meson.source_root(),
  '        compiler:                     ' + cc.get_id(),
  '        debugging support:            ' + get_option('buildtype'),
  '',
]))


if cppcheck.found()
    run_target('analyze-cppcheck',
    command : [ 'misc/static_code_analyze.sh',
        cppcheck.path(),
        meson.build_root(),
        meson.source_root()
        ]
    )
endif

if clangtidy.found()
    run_target('analyze-clang-tidy',
    command : [ 'misc/static_code_analyze.sh',
        clangtidy.path(),
        meson.build_root(),
        meson.source_root()
        ]
    )
endif
