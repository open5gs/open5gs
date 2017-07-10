Prerequisite
============

    sudo apt-get install git cscope gdb
    sudo apt-get install autoconf libtool m4
    sudo apt-get install libsctp-dev
    sudo apt-get install freediameter-dev


Build from Git
==============

    test -f configure || autoreconf -iv
    CFLAGS='-g3' ./configure --prefix=$HOME/Documents/git/nextepc/install
    make -j `nproc`
    sudo ip tuntap add name pgwtun mode tun
    sudo ./scripts/linux_netconfig.sh
    sudo ./test/testnextepc
    ./nextepcd



Install SCTP for Mac OS X 10.11(El Captian)
===========================================
 - Reference : https://github.com/sctplab/SCTP_NKE_ElCapitan
 - Start up in OS X Recovery by holding down Command-R while the Mac is starting up
   Choose Terminal from the Utilities menu.
   Type 'csrutil disable' in Terminal and Reboot
 - Download SCTP_NKE_ElCapitan_Install_01.dmg
   xcode-select --install    
   sudo cp -R /Volumes/SCTP_NKE_ElCapitan_01/SCTPSupport.kext /Library/Extensions
   sudo cp -R /Volumes/SCTP_NKE_ElCapitan_01/SCTP.kext /Library/Extensions
   sudo cp /Volumes/SCTP_NKE_ElCapitan_01/socket.h /usr/include/sys/
   sudo cp /Volumes/SCTP_NKE_ElCapitan_01/sctp.h /usr/include/netinet/
   sudo cp /Volumes/SCTP_NKE_ElCapitan_01/sctp_uio.h /usr/include/netinet/
   sudo cp /Volumes/SCTP_NKE_ElCapitan_01/libsctp.dylib /usr/lib/
   sudo kextload /Library/Extensions/SCTP.kext

Install Tuntap for Mac OS X
===========================================
 - brew tap caskroom/cask
 - brew cask install tuntap
 - Install tuntap_20150118.pkg in /usr/local/Caskroom/tuntap/20150118
 - sudo kextload /Library/Extensions/tun.kext

