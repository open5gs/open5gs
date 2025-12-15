# Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>

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

examples_conf = '''
    5gc-sepp1-999-70.yaml
    5gc-sepp2-001-01.yaml
    5gc-sepp3-315-010.yaml
    5gc-tls-sepp1-999-70.yaml
    5gc-tls-sepp2-001-01.yaml
    5gc-tls-sepp3-315-010.yaml
    gnb-999-70-ue-999-70.yaml
    gnb-999-70-ue-001-01.yaml
    gnb-999-70-ue-315-010.yaml
    gnb-001-01-ue-999-70.yaml
    gnb-001-01-ue-001-01.yaml
    gnb-001-01-ue-315-010.yaml
    gnb-315-010-ue-999-70.yaml
    gnb-315-010-ue-001-01.yaml
    gnb-315-010-ue-315-010.yaml
'''.split()

foreach file : examples_conf
    gen = configure_file(
            input : file + '.in',
            output : file,
            configuration : conf_data)
endforeach
