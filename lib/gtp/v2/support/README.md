
* Install python-docx
user@host ~/Documents/git/open5gs/lib/gtp/v2/support$ \
    sudo pip3 install python-docx

* Change the format of standard specification
  from 29274-h70.doc to 29274-h70.docx
  using Microsoft Office 2007+

* Adjust table cell in 29274-h70.docx

* Generate TLV support files
user@host ~/Documents/git/open5gs/lib/gtp/v2/support$ \
    python3 gtp-tlv.py -f 29274-h70.docx -o ..
