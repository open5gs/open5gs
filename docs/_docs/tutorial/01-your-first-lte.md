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
 - i5-8500 PC with Ubuntu 22.04(jammy)
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
$ sudo apt-get install pcscd pcsc-tools libccid libpcsclite-dev python3-pyscard
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
$ sudo apt-get install --no-install-recommends \
	pcscd libpcsclite-dev \
	python3 \
	python3-setuptools \
	python3-pyscard \
	python3-pip
 pip3 install --user -r requirements.txt
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

#### 2. srsRAN

On *Ubuntu 22.04(jammy)*, one can install the required libraries with:

```bash
$ sudo apt install cmake libfftw3-dev libmbedtls-dev libboost-program-options-dev libconfig++-dev libsctp-dev
```

Download and build srsLTE:

```bash
$ git clone https://github.com/srsRAN/srsRAN.git
$ cd srsRAN
$ git checkout release_22_10
$ git rev-parse HEAD
254cc719a9a31f64ce0262f4ca6ab72b1803477d
$ mkdir build
$ cd build
$ cmake ../
$ make
$ make test
```

#### 3. Open5GS


The Open5GS package is available on the recent versions of *Ubuntu*.
```bash
# Install the MongoDB Packages
$ curl -fsSL https://pgp.mongodb.com/server-6.0.asc | sudo gpg -o /usr/share/keyrings/mongodb-server-6.0.gpg --dearmor
$ echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-6.0.gpg] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/6.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-6.0.list
$ sudo apt update
$ sudo apt install mongodb-org

# Installing Open5GS
$ sudo add-apt-repository ppa:open5gs/latest
$ sudo apt update
$ sudo apt install open5gs
```

The following shows how to install the Web UI of Open5GS.

```bash
# Download and import the Nodesource GPG key
$ sudo apt update
$ sudo apt install -y ca-certificates curl gnupg
$ sudo mkdir -p /etc/apt/keyrings
$ curl -fsSL https://deb.nodesource.com/gpgkey/nodesource-repo.gpg.key | sudo gpg --dearmor -o /etc/apt/keyrings/nodesource.gpg

# Create deb repository
$ NODE_MAJOR=20
$ echo "deb [signed-by=/etc/apt/keyrings/nodesource.gpg] https://deb.nodesource.com/node_$NODE_MAJOR.x nodistro main" | sudo tee /etc/apt/sources.list.d/nodesource.list

# Run Update and Install
$ sudo apt update
$ sudo apt install nodejs -y

# Install the WebUI of Open5GS
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
999700000017408    8988211000000174089    0100    3623    84724035    8774    57473966    B1233463AB9BC2AD2DB1830EB6417E7B    625150E2A943E3353DD23554101CAFD4    47190711    C865CAA0A54542333929B29B116F4375    7D7F65DCD99003C0A0D5D31CA3E5253E    5B27983AF628FC3FCB36B89300012944
```

Here's my subscriber information from above.

```
MCC/MNC : 999/70
IMSI : 999700000017408
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

Connect to `http://localhost:9999` and login with **admin** account.

> Username : admin  
> Password : 1423

Then proceed as follows:

  1. Go to `Subscriber` Menu.
  2. Click `+` Button to add a new subscriber.
  3. Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  4. Click `SAVE` Button

**Note:** Subscribers added with this tool immediately register in the Open5GS HSS/UDR without the need to restart any daemon. However, if you use the WebUI to change subscriber profile, you must restart the Open5GS AMF/MME daemon for the changes to take effect.
{: .notice--warning}

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC.

```diff
$ diff --git a/configs/open5gs/mme.yaml.in b/configs/open5gs/mme.yaml.in
index db2cdaef1..49bbeef76 100644
--- a/configs/open5gs/mme.yaml.in
+++ b/configs/open5gs/mme.yaml.in
@@ -10,7 +10,7 @@ mme:
     freeDiameter: @sysconfdir@/freeDiameter/mme.conf
     s1ap:
       server:
-        - address: 127.0.0.2
+        - address: 127.0.1.2
     gtpc:
       server:
         - address: 127.0.0.2
@@ -25,15 +25,15 @@ mme:
           port: 9090
     gummei:
       plmn_id:
-        mcc: 999
-        mnc: 70
+        mcc: 310
+        mnc: 789
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 999
-        mnc: 70
-      tac: 1
+        mcc: 310
+        mnc: 789
+      tac: 5
     security:
         integrity_order : [ EIA2, EIA1, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed.service
```

In order to bridge between the PGWU/UPF and WAN (Internet), you must enable IP forwarding and add a NAT rule to your IP Tables.

To enable forwarding and add the NAT rule, enter
```bash
### Enable IPv4/IPv6 Forwarding
$ sudo sysctl -w net.ipv4.ip_forward=1
$ sudo sysctl -w net.ipv6.conf.all.forwarding=1

### Add NAT Rule
$ sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
$ sudo ip6tables -t nat -A POSTROUTING -s 2001:db8:cafe::/48 ! -o ogstun -j MASQUERADE
```

Configure the firewall correctly. Some operating systems (Ubuntu) by default enable firewall rules to block traffic.
```bash
$ sudo ufw status
Status: active
$ sudo ufw disable
Firewall stopped and disabled on system startup
$ sudo ufw status
Status: inactive
```

Optionally, you may consider the settings below for security purposes.

```bash
### Ensure that the packets in the `INPUT` chain to the `ogstun` interface are accepted
$ sudo iptables -I INPUT -i ogstun -j ACCEPT

### Prevent UE's from connecting to the host on which UPF is running
$ sudo iptables -I INPUT -s 10.45.0.0/16 -j DROP
$ sudo ip6tables -I INPUT -s 2001:db8:cafe::/48 -j DROP

### If your core network runs over multiple hosts, you probably want to block
### UE originating traffic from accessing other network functions.
### Replace x.x.x.x/y with the VNFs IP/subnet
$ sudo iptables -I FORWARD -s 10.45.0.0/16 -d x.x.x.x/y -j DROP
```

#### 2. srsRAN
Change back to the srsRAN source directory and copy the main config example as well as all additional config files for RR, SIB and DRB.

```bash
$ cp srsenb/enb.conf.example srsenb/enb.conf
$ cp srsenb/rr.conf.example srsenb/rr.conf
$ cp srsenb/rb.conf.example srsenb/rb.conf
$ cp srsenb/sib.conf.example srsenb/sib.conf
```

You should check your phone frequency. If your phone does not support Band-3, you should use a different DL EARFCN value.

```diff
$ diff -u enb.conf.example enb.conf
--- enb.conf.example	2022-12-11 10:04:37.519188021 +0900
+++ enb.conf	2022-12-11 10:45:13.746995146 +0900
@@ -20,9 +20,9 @@
 #####################################################################
 [enb]
 enb_id = 0x19B
-mcc = 001
-mnc = 01
-mme_addr = 127.0.1.100
+mcc = 310
+mnc = 789
+mme_addr = 127.0.1.2
 gtp_bind_addr = 127.0.1.1
 s1c_bind_addr = 127.0.1.1
 s1c_bind_port = 0
```

```diff
$ diff -u rr.conf.example rr.conf
--- rr.conf.example	2022-12-11 10:04:37.523187831 +0900
+++ rr.conf	2022-12-11 10:42:23.590401941 +0900
@@ -55,10 +55,10 @@
   {
     // rf_port = 0;
     cell_id = 0x01;
-    tac = 0x0007;
+    tac = 0x0005;
     pci = 1;
     // root_seq_idx = 204;
-    dl_earfcn = 3350;
+    dl_earfcn = 1600;
     //ul_earfcn = 21400;
     ho_active = false;
     //meas_gap_period = 0; // 0 (inactive), 40 or 80
@@ -114,4 +114,4 @@
 nr_cell_list =
 (
   // no NR cells
-);
\ No newline at end of file
+);
```

MME Address, TAC, PLMN ID, DL EARFCN, and Device Argument are updated as belows.

```
MME Address : 127.0.1.2
TAC : 5
PLMN ID : MNC(310), MCC(789) programmed USIM with a card reader
DL EARFCN : Band-3 - from your Phone
Device Argument : Clock source from external GPS-DO
```

If you are using GPS-DO, you need to add `device_args` as shown below.
```diff
$ diff -u enb.conf.example enb.conf
--- enb.conf.example	2022-12-11 10:04:37.519188021 +0900
+++ enb.conf	2022-12-11 10:44:25.565094551 +0900
@@ -20,9 +20,9 @@
 #####################################################################
 [enb]
 enb_id = 0x19B
-mcc = 001
-mnc = 01
-mme_addr = 127.0.1.100
+mcc = 310
+mnc = 789
+mme_addr = 127.0.1.2
 gtp_bind_addr = 127.0.1.1
 s1c_bind_addr = 127.0.1.1
 s1c_bind_port = 0
@@ -82,6 +82,7 @@
 # Example for ZMQ-based operation with TCP transport for I/Q samples
 #device_name = zmq
 #device_args = fail_on_disconnect=true,tx_port=tcp://*:2000,rx_port=tcp://localhost:2001,id=enb,base_srate=23.04e6
+device_args = clock=external

 #####################################################################
 # Packet capture configuration
```

Now, run the srsRAN as follows:

```bash
$ cd srsenb/
$ sudo ../build/srsenb/src/srsenb ./enb.conf
[sudo] password for acetcom:
Active RF plugins: libsrsran_rf_uhd.so
Inactive RF plugins:
---  Software Radio Systems LTE eNodeB  ---

Reading configuration file ./enb.conf...
WARNING: cpu0 scaling governor is not set to performance mode. Realtime processing could be compromised. Consider setting it to performance mode before running the application.

Built in Release mode using commit 254cc719a on branch HEAD.

Opening 1 channels in RF device=default with args=default
Supported RF device list: UHD file
Trying to open RF device 'UHD'
[INFO] [UHD] linux; GNU C++ version 11.2.0; Boost_107400; UHD_4.3.0.0-0ubuntu1~jammy1
[INFO] [LOGGING] Fastpath logging disabled at runtime.
[INFO] [B200] Loading firmware image: /usr/share/uhd/images/usrp_b200_fw.hex...
Opening USRP channels=1, args: type=b200,master_clock_rate=23.04e6
[INFO] [UHD RF] RF UHD Generic instance constructed
[INFO] [B200] Detected Device: B200
[INFO] [B200] Loading FPGA image: /usr/share/uhd/images/usrp_b200_fpga.bin...
[INFO] [B200] Operating over USB 3.
[INFO] [B200] Detecting internal GPSDO....
[INFO] [GPS] No GPSDO found
[INFO] [B200] Initialize CODEC control...
[INFO] [B200] Initialize Radio control...
[INFO] [B200] Performing register loopback test...
[INFO] [B200] Register loopback test passed
[INFO] [B200] Asking for clock rate 23.040000 MHz...
[INFO] [B200] Actually got clock rate 23.040000 MHz.
RF device 'UHD' successfully opened

==== eNodeB started ===
Type <t> to view trace
Setting frequency: DL=1845.0 Mhz, UL=1750.0 MHz for cc_idx=0 nof_prb=50
```

### Turn on your eNodeB and Phone
---

- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `/var/log/open5gs/*.log`.
