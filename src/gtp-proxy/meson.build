gtp_proxy_sources = files('''
    event.h
    timer.h
    context.h 
    gtp-path.h
    s11-build.h
    s11-handler.h
    s5c-handler.h 
    sxa-build.h
    sxa-handler.h
    pfcp-path.h
    sgwc-sm.h

    init.c
    event.c
    timer.c
    context.c 
    gtp-path.c
    s11-build.c
    s11-handler.c
    s5c-handler.c 
    sxa-build.c
    sxa-handler.c
    pfcp-path.c
    pfcp-sm.c
    sgwc-sm.c
    app.c
    ../main.c
'''.split())

executable('gtp-proxyd',
    sources : gtp_proxy_sources,
    c_args : '-DDEFAULT_CONFIG_FILENAME="@0@/gtp-proxy.yaml"'.format(open5gs_sysconfdir),
    include_directories : srcinc,
    dependencies : [ libpfcp_dep ],
    install_rpath : libdir,
    install : true)
