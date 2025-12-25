# NRPPa ASN.1 codec (v17.2.0 - ProtocolIE-Container format)
# Generated from 3GPP TS 38.455 v17.2.0

# Get all C source files (includes custom PDU wrappers)
nrppa_sources = run_command('sh', '-c', 'ls -1 *.c', check: true).stdout().strip().split('\n')

# Old v17.4.0 wrappers (working encoders) adapted for v17.2.0 content
# These have proper asn1c-generated APER encoders

libasn1c_nrppa_inc = include_directories('.')

libasn1c_nrppa = library('ogsasn1c-nrppa',
    sources : nrppa_sources,
    version : libogslib_version,
    c_args : libasn1c_common_cc_flags,
    include_directories : libasn1c_nrppa_inc,
    dependencies : [libasn1c_common_dep, libasn1c_util_dep],
    install : true)

libasn1c_nrppa_dep = declare_dependency(
    link_with : libasn1c_nrppa,
    include_directories : libasn1c_nrppa_inc,
    dependencies : [libasn1c_common_dep, libasn1c_util_dep])
