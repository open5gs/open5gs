libdiameter_s13_sources = files('''
    ogs-diameter-s13.h

    message.h

    dict.c
    message.c
'''.split())

libdiameter_s13_inc = include_directories('.')

libdiameter_s13 = library('ogsdiameter-s13',
    sources : libdiameter_s13_sources,
    version : libogslib_version,
    c_args : libdiameter_common_cc_flags,
    include_directories : [libdiameter_s13_inc, libcrypt_inc],
    dependencies : libdiameter_common_dep,
    install : true)

libdiameter_s13_dep = declare_dependency(
    link_with : libdiameter_s13,
    include_directories : [libdiameter_s13_inc, libcrypt_inc],
    dependencies : libdiameter_common_dep)