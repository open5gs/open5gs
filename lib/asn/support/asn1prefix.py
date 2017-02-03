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
import re, os, sys, string
import getopt

version = "0.1.0"

def printFail(string):
    sys.stderr.write(FAIL + string + ENDC + "\n")
    sys.exit(0)

def usage():
    print "Python adding prefix for asn1 v%s" % (version)
    print "Usage: python asn1prefix.py [options]"
    print "Available options:"
    print "-i [file] Input file to parse"
    print "-o [file] Output file for result"
    print "-h        Print this help and return"

in_filename = ""
out_filename = './S1AP-PDU.asn'

FAIL = '\033[91m'
ENDC = '\033[0m'

try:
    opts, args = getopt.getopt(sys.argv[1:], "i:ho:", ["input", "help", "output"])
except getopt.GetoptError as err:
    # print help information and exit:
    usage()
    sys.exit(2)

for o, a in opts:
    if o in ("-i", "--input"):
        in_filename = a
    if o in ("-o", "--output"):
        out_filename = a
    if o in ("-h", "--help"):
        usage()
        sys.exit(2)

if os.path.isfile(in_filename) and os.access(in_filename, os.R_OK):
    in_file = open(in_filename, 'r') 
else:
    printFail("Cannot find file : " + in_filename)

lines = ""

for line in in_file:
    # Removing any comment
    if line.find('--') >= 0:
        line = line[:line.find('--')]
    # Removing any carriage return
    lines += re.sub('\r', '', line)

asnType = '(INTEGER|'
asnType += 'BIT\sSTRING|'
asnType += 'ENUMERATED|'
asnType += 'CHOICE|'
asnType += 'OCTET\sSTRING|'
asnType += 'TBCD-STRING|'
asnType += 'PrintableString|'
asnType += 'SEQUENCE)'

structReplacements = {} 
constantReplacements = {} 

# Extracting S1AP-PDU-Contents
for i in re.findall(r'^([\w-]+)\s*S1AP-PROTOCOL-', lines, re.MULTILINE):
    structReplacements[i] = 'S1ap-' + i
for i in re.findall(r'([\w-]+)\s*::=\s+E-RAB-IE-Container', lines, re.MULTILINE):
    structReplacements[i] = 'S1ap-' + i
# Extracting S1AP-IEs
for i in re.findall(r'([\w-]+)\s*::=\s*' + asnType, lines, re.MULTILINE):
    structReplacements[i[0]] = 'S1ap-' + i[0]

# Removing not to replace structures
del structReplacements['S1AP-PDU']
del structReplacements['S1ap-IE']

# Extracting S1AP-Constant
for i in re.findall(r'([\w-]+)\s*INTEGER\s*::=', lines, re.MULTILINE):
    constantReplacements[i] = 's1ap-' + i

in_file.seek(0, 0)

structPattern = re.compile(r'\b(' + '|'.join(structReplacements.keys()) + r')\b')
constantPattern = re.compile(r'\b(' + '|'.join(constantReplacements.keys()) + r')\b')

out_file = open(out_filename, 'w')

for line in in_file:
    # Extracting S1AP-PDU-Containers
    iesName = ''
    for i in re.findall(r'(\s*protocolIEs\s*ProtocolIE-Container\s*\{\s*\{\s*([\w-]+)\s*\}\s*\})', line):
        iesName = re.sub('[-]*IEs', '', i[1])

    if (iesName != ''):
        line = "    s1ap-" + iesName + "-ies SEQUENCE (SIZE (0..s1ap-maxProtocolIEs)) OF S1ap-IE,\n"
    else:
        line = structPattern.sub(lambda x: structReplacements[x.group()], line)
        line = constantPattern.sub(lambda x: constantReplacements[x.group()], line)
        line = line.replace('id-S1ap', 'id')
        line = line.replace('GW-S1ap-S1ap-TransportLayerAddress', 'GW-TransportLayerAddress')
        line = line.replace('GW-S1ap-TransportLayerAddress', 'GW-TransportLayerAddress')
        line = line.replace('SourceMME-S1ap-S1ap-GUMMEI', 'SourceMME-GUMMEI')
        line = line.replace('SourceMME-S1ap-GUMMEI', 'SourceMME-GUMMEI')
#        line = re.sub(r'OF\sProtocolIE-SingleContainer\s*{\s*{\s*[\w-]+\s*}\s*}', 'OF S1ap-IE', line)
    out_file.write(line)

in_file.close()
out_file.close()
