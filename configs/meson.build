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

conf_data = configuration_data()

conf_data.set('bindir', bindir)
conf_data.set('sysconfdir', sysconfdir)
conf_data.set('libdir', libdir)
conf_data.set('localstatedir', localstatedir)

build_configs_dir = join_paths(open5gs_build_dir, 'configs')
conf_data.set('build_configs_dir', build_configs_dir)

build_subprojects_freeDiameter_extensions_dir = join_paths(
        open5gs_build_dir, 'subprojects', 'freeDiameter', 'extensions')
conf_data.set('build_subprojects_freeDiameter_extensions_dir',
        build_subprojects_freeDiameter_extensions_dir)

example_conf = '''
    sample.yaml
    310014.yaml
    csfb.yaml
    volte.yaml
    vonr.yaml
    slice.yaml
    srsenb.yaml
    non3gpp.yaml
    transfer.yaml
'''.split()

foreach file : example_conf
    gen = configure_file(
            input : file + '.in',
            output : file,
            configuration : conf_data)
endforeach

subdir('open5gs')
subdir('freeDiameter')
subdir('systemd')
subdir('logrotate')
subdir('newsyslog')
subdir('examples')
