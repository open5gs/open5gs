
* Install python-docx
user@host ~/Documents/git/open5gs/lib/gtp/v1/support$ \
    sudo pip3 install python-docx

* Change the format of standard specification
  from 29060-h40.doc to 29060-h40.docx
  using Microsoft Office 2007+

* Adjust table cell in 29060-h40.docx

* Generate TLV support files
user@host ~/Documents/git/open5gs/lib/gtp/v1/support$ \
    python3 gtp1-tlv.py -f 29060-h40.docx -o ..
