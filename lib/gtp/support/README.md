
* Install python-pip
user@host ~/Documents/git/open5gs/lib/gtp/support$ \
    sudo apt-get install python-pip

* Install python-docx
user@host ~/Documents/git/open5gs/lib/gtp/support$ \
    sudo pip install python-docx

* Change the format of standard specification 
  from 29274-g30.doc to 29274-g30.docx
  using Microsoft Office 2007+

* Generate TLV support files
user@host ~/Documents/git/open5gs/lib/s1ap/support$ \
    python gtp-tlv.py -f 29274-g30.docx -o ..
