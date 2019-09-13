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

pkglib_LTLIBRARIES = libogsnas.la

libogsnas_la_SOURCES = \
	ies.h ies.c types.h types.c conv.h conv.c \
	message.h decoder.c encoder.c \
	$(NULL)

libogsnas_la_LIBADD = \
	@OGSCORE_LIBS@ \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib \
	$(NULL)

AM_CFLAGS = \
	-DOGS_NAS_COMPILATION \
	-Wall -Werror \
	$(NULL)
