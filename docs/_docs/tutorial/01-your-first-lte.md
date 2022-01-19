---
title: Your First LTE
head_inline: "<style> .blue { color: blue; } .bold { font-weight: bold; } </style>"
---

This post is the perfect starting point for learning to build your own LTE network. View this lession as a guided introduction -- including the installation, configuration, and best practices that will ease your learning.

### Prerequisites
---

First, you have to prepare USRP B200/B210 to run srsRAN. However, please keep in mind that you would still need a fairly high-end PC (at least dual-core i5, better quad-core i7) with USB 3.0 to attach the USRP B200/B210. 

For USRP B200/B210, you can use a GPS antenna for clock synchronization. Of course, it can work without a GPS antenna, but if you have that antenna, it's a good to have a window near your desk where you can put the small GPS patch antenna. In my case, a 1 to 2 meters antenna cable is used between desk/computer and the window.

This document will be described with the following equipment.
 - i5-8500 PC with Ubuntu 20.04(focal)
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
  1. **Small Antennas** should be connected to USRP Rx/Tx ports (RF-A/RF-B)
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

We will use *Ubuntu 20.04(focal)* installed PC.
{: .blue .bold}

#### 1. USRP Hardware Driver

Most Linux distributions provide UHD as part of their package management. On *Debian and Ubuntu* systems, this will install the base UHD library, all headers and build-specific files, as well as utilities:

```bash
$ sudo add-apt-repository ppa:ettusresearch/uhd
$ sudo apt update
$ sudo apt install libuhd-dev uhd-host
```

After installing, you need to download the FPGA images packages by running _uhd images downloader_ on the command line (the actual path may differ based on your installation):

```bash
$ sudo /usr/lib/uhd/utils/uhd_images_downloader.py
```

The Laptop then needs to reboot, with the USRP unplugged. Only plug in the USRP after booting is done.

```bash
$ cd /usr/share/uhd/images/
$ sudo uhd_usrp_probe
```

If Issues akin to `IOError: Could not find path for image: usrp_b200_fw.hex` come up and you can not start execution from the `/usr/share/uhd/iamges` path, copy `/usr/share/uhd/images/usrp_b200_fw.hex` & `/usr/share/uhd/images/usrp_b210_fpga.bin` or the matching files for your usrp to the execution directory

Furter execution should work even without sudo

#### 2. srsRAN

```bash
$ sudo add-apt-repository ppa:softwareradiosystems/srsran
$ sudo apt-get update
$ sudo apt-get install srsran
```

#### 3. Open5GS

See also [Quickstart Guide: Install Open5GS with a package manager](https://open5gs.org/open5gs/docs/guide/01-quickstart/#2-install-open5gs-with-a-package-manager)

```bash
$ sudo add-apt-repository ppa:open5gs/latest
$ sudo apt update
$ sudo apt install open5gs
```

The following shows how to install the Web UI of Open5GS.

```bash
$ curl -fsSL https://deb.nodesource.com/setup_14.x | sudo -E bash -
$ sudo apt install nodejs
$ curl -fsSL https://open5gs.org/open5gs/assets/webui/install | sudo -E bash -
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

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC.

```diff
$ diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- ./examples/mme.yaml 2022-01-19 10:36:02.462763600 +0100
+++ mme.yaml    2022-01-19 10:37:21.862156800 +0100
@@ -213,14 +213,14 @@
       - addr: 127.0.0.2
     gummei:
       plmn_id:
-        mcc: 901
-        mnc: 70
+        mcc: 999
+        mnc: 99
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 901
-        mnc: 70
+        mcc: 999
+        mnc: 99
       tac: 1
     security:
         integrity_order : [ EIA2, EIA1, EIA0 ]
```

```diff
$ diff -u ./examples/amf.yaml amf.yaml
--- ./examples/amf.yaml 2022-01-19 10:35:48.457438300 +0100
+++ amf.yaml    2022-01-19 10:38:14.523755800 +0100
@@ -183,20 +183,20 @@
       - addr: 127.0.0.5
     guami:
       - plmn_id:
-          mcc: 901
-          mnc: 70
+          mcc: 999
+          mnc: 99
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 901
-          mnc: 70
+          mcc: 999
+          mnc: 99
         tac: 1
     plmn_support:
       - plmn_id:
-          mcc: 901
-          mnc: 70
+          mcc: 999
+          mnc: 99
         s_nssai:
           - sst: 1
     security:
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed.service
$ sudo systemctl restart open5gs-amfd.service
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

### Enable IPv4/IPv6 Forwarding
$ sudo sysctl -w net.ipv4.ip_forward=1
$ sudo sysctl -w net.ipv6.conf.all.forwarding=1

### Add NAT Rule
$ sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
$ sudo ip6tables -t nat -A POSTROUTING -s 2001:db8:cafe::/48 ! -o ogstun -j MASQUERADE
```

**Note:** For the first time, it is a good condition if you do not have any rules in the IP/NAT tables. If a program such as docker has already set up a rule, you will need to add a rule differently.
{: .notice--danger}

#### 2. srsRAN

You should check your phone frequency. If your phone does not support Band-3, you should use a different DL EARFCN value.

```diff
$ diff -u /root/.config/srsran/enb.conf.old /root/.config/srsran/enb.conf
--- enb.conf.example 2022-01-19 09:56:18.317977900 +0100
+++ enb.conf    2022-01-19 10:07:28.943500800 +0100
@@ -20,9 +20,9 @@
 #####################################################################
 [enb]
 enb_id = 0x19B
-mcc = 001
-mnc = 01
-mme_addr = 127.0.1.100
+mcc = 999
+mnc = 99
+mme_addr = 127.0.0.2
 gtp_bind_addr = 127.0.1.1
 s1c_bind_addr = 127.0.1.1
 s1c_bind_port = 0
@@ -67,7 +67,7 @@
 tx_gain = 80
 rx_gain = 40

-#device_name = auto
+device_name = auto

 # For best performance in 2x2 MIMO and >= 15 MHz use the following device_args settings:
 #     USRP B210: num_recv_frames=64,num_send_frames=64
```

If you use the GPS-DO, you should use:
```diff
$ diff -u /root/.config/srsran/enb.conf.old /root/.config/srsran/enb.conf
--- enb.conf.old	2021-08-23 14:32:35.585438813 +0900
+++ enb.conf	2021-08-23 14:32:08.350450409 +0900
 @@ -80,6 +82,7 @@
 # Example for ZMQ-based operation with TCP transport for I/Q samples
 #device_name = zmq
 #device_args = fail_on_disconnect=true,tx_port=tcp://*:2000,rx_port=tcp://localhost:2001,id=enb,base_srate=23.04e6
+device_args = clock=external

 #####################################################################
```

```diff
$ diff -u /root/.config/srsran/rr.conf.old /root/.config/srsran/rr.conf
--- rr.conf.old  2022-01-19 10:01:43.027197000 +0100
+++ rr.conf     2022-01-19 10:01:43.042255800 +0100
@@ -55,10 +55,10 @@
   {
     // rf_port = 0;
     cell_id = 0x01;
-    tac = 0x0007;
+    tac = 0x0001;
     pci = 1;
     // root_seq_idx = 204;
-    dl_earfcn = 3350;
+    dl_earfcn = 1600;
     //ul_earfcn = 21400;
     ho_active = false;
     //meas_gap_period = 0; // 0 (inactive), 40 or 80
```

PLMN ID, DL EARFCN, and Device Argument are updated as belows.

```
PLMN ID : MNC(310), MCC(789) programmed USIM with a card reader
DL EARFCN : Band-3 - from your Phone
Device Argument : Clock source from external GPS-DO
```

Now, run the srsRAN as follows:

```bash
$ sudo srsenb

---  Software Radio Systems LTE eNodeB  ---

Reading configuration file ./enb.conf...
Opening 1 RF devices with 1 RF channels...
[INFO] [UHD] linux; GNU C++ version 7.4.0; Boost_106501; UHD_3.14.1.1-release
[INFO] [LOGGING] Fastpath logging disabled at runtime.
Opening USRP with args: type=b200,master_clock_rate=23.04e6
[INFO] [B200] Detected Device: B200
[INFO] [B200] Operating over USB 3.
[INFO] [B200] Initialize CODEC control...
[INFO] [B200] Initialize Radio control...
[INFO] [B200] Performing register loopback test...
[INFO] [B200] Register loopback test passed
[INFO] [B200] Asking for clock rate 23.040000 MHz...
[INFO] [B200] Actually got clock rate 23.040000 MHz.
Setting frequency: DL=1845.0 Mhz, UL=1750.0 MHz
Setting Sampling frequency 11.52 MHz

==== eNodeB started ===
Type <t> to view trace
```

### Turn on your eNodeB and Phone
---

- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `/var/log/open5gs/*.log`.
