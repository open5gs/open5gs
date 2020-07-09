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

libasn1c_common_sources = files('''
    ANY.h
    OCTET_STRING.h
    OPEN_TYPE.h
    constr_CHOICE.h
    GraphicString.h
    INTEGER.h
    NULL.h
    NativeEnumerated.h
    NativeInteger.h
    OBJECT_IDENTIFIER.h
    ObjectDescriptor.h
    PrintableString.h
    asn_SEQUENCE_OF.h
    asn_SET_OF.h
    constr_SEQUENCE.h
    constr_SEQUENCE_OF.h
    constr_SET_OF.h
    asn_application.h
    asn_ioc.h
    asn_system.h
    asn_codecs.h
    asn_internal.h
    asn_random_fill.h
    asn_bit_data.h
    BIT_STRING.h
    asn_codecs_prim.h
    ber_tlv_length.h
    ber_tlv_tag.h
    ber_decoder.h
    der_encoder.h
    constr_TYPE.h
    constraints.h
    xer_support.h
    xer_decoder.h
    xer_encoder.h
    per_support.h
    per_decoder.h
    per_encoder.h
    per_opentype.h
    oer_decoder.h
    oer_encoder.h
    oer_support.h

    ANY.c
    OPEN_TYPE.c
    GraphicString.c
    INTEGER.c
    NULL.c
    NativeEnumerated.c
    NativeInteger.c
    OBJECT_IDENTIFIER.c
    ObjectDescriptor.c
    PrintableString.c
    asn_SEQUENCE_OF.c
    asn_SET_OF.c
    constr_CHOICE.c
    constr_SEQUENCE.c
    constr_SEQUENCE_OF.c
    constr_SET_OF.c
    asn_application.c
    asn_internal.c
    asn_random_fill.c
    asn_bit_data.c
    OCTET_STRING.c
    BIT_STRING.c
    asn_codecs_prim.c
    ber_tlv_length.c
    ber_tlv_tag.c
    ber_decoder.c
    der_encoder.c
    constr_TYPE.c
    constraints.c
    xer_support.c
    xer_decoder.c
    xer_encoder.c
    per_support.c
    per_decoder.c
    per_encoder.c
    per_opentype.c
    oer_decoder.c
    oer_encoder.c
    oer_support.c
    OPEN_TYPE_oer.c
    INTEGER_oer.c
    BIT_STRING_oer.c
    OCTET_STRING_oer.c
    NativeInteger_oer.c
    NativeEnumerated_oer.c
    constr_CHOICE_oer.c
    constr_SEQUENCE_oer.c
    constr_SET_OF_oer.c
'''.split())

libasn1c_common_cc_flags = cc.get_supported_arguments([
    '-Wno-parentheses-equality',
    '-Wno-overflow',
    '-Wno-format',
    
    '-Wno-implicit-fallthrough',
    '-Wno-missing-prototypes',
    '-Wno-missing-declarations',
    '-Wno-missing-field-initializers',
    '-Wno-format-nonliteral',
    '-Wno-format-security',
    '-std=gnu99',
])

libasn1c_common_inc = include_directories('.')

libasn1c_common = library('ogsasn1c-common',
    sources : libasn1c_common_sources,
    version : libogslib_version,
    c_args : libasn1c_common_cc_flags,
    include_directories : libasn1c_common_inc,
    dependencies : libcore_dep,
    install : true)

libasn1c_common_dep = declare_dependency(
    link_with : libasn1c_common,
    include_directories : libasn1c_common_inc,
    dependencies : libcore_dep)
