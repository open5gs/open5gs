test5gc_gtp_proxy_sources = files('''
    abts-main.c
    gtp-path.c
    s8-build.c
    s8-handler.c
    4g-proxy-test.c
'''.split())

test5gc_gtp_proxy_exe = executable('gtp-proxy',
    sources : test5gc_gtp_proxy_sources,
    c_args : testunit_core_cc_flags + ['-DCONFIG_FILE="@0@/configs/gtp-proxy.yaml"'.format(open5gs_build_dir),],
    dependencies : libtest5gc_dep)

test('gtp-proxy',
    test5gc_gtp_proxy_exe,
    is_parallel : false,
    suite: 'roaming')
