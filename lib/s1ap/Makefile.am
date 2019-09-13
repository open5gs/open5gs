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

pkglib_LTLIBRARIES = libogss1ap.la

libogss1ap_la_SOURCES = \
	conv.h conv.c message.h message.c \
	$(NULL)

libogss1ap_la_DEPENDENCIES = \
    $(top_srcdir)/lib/asn1c/libasn1c.la \
	$(NULL)

libogss1ap_la_LIBADD = \
	@OGSCORE_LIBS@ \
    $(top_srcdir)/lib/asn1c/libasn1c.la \
	$(NULL)

AM_CPPFLAGS = \
	@OGSCORE_CFLAGS@ \
    -I$(top_srcdir)/lib/asn1c/common \
    -I$(top_srcdir)/lib/asn1c/s1ap \
    -I$(top_srcdir)/lib \
	$(NULL)

AM_CFLAGS = \
	-DOGS_S1AP_COMPILATION \
	-Wall -Werror \
	$(NULL)
