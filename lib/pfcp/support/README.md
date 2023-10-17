
* Install python-docx
user@host ~/Documents/git/open5gs/lib/pfcp/support$ \
    sudo pip3 install python-docx

* Change the format of standard specification 
  from 29244-h71.doc to 29244-h71.docx
  using Microsoft Office 2007+

* Adjust table cell in 29244-h71.docx

* Copy 29244-h71.docx to 29244-h71-modified.docx

* Add User Plane IP Resource Information to 29244-h71-modified.docx

* Generate TLV support files
user@host ~/Documents/git/open5gs/lib/pfcp/support$ \
    python3 pfcp-tlv.py -f 29244-h71-modified.docx -o ..
