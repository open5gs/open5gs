include Makefile.am.libasncodec

LIBS += -lm
CFLAGS += $(ASN_MODULE_CFLAGS) -DASN_DISABLE_BER_SUPPORT -DASN_DISABLE_XER_SUPPORT -DASN_DISABLE_OER_SUPPORT -DASN_DISABLE_UPER_SUPPORT -DASN_PDU_COLLECTION -I.
ASN_LIBRARY ?= libasncodec.a
ASN_PROGRAM ?= converter-example
ASN_PROGRAM_SRCS ?= \
	pdu_collection.c

all: $(ASN_PROGRAM)

$(ASN_PROGRAM): $(ASN_LIBRARY) $(ASN_PROGRAM_SRCS:.c=.o)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(ASN_PROGRAM) $(ASN_PROGRAM_SRCS:.c=.o) $(LDFLAGS) $(ASN_LIBRARY) $(LIBS)

$(ASN_LIBRARY): $(ASN_MODULE_SRCS:.c=.o)
	$(AR) rcs $@ $(ASN_MODULE_SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(ASN_PROGRAM) $(ASN_LIBRARY)
	rm -f $(ASN_MODULE_SRCS:.c=.o) $(ASN_PROGRAM_SRCS:.c=.o)

regen: regenerate-from-asn1-source

regenerate-from-asn1-source:
	/tmp/open5gs-asn1c/asn1c/asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER -no-gen-JER ../support/nrppa-r18/NRPPA-CommonDataTypes.asn ../support/nrppa-r18/NRPPA-Constants.asn ../support/nrppa-r18/NRPPA-Containers.asn ../support/nrppa-r18/NRPPA-IEs.asn ../support/nrppa-r18/NRPPA-PDU-Contents.asn

