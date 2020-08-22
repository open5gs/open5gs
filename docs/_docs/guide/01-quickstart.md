---
title: Quickstart
---

**Note:** Open5GS supports installation of packages in *Debian/Ubuntu and openSUSE* environments. *CentOS, Fedora, FreeBSD, and Mac OSX* require you to [build with source code]({{ site.url }}{{ site.baseurl }}/docs/guide/02-building-open5gs-from-sources)
{: .notice--warning}

### Install Open5GS with a Package Manager
---

#### Ubuntu

*Ubuntu* makes it easy to install Open5GS as shown below,

```bash
$ sudo apt update
$ sudo apt install software-properties-common
$ sudo add-apt-repository ppa:open5gs/latest
$ sudo apt update
$ sudo apt install open5gs
```

#### Debian

The Open5GS packages for Debian are available on [OBS](https://build.opensuse.org/project/show/home:acetcom:open5gs). If you want to use the latest Debian version like *Debian* 10(Buster), you need to install MongoDB first.


```bash
$ sudo apt update
$ sudo apt install wget gnupg
$ wget -qO - https://www.mongodb.org/static/pgp/server-4.2.asc | sudo apt-key add -
$ echo "deb http://repo.mongodb.org/apt/debian buster/mongodb-org/4.2 main" | sudo tee /etc/apt/sources.list.d/mongodb-org.list
$ wget -qO - https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_10/Release.key | sudo apt-key add -
$ sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_10/ ./' > /etc/apt/sources.list.d/open5gs.list"
$ sudo apt update
$ sudo apt install mongodb-org
$ sudo apt install open5gs
```

Other distributions can be installed by changing the path.

```
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_10/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_Testing/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_Unstable/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Raspbian_10/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_19.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_19.10/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_20.04/
```

Nightly package build are provided by [Osmocom](https://osmocom.org) on [OBS](https://build.opensuse.org/package/show/network:osmocom:nightly/open5gs).

```
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Debian_10/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Debian_Testing/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Debian_Unstable/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Raspbian_10/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_18.04/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_18.10/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_19.04/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_19.10/
```

#### openSUSE

[Martin Hauke](https://build.opensuse.org/user/show/mnhauke) packaged Open5GS for *openSUSE* on [OBS](https://build.opensuse.org/package/show/home:mnhauke:open5gs/open5gs).

```bash
$ sudo zypper addrepo -f obs://home:mnhauke:open5gs home:mnhauke:open5gs
$ sudo zypper install mongodb-server mongodb-shell
$ sudo zypper install open5gs
```

### Configure Open5GS
---

From now on, we'll use MCC-MNC of 901-70 and set the IP address as below.

_Cloud Authentication and Session Management Server_

```
MongoDB   = 127.0.0.1 (subscriber data) - http://localhost:3000
MME-s1ap  = 10.10.0.2 :36412 (authVPN) for S1-MME
MME-gtpc  = 127.0.0.2 :any for S11
MME-frDi  = 127.0.0.2 :3868 for S6a auth
SGWC-gtpc = 127.0.0.3 :2123 for S11
SGWC-pfcp = 10.10.0.3 :8805 (authVPN) for Sxa
SMF-gtpc  = 127.0.0.4 :2123 for S5/8c, N11
SMF-pfcp  = 10.10.0.4 :8805 (authVPN) for N4
SMF-frDi  = 127.0.0.4 :3868 for Gx auth
SMF-sbi   = 127.0.0.4 :7777 for 5G SBI (N7,N10,N11)
AMF-ngap  = 10.10.0.5 :36412 (authVPN) for N2
AMF-sbi   = 127.0.0.5 :7777 for 5G SBI (N8,N12,N11)
HSS-frDi  = 127.0.0.8 :3868 for S6a auth
PCRF-frDi = 127.0.0.9 :3868 for Gx auth
NRF-sbi   = 127.0.0.10:7777 for 5G SBI
AUSF-sbi  = 127.0.0.11:7777 for 5G SBI
UDM-sbi   = 127.0.0.12:7777 for 5G SBI
UDR-sbi   = 127.0.0.13:7777 for 5G SBI
```

_Cloud Userplane Server (to host IMS etc)_

```
SGWU-pfcp = 10.10.0.6 :8805 (authVPN) for Sxa
SGWU-gtpu = 10.11.0.6 :2152 (userplaneVPN) for S1-U, S5/8u
UPF-pfcp  = 10.10.0.7 :8805 (authVPN) for N4
UPF-gtpu  = 10.11.0.7 :2152 (userplaneVPN) for S5/8u, N3
```

You can refer to the network settings at [https://open5gs.org/open5gs/assets/Open5GS-Diagram.pdf](https://open5gs.org/open5gs/assets/Open5GS-Diagram.pdf) provided by [@kbarlee](https://github.com/kbarlee) in issue [#528](https://github.com/open5gs/open5gs/issues/528)
{: .notice--danger}

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC. 

```diff
$ diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml.old	2020-08-22 11:36:40.512418765 -0400
+++ mme.yaml	2020-08-22 11:36:27.081466682 -0400
@@ -204,20 +204,20 @@
 mme:
     freeDiameter: /home/acetcom/Documents/git/open5gs/install/etc/freeDiameter/mme.conf
     s1ap:
-      addr: 127.0.0.2
+      addr: 10.10.0.2
     gtpc:
       addr: 127.0.0.2
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
-      tac: 7
+        mcc: 901
+        mnc: 70
+      tac: 1
     security:
         integrity_order : [ EIA1, EIA2, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]
```

Modify [install/etc/open5gs/sgwc.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgwc.yaml.in) to set the PFCP IP address.  
```diff
$ diff -u /etc/open5gs/sgwc.yaml.old /etc/open5gs/sgwc.yaml
@@ -49,7 +49,7 @@
     gtpc:
       addr: 127.0.0.3
     pfcp:
-      addr: 127.0.0.3
+      addr: 10.10.0.3

 #
 # sgwu:
@@ -100,7 +100,7 @@
 #
 sgwu:
     pfcp:
-      addr: 127.0.0.6
+      addr: 10.10.0.6

 #
 # parameter:
```

Modify [install/etc/open5gs/smf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/smf.yaml.in) to set the PFCP IP address.  
```diff
$ diff -u /etc/open5gs/smf.yaml.old /etc/open5gs/smf.yaml
--- smf.yaml.old	2020-08-22 11:37:39.990816411 -0400
+++ smf.yaml	2020-08-22 11:38:18.647999952 -0400
@@ -187,8 +187,7 @@
       - addr: 127.0.0.4
       - addr: ::1
     pfcp:
-      - addr: 127.0.0.4
-      - addr: ::1
+      - addr: 10.10.0.4
     pdn:
       - addr: 10.45.0.1/16
       - addr: cafe::1/64
@@ -282,7 +281,7 @@
 #
 upf:
     pfcp:
-      - addr: 127.0.0.7
+      - addr: 10.10.0.7

 #
 # parameter:

```

Modify [install/etc/open5gs/amf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/amf.yaml.in) to set the NGAP IP address, PLMN ID, TAC and NSSAI. 

```diff
diff -u /etc/open5gs/amf.yaml.old /etc/open5gs/amf.yaml
--- amf.yaml.old	2020-06-21 23:34:14.643114779 -0400
+++ amf.yaml	2020-06-21 23:34:28.718482095 -0400
@@ -67,25 +67,25 @@
       - addr: 127.0.0.5
         port: 7777
     ngap:
-      - addr: 127.0.0.5
+      - addr: 10.10.0.5
     guami:
       - plmn_id:
-          mcc: 001
-          mnc: 01
+          mcc: 901
+          mnc: 70
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 001
-          mnc: 01
-        tac: 7
+          mcc: 901
+          mnc: 70
+        tac: 1
     plmn:
       - plmn_id:
-          mcc: 001
-          mnc: 01
+          mcc: 901
+          mnc: 70
         s_nssai:
           - sst: 1
-          - sd: 2
     security:
         integrity_order : [ NIA1, NIA2, NIA0 ]
         ciphering_order : [ NEA0, NEA1, NEA2 ]
```

Modify [install/etc/open5gs/sgwu.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgwu.yaml.in) to set the GTP-U and PFCP IP address.  
```diff
$ diff -u /etc/open5gs/sgwu.yaml.old /etc/open5gs/sgwu.yaml
--- sgwu.yaml.old	2020-08-22 11:41:09.214670723 -0400
+++ sgwu.yaml	2020-08-22 11:41:27.433937124 -0400
@@ -51,9 +51,9 @@
 #
 sgwu:
     gtpu:
-      addr: 127.0.0.6
+      addr: 10.11.0.6
     pfcp:
-      addr: 127.0.0.6
+      addr: 10.10.0.6

 #
 # sgwc:
```

Modify [install/etc/open5gs/upf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/upf.yaml.in) to set the GTP-U and PFCP IP address.  
```diff
$ diff -u /etc/open5gs/upf.yaml.old /etc/open5gs/upf.yaml
--- upf.yaml.old	2020-08-22 11:42:57.781750067 -0400
+++ upf.yaml	2020-08-22 11:43:13.268901616 -0400
@@ -59,11 +59,9 @@
 #
 upf:
     pfcp:
-      - addr: 127.0.0.7
+      - addr: 10.10.0.7
     gtpu:
-      - addr:
-        - 127.0.0.7
-        - ::1
+      - addr: 10.11.0.7
     pdn:
       - addr: 10.45.0.1/16
       - addr: cafe::1/64
```

After changing conf files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed.service
$ sudo systemctl restart open5gs-sgwcd.service
$ sudo systemctl restart open5gs-smfd.service
$ sudo systemctl restart open5gs-amfd.service
$ sudo systemctl restart open5gs-upfd.service
$ sudo systemctl restart open5gs-sgwud.service
```

### Install WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to install WebUI of Open5GS

1. *Debian and Ubuntu* based Linux distributions can install [Node.js](https://nodejs.org/) as follows:

    ```bash
    $ sudo apt update
    $ sudo apt install curl
    $ curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
    $ sudo apt install nodejs
    ```

2. To install [Node.js](https://nodejs.org/) on *openSUSE*, run the following:

    ```bash
    $ sudo zypper install nodejs8
    ```

You can now install WebUI of Open5GS.

```bash
$ curl -sL {{ site.url }}{{ site.baseurl }}/assets/webui/install | sudo -E bash -
```

### Register Subscriber Information
---

Connect to `http://localhost:3000` and login with **admin** account.

> Username : admin  
> Password : 1423

**Tip:** You can change the password in _Account_ Menu.
{: .notice--info}

To add subscriber information, you can do WebUI operations in the following order:

  1. Go to `Subscriber` Menu.
  2. Click `+` Button to add a new subscriber.
  3. Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  4. Click `SAVE` Button

**Tip:** This addition immediately affects Open5GS without restaring any daemon.
{: .notice--info}


### Adding a route for UE to have Internet connectivity {#UEInternet}
---

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
$ sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
```

**Note:** For the first time, it is a good condition if you do not have any rules in the IP/NAT tables. If a program such as docker has already set up a rule, you will need to add a rule differently.
{: .notice--danger}

### Turn on your gNB/eNB and Phone
---
- Connect your gNB/eNB to the IP of your server via the standard NGAP/S1AP port of SCTP 38412/36412 (for AMF/MME)
- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `/var/log/open5gs/*.log`.


### Uninstall Open5GS and WebUI

How to remove Open5GS package:

1. On *Ubuntu/Debian*:

    ```bash
    $ sudo apt purge open5gs
    $ sudo apt autoremove
    ```

2. On *openSUSE*:

    ```bash
    $ sudo zypper rm open5gs
    ```

You may need to remove manually `/var/log/open5gs` unless it is empty.

```bash
$ sudo rm -Rf /var/log/open5gs
```

The WebUI of Open5GS can be removed as follows:

```bash
curl -sL {{ site.url }}{{ site.baseurl }}/assets/webui/uninstall | sudo -E bash -
```

