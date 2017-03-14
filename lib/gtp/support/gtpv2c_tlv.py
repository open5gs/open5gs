#
# Copyright (c) 2017, CellWire Group
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
from docx import Document
import re, os, sys, string
import getopt

version = "0.1.0"

verbosity = 0
filename = ""
outdir = './'
cachedir = './cache/'

FAIL = '\033[91m'
INFO = '\033[93m'
ENDC = '\033[0m'

def writeCacheFile(f, string):
    f.write(string + "\n")
    if verbosity > 0:
        print string

def printDebug(string):
    if verbosity > 0:
        print string

def printInfo(string):
    sys.stderr.write(INFO + string + ENDC + "\n")

def printFail(string):
    sys.stderr.write(FAIL + string + ENDC + "\n")
    sys.exit(0)


def usage():
    print "Python adding prefix for asn1 v%s" % (version)
    print "Usage: python asn1prefix.py [options]"
    print "Available options:"
    print "-d        Enable script debug"
    print "-f [file] Input file to parse"
    print "-o [dir]  Output files to given directory"
    print "-h        Print this help and return"

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

if os.path.isfile(filename) and os.access(filename, os.R_OK):
    file = open(filename, 'r') 
else:
    printFail("Cannot find file : " + filename)

document = Document(filename)

printInfo("[Message Type]")
msg_list = {}
cachefile = cachedir + 'gtpv2c_msg_list.py'
if os.path.isfile(cachefile) and os.access(cachefile, os.R_OK):
    execfile(cachefile)
    print "Read from " + cachefile
else:
    f = open(cachefile, 'w') 

    msg_table = ""
    for table in document.tables:
        cell = table.rows[0].cells[0]
        if cell.text.find('Message Type value') != -1:
            msg_table = table

    for row in msg_table.rows[2:-4]:
        key = row.cells[1].text
        type = row.cells[0].text
        if type.isdigit() is False:
            continue
        if int(type) in range(128, 160):
            continue
        if int(type) in range(231, 240):
            continue
        if key.find('Reserved') != -1:
            continue
        key = re.sub('\s*\n*\s*\([^\)]*\)*', '', key)
        msg_list[key] = { "type": type }
        writeCacheFile(f, "msg_list[\"" + key + "\"] = { \"type\" : \"" + type + "\" }")
    f.close()

printInfo("[Information Element Type]")
ie_list = {}
cachefile = cachedir + 'gtpv2c_ie_list.py'
if os.path.isfile(cachefile) and os.access(cachefile, os.R_OK):
    execfile(cachefile)
    print "Read from " + cachefile
else:
    f = open(cachefile, 'w') 

    ie_table = ""
    for table in document.tables:
        cell = table.rows[0].cells[0]
        if cell.text.find('IE Type value') != -1:
            ie_table = table

    for row in ie_table.rows[1:-5]:
        key = row.cells[1].text
        if key.find('Reserved') != -1:
            continue
        if key.find('MM Context') != -1:
            continue
        elif key.find('Recovery') != -1:
            key = 'Recovery'
        elif key.find('Trusted WLAN Mode Indication') != -1:
            key = 'TWMI'
        elif key.find('LDN') != -1:
            key = 'LDN'
        elif key.find('APCO') != -1:
            key = 'APCO'
        else:
            key = re.sub('.*\(', '', row.cells[1].text)
            key = re.sub('\)', '', key)
        type = row.cells[0].text
        ie_list[key] = { "type": type }
        writeCacheFile(f, "ie_list[\"" + key + "\"] = { \"type\" : \"" + type + "\" }")
    f.close()
ie_list['MM Context'] = { "type": "107" }
ie_list['Bearer Context'] = { "type": "93" }

msg_list["Echo Request"]["table"] = document.tables[6]
msg_list["Echo Response"]["table"] =  document.tables[7]
msg_list["Create Session Request"]["table"] = document.tables[8]

for key in msg_list.keys():
    if "table" in msg_list[key].keys():
        rows = []

        printInfo("[" + key + "]")
        cachefile = cachedir + "gtpv2c_msg_ies-" + msg_list[key]["type"] + ".py"
        if os.path.isfile(cachefile) and os.access(cachefile, os.R_OK):
            execfile(cachefile)
            print "Read from " + cachefile
        else:
            f = open(cachefile, 'w') 

            table = msg_list[key]["table"]
            for row in table.rows[1:]:
                instance = row.cells[4].text
                if instance.isdigit() is not True:
                    continue
                ie_type = re.sub('\s*\n*\s*\(NOTE.*\)*', '', row.cells[3].text)
                if ie_type.find('LDN') != -1:
                    ie_type = 'LDN'
                elif ie_type.find('APCO') != -1:
                    ie_type = 'APCO'
                if ie_type not in ie_list.keys():
                    assert False, "Unknown IE type : [" \
                            + row.cells[3].text + "]" + "(" + ie_type + ")"
                presence = row.cells[1].text
                ie_value = row.cells[0].text
                comment = row.cells[2].text.encode('ascii', 'ignore').decode('ascii')
                comment = re.sub('\n|\"|\'|\\\\', '', comment);

                rows.append({ "ie_type" : ie_type, "ie_value" : ie_value, 
                    "presence" : presence, "instance" : instance, 
                    "comment" : comment })
                writeCacheFile(f, "rows.append({ \"ie_type\" : \"" + ie_type + \
                    "\", \"ie_value\" : \"" + ie_value + \
                    "\", \"presence\" : \"" + presence + \
                    "\", \"instance\" : \"" + instance + \
                    "\", \"comment\" : \"" + comment + "\" })")
            msg_list[key]["ies"] = rows
            writeCacheFile(f, "msg_list[key][\"ies\"] = rows")
            f.close()

# Data will be a list of rows represented as dictionaries
# containing each row's data.
#data = []

#keys = None
#for i, row in enumerate(tlv_table.rows):
#    text = (cell.text for cell in row.cells)
#
#    # Establish the mapping based on the first row
#    # headers; these will become the keys of our dictionary
#    if i == 0:
#        keys = tuple(text)
#        continue
#
#    # Construct a dictionary for this row, mapping
#    # keys to values for this row
#    row_data = dict(zip(keys, text))
#    data.append(row_data)
#
#print data
