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

FAIL = '\033[91m'
INFO = '\033[93m'
ENDC = '\033[0m'

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
    opts, args = getopt.getopt(sys.argv[1:], "df:ho:", ["debug", "file", "help", "output"])
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
    if o in ("-h", "--help"):
        usage()
        sys.exit(2)

if os.path.isfile(filename) and os.access(filename, os.R_OK):
    in_file = open(filename, 'r') 
else:
    printFail("Cannot find file : " + filename)

document = Document(filename)

ie_table = ""
msg_table = ""

for table in document.tables:
    cell = table.rows[0].cells[0]
    if cell.text.find('IE Type value') != -1:
        ie_table = table
        printInfo("[Information Element Table]")
        printInfo("...done");
    if cell.text.find('Message Type value') != -1:
        msg_table = table
        printInfo("[Message Table]")
        printInfo("...done");

printInfo("[Message Type]")
msg_list = {}
for row in msg_table.rows[2:-4]:
    if len(row.cells[0].text) == 0:
        continue
    if row.cells[0].text.find('to') != -1:
        continue
    if row.cells[1].text.find('Reserved') != -1:
        continue
    name = row.cells[1].text
    name = re.sub('\n', '', name)
    name = re.sub('\([^\)]*\)*', '', name)
    name = re.sub('\s$', '', name)
    msg_list[name] = { "type": row.cells[0].text }
    printDebug(name + " " + "type:" + row.cells[0].text)
printInfo("...done")

#printInfo("[Information Element Type]")
#ie_list = {}
#for row in ie_table.rows[1:-5]:
#    if row.cells[1].text.find('Reserved') != -1:
#        continue
#    ie_list[row.cells[1].text] = { "type": row.cells[0].text }
#    printDebug(row.cells[1].text + " " + "type:" + row.cells[0].text)
#printInfo("...done")
            

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
