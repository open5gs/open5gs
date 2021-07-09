
* Install python-docx
user@host ~/Documents/git/open5gs/lib/pfcp/support$ \
    sudo pip3 install python-docx

* Change the format of standard specification 
  from 29244-f40.doc to 29274-f40.docx 
  using Microsoft Office 2007+

* Generate TLV support files
user@host ~/Documents/git/open5gs/lib/pfcp/support$ \
    python3 pfcp-tlv.py -f 29244-g10.docx -o ..
