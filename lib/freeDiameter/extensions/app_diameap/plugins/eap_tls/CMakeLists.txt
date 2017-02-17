# The EAP-TLS Plugin
PROJECT("EAP-TLS Plugin" C)

# Parser for the configuration file 
BISON_FILE(eaptls.y)
FLEX_FILE(eaptls.l)

SET_SOURCE_FILES_PROPERTIES(lex.eaptls.c eaptls.tab.c PROPERTIES COMPILE_FLAGS "-I \"${CMAKE_CURRENT_SOURCE_DIR}\"")

EAP_ADD_METHOD(eap_tls
    eap_tls.c
    eap_tls.h    
    lex.eaptls.c
    eaptls.tab.c
    eaptls.tab.h
    )

TARGET_LINK_LIBRARIES(eap_tls ${GNUTLS_LIBRARIES})
