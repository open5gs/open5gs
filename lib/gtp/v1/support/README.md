
* Install python-docx
user@host ~/Documents/git/open5gs/lib/gtp/v1/support$ \
    sudo pip3 install python-docx

* Change the format of standard specification
  from 29060-g00.doc to 29060-g00.docx
  using Microsoft Office 2007+

* Generate TLV support files
user@host ~/Documents/git/open5gs/lib/gtp/v1/support$ \
    python3 gtp1-tlv.py -f 29060-g00.docx -o ..
