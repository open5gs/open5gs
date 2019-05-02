---
title: Your First LTE
head_inline: "<style> .blue { color: blue; } .bold { font-weight: bold; } </style>"
---

This post is the perfect starting point for learning to build your own LTE network. View this lession as a guided introduction -- including the installation, configuration, and best practices that will ease your learning.

### Prerequisites
---

First, you have to prepare USRP B200/B210 to run srsENB. However, please keep in mind that you would still need a fairly high-end PC (at least dual-core i5, better quad-core i7) with USB 3.0 to attach the USRP B200/B210. 

Also, for USRP B200/B210 you will need a GPS antenna for clock synchronization. It is good to have a window near your desk where you can put the small GPS patch antenna. In my case, a 1 to 2 meters antenna cable is used between desk/computer and the window.

For stable operation of USRP B200/B210, I used 10Mhz GPS-DO(GPS disciplined oscillator). Of course, a USIM card(sysmoUSIM-SJS1) was also inserted into the phone.

### Overall Physical Setup
---

Setup your devices in the following order:

  1. GPS antenna near window
  2. GPS antenna connected to "GPS ANT" connector of GPS-DO (SMA)
  3. 10MHz output (BNC) of GPS-DO connected to 10MHz input of USRP (SMA)
  4. GPS input of USRP open/unused!
  5. 1PPS input of USRP open/unused!
  6. 4x Small Antennas connected to USRP Rx/Tx ports (RF-A/RF-B)
  7. USRP powered via power supply or over USB
  8. USRP USB port connected to your PC 
  9. GPS-DO powered via power supply

**Note:** When the GPS-DO acquires a lock on the GPS signal, a **GREEN** LED is displayed. GPS takes time to function normally. You also need to wait for the **RED** LED(ALARM) to turn off.
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

#### 3. NextEPC

The NextEPC package is available on the recent versions of *Ubuntu*.

```bash
# Getting the authentication key
$ sudo apt install wget
$ wget https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/Release.key
$ sudo apt-key add Release.key

# Installing NextEPC
$ sudo sh -c "echo 'deb https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/ ./' > /etc/apt/sources.list.d/open5gs.list"
$ sudo apt update
$ sudo apt install nextepc
```

The following shows how to install the Web UI of NextEPC.

```bash
$ curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
$ sudo apt install nodejs
$ curl -sL http://nextepc.org/static/webui/install | sudo -E bash -
```

### Configuration & Running
---

#### 1. NextEPC

When you purchase the sysmoUSIM, you will receive the following information via e-mail.

```
Title : sysmocom SIM Card Details / AM93\PICK\00859

IMSI    ICCID   ACC PIN1    PUK1    PIN2    PUK2    Ki  OPC ADM1    KIC1    KID1    KIK1
...
901700000017408	8988211000000174089	0100	3623	84724035	8774	57473966	B1233463AB9BC2AD2DB1830EB6417E7B	625150E2A943E3353DD23554101CAFD4	47190711	C865CAA0A54542333929B29B116F4375	7D7F65DCD99003C0A0D5D31CA3E5253E	5B27983AF628FC3FCB36B89300012944
```

Here's my subscriber information from above.

```
IMSI : 901700000017408
K : B1233463AB9BC2AD2DB1830EB6417E7B
OPc : 625150E2A943E3353DD23554101CAFD4  
```

Connect to `http://localhost:3000` and login with **admin** account.

> Username : admin  
> Password : 1423

Then proceed as follows:

  1. Go to `Subscriber` Menu.
  2. Click `+` Button to add a new subscriber.
  3. Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  4. Click `SAVE` Button

Modify [/etc/nextepc/mme.conf](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/config/mme.conf.in) to set the S1AP/GTP-C IP address, PLMN ID, and TAC

```diff
diff -u mme.conf.old mme.conf
--- mme.conf.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.conf	2018-04-15 19:53:10.000000000 +0900
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
+        mcc: 901
+        mnc: 70
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 001
-        mnc: 01
-      tac: 12345
+        mcc: 901
+        mnc: 70
+      tac: 7
     security:
         integrity_order : [ EIA1, EIA2, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]
```

S1AP/GTP-C IP address, PLMN ID, TAC are changed as follows.

```
S1AP address : 127.0.1.100 - srsENB default value
GTP-C address : 127.0.1.100 - Use loopback interface
PLMN ID : MNC(901), MCC(70) - sysmoUSIM default value
TAC : 7 - srsENB default value
```


The GTP-U IP address will be set to 127.0.0.2. To do this, modify [/etc/nextepc/sgw.conf](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/config/sgw.conf.in) to set the GTP-U IP address.  

```diff
diff -u /etc/nextepc/sgw.conf.old /etc/nextepc/sgw.conf
--- sgw.conf.old	2018-04-15 18:30:25.000000000 +0900
+++ sgw.conf	2018-04-15 18:30:30.000000000 +0900
@@ -14,3 +14,4 @@
     gtpc:
       addr: 127.0.0.2
     gtpu:
+      addr: 127.0.0.2
```

After changing conf files, please restart NextEPC daemons.

```bash
$ sudo systemctl restart nextepc-mmed
$ sudo systemctl restart nextepc-sgwd
```

If your phone can connect to internet, you must run the following command in NextEPC-PGW installed host. 

```bash
$ sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
$ sudo iptables -t nat -A POSTROUTING -o 'interface-name' -j MASQUERADE
$ sudo iptables -I INPUT -i pgwtun -j ACCEPT
```

**Note:** In the above command, you should replace `'interface-name'` with your interface name that can connect to the internet. (For example, `enp0s25`, `wls3`, and so on).
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
+mcc = 901
+mnc = 70
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
PLMN ID : MNC(901), MCC(70) sysmoUSIM default value
DL EARFCN : Band-3 - from your Phone
Device Argument : Clock source from external GPS-DO
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
-- Operating over USB 3.
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
- You can view the log at `/var/log/nextepc/*.log`.
