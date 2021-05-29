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

srcinc = include_directories('.')

package_version = 'v' + meson.project_version()
if git.found()
    git_version = run_command('git', ['describe', '--abbrev=7', '--dirty=+'])
    if git_version.returncode() == 0
        package_version = git_version.stdout().strip()
    endif
endif

version_conf = configuration_data()
version_conf.set_quoted('OPEN5GS_VERSION', package_version)
configure_file(output : 'version.h', configuration : version_conf)

subdir('mme')
subdir('hss')
subdir('sgwc')
subdir('sgwu')
subdir('pcrf')

subdir('nrf')
subdir('udr')
subdir('udm')
subdir('pcf')
subdir('nssf')
subdir('bsf')
subdir('ausf')
subdir('upf')
subdir('smf')
subdir('amf')
