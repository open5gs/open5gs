
* Install python-docx
user@host ~/Documents/git/open5gs/lib/nas/eps/support$ \
    sudo pip3 install python-docx

* Change the format of standard specification 
  from 24301-d80.doc to 24301-d80.docx 
  using Microsoft Office 2007+

* Generate Message support files
user@host ~/Documents/git/open5gs/lib/nas/eps/support$ \
    python3 nas-message.py -f 24301-g40.docx -o ..
