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

version = "0.2.0"

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
    print("Python generating NAS Message encoder/decoder v%s" % (version))
    print("Usage: python nas-message.py [options]")
    print("Available options:")
    print("-d        Enable script debug")
    print("-f [file] Input file to parse")
    print("-o [dir]  Output files to given directory")
    print("-c [dir]  Cache files to given directory")
    print("-h        Print this help and return")

def v_upper(v):
    return re.sub('_TO_UE', '', re.sub('_FROM_UE', '', re.sub('\'', '_', re.sub('/', '_', re.sub('-', '_', re.sub(' ', '_', v)))).upper()))

def v_lower(v):
    return re.sub('\'', '_', re.sub('/', '_', re.sub('-', '_', re.sub(' ', '_', v)))).lower()

def get_value(v):
    return re.sub('5gs_', '', re.sub('5g_', '', re.sub('5gsm', 'gsm', re.sub('5gmm', 'gmm', re.sub('\'', '_', re.sub('/', '_', re.sub('-', '_', re.sub(' ', '_', v)))).lower()))))

def get_cells(cells):
    iei = cells[0].text
    value = re.sub("\s*$", "", re.sub("\s*\n*\s*\([^\)]*\)*", "", re.sub("'s", "", cells[1].text)))
    type = re.sub("^NAS ", "", re.sub("'s", "", re.sub('\s*\n\s*[a-zA-Z0-9.]*', '', cells[2].text)))
    reference = re.sub('[a-zA-Z0-9\'\-\s]*\n\s*', '', cells[2].text)
    presence = cells[3].text
    format = cells[4].text
    length = cells[5].text

# Spec errata - workaround
    if (type == "Request type" and value == "Request type"):
        iei = "8-"

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

msg_list["REGISTRATION REQUEST"] = { "type" : "65" }
msg_list["REGISTRATION ACCEPT"] = { "type" : "66" }
msg_list["REGISTRATION COMPLETE"] = { "type" : "67" }
msg_list["REGISTRATION REJECT"] = { "type" : "68" }
msg_list["DEREGISTRATION REQUEST FROM UE"] = { "type" : "69" }
msg_list["DEREGISTRATION ACCEPT FROM UE"] = { "type" : "70" }
msg_list["DEREGISTRATION REQUEST TO UE"] = { "type" : "71" }
msg_list["DEREGISTRATION ACCEPT TO UE"] = { "type" : "72" }
msg_list["SERVICE REQUEST"] = { "type" : "76" }
msg_list["SERVICE REJECT"] = { "type" : "77" }
msg_list["SERVICE ACCEPT"] = { "type" : "78" }
msg_list["CONFIGURATION UPDATE COMMAND"] = { "type" : "84" }
msg_list["CONFIGURATION UPDATE COMPLETE"] = { "type" : "85" }
msg_list["AUTHENTICATION REQUEST"] = { "type" : "86" }
msg_list["AUTHENTICATION RESPONSE"] = { "type" : "87" }
msg_list["AUTHENTICATION REJECT"] = { "type" : "88" }
msg_list["AUTHENTICATION FAILURE"] = { "type" : "89" }
msg_list["AUTHENTICATION RESULT"] = { "type" : "90" }
msg_list["IDENTITY REQUEST"] = { "type" : "91" }
msg_list["IDENTITY RESPONSE"] = { "type" : "92" }
msg_list["SECURITY MODE COMMAND"] = { "type" : "93" }
msg_list["SECURITY MODE COMPLETE"] = { "type" : "94" }
msg_list["SECURITY MODE REJECT"] = { "type" : "95" }
msg_list["5GMM STATUS"] = { "type" : "100" }
msg_list["NOTIFICATION"] = { "type" : "101" }
msg_list["NOTIFICATION RESPONSE"] = { "type" : "102" }
msg_list["UL NAS TRANSPORT"] = { "type" : "103" }
msg_list["DL NAS TRANSPORT"] = { "type" : "104" }

msg_list["PDU SESSION ESTABLISHMENT REQUEST"] = { "type" : "193" }
msg_list["PDU SESSION ESTABLISHMENT ACCEPT"] = { "type" : "194" }
msg_list["PDU SESSION ESTABLISHMENT REJECT"] = { "type" : "195" }
msg_list["PDU SESSION AUTHENTICATION COMMAND"] = { "type" : "197" }
msg_list["PDU SESSION AUTHENTICATION COMPLETE"] = { "type" : "198" }
msg_list["PDU SESSION AUTHENTICATION RESULT"] = { "type" : "199" }
msg_list["PDU SESSION MODIFICATION REQUEST"] = { "type" : "201" }
msg_list["PDU SESSION MODIFICATION REJECT"] = { "type" : "202" }
msg_list["PDU SESSION MODIFICATION COMMAND"] = { "type" : "203" }
msg_list["PDU SESSION MODIFICATION COMPLETE"] = { "type" : "204" }
msg_list["PDU SESSION MODIFICATION COMMAND REJECT"] = { "type" : "205" }
msg_list["PDU SESSION RELEASE REQUEST"] = { "type" : "209" }
msg_list["PDU SESSION RELEASE REJECT"] = { "type" : "210" }
msg_list["PDU SESSION RELEASE COMMAND"] = { "type" : "211" }
msg_list["PDU SESSION RELEASE COMPLETE"] = { "type" : "212" }
msg_list["5GSM STATUS"] = { "type" : "214" }

# Table number for Message List
msg_list["AUTHENTICATION REQUEST"]["table"] = 0
msg_list["AUTHENTICATION RESPONSE"]["table"] = 1
msg_list["AUTHENTICATION RESULT"]["table"] = 2
msg_list["AUTHENTICATION FAILURE"]["table"] = 3
msg_list["AUTHENTICATION REJECT"]["table"] = 4
msg_list["REGISTRATION REQUEST"]["table"] = 5
msg_list["REGISTRATION ACCEPT"]["table"] = 6
msg_list["REGISTRATION COMPLETE"]["table"] = 7
msg_list["REGISTRATION REJECT"]["table"] = 8
msg_list["UL NAS TRANSPORT"]["table"] = 9
msg_list["DL NAS TRANSPORT"]["table"] = 10
msg_list["DEREGISTRATION REQUEST FROM UE"]["table"] = 11
msg_list["DEREGISTRATION ACCEPT FROM UE"]["table"] = 12
msg_list["DEREGISTRATION REQUEST TO UE"]["table"] = 13
msg_list["DEREGISTRATION ACCEPT TO UE"]["table"] = 14
msg_list["SERVICE REQUEST"]["table"] = 15
msg_list["SERVICE ACCEPT"]["table"] = 16
msg_list["SERVICE REJECT"]["table"] = 17
msg_list["CONFIGURATION UPDATE COMMAND"]["table"] = 18
msg_list["CONFIGURATION UPDATE COMPLETE"]["table"] = 19
msg_list["IDENTITY REQUEST"]["table"] = 20
msg_list["IDENTITY RESPONSE"]["table"] = 21
msg_list["NOTIFICATION"]["table"] = 22
msg_list["NOTIFICATION RESPONSE"]["table"] = 23
msg_list["SECURITY MODE COMMAND"]["table"] = 24
msg_list["SECURITY MODE COMPLETE"]["table"] = 25
msg_list["SECURITY MODE REJECT"]["table"] = 26
msg_list["5GMM STATUS"]["table"] = 28

msg_list["PDU SESSION ESTABLISHMENT REQUEST"]["table"] = 33
msg_list["PDU SESSION ESTABLISHMENT ACCEPT"]["table"] = 34
msg_list["PDU SESSION ESTABLISHMENT REJECT"]["table"] = 35
msg_list["PDU SESSION AUTHENTICATION COMMAND"]["table"] = 36
msg_list["PDU SESSION AUTHENTICATION COMPLETE"]["table"] = 37
msg_list["PDU SESSION AUTHENTICATION RESULT"]["table"] = 38
msg_list["PDU SESSION MODIFICATION REQUEST"]["table"] = 39
msg_list["PDU SESSION MODIFICATION REJECT"]["table"] = 40
msg_list["PDU SESSION MODIFICATION COMMAND"]["table"] = 41
msg_list["PDU SESSION MODIFICATION COMPLETE"]["table"] = 42
msg_list["PDU SESSION MODIFICATION COMMAND REJECT"]["table"] = 43
msg_list["PDU SESSION RELEASE REQUEST"]["table"] = 44
msg_list["PDU SESSION RELEASE REJECT"]["table"] = 45
msg_list["PDU SESSION RELEASE COMMAND"]["table"] = 46
msg_list["PDU SESSION RELEASE COMPLETE"]["table"] = 47
msg_list["5GSM STATUS"]["table"] = 48

for key in msg_list.keys():
    if "table" not in msg_list[key].keys():
        continue;

    d_info("[" + key + "]")
    cachefile = cachedir + "nas-msg-" + msg_list[key]["type"] + ".py"
    if os.path.isfile(cachefile) and os.access(cachefile, os.R_OK):
        exec(open(cachefile).read())
        print("Read from " + cachefile)
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
    exec(open(typefile).read())
    print("Read from " + typefile)

tmp = [(k, v["reference"]) for k, v in type_list.items()]
sorted_type_list = sorted(tmp, key=lambda tup: tup[1])

f = open(outdir + 'ies.h', 'w')
output_header_to_file(f)
f.write("""#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_5GS_IES_H
#define OGS_NAS_5GS_IES_H

#ifdef __cplusplus
extern "C" {
#endif

int ogs_nas_5gs_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type);

""")

for (k, v) in sorted_type_list:
    f.write("int ogs_nas_5gs_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf);\n" % (v_lower(k), v_lower(k), get_value(k)))
f.write("\n")

for (k, v) in sorted_type_list:
    f.write("int ogs_nas_5gs_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s);\n" % (v_lower(k), v_lower(k), get_value(k)))
f.write("\n")


f.write("""#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_5GS_IES_H */

""")
f.close()

f = open(outdir + 'ies.c', 'w')
output_header_to_file(f)
f.write("""#include "ogs-nas-5gs.h"

int ogs_nas_5gs_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type)
{
    int size = sizeof(uint8_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &type, size);

    return size;
}
""")

for (k, v) in sorted_type_list:
#    d_print("%s = %s\n" % (k, type_list[k]))
    f.write("/* %s %s\n" % (type_list[k]["reference"], k))
    f.write(" * %s %s %s */\n" % (type_list[k]["presence"], type_list[k]["format"], type_list[k]["length"]))
    if (type_list[k]["format"] == "TV" or type_list[k]["format"] == "T") and type_list[k]["length"] == "1":
        f.write("int ogs_nas_5gs_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        f.write("    int size = sizeof(ogs_nas_%s_t);\n\n" % v_lower(k))
        f.write("    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {\n")
        f.write("       ogs_error(\"ogs_pkbuf_pull() failed [size:%d]\", (int)size);\n")
        f.write("       return -1;\n")
        f.write("    }\n\n")
        f.write("    memcpy(%s, pkbuf->data - size, size);\n\n" % get_value(k))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_5gs_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        f.write("    int size = sizeof(ogs_nas_%s_t);\n\n" % v_lower(k))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(pkbuf->data - size, %s, size);\n\n" % get_value(k))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
    elif type_list[k]["format"] == "TV" or type_list[k]["format"] == "V":
        f.write("int ogs_nas_5gs_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        if type_list[k]["length"] == "4":
            f.write("    int size = 3;\n\n")
        else:
            f.write("    int size = sizeof(ogs_nas_%s_t);\n\n" % v_lower(k))
        f.write("    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {\n")
        f.write("       ogs_error(\"ogs_pkbuf_pull() failed [size:%d]\", (int)size);\n")
        f.write("       return -1;\n")
        f.write("    }\n\n")
        f.write("    memcpy(%s, pkbuf->data - size, size);\n\n" % get_value(k))
        if "decode" in type_list[k]:
            f.write("%s" % type_list[k]["decode"])
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_5gs_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        if type_list[k]["length"] == "4":
            f.write("    int size = 3;\n")
        else:
            f.write("    int size = sizeof(ogs_nas_%s_t);\n" % v_lower(k))
        f.write("    ogs_nas_%s_t target;\n\n" % v_lower(k))
        f.write("    memcpy(&target, %s, size);\n" % get_value(k))
        if "encode" in type_list[k]:
            f.write("%s" % type_list[k]["encode"])
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(pkbuf->data - size, &target, size);\n\n")
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
    elif type_list[k]["format"] == "LV-E" or type_list[k]["format"] == "TLV-E":
        f.write("int ogs_nas_5gs_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        f.write("    int size = 0;\n")
        f.write("    ogs_nas_%s_t *source = (ogs_nas_%s_t *)pkbuf->data;\n\n" % (v_lower(k), v_lower(k)))
        f.write("    %s->length = be16toh(source->length);\n" % get_value(k))
        f.write("    size = %s->length + sizeof(%s->length);\n\n" % (get_value(k), get_value(k)))
        f.write("    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {\n")
        f.write("       ogs_error(\"ogs_pkbuf_pull() failed [size:%d]\", (int)size);\n")
        f.write("       return -1;\n")
        f.write("    }\n\n")
        f.write("    %s->buffer = pkbuf->data - size + sizeof(%s->length);\n\n" % (get_value(k), get_value(k)))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, (void*)%s->buffer, %s->length);\n\n" % (get_value(k), get_value(k)));
        f.write("    return size;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_5gs_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        f.write("    int size = 0;\n")
        f.write("    int target;\n\n")
        f.write("    ogs_assert(%s);\n" % get_value(k))
        f.write("    ogs_assert(%s->buffer);\n\n" % get_value(k))
        f.write("    size = sizeof(%s->length);\n" % get_value(k))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    target = htobe16(%s->length);\n" % get_value(k))
        f.write("    memcpy(pkbuf->data - size, &target, size);\n\n")
        f.write("    size = %s->length;\n" % get_value(k))
        f.write("    ogs_assert(ogs_pkbuf_pull(pkbuf, size));\n")
        f.write("    memcpy(pkbuf->data - size, %s->buffer, size);\n\n" % get_value(k))
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return %s->length + sizeof(%s->length);\n" % (get_value(k), get_value(k)))
        f.write("}\n\n");
    else:
        f.write("int ogs_nas_5gs_decode_%s(ogs_nas_%s_t *%s, ogs_pkbuf_t *pkbuf)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        f.write("    int size = 0;\n")
        f.write("    ogs_nas_%s_t *source = (ogs_nas_%s_t *)pkbuf->data;\n\n" % (v_lower(k), v_lower(k)))
        f.write("    %s->length = source->length;\n" % get_value(k))
        f.write("    size = %s->length + sizeof(%s->length);\n\n" % (get_value(k), get_value(k)))
        f.write("    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {\n")
        f.write("       ogs_error(\"ogs_pkbuf_pull() failed [size:%d]\", (int)size);\n")
        f.write("       return -1;\n")
        f.write("    }\n\n")
        f.write("    if (sizeof(*%s) < size) return -1;\n" % get_value(k))
        f.write("    memcpy(%s, pkbuf->data - size, size);\n\n" % get_value(k))
        if "decode" in type_list[k]:
            f.write("%s" % type_list[k]["decode"])
        f.write("    ogs_trace(\"  %s - \");\n" % v_upper(k))
        f.write("    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);\n\n");
        f.write("    return size;\n")
        f.write("}\n\n")
        f.write("int ogs_nas_5gs_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_%s_t *%s)\n" % (v_lower(k), v_lower(k), get_value(k)))
        f.write("{\n")
        f.write("    int size = %s->length + sizeof(%s->length);\n" % (get_value(k), get_value(k)))
        f.write("    ogs_nas_%s_t target;\n\n" % v_lower(k))
        f.write("    memcpy(&target, %s, sizeof(ogs_nas_%s_t));\n" % (get_value(k), v_lower(k)))
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

#ifndef OGS_NAS_5GS_MESSAGE_H
#define OGS_NAS_5GS_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM 0x2e
#define OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM 0x7e

#define OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED 0

typedef struct ogs_nas_5gmm_header_s {
    uint8_t extended_protocol_discriminator;
    uint8_t security_header_type;
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_5gmm_header_t;

typedef struct ogs_nas_5gsm_header_s {
    uint8_t extended_protocol_discriminator;
    uint8_t pdu_session_identity;
    uint8_t procedure_transaction_identity;
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_5gsm_header_t;

typedef struct ogs_nas_5gs_security_header_s {
    uint8_t extended_protocol_discriminator;
    uint8_t security_header_type;
    uint32_t message_authentication_code;
    uint8_t sequence_number;
} __attribute__ ((packed)) ogs_nas_5gs_security_header_t;

""")

for (k, v) in sorted_msg_list:
    if k.find("TO UE") == -1:
        f.write("#define OGS_NAS_5GS_" + v_upper(k) + " " + v.split('.')[0] + "\n")
f.write("\n")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue;

    f.write("\n/*******************************************************\n")
    f.write(" * %s\n" % k)
    f.write(" ******************************************************/")

    for i, ie in enumerate([ies for ies in msg_list[k]["ies"] if ies["presence"] != "M"]):
        f.write("\n#define OGS_NAS_5GS_%s_%s_PRESENT ((uint64_t)1<<%d)" % (v_upper(k), v_upper(ie["value"]), i))

    for i, ie in enumerate([ies for ies in msg_list[k]["ies"] if ies["presence"] != "M"]):
        f.write("\n#define OGS_NAS_5GS_%s_%s_TYPE 0x%s" % (v_upper(k), v_upper(ie["value"]), re.sub('-', '0', ie["iei"])))

    f.write("\n\ntypedef struct ogs_nas_5gs_%s_s {\n" % v_lower(k))

    mandatory_fields = False;
    optional_fields = False;
    for ie in msg_list[k]["ies"]:
        if ie["presence"] == "M" and mandatory_fields is False:
            f.write("    /* Mandatory fields */\n")
            mandatory_fields = True;

        if ie["presence"] != "M" and optional_fields is False:
            f.write("\n    /* Optional fields */\n")
            f.write("    uint64_t presencemask;\n");
            optional_fields = True;

        f.write("    ogs_nas_" + v_lower(ie["type"]) + "_t " + \
                get_value(ie["value"]) + ";\n")

    f.write("} ogs_nas_5gs_%s_t;\n\n" % v_lower(k))

f.write("\n")

f.write("""typedef struct ogs_nas_5gmm_message_s {
    ogs_nas_5gmm_header_t h;
    union {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue;
    if float(msg_list[k]["type"]) < 192:
        f.write("        ogs_nas_5gs_%s_t %s;\n" % (v_lower(k), get_value(k)))
f.write("""    };
} ogs_nas_5gmm_message_t;

typedef struct ogs_nas_5gsm_message_s {
    ogs_nas_5gsm_header_t h;
    union {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue;
    if float(msg_list[k]["type"]) >= 192:
        f.write("        ogs_nas_5gs_%s_t %s;\n" % (v_lower(k), get_value(k)))

f.write("""    };
} ogs_nas_5gsm_message_t;

typedef struct ogs_nas_5gs_message_s {
    ogs_nas_5gs_security_header_t h;
    union {
        ogs_nas_5gmm_message_t gmm;
        ogs_nas_5gsm_message_t gsm;
    };
} ogs_nas_5gs_message_t;

ogs_pkbuf_t *ogs_nas_5gmm_encode(ogs_nas_5gs_message_t *message);
ogs_pkbuf_t *ogs_nas_5gsm_encode(ogs_nas_5gs_message_t *message);
int ogs_nas_5gmm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gsm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
ogs_pkbuf_t *ogs_nas_5gs_plain_encode(ogs_nas_5gs_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_5GS_MESSAGE_H */
""")

f.close()



f = open(outdir + 'decoder.c', 'w')
output_header_to_file(f)
f.write("""#include "ogs-nas-5gs.h"

""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue
    if len(msg_list[k]["ies"]) == 0:
        continue
    f.write("int ogs_nas_5gs_decode_%s(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);\n" % v_lower(k))

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue
    if len(msg_list[k]["ies"]) == 0:
        continue

    f.write("int ogs_nas_5gs_decode_%s(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)\n{\n" % v_lower(k))
    if float(msg_list[k]["type"]) < 192:
        f.write("    ogs_nas_5gs_%s_t *%s = &message->gmm.%s;\n" % (v_lower(k), get_value(k), get_value(k)))
    else:
        f.write("    ogs_nas_5gs_%s_t *%s = &message->gsm.%s;\n" % (v_lower(k), get_value(k), get_value(k)))
    f.write("    int decoded = 0;\n")
    f.write("    int size = 0;\n\n")
    f.write("    ogs_trace(\"[NAS] Decode %s\\n\");\n\n" % v_upper(k))

    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] == "M"]:
        f.write("    size = ogs_nas_5gs_decode_%s(&%s->%s, pkbuf);\n" % (v_lower(ie["type"]), get_value(k), get_value(ie["value"])))
        f.write("    if (size < 0) {\n")
        f.write("        ogs_error(\"ogs_nas_5gs_decode_%s() failed\");\n" % v_lower(ie["type"]))
        f.write("        return size;\n")
        f.write("    }\n\n")
        f.write("    decoded += size;\n\n")

    optional_fields = False;
    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] != "M"]:
        if optional_fields is False:
            f.write("""    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
""")
            optional_fields = True;

        f.write("        case OGS_NAS_5GS_%s_%s_TYPE:\n" % (v_upper(k), v_upper(ie["value"])))
        if (ie["format"] == "TV" or ie["format"] == "T") and ie["length"] == "1":
            f.write("            decoded--;\n")
            f.write("            ogs_assert(ogs_pkbuf_push(pkbuf, 1));\n")
        f.write("            size = ogs_nas_5gs_decode_%s(&%s->%s, pkbuf);\n" % (v_lower(ie["type"]), get_value(k), get_value(ie["value"])))
        f.write("            if (size < 0) {\n")
        f.write("               ogs_error(\"ogs_nas_5gs_decode_%s() failed\");\n" % v_lower(ie["type"]))
        f.write("               return size;\n")
        f.write("            }\n\n")
        f.write("            %s->presencemask |= OGS_NAS_5GS_%s_%s_PRESENT;\n" % (get_value(k), v_upper(k), v_upper(ie["value"])))
        f.write("            decoded += size;\n")
        f.write("            break;\n")

    if [ies for ies in msg_list[k]["ies"] if ies["presence"] != "M"]:
        f.write("""        default:
            ogs_error("Unknown type(0x%x) or not implemented\\n", type);
            break;
        }
    }

""")
    f.write("""    return decoded;
}

""")

f.write("""int ogs_nas_5gmm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    int decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    memset(message, 0, sizeof(ogs_nas_5gs_message_t));

    size = sizeof(ogs_nas_5gmm_header_t);
    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return OGS_ERROR;
    }
    memcpy(&message->gmm.h, pkbuf->data - size, size);
    decoded += size;

    switch(message->gmm.h.message_type) {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) < 192 and k.find("TO UE") == -1:
        f.write("    case OGS_NAS_5GS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("        size = ogs_nas_5gs_decode_%s(message, pkbuf);\n" % v_lower(k))
            f.write("        if (size < 0) {\n")
            f.write("           ogs_error(\"ogs_nas_5gs_decode_%s() failed\");\n" % v_lower(k))
            f.write("           return size;\n")
            f.write("        }\n\n")
            f.write("        decoded += size;\n")
        f.write("        break;\n")

f.write("""    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gmm.h.message_type);
        break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}
""")

f.write("""int ogs_nas_5gsm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    int decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    memset(message, 0, sizeof(ogs_nas_5gs_message_t));

    size = sizeof(ogs_nas_5gsm_header_t);
    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return OGS_ERROR;
    }
    memcpy(&message->gsm.h, pkbuf->data - size, size);
    decoded += size;

    switch(message->gsm.h.message_type) {
""")
for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) >= 192:
        f.write("    case OGS_NAS_5GS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("        size = ogs_nas_5gs_decode_%s(message, pkbuf);\n" % v_lower(k))
            f.write("        if (size < 0) {\n")
            f.write("           ogs_error(\"ogs_nas_5gs_decode_%s() failed\");\n" % v_lower(k))
            f.write("           return size;\n")
            f.write("        }\n\n")
            f.write("        decoded += size;\n")
        f.write("        break;\n")

f.write("""    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gsm.h.message_type);
        break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}

""")

f.close()

f = open(outdir + 'encoder.c', 'w')
output_header_to_file(f)
f.write("""#include "ogs-nas-5gs.h"

""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue
    f.write("int ogs_nas_5gs_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);\n" % v_lower(k))

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if len(msg_list[k]["ies"]) == 0:
        continue

    f.write("int ogs_nas_5gs_encode_%s(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)\n{\n" % v_lower(k))
    if float(msg_list[k]["type"]) < 192:
        f.write("    ogs_nas_5gs_%s_t *%s = &message->gmm.%s;\n" % (v_lower(k), get_value(k), get_value(k)))
    else:
        f.write("    ogs_nas_5gs_%s_t *%s = &message->gsm.%s;\n" % (v_lower(k), get_value(k), get_value(k)))
    f.write("    int encoded = 0;\n")
    f.write("    int size = 0;\n\n")
    f.write("    ogs_trace(\"[NAS] Encode %s\");\n\n" % v_upper(k))

    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] == "M"]:
        f.write("    size = ogs_nas_5gs_encode_%s(pkbuf, &%s->%s);\n" % (v_lower(ie["type"]), get_value(k), get_value(ie["value"])))
        f.write("    ogs_assert(size >= 0);\n")
        f.write("    encoded += size;\n\n")

    for ie in [ies for ies in msg_list[k]["ies"] if ies["presence"] != "M"]:
        f.write("    if (%s->presencemask & OGS_NAS_5GS_%s_%s_PRESENT) {\n" % (get_value(k), v_upper(k), v_upper(ie["value"])))
        if ie["length"] == "1" and ie["format"] == "TV":
            f.write("        %s->%s.type = (OGS_NAS_5GS_%s_%s_TYPE >> 4);\n\n" % (get_value(k), get_value(ie["value"]), v_upper(k), v_upper(ie["value"])))
        elif ie["length"] == "1" and ie["format"] == "T":
            f.write("        %s->%s.type = OGS_NAS_5GS_%s_%s_TYPE;\n\n" % (get_value(k), get_value(ie["value"]), v_upper(k), v_upper(ie["value"])))
        else:
            f.write("        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_%s_%s_TYPE);\n" % (v_upper(k), v_upper(ie["value"])))
            f.write("        ogs_assert(size >= 0);\n")
            f.write("        encoded += size;\n\n")
        f.write("        size = ogs_nas_5gs_encode_%s(pkbuf, &%s->%s);\n" % (v_lower(ie["type"]), get_value(k), get_value(ie["value"])))
        f.write("        ogs_assert(size >= 0);\n")
        f.write("        encoded += size;\n")
        f.write("    }\n\n")

    f.write("""    return encoded;
}

""")


f.write("""ogs_pkbuf_t *ogs_nas_5gmm_encode(ogs_nas_5gs_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_expect_or_return_val(pkbuf, NULL);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_5gmm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));

    memcpy(pkbuf->data - size, &message->gmm.h, size);
    encoded += size;

    switch(message->gmm.h.message_type) {
""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) < 192 and k.find("FROM UE") == -1:
        f.write("    case OGS_NAS_5GS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("        size = ogs_nas_5gs_encode_%s(pkbuf, message);\n" % v_lower(k))
            f.write("        ogs_assert(size >= 0);\n")
            f.write("        encoded += size;\n")
        f.write("        break;\n")

f.write("""    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gmm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));

    pkbuf->len = encoded;

    return pkbuf;
}

""")

f.write("""ogs_pkbuf_t *ogs_nas_5gsm_encode(ogs_nas_5gs_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_expect_or_return_val(pkbuf, NULL);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_5gsm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &message->gsm.h, size);
    encoded += size;

    switch(message->gsm.h.message_type) {
""")

for (k, v) in sorted_msg_list:
    if "ies" not in msg_list[k]:
        continue;
    if float(msg_list[k]["type"]) >= 192:
        f.write("    case OGS_NAS_5GS_%s:\n" % v_upper(k))
        if len(msg_list[k]["ies"]) != 0:
            f.write("        size = ogs_nas_5gs_encode_%s(pkbuf, message);\n" % v_lower(k))
            f.write("        ogs_assert(size >= 0);\n")
            f.write("        encoded += size;\n")
        f.write("        break;\n")

f.write("""    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gsm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));
    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *ogs_nas_5gs_plain_encode(ogs_nas_5gs_message_t *message)
{
    ogs_assert(message);

    ogs_assert(message->gmm.h.extended_protocol_discriminator ==
            message->gsm.h.extended_protocol_discriminator);

    if (message->gmm.h.extended_protocol_discriminator == 
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM)
        return ogs_nas_5gmm_encode(message);
    else if (message->gmm.h.extended_protocol_discriminator == 
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM)
        return ogs_nas_5gsm_encode(message);

    return NULL;
}
""")

f.close()

