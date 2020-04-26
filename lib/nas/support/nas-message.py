# The MIT License

# Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

from docx import Document
import re, os, sys, string
import datetime
import getopt
import getpass

version = "0.1.0"

msg_list = {}
type_list = {}

verbosity = 0
filename = ""
outdir = './'
cachedir = './cache/'
currentdir = './'

FAIL = '\033[91m'
INFO = '\033[93m'
ENDC = '\033[0m'

def d_print(string):
    if verbosity > 0:
        sys.stdout.write(string)

def d_info(string):
    sys.stdout.write(INFO + string + ENDC + "\n")

def d_error(string):
    sys.stderr.write(FAIL + string + ENDC + "\n")
    sys.exit(0)

def write_file(f, string):
    f.write(string)
    d_print(string)

def output_header_to_file(f):
    now = datetime.datetime.now()
    f.write("""/*
 * The MIT License
 *
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

""")
    f.write("/*******************************************************************************\n")
    f.write(" * This file had been created by nas-message.py script v%s\n" % (version))
    f.write(" * Please do not modify this file but regenerate it via script.\n")
    f.write(" * Created on: %s by %s\n * from %s\n" % (str(now), getpass.getuser(), filename))
    f.write(" ******************************************************************************/\n\n")

def usage():
    print "Python generating NAS Message encoder/decoder v%s" % (version)
    print "Usage: python nas-message.py [options]"
    print "Available options:"
    print "-d        Enable script debug"
    print "-f [file] Input file to parse"
    print "-o [dir]  Output files to given directory"
    print "-c [dir]  Cache files to given directory"
    print "-h        Print this help and return"

def v_upper(v):
    return re.sub('_TO_UE', '', re.sub('_FROM_UE', '', re.sub('3GPP', '', re.sub('\'', '_', re.sub('/', '_', re.sub('-', '_', re.sub(' ', '_', v)))).upper())))

def v_lower(v):
    return re.sub('3gpp', '', re.sub('\'', '_', re.sub('/', '_', re.sub('-', '_', re.sub(' ', '_', v)))).lower())

def get_cells(cells):
    iei = cells[0].text.encode('ascii', 'ignore')
    value = re.sub("\s*$", "", re.sub("\s*\n*\s*\([^\)]*\)*", "", re.sub("'s", "", cells[1].text))).encode('ascii', 'ignore')
    type = re.sub("^NAS ", "", re.sub("'s", "", re.sub('\s*\n\s*[a-zA-Z0-9.]*', '', cells[2].text))).encode('ascii', 'ignore')
    reference = re.sub('[a-zA-Z0-9\'\-\s]*\n\s*', '', cells[2].text).encode('ascii', 'ignore')
    presence = cells[3].text.encode('ascii', 'ignore')
    format = cells[4].text.encode('ascii', 'ignore')
    length = cells[5].text.encode('ascii', 'ignore')

    return { "iei" : iei, "value" : value, "type" : type, "reference" : reference, "presence" : presence, "format" : format, "length" : length }

def write_cells_to_file(name, cells):
    write_file(f, name + ".append({ \"iei\" : \"" + cells["iei"] + \
        "\", \"value\" : \"" + cells["value"] + \
        "\", \"type\" : \"" + cells["type"] + \
        "\", \"reference\" : \"" + cells["reference"] + \
        "\", \"presence\" : \"" + cells["presence"] + \
        "\", \"format\" : \"" + cells["format"] + \
        "\", \"length\" : \"" + cells["length"] + "\"})\n")

try:
    opts, args = getopt.getopt(sys.argv[1:], "df:ho:c:", ["debug", "file", "help", "output", "cache"])
except getopt.GetoptError as err:
    # print help information and exit:
    usage()
    sys.exit(2)

for o, a in opts:
    if o in ("-d", "--debug"):
        verbosity = 1
    if o in ("-f", "--file"):
        filename = a
    if o in ("-o", "--output"):
        outdir = a
        if outdir.rfind('/') != len(outdir):
            outdir += '/'
    if o in ("-c", "--cache"):
        cache = a
        if cachedir.rfind('/') != len(cachedir):
            cachedir += '/'
    if o in ("-h", "--help"):
        usage()
        sys.exit(2)

# Message Type List
msg_list["ATTACH REQUEST"] = { "type" : "65" }
msg_list["ATTACH ACCEPT"]= { "type" : "66" }
msg_list["ATTACH COMPLETE"] = { "type" : "67" }
msg_list["ATTACH REJECT"] = { "type" : "68" }
msg_list["DETACH REQUEST FROM UE"] = { "type" : "69.1" }
msg_list["DETACH REQUEST TO UE"] = { "type" : "69.2" }
msg_list["DETACH ACCEPT"] = { "type" : "70" }
msg_list["TRACKING AREA UPDATE REQUEST"] = { "type" : "72" }
msg_list["TRACKING AREA UPDATE ACCEPT"] = { "type" : "73" }
msg_list["TRACKING AREA UPDATE COMPLETE"] = { "type" : "74" }
msg_list["TRACKING AREA UPDATE REJECT"] = { "type" : "75" }
msg_list["EXTENDED SERVICE REQUEST"] = { "type" : "76" }
msg_list["SERVICE REQUEST"] = { "type" : "77.1" }
msg_list["SERVICE REJECT"] = { "type" : "78" }
msg_list["GUTI REALLOCATION COMMAND"] = { "type" : "80" }
msg_list["GUTI REALLOCATION COMPLETE"] = { "type" : "81" }
msg_list["AUTHENTICATION REQUEST"] = { "type" : "82" }
msg_list["AUTHENTICATION RESPONSE"] = { "type" : "83" }
msg_list["AUTHENTICATION REJECT"] = { "type" : "84" }
msg_list["AUTHENTICATION FAILURE"] = { "type" : "92" }
msg_list["IDENTITY REQUEST"] = { "type" : "85" }
msg_list["IDENTITY RESPONSE"] = { "type" : "86" }
msg_list["SECURITY MODE COMMAND"] = { "type" : "93" }
msg_list["SECURITY MODE COMPLETE"] = { "type" : "94" }
msg_list["SECURITY MODE REJECT"] = { "type" : "95" }
msg_list["EMM STATUS"] = { "type" : "96" }
msg_list["EMM INFORMATION"] = { "type" : "97" }
msg_list["DOWNLINK NAS TRANSPORT"] = { "type" : "98" }
msg_list["UPLINK NAS TRANSPORT"] = { "type" : "99" }
msg_list["CS SERVICE NOTIFICATION"] = { "type" : "100" }
msg_list["DOWNLINK GENERIC NAS TRANSPORT"] = { "type" : "104" }
msg_list["UPLINK GENERIC NAS TRANSPORT"] = { "type" : "101" }

msg_list["ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST"] = { "type" : "193" }
msg_list["ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT"] = { "type" : "194" }
msg_list["ACTIVATE DEFAULT EPS BEARER CONTEXT REJECT"] = { "type" : "195" }
msg_list["ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST"] = { "type" : "197" }
msg_list["ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT"] = { "type" : "198" }
msg_list["ACTIVATE DEDICATED EPS BEARER CONTEXT REJECT"] = { "type" : "199" }
msg_list["MODIFY EPS BEARER CONTEXT REQUEST"] = { "type" : "201" }
msg_list["MODIFY EPS BEARER CONTEXT ACCEPT"] = { "type" : "202" }
msg_list["MODIFY EPS BEARER CONTEXT REJECT"] = { "type" : "203" }
msg_list["DEACTIVATE EPS BEARER CONTEXT REQUEST"] = { "type" : "205" }
msg_list["DEACTIVATE EPS BEARER CONTEXT ACCEPT"] = { "type" : "206" }
msg_list["PDN CONNECTIVITY REQUEST"] = { "type" : "208" }
msg_list["PDN CONNECTIVITY REJECT"] = { "type" : "209" }
msg_list["PDN DISCONNECT REQUEST"] = { "type" : "210" }
msg_list["PDN DISCONNECT REJECT"] = { "type" : "211" }
msg_list["BEARER RESOURCE ALLOCATION REQUEST"] = { "type" : "212" }
msg_list["BEARER RESOURCE ALLOCATION REJECT"] = { "type" : "213" }
msg_list["BEARER RESOURCE MODIFICATION REQUEST"] = { "type" : "214" }
msg_list["BEARER RESOURCE MODIFICATION REJECT"] = { "type" : "215" }
msg_list["ESM INFORMATION REQUEST"] = { "type" : "217" }
msg_list["ESM INFORMATION RESPONSE"] = { "type" : "218" }
msg_list["ESM STATUS"] = { "type" : "232" }

# Table number for Message List
msg_list["ATTACH ACCEPT"]["table"] = 0
msg_list["ATTACH COMPLETE"]["table"] = 1
msg_list["ATTACH REJECT"]["table"] = 2
msg_list["ATTACH REQUEST"]["table"] = 3
msg_list["AUTHENTICATION FAILURE"]["table"] = 4
msg_list["AUTHENTICATION REJECT"]["table"] = 5
msg_list["AUTHENTICATION REQUEST"]["table"] = 6
msg_list["AUTHENTICATION RESPONSE"]["table"] = 7
msg_list["CS SERVICE NOTIFICATION"]["table"] = 8
msg_list["DETACH ACCEPT"]["table"] = 9
msg_list["DETACH REQUEST FROM UE"]["table"] = 11
msg_list["DETACH REQUEST TO UE"]["table"] = 12
msg_list["DOWNLINK NAS TRANSPORT"]["table"] = 13
msg_list["EMM INFORMATION"]["table"] = 14
msg_list["EMM STATUS"]["table"] = 15
msg_list["EXTENDED SERVICE REQUEST"]["table"] = 16
msg_list["GUTI REALLOCATION COMMAND"]["table"] = 17
msg_list["GUTI REALLOCATION COMPLETE"]["table"] = 18
msg_list["IDENTITY REQUEST"]["table"] = 19
msg_list["IDENTITY RESPONSE"]["table"] = 20
msg_list["SECURITY MODE COMMAND"]["table"] = 21
msg_list["SECURITY MODE COMPLETE"]["table"] = 22
msg_list["SECURITY MODE REJECT"]["table"] = 23
msg_list["SERVICE REJECT"]["table"] = 25
msg_list["SERVICE REQUEST"]["table"] = 26
msg_list["TRACKING AREA UPDATE ACCEPT"]["table"] = 27
msg_list["TRACKING AREA UPDATE COMPLETE"]["table"] = 28
msg_list["TRACKING AREA UPDATE REJECT"]["table"] = 29
msg_list["TRACKING AREA UPDATE REQUEST"]["table"] = 30
msg_list["UPLINK NAS TRANSPORT"]["table"] = 31
msg_list["DOWNLINK GENERIC NAS TRANSPORT"]["table"] = 32
msg_list["UPLINK GENERIC NAS TRANSPORT"]["table"] = 33

msg_list["ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT"]["table"] = 36
msg_list["ACTIVATE DEDICATED EPS BEARER CONTEXT REJECT"]["table"] = 37
msg_list["ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST"]["table"] = 38
msg_list["ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT"]["table"] = 39
msg_list["ACTIVATE DEFAULT EPS BEARER CONTEXT REJECT"]["table"] = 40
msg_list["ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST"]["table"] = 41
msg_list["BEARER RESOURCE ALLOCATION REJECT"]["table"] = 42
msg_list["BEARER RESOURCE ALLOCATION REQUEST"]["table"] = 43
msg_list["BEARER RESOURCE MODIFICATION REJECT"]["table"] = 44
msg_list["BEARER RESOURCE MODIFICATION REQUEST"]["table"] = 45
msg_list["DEACTIVATE EPS BEARER CONTEXT ACCEPT"]["table"] = 46
msg_list["DEACTIVATE EPS BEARER CONTEXT REQUEST"]["table"] = 47
msg_list["ESM INFORMATION REQUEST"]["table"] = 49
msg_list["ESM INFORMATION RESPONSE"]["table"] = 50
msg_list["ESM STATUS"]["table"] = 51
msg_list["MODIFY EPS BEARER CONTEXT ACCEPT"]["table"] = 52
msg_list["MODIFY EPS BEARER CONTEXT REJECT"]["table"] = 53
msg_list["MODIFY EPS BEARER CONTEXT REQUEST"]["table"] = 54
msg_list["PDN CONNECTIVITY REJECT"]["table"] = 56
msg_list["PDN CONNECTIVITY REQUEST"]["table"] = 57
msg_list["PDN DISCONNECT REJECT"]["table"] = 58
msg_list["PDN DISCONNECT REQUEST"]["table"] = 59

for key in msg_list.keys():
    if "table" not in msg_list[key].keys():
        continue;

    d_info("[" + key + "]")
    cachefile = cachedir + "nas-msg-" + msg_list[key]["type"] + ".py"
    if os.path.isfile(cachefile) and os.access(cachefile, os.R_OK):
        execfile(cachefile)
        print "Read from " + cachefile
    else:
        document = Document(filename)
        f = open(cachefile, 'w') 

        ies = []
        write_file(f, "ies = []\n")
        table = document.tables[msg_list[key]["table"]]

        start_row = 0
        for start_row, row in enumerate(table.rows):
            cells = get_cells(row.cells);
            if cells["type"].find('Message type') != -1:
                break
            if cells["type"].find('KSI and sequence number') != -1:
                start_row -= 1
                break

        assert start_row <= 4, "Can't find message type"

        half_length = True;
        for row in table.rows[start_row+1:]:
            cells = get_cells(row.cells)
            if cells is None:
                continue

            if cells["length"] == "1/2":
                if half_length is True:
                    half_length = False;
                else:
                    half_length = True;
                    continue;

            ies.append(cells)
            write_cells_to_file("ies", cells)

        msg_list[key]["ies"] = ies
        write_file(f, "msg_list[key][\"ies\"] = ies\n")

        f.close()


tmp = [(k, v["type"]) for k, v in msg_list.items()]
sorted_msg_list = sorted(tmp, key=lambda tup: float(tup[1]))

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;

    for ie in msg_list[k]["ies"]:
        key = ie["type"]
        if key in type_list.keys() and (type_list[key]["presence"] != ie["presence"] or type_list[key]["format"] != ie["format"] or type_list[key]["length"] != ie["length"]):
            d_print("KEY type different : %s\n" % key)
            d_print("%s.%s %s %s %s\n" % (v_lower(type_list[key]["message"]), type_list[key]["value"], type_list[key]["presence"], type_list[key]["format"], type_list[key]["length"]))
            d_print("%s.%s %s %s %s\n\n" % (v_lower(k), ie["value"], ie["presence"], ie["format"], ie["length"]))
            continue
        type_list[key] = { "reference" : ie["reference"], "presence" : ie["presence"], "format" : ie["format"], "length" : ie["length"], "message" : k, "value" : ie["value"] }

d_info("[Type List]")
typefile = currentdir + "type-list.py"
if os.path.isfile(typefile) and os.access(typefile, os.R_OK):
    execfile(typefile)
    print "Read from " + typefile

tmp = [(k, v["reference"]) for k, v in type_list.items()]
sorted_type_list = sorted(tmp, key=lambda tup: tup[1])

f = open(outdir + 'ies.h', 'w')
output_header_to_file(f)
f.write("""#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_IES_H
#define OGS_NAS_IES_H

#ifdef __cplusplus
extern "C" {
#endif

int ogs_nas_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type);

""")

for (k, v) in sorted_type_list:
    f.write("int ogs_nas_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf);\n" % (v_lower(k), v_lower(k), v_lower(k)))
f.write("\n")

for (k, v) in sorted_type_list:
    f.write("int ogs_nas_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s);\n" % (v_lower(k), v_lower(k), v_lower(k)))
f.write("\n")


f.write("""#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_IES_H */

""")
f.close()

f = open(outdir + 'ies.c', 'w')
output_header_to_file(f)
f.write("""#include "ogs-nas.h"

int ogs_nas_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type)
{
    uint16_t size = sizeof(uint8_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &type, size);

    return size;
}
""")

for (k, v) in sorted_type_list:
#    d_print("%s = %s\n" % (k, type_list[k]))
    f.write("/* %s %s\n" % (type_list[k]["reference"], k))
    f.write(" * %s %s %s */\n" % (type_list[k]["presence"], type_list[k]["format"], type_list[k]["length"]))
    if type_list[k]["format"] == "TV" and type_list[k]["length"] == "1":
        f.write("int ogs_nas_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        f.write("    memcpy(%s, pkbuf->data - 1, 1);\n\n" % v_lower(k))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);\n\n");
        f.write("    return 0;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        f.write("    uint16_t size = sizeof(ogs_nas_%s_t);\n\n" % v_lower(k))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(pkbuf->data - size, %s, size);\n\n" % v_lower(k))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
    elif type_list[k]["format"] == "TV" or type_list[k]["format"] == "V":
        f.write("int ogs_nas_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        if type_list[k]["length"] == "4":
            f.write("    uint16_t size = 3;\n\n")
        else:
            f.write("    uint16_t size = sizeof(ogs_nas_%s_t);\n\n" % v_lower(k))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(%s, pkbuf->data - size, size);\n\n" % v_lower(k))
        if "decode" in type_list[k]:
            f.write("%s" % type_list[k]["decode"])
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        if type_list[k]["length"] == "4":
            f.write("    uint16_t size = 3;\n")
        else:
            f.write("    uint16_t size = sizeof(ogs_nas_%s_t);\n" % v_lower(k))
        f.write("    ogs_nas_%s_t target;\n\n" % v_lower(k))
        f.write("    memcpy(&target, %s, size);\n" % v_lower(k))
        if "encode" in type_list[k]:
            f.write("%s" % type_list[k]["encode"])
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(pkbuf->data - size, &target, size);\n\n")
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
    elif type_list[k]["format"] == "LV-E" or type_list[k]["format"] == "TLV-E":
        f.write("int ogs_nas_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        f.write("    uint16_t size = 0;\n")
        f.write("    ogs_nas_%s_t *source = (ogs_nas_%s_t *)pkbuf->data;\n\n" % (v_lower(k), v_lower(k)))
        f.write("    %s->length = be16toh(source->length);\n" % v_lower(k))
        f.write("    size = %s->length + sizeof(%s->length);\n\n" % (v_lower(k), v_lower(k)))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    %s->buffer = pkbuf->data - size + sizeof(%s->length);\n\n" % (v_lower(k), v_lower(k)))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, (void*)%s->buffer, %s->length);\n\n" % (v_lower(k), v_lower(k)));
        f.write("    return size;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        f.write("    uint16_t size = 0;\n")
        f.write("    uint16_t target;\n\n")
        f.write("    ogs_assert(%s);\n" % v_lower(k))
        f.write("    ogs_assert(%s->buffer);\n\n" % v_lower(k))
        f.write("    size = sizeof(%s->length);\n" % v_lower(k))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    target = htobe16(%s->length);\n" % v_lower(k))
        f.write("    memcpy(pkbuf->data - size, &target, size);\n\n")
        f.write("    size = %s->length;\n" % v_lower(k))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(pkbuf->data - size, %s->buffer, size);\n\n" % v_lower(k))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return %s->length + sizeof(%s->length);\n" % (v_lower(k), v_lower(k)))
        f.write("}\n\n");
    else:
        f.write("int ogs_nas_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        f.write("    uint16_t size = 0;\n")
        f.write("    ogs_nas_%s_t *source = (ogs_nas_%s_t *)pkbuf->data;\n\n" % (v_lower(k), v_lower(k)))
        f.write("    %s->length = source->length;\n" % v_lower(k))
        f.write("    size = %s->length + sizeof(%s->length);\n\n" % (v_lower(k), v_lower(k)))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(%s, pkbuf->data - size, size);\n\n" % v_lower(k))
        if "decode" in type_list[k]:
            f.write("%s" % type_list[k]["decode"])
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), v_lower(k)))
        f.write("{\n")
        f.write("    uint16_t size = %s->length + sizeof(%s->length);\n" % (v_lower(k), v_lower(k)))
        f.write("    ogs_nas_%s_t target;\n\n" % v_lower(k))
        f.write("    memcpy(&target, %s, sizeof(ogs_nas_%s_t));\n" % (v_lower(k), v_lower(k)))
        if "encode" in type_list[k]:
            f.write("%s" % type_list[k]["encode"])
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(pkbuf->data - size, &target, size);\n\n")
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n");
f.close()

f = open(outdir + 'message.h', 'w')
output_header_to_file(f)
f.write("""#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_MESSAGE_H
#define OGS_NAS_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
 * When calculating AES_CMAC, we need to use the headroom of the packet. */
#define OGS_NAS_HEADROOM 16

#define OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE 0
#define OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED 1
#define OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED 2
#define OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT 3
#define OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT 4
#define OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_PARTICALLY_CIPHTERD 5
#define OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE 12

#define OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM 0x2
#define OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM 0x7

#define OGS_NAS_EPS_BEARER_IDENTITY_UNASSIGNED 0

typedef struct ogs_nas_emm_header_s {
ED2(uint8_t security_header_type:4;,
    uint8_t protocol_discriminator:4;)
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_emm_header_t;

typedef struct ogs_nas_esm_header_s {
ED2(uint8_t eps_bearer_identity:4;,
    uint8_t protocol_discriminator:4;)
    uint8_t procedure_transaction_identity;
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_esm_header_t;

typedef struct ogs_nas_security_header_s {
ED2(uint8_t security_header_type:4;,
    uint8_t protocol_discriminator:4;)
    uint32_t message_authentication_code;
    uint8_t sequence_number;
} __attribute__ ((packed)) ogs_nas_security_header_t;

""")

for (k, v) in sorted_msg_list:
    if k.find("TO UE") == -1 and k != "SERVICE REQUEST":
        f.write("#define OGS_NAS_" + v_upper(k) + " " + v.split('.')[0] + "\n")
f.write("\n")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue;

    f.write("\n/*******************************************************\n")
    f.write(" * %s\n" % k)
    f.write(" ******************************************************/")

    for i, ie in enumerate([ies for ies in msg_list[k]["ies"] if ies["presence"] == "O"]):
        f.write("\n#define OGS_NAS_%s_%s_PRESENT (1<<%d)" % (v_upper(k), v_upper(ie["value"]), i))

    for i, ie in enumerate([ies for ies in msg_list[k]["ies"] if ies["presence"] == "O"]):
        f.write("\n#define OGS_NAS_%s_%s_TYPE 0x%s" % (v_upper(k), v_upper(ie["value"]), re.sub('-', '0', ie["iei"])))

    f.write("\n\ntypedef struct ogs_nas_%s_s {\n" % v_lower(k))

    mandatory_fields = False;
    optional_fields = False;
    for ie in msg_list[k]["ies"]:
        if ie["presence"] == "M" and mandatory_fields is False:
            f.write("    /* Mandatory fields */\n")
            mandatory_fields = True;

        if ie["presence"] == "O" and optional_fields is False:
            f.write("\n    /* Optional fields */\n")
            f.write("    uint32_t presencemask;\n");
            optional_fields = True;

        f.write("    ogs_nas_" + v_lower(ie["type"]) + "_t " + \
                v_lower(ie["value"]) + ";\n")

    f.write("} ogs_nas_%s_t;\n\n" % v_lower(k))

f.write("\n")

f.write("""typedef struct ogs_nas_emm_message_s {
    ogs_nas_emm_header_t h;
    union {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue;
    if float(msg_list[k]["type"]) < 192:
        f.write("        ogs_nas_%s_t %s;\n" % (v_lower(k), v_lower(k)))
f.write("""    };
} ogs_nas_emm_message_t;

typedef struct ogs_nas_esm_message_s {
    ogs_nas_esm_header_t h;
    union {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue;
    if float(msg_list[k]["type"]) >= 192:
        f.write("        ogs_nas_%s_t %s;\n" % (v_lower(k), v_lower(k)))

f.write("""    };
} ogs_nas_esm_message_t;

typedef struct ogs_nas_message_s {
    ogs_nas_security_header_t h;
    union {
        ogs_nas_emm_message_t emm;
        ogs_nas_esm_message_t esm;
    };
} ogs_nas_message_t;

ogs_pkbuf_t *ogs_nas_emm_encode(ogs_nas_message_t *message);
ogs_pkbuf_t *ogs_nas_esm_encode(ogs_nas_message_t *message);
int ogs_nas_emm_decode(ogs_nas_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_esm_decode(ogs_nas_message_t *message, ogs_pkbuf_t *pkbuf);
ogs_pkbuf_t *ogs_nas_plain_encode(ogs_nas_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_MESSAGE_H */
""")

f.close()



f = open(outdir + 'decoder.c', 'w')
output_header_to_file(f)
f.write("""#include "ogs-nas.h"

""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue
    if len(msg_list[k]["ies"]) == 0:
        continue
    f.write("int ogs_nas_decode_%s(ogs_nas_message_t *message, ogs_pkbuf_t *pkbuf);\n" % v_lower(k))

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue
    if len(msg_list[k]["ies"]) == 0:
        continue

    f.write("int ogs_nas_decode_%s(ogs_nas_message_t *message, ogs_pkbuf_t *pkbuf)\n{\n" % v_lower(k))
    if float(msg_list[k]["type"]) < 192:
        f.write("    ogs_nas_%s_t *%s = &message->emm.%s;\n" % (v_lower(k), v_lower(k), v_lower(k)))
    else:
        f.write("    ogs_nas_%s_t *%s = &message->esm.%s;\n" % (v_lower(k), v_lower(k), v_lower(k)))
    f.write("    int decoded = 0;\n")
    f.write("    int size = 0;\n\n")
    f.write("    ogs_trace(\"[NAS] Decode %s\\n\");\n\n" % v_upper(k))

    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] == "M"]:
        f.write("    size = ogs_nas_decode_%s(&%s->%s, pkbuf);\n" % (v_lower(ie["type"]), v_lower(k), v_lower(ie["value"])))
        f.write("    ogs_assert(size >= 0);\n")
        f.write("    decoded += size;\n\n")

    optional_fields = False;
    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] == "O"]:
        if optional_fields is False:
            f.write("""    while(pkbuf->len > 0) 
    {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        ogs_assert(ogs_pkbuf_pull(pkbuf, size));
        decoded += size;

        switch(type)
        {
""")
            optional_fields = True;

        f.write("             case OGS_NAS_%s_%s_TYPE:\n" % (v_upper(k), v_upper(ie["value"])))
        f.write("                 size = ogs_nas_decode_%s(&%s->%s, pkbuf);\n" % (v_lower(ie["type"]), v_lower(k), v_lower(ie["value"])))
        f.write("                 ogs_assert(size >= 0);\n")
        f.write("                 %s->presencemask |= OGS_NAS_%s_%s_PRESENT;\n" % (v_lower(k), v_upper(k), v_upper(ie["value"])))
        f.write("                 decoded += size;\n")
        f.write("                 break;\n")

    if [ies for ies in msg_list[k]["ies"] if ies["presence"] == "O"]:
        f.write("""             default:
                ogs_warn("Unknown type(0x%x) or not implemented\\n", type);
                break;
        }
    }

""")
    f.write("""    return decoded;
}

""")

f.write("""int ogs_nas_emm_decode(ogs_nas_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    uint16_t decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    memset(message, 0, sizeof(ogs_nas_message_t));

    size = sizeof(ogs_nas_emm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(&message->emm.h, pkbuf->data - size, size);
    decoded += size;

    if (message->emm.h.security_header_type >=
            OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
    {
        ogs_assert(ogs_pkbuf_push(pkbuf, 1));
        decoded -= 1;
        size = ogs_nas_decode_service_request(message, pkbuf);
        ogs_assert(size >= 0);
        decoded += size;

        goto out;
    }

    switch(message->emm.h.message_type)
    {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) < 192 and k.find("TO UE") == -1 and k != "SERVICE REQUEST":
        f.write("        case OGS_NAS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("            size = ogs_nas_decode_%s(message, pkbuf);\n" % v_lower(k))
            f.write("            ogs_assert(size >= 0);\n")
            f.write("            decoded += size;\n")
        f.write("            break;\n")

f.write("""        default:
            ogs_error("Unknown message type (0x%x) or not implemented", 
                    message->emm.h.message_type);
            break;
    }

out:
    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}
""")

f.write("""int ogs_nas_esm_decode(ogs_nas_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    uint16_t decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    memset(message, 0, sizeof(ogs_nas_message_t));

    size = sizeof(ogs_nas_esm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(&message->esm.h, pkbuf->data - size, size);
    decoded += size;

    switch(message->esm.h.message_type)
    {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) >= 192:
        f.write("        case OGS_NAS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("            size = ogs_nas_decode_%s(message, pkbuf);\n" % v_lower(k))
            f.write("            ogs_assert(size >= 0);\n")
            f.write("            decoded += size;\n")
        f.write("            break;\n")

f.write("""        default:
            ogs_error("Unknown message type (0x%x) or not implemented", 
                    message->esm.h.message_type);
            break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}

""")

f.close()

f = open(outdir + 'encoder.c', 'w')
output_header_to_file(f)
f.write("""#include "ogs-nas.h"

""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue
    f.write("int ogs_nas_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_message_t *message);\n" % v_lower(k))

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue

    f.write("int ogs_nas_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_message_t *message)\n{\n" % v_lower(k))
    if float(msg_list[k]["type"]) < 192:
        f.write("    ogs_nas_%s_t *%s = &message->emm.%s;\n" % (v_lower(k), v_lower(k), v_lower(k)))
    else:
        f.write("    ogs_nas_%s_t *%s = &message->esm.%s;\n" % (v_lower(k), v_lower(k), v_lower(k)))
    f.write("    int encoded = 0;\n")
    f.write("    int size = 0;\n\n")
    f.write("    ogs_trace(\"[NAS] Encode %s\");\n\n" % v_upper(k))

    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] == "M"]:
        f.write("    size = ogs_nas_encode_%s(pkbuf, &%s->%s);\n" % (v_lower(ie["type"]), v_lower(k), v_lower(ie["value"])))
        f.write("    ogs_assert(size >= 0);\n")
        f.write("    encoded += size;\n\n")

    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] == "O"]:
        f.write("    if (%s->presencemask & OGS_NAS_%s_%s_PRESENT)\n" % (v_lower(k), v_upper(k), v_upper(ie["value"])))
        f.write("    {\n")
        if ie["length"] == "1" and ie["format"] == "TV":
            f.write("        %s->%s.type = (OGS_NAS_%s_%s_TYPE >> 4);\n\n" % (v_lower(k), v_lower(ie["value"]), v_upper(k), v_upper(ie["value"])))
        else:
            f.write("        size = ogs_nas_encode_optional_type(pkbuf, OGS_NAS_%s_%s_TYPE);\n" % (v_upper(k), v_upper(ie["value"])))
            f.write("        ogs_assert(size >= 0);\n")
            f.write("        encoded += size;\n\n")
        f.write("        size = ogs_nas_encode_%s(pkbuf, &%s->%s);\n" % (v_lower(ie["type"]), v_lower(k), v_lower(ie["value"])))
        f.write("        ogs_assert(size >= 0);\n")
        f.write("        encoded += size;\n")
        f.write("    }\n\n")

    f.write("""    return encoded;
}

""")


f.write("""ogs_pkbuf_t *ogs_nas_emm_encode(ogs_nas_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_emm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));

    memcpy(pkbuf->data - size, &message->emm.h, size);
    encoded += size;

    if (message->emm.h.security_header_type >=
            OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
    {
        ogs_assert(ogs_pkbuf_push(pkbuf, 1));
        encoded -= 1;
        size = ogs_nas_encode_service_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;

        goto out;
    }

    switch(message->emm.h.message_type)
    {
""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) < 192 and k.find("FROM UE") == -1 and k != "SERVICE REQUEST":
        f.write("        case OGS_NAS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("            size = ogs_nas_encode_%s(pkbuf, message);\n" % v_lower(k))
            f.write("            ogs_assert(size >= 0);\n")
            f.write("            encoded += size;\n")
        f.write("            break;\n")

f.write("""        default:
            ogs_error("Unknown message type (0x%x) or not implemented", 
                    message->emm.h.message_type);
            ogs_pkbuf_free(pkbuf);
            return NULL;
    }

out:
    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));

    pkbuf->len = encoded;

    return pkbuf;
}

""")

f.write("""ogs_pkbuf_t *ogs_nas_esm_encode(ogs_nas_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_esm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &message->esm.h, size);
    encoded += size;

    switch(message->esm.h.message_type)
    {
""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) >= 192:
        f.write("        case OGS_NAS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("            size = ogs_nas_encode_%s(pkbuf, message);\n" % v_lower(k))
            f.write("            ogs_assert(size >= 0);\n")
            f.write("            encoded += size;\n")
        f.write("            break;\n")

f.write("""        default:
            ogs_error("Unknown message type (0x%x) or not implemented", 
                    message->esm.h.message_type);
            ogs_pkbuf_free(pkbuf);
            return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));
    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *ogs_nas_plain_encode(ogs_nas_message_t *message)
{
    ogs_assert(message);

    ogs_assert(message->emm.h.protocol_discriminator ==
            message->esm.h.protocol_discriminator);

    if (message->emm.h.protocol_discriminator == 
            OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM)
        return ogs_nas_emm_encode(message);
    else if (message->emm.h.protocol_discriminator == 
            OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM)
        return ogs_nas_esm_encode(message);

    return NULL;
}
""")

f.close()

