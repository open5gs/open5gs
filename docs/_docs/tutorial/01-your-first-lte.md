---
title: Your First LTE
head_inline: "<style> .blue { color: blue; } .bold { font-weight: bold; } </style>"
---

This post is the perfect starting point for learning to build your own LTE network. View this lession as a guided introduction -- including the installation, configuration, and best practices that will ease your learning.

### Prerequisites
---

First, you have to prepare USRP B200/B210 to run srsENB. However, please keep in mind that you would still need a fairly high-end PC (at least dual-core i5, better quad-core i7) with USB 3.0 to attach the USRP B200/B210. 

For USRP B200/B210, you can use a GPS antenna for clock synchronization. Of course, it can work without a GPS antenna, but if you have that antenna, it's a good to have a window near your desk where you can put the small GPS patch antenna. In my case, a 1 to 2 meters antenna cable is used between desk/computer and the window.

This document will be described with the following equipment.
 - i5-8500 PC with Ubuntu 18.04(bionic)
 - USRP B200/B210 with USB 3.0
 - iPhone XS
 - sysmoUSIM-SJS1
 - 10Mhz GPS-DO(Optional)

### Overall Physical Setup
---

If you want to use GPS antenna, setup your devices in the following order:

  1. GPS antenna near window
  2. GPS antenna connected to "GPS ANT" connector of GPS-DO (SMA)
  3. 10MHz output (BNC) of GPS-DO connected to 10MHz input of USRP (SMA)
  4. GPS input of USRP open/unused!
  5. 1PPS input of USRP open/unused!
  6. GPS-DO powered via power supply

**Note:** When the GPS-DO acquires a lock on the GPS signal, a **GREEN** LED is displayed. GPS takes time to function normally. You also need to wait for the **RED** LED(ALARM) to turn off.
{: .notice--warning}

Then, setup the USRP B200/B210 as below:
  1. **4x Small Antennas** should be connected to USRP Rx/Tx ports (RF-A/RF-B)
  2. USRP powered via power supply or over **USB 3.0**
  3. USRP **USB 3.0** port connected to your PC 

### USIM Setup
---

Bascially, you can learn how to use it in the [sysmoUSIM manual](https://www.sysmocom.de/manuals/sysmousim-manual.pdf) or on the [official homepage of pysim project](http://osmocom.org/projects/pysim/wiki). Let's take a quickstart guide for this experiment.

###### Install dependencies:
```
$ sudo apt-get install pcscd pcsc-tools libccid libpcsclite-dev python-pyscard
```
 - Connect SIM card reader to your computer and insert programmable SIM card to the reader. 

###### Check the status of connection by entering the following command:
```
$ pcsc_scan
PC/SC device scanner
V 1.5.2 (c) 2001-2017, Ludovic Rousseau <ludovic.rousseau@free.fr>
Using reader plug'n play mechanism
Scanning present readers...
0: HID Global OMNIKEY 3x21 Smart Card Reader [OMNIKEY 3x21 Smart Card Reader] 00

Sun May 26 14:26:12 2019
 Reader 0: HID Global OMNIKEY 3x21 Smart Card Reader [OMNIKEY 3x21 Smart Card Re
  Card state: Card inserted,
  ATR: 3B 9F 96 80 1F C7 80 31 A0 73 BE 21 13 67 43 20 07 18 00 00 01 A5
...
```
 - If SIM card reader is recognized then we can expect to print "Card inserted".

###### Get the code of PySIM with installing dependency:

```
$ sudo apt-get install python-pyscard python-serial python-pip
$ pip install pytlv
$ git clone git://git.osmocom.org/pysim
```

###### Read your SIM card:
```
$ ./pySim-read.py -p0 or ./pySim-read.py -p1
Using PC/SC reader (dev=0) interface
Reading ...
ICCID: 8988211000000213010
IMSI: 310789012345301
SMSP: ffffffffffffffffffffffffffffffffffffffffffffffffe1ffffffffffffffffffffffff
...
```

###### Program your SIM card like the followings:
```
./pySim-prog.py -p 0 -n Open5GS -a 62416296 -s 8988211000000213010 -i 310789012345301 -x 310 -y 789 -k 82E9053A1882085FF2C020359938DAE9 -o BFD5771AAF4F6728E9BC6EF2C2533BDB
Using PC/SC reader (dev=0) interface
Insert card now (or CTRL-C to cancel)
Autodetected card type: sysmoUSIM-SJS1
Generated card parameters :
 > Name    : Open5GS
 > SMSP    : e1ffffffffffffffffffffffff0581005155f5ffffffffffff000000
 > ICCID   : 8988211000000213010
 > MCC/MNC : 310/789
 > IMSI    : 310789012345301
 > Ki      : 82E9053A1882085FF2C020359938DAE9
 > OPC     : BFD5771AAF4F6728E9BC6EF2C2533BDB
 > ACC     : None

Programming ...
Done !
```

**Note:** You should use your ADM value to program USIM card, not my ADM(-a 62416296).
{: .notice--warning}

### Installation
---

We will use *Ubuntu 18.04(Bionic)* installed PC.
{: .blue .bold}

#### 1. USRP Hardware Driver

Most Linux distributions provide UHD as part of their package management. On *Debian and Ubuntu* systems, this will install the base UHD library, all headers and build-specific files, as well as utilities:

```bash
$ sudo add-apt-repository ppa:ettusresearch/uhd
$ sudo apt update
$ sudo apt install libuhd-dev libuhd003 uhd-host
```

After installing, you need to download the FPGA images packages by running _uhd images downloader_ on the command line (the actual path may differ based on your installation):

```bash
$ sudo /usr/lib/uhd/utils/uhd_images_downloader.py
```

#### 2. srsENB

On *Ubuntu 18.04(Bionic)*, one can install the required libraries with:

```bash
$ sudo apt install cmake libfftw3-dev libmbedtls-dev libboost-program-options-dev libconfig++-dev libsctp-dev
```

Download and build srsLTE:

```bash
➜  git git clone https://github.com/srsLTE/srsLTE.git
➜  git cd srsLTE
➜  srsLTE git:(master) ✗ mkdir build
➜  srsLTE git:(master) ✗ cd build
➜  build git:(master) ✗ cmake ../
➜  build git:(master) ✗ make
➜  build git:(master) ✗ make test
```

#### 3. Open5GS

The Open5GS package is available on the recent versions of *Ubuntu*.

```bash
# Getting the authentication key
$ sudo apt install wget
$ wget https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/Release.key
$ sudo apt-key add Release.key

# Installing Open5GS
$ sudo sh -c "echo 'deb https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/ ./' > /etc/apt/sources.list.d/open5gs.list"
$ sudo apt update
$ sudo apt install open5gs
```

The following shows how to install the Web UI of Open5GS.

```bash
$ curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
$ sudo apt install nodejs
$ curl -sL http://open5gs.org/static/webui/install | sudo -E bash -
```

### Configuration & Running
---

#### 1. Open5GS

When you purchase the sysmoUSIM, you will receive the following information via e-mail.

```
Title : sysmocom SIM Card Details / AM93\PICK\00859

IMSI    ICCID   ACC PIN1    PUK1    PIN2    PUK2    Ki  OPC ADM1    KIC1    KID1    KIK1
...
901700000017408	8988211000000174089	0100	3623	84724035	8774	57473966	B1233463AB9BC2AD2DB1830EB6417E7B	625150E2A943E3353DD23554101CAFD4	47190711	C865CAA0A54542333929B29B116F4375	7D7F65DCD99003C0A0D5D31CA3E5253E	5B27983AF628FC3FCB36B89300012944
```

Here's my subscriber information from above.

```
MCC/MNC : 901/70
IMSI : 901700000017408
K : B1233463AB9BC2AD2DB1830EB6417E7B
OPc : 625150E2A943E3353DD23554101CAFD4  
```

If you programmed USIM using a card reader like me, you should use your SIM information.
```
MCC/MNC : 310/789
IMSI : 310789012345301
K : 82E9053A1882085FF2C020359938DAE9
OPc : BFD5771AAF4F6728E9BC6EF2C2533BDB  
```

Connect to `http://localhost:3000` and login with **admin** account.

> Username : admin  
> Password : 1423

Then proceed as follows:

  1. Go to `Subscriber` Menu.
  2. Click `+` Button to add a new subscriber.
  3. Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  4. Click `SAVE` Button

Modify [/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/mme.yaml.in) to set the S1AP/GTP-C IP address, PLMN ID, and TAC

```diff
diff -u mme.yaml.old mme.yaml
--- mme.yaml.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.yaml	2018-04-15 19:53:10.000000000 +0900
@@ -14,18 +14,20 @@
 mme:
     freeDiameter: mme.conf
     s1ap:
+      addr: 127.0.1.100
     gtpc:
+      addr: 127.0.1.100
     gummei:
       plmn_id:
-        mcc: 001
-        mnc: 01
+        mcc: 310
+        mnc: 789
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 001
-        mnc: 01
-      tac: 12345
+        mcc: 310
+        mnc: 789
+      tac: 7
     security:
         integrity_order : [ EIA1, EIA2, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]
```

S1AP/GTP-C IP address, PLMN ID, TAC are changed as follows.

```
S1AP address : 127.0.1.100 - srsENB default value
GTP-C address : 127.0.1.100 - Use loopback interface
PLMN ID : MNC(310), MCC(789) - Programmed USIM with a card reader
TAC : 7 - srsENB default value
```


The GTP-U IP address will be set to 127.0.0.2. To do this, modify [/etc/open5gs/sgw.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgw.yaml.in) to set the GTP-U IP address.  

```diff
diff -u /etc/open5gs/sgw.yaml.old /etc/open5gs/sgw.yaml
--- sgw.yaml.old	2018-04-15 18:30:25.000000000 +0900
+++ sgw.yaml	2018-04-15 18:30:30.000000000 +0900
@@ -14,3 +14,4 @@
     gtpc:
       addr: 127.0.0.2
     gtpu:
+      addr: 127.0.0.2
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed
$ sudo systemctl restart open5gs-sgwd
```

If your phone can connect to internet, you must run the following command in Open5GS-PGW installed host. 

```bash
### Check IP Tables
$ sudo iptables -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

### Check NAT Tables
$ sudo iptables -L -t nat
Chain PREROUTING (policy ACCEPT)
target     prot opt source               destination

Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

Chain POSTROUTING (policy ACCEPT)
target     prot opt source               destination

### Enable IPv4 Forwarding
$ sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"

### Add NAT Rule
$ sudo iptables -t nat -A POSTROUTING -s 45.45.0.0/16 ! -o ogstun -j MASQUERADE
```

**Note:** For the first time, it is a good condition if you do not have any rules in the IP/NAT tables. If a program such as docker has already set up a rule, you will need to add a rule differently.
{: .notice--danger}

#### 2. srsENB
Change back to the srsENB source directory and copy the main config example as well as all additional config files for RR, SIB and DRB.

```bash
➜  srsLTE git:(master) ✗ cp srsenb/enb.conf.example srsenb/enb.conf
➜  srsLTE git:(master) ✗ cp srsenb/rr.conf.example srsenb/rr.conf
➜  srsLTE git:(master) ✗ cp srsenb/sib.conf.example srsenb/sib.conf
➜  srsLTE git:(master) ✗ cp srsenb/drb.conf.example srsenb/drb.conf
```

You should check your phone frequency. If your phone does not support Band-3, you should use a different DL EARFCN value.

```diff
--- enb.conf.example	2018-11-19 18:16:06.953631893 +0900
+++ enb.conf	2019-04-08 11:15:18.051261318 +0900
@@ -23,8 +23,8 @@
 cell_id = 0x01
 phy_cell_id = 1
 tac = 0x0007
-mcc = 001
-mnc = 01
+mcc = 310
+mnc = 789
 mme_addr = 127.0.1.100
 gtp_bind_addr = 127.0.1.1
 s1c_bind_addr = 127.0.1.1
@@ -66,12 +66,13 @@
 #                     Default "auto". B210 USRP: 400 us, bladeRF: 0 us.
 #####################################################################
 [rf]
-dl_earfcn = 3400
+dl_earfcn = 1600
 tx_gain = 80
 rx_gain = 40

 #device_name = auto
 #device_args = auto
+device_args="clock=external"
 #time_adv_nsamples = auto
 #burst_preamble_us = auto
```

PLMN ID, DL EARFCN, and Device Argument are updated as belows.

```
PLMN ID : MNC(310), MCC(789) programmed USIM with a card reader
DL EARFCN : Band-3 - from your Phone
Device Argument : Clock source from external GPS-DO
```

If you do not use the GPS-DO, you should use:
```diff
 #device_name = auto
-#device_args = auto
+device_args = auto
 #time_adv_nsamples = auto
 #burst_preamble_us = auto
```

Now, run the srsENB as follows:

```bash
➜  srsLTE git:(master) ✗ cd srsenb/
➜  srsenb git:(master) ✗ sudo ../build/srsenb/src/srsenb ./enb.conf
linux; GNU C++ version 6.2.0 20161027; Boost_106200; UHD_003.009.005-0-unknow

---  Software Radio Systems LTE eNodeB  ---

Reading configuration file ./enb.conf...
-- Loading firmware image: /usr/share/uhd/images/usrp_b200_fw.hex...
Opening USRP with args: "",master_clock_rate=30.72e6
-- Detected Device: B200
-- Loading FPGA image: /usr/share/uhd/images/usrp_b200_fpga.bin... done
-- Operating over 'USB 2'.
-- Detecting internal GPSDO.... 'No GPSDO found'
-- Initialize CODEC control...
-- Initialize Radio control...
-- Performing register loopback test... pass
-- Performing CODEC loopback test... pass
-- Asking for clock rate 30.720000 MHz...
-- Actually got clock rate 30.720000 MHz.
-- Performing timer loopback test... pass
Setting frequency: DL=1845.0 Mhz, UL=1750.0 MHz
Setting Sampling frequency 11.52 MHz

==== eNodeB started ===
Type <t> to view trace
```
If you see the `No GPSDO found`, please exit the program with Ctrl-C.
And also, if you see the `USB 2`, it will not be working properly.

The following console output is the correct result of srsENB.
```bash
linux; GNU C++ version 6.2.0 20161027; Boost_106200; UHD_003.009.005-0-unknow

---  Software Radio Systems LTE eNodeB  ---

Reading configuration file ./enb.conf...
Opening USRP with args: "",master_clock_rate=30.72e6
-- Detected Device: B200
-- Operating over USB 3.
-- Initialize CODEC control...
-- Initialize Radio control...
-- Performing register loopback test... pass
-- Performing CODEC loopback test... pass
-- Asking for clock rate 30.720000 MHz...
-- Actually got clock rate 30.720000 MHz.
-- Performing timer loopback test... pass
Setting frequency: DL=1845.0 Mhz, UL=1750.0 MHz
Setting Sampling frequency 11.52 MHz

==== eNodeB started ===
Type <t> to view trace
```

### Turn on your eNodeB and Phone
---

- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `/var/log/open5gs/*.log`.
