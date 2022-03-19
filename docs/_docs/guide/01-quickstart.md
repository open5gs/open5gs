---
title: Quickstart
---

## 1. Introduction to Open5GS
---

Welcome! If you want to set up your first Open5GS core you have come to the right place. Before we get started, we'll spend a moment to understand the basic architecture of the software.

**TL;DR:** Open5GS contains a series of software components and network functions that implement the 4G/ 5G NSA and 5G SA core functions. If you know what each of these do already and how they interface with each other, skip to section 2.
{: .notice--info}

<style>
  img {
    max-width: 100%;
    height: auto;
  }
</style>

![Open5GS Diagram]({{ site.url }}{{ site.baseurl }}/assets/images/Open5GS_CUPS-01.jpg)

[[Higher quality PDF diagram available HERE]]({{ site.url }}{{ site.baseurl }}/assets/images/Open5GS_CUPS-01.pdf)

#### 4G/ 5G NSA Core

The Open5GS 4G/ 5G NSA Core contains the following components:
* MME - Mobility Management Entity
* HSS - Home Subscriber Server
* PCRF - Policy and Charging Rules Function
* SGWC - Serving Gateway Control Plane
* SGWU - Serving Gateway User Plane
* PGWC/SMF - Packet Gateway Control Plane / (component contained in Open5GS SMF)
* PGWU/UPF - Packet Gateway User Plane / (component contained in Open5GS UPF)

The core has two main planes: the control plane and the user plane. These are physically separated in Open5GS as CUPS (control/ user plane separation) is implemented.

The MME is the main **control plane** hub of the core. It primarily manages sessions, mobility, paging and bearers. It links to the HSS, which generates SIM authentication vectors and holds the subscriber profile; and also to the SGWC and PGWC/SMF, which are the control planes of the gateway servers. All the eNBs in the mobile network (4G basestations) connect to the MME. The final element of the control plane is the PCRF, which sits in-between the PGWC/SMF and the HSS, and handles charging and enforces subscriber policies.

The **user plane** carries user data packets between the eNB/ NSA gNB (5G NSA basestations) and the external WAN. The two user plane core components are the SGWU and PGWU/UPF. Each of these connect back to their control plane counterparts. eNBs/ NSA gNBs connect to the SGWU, which connects to the PGWU/UPF, and on to the WAN.  *By having the control and user planes physically separated like this, it means you can deploy multiple user plane servers in the field (eg somewhere with a high speed Internet connection), whilst keeping control functionality centralised. This enables support of MEC use cases, for example.*

All of these Open5GS components have config files. Each config file contains the component's IP bind addresses/ local Interface names **and** the IP addresses/ DNS names of the other components it needs to connect to. We'll come back to this in Section 3.


#### 5G SA Core

The Open5GS 5G SA Core contains the following functions:
* AMF - Access and Mobility Management Function
* SMF - Session Management Function
* UPF - User Plane Function
* AUSF - Authentication Server Function
* NRF - NF Repository Function
* UDM - Unified Data Management
* UDR - Unified Data Repository
* PCF - Policy and Charging Function
* NSSF - Network Slice Selection Function
* BSF - Binding Support Function

The 5G SA core works in a different way to the 4G core - it uses a **Service Based Architecture** (SBI). **Control plane** functions are configured to register with the NRF, and the NRF then helps them discover the other core functions. Running through the other functions: The AMF handles connection and mobility management; a subset of what the 4G MME is tasked with. gNBs (5G basestations) connect to the AMF. The UDM, AUSF and UDR carry out similar operations as the 4G HSS, generating SIM authentication vectors and holding the subscriber profile. Session management is all handled by the SMF (previously the responsibility of the 4G MME/ SGWC/ PGWC). The NSSF provides a way to select the network slice. Finally there is the PCF, used for charging and enforcing subscriber policies.

The 5G SA core **user plane** is much simpler, as it only contains a single function. The UPF carries user data packets between the gNB and the external WAN. It connects back to the SMF too. 

With the exception of the SMF and UPF, all config files for the 5G SA core functions only contain the function's IP bind addresses/ local Interface names and the IP address/ DNS name of the NRF.


## 2. Install Open5GS with a Package Manager
---

**Note:** Package managers can be used to install Open5GS in *Debian/Ubuntu and openSUSE* environments (for major and minor builds). *CentOS, Fedora, and Mac OSX* require you to [build with source code]({{ site.url }}{{ site.baseurl }}/docs/guide/02-building-open5gs-from-sources).
{: .notice--warning}
**Note:** Nighly builds are offered by [Osmocom](https://osmocom.org) on [OBS](https://build.opensuse.org/package/show/network:osmocom:nightly/open5gs). Scroll down to use a nightly build package.
{: .notice--warning}


#### Ubuntu

*Ubuntu* makes it easy to install Open5GS as shown below.

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
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_20.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_20.10/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_21.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_21.10/
```

#### openSUSE

[Martin Hauke](https://build.opensuse.org/user/show/mnhauke) packaged Open5GS for *openSUSE* on [OBS](https://build.opensuse.org/package/show/home:mnhauke:open5gs/open5gs).

```bash
$ sudo zypper addrepo -f obs://home:mnhauke:open5gs home:mnhauke:open5gs
$ sudo zypper install mongodb-server mongodb-shell
$ sudo zypper install open5gs
```

#### Nightly Builds

Nightly bulit package are provided by [Osmocom](https://osmocom.org) on [OBS](https://build.opensuse.org/package/show/network:osmocom:nightly/open5gs). On *Ubuntu 20.04* you can install it like this:

```bash
$ sudo apt update
$ sudo apt install wget gnupg
$ wget -qO - https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_20.04/Release.key | sudo apt-key add -
$ sudo sh -c "echo 'deb http://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_20.04/ ./' > /etc/apt/sources.list.d/open5gs.list"
$ sudo apt update
$ sudo apt install open5gs
```

Other distributions can be installed by changing the path.

```
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Debian_10/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Debian_Testing/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Debian_Unstable/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/Raspbian_10/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_18.04/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_20.04/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_20.10/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_21.04/
https://download.opensuse.org/repositories/network:/osmocom:/nightly/xUbuntu_21.10/
```


## 3. Install the  WebUI of Open5GS
---

The WebUI allows you to interactively edit subscriber data. While it is not essential to use this, it makes things easier when you are just starting out on your Open5GS adventure. (A [command line tool](https://github.com/{{ site.github_username }}/open5gs/blob/main/misc/db/open5gs-dbctl) is available for advanced users).


[Node.js](https://nodejs.org/) is required to install the WebUI of Open5GS

1. *Debian and Ubuntu* based Linux distributions can install [Node.js](https://nodejs.org/) as follows:

    ```bash
    $ sudo apt update
    $ sudo apt install curl
    $ curl -fsSL https://deb.nodesource.com/setup_14.x | sudo -E bash -
    $ sudo apt install nodejs
    ```

2. To install [Node.js](https://nodejs.org/) on *openSUSE*, run the following:

    ```bash
    $ sudo zypper install nodejs8
    ```

You can now install WebUI of Open5GS.

```bash
$ curl -fsSL {{ site.url }}{{ site.baseurl }}/assets/webui/install | sudo -E bash -
```

## 4. Configure Open5GS
---

Okay - you have installed the software, now what to do with it? Well, there are some tweaks you will need to make to the config files, and you will need to enter subscriber data into your HSS/ UDR. You will also need to set some IP Table rules to bridge the PGWU/UPF to the WAN.

Out of the box, the default configurations see all of the Open5GS components fully configured for use on a single computer. They are set to communicate with each other using the local loopback address space (`127.0.0.X`). The default addresses for each of the bind interfaces for these components and functions are as follows:

```
MongoDB   = 127.0.0.1 (subscriber data) - http://localhost:3000

MME-s1ap  = 127.0.0.2 :36412 for S1-MME
MME-gtpc  = 127.0.0.2 :2123 for S11
MME-frDi  = 127.0.0.2 :3868 for S6a

SGWC-gtpc = 127.0.0.3 :2123 for S11
SGWC-pfcp = 127.0.0.3 :8805 for Sxa

SMF-gtpc  = 127.0.0.4 :2123 for S5c, N11
SMF-gtpu  = 127.0.0.4 :2152 for N4u (Sxu)
SMF-pfcp  = 127.0.0.4 :8805 for N4 (Sxb)
SMF-frDi  = 127.0.0.4 :3868 for Gx auth
SMF-sbi   = 127.0.0.4 :7777 for 5G SBI (N7,N10,N11)

AMF-ngap  = 127.0.0.5 :38412 for N2
AMF-sbi   = 127.0.0.5 :7777 for 5G SBI (N8,N12,N11)

SGWU-pfcp = 127.0.0.6 :8805 for Sxa
SGWU-gtpu = 127.0.0.6 :2152 for S1-U, S5u

UPF-pfcp  = 127.0.0.7 :8805 for N4 (Sxb)
UPF-gtpu  = 127.0.0.7 :2152 for S5u, N3, N4u (Sxu)

HSS-frDi  = 127.0.0.8 :3868 for S6a, Cx

PCRF-frDi = 127.0.0.9 :3868 for Gx

NRF-sbi   = 127.0.0.10:7777 for 5G SBI
AUSF-sbi  = 127.0.0.11:7777 for 5G SBI
UDM-sbi   = 127.0.0.12:7777 for 5G SBI
PCF-sbi   = 127.0.0.13:7777 for 5G SBI
NSSF-sbi  = 127.0.0.14:7777 for 5G SBI
BSF-sbi   = 127.0.0.15:7777 for 5G SBI
UDR-sbi   = 127.0.0.20:7777 for 5G SBI
```

#### Setup a 4G/ 5G NSA Core

You will need to modify your 4G MME config to support your PLMN and TAC. The international test PLMN is 001/01, and the international private network PLMN is 999/99. You should stick to using either of these PLMNs unless you have been issued a PLMN by your national regulator. (This PLMN will need to be configured in your eNB).

If you are aiming to connect an external eNB to your core, you will also need to change the S1AP bind address of the MME **and** the GTP-U bind address of the SGWU. If you are running an eNB stack locally, you will not need to make these changes.


Modify [/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC.

```diff
$ diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml

 mme:
     freeDiameter: /etc/freeDiameter/mme.conf
     s1ap:
-      addr: 127.0.0.2
+      addr: 10.10.0.2 # for external eNB - a local address that can be reached by the eNB
     gtpc:
       addr: 127.0.0.2
     gummei:
       plmn_id:
-        mcc: 901
-        mnc: 70
+        mcc: 001 # set your PLMN-MCC
+        mnc: 01  # set your PLMN-MNC
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 901
-        mnc: 70
-      tac: 1
+        mcc: 001 # set your PLMN-MCC
+        mnc: 01  # set your PLMN-MNC
+      tac: 2 # should match the TAC used by your eNB
     security:

```

Modify [/etc/open5gs/sgwu.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/sgwu.yaml.in) to set the GTP-U IP address.
```diff
$ diff -u /etc/open5gs/sgwu.yaml.old /etc/open5gs/sgwu.yaml

 sgwu:
     gtpu:
-      addr: 127.0.0.6
+      addr: 10.11.0.6  # for external eNB - a local address that can be reached by the eNB
     pfcp:
       addr: 127.0.0.6

```

After changing config files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-mmed
$ sudo systemctl restart open5gs-sgwud
```

#### Setup a 5G Core

You will need to modify your 5G AMF config to support your PLMN and TAC. The international test PLMN is 001/01, and the international private network PLMN is 999/99. You should stick to using either of these PLMNs unless you have been issued a PLMN by your national regulator. (This PLMN will need to be configured in your gNB).

If you are aiming to connect an external gNB to your core, you will also need to change the NGAP bind address of the AMF **and** the GTPU bind address of the UPF. If you are running an gNB stack locally, you will not need to make these changes.


Modify [/etc/open5gs/amf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/amf.yaml.in) to set the NGAP IP address, PLMN ID, TAC and NSSAI.

```diff
$ diff -u /etc/open5gs/amf.yaml.old /etc/open5gs/amf.yaml

amf:
    sbi:
      - addr: 127.0.0.5
        port: 7777
     ngap:
-      - addr: 127.0.0.5
+      - addr: 10.10.0.5 # for external gNB - a local address that can be reached by the gNB
     guami:
       - plmn_id:
-          mcc: 901
-          mnc: 70
+          mcc: 001 # set your PLMN-MCC
+          mnc: 01  # set your PLMN-MNC
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 901
-          mnc: 70
-        tac: 1
+          mcc: 001 # set your PLMN-MCC
+          mnc: 01  # set your PLMN-MNC
+        tac: 2 # should match the TAC used by your gNB
     plmn_support:
       - plmn_id:
-          mcc: 901
-          mnc: 70
+          mcc: 001 # set your PLMN-MCC
+          mnc: 01  # set your PLMN-MNC
         s_nssai:
           - sst: 1
     security:

```

Modify [/etc/open5gs/upf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/upf.yaml.in) to set the GTP-U address.
```diff
$ diff -u /etc/open5gs/upf.yaml.old /etc/open5gs/upf.yaml

upf:
     pfcp:
       - addr: 127.0.0.7
     gtpu:
-      - addr: 127.0.0.7
+      - addr: 10.11.0.7 # for external gNB - a local address that can be reached by the gNB
     subnet:
       - addr: 10.45.0.1/16
       - addr: 2001:db8:cafe::1/48

```

After changing config files, please restart Open5GS daemons.

```bash
$ sudo systemctl restart open5gs-amfd
$ sudo systemctl restart open5gs-upfd
```


#### Register Subscriber Information
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

Enter the subscriber details of your SIM cards using this tool, to save the subscriber profile in the HSS and UDR MongoDB database backend. If you are using test SIMs, the details are normally printed on the card.

**Tip:** Subscribers added with this tool immediately register in the Open5GS HSS/ UDR without the need to restart any daemon.
{: .notice--info}


#### Adding a route for the UE to have WAN connectivity {#UEInternet}
---

In order to bridge between the PGWU/UPF and WAN (Internet), you must enable IP forwarding and add a NAT rule to your IP Tables.  

**Note:** For the first run, it makes things simpler if you do not have any rules in the IP/NAT tables. If a program such as docker has already set up a rule, you will need to add rules differently.
{: .notice--danger}

You can check your current IP Table rules with the following commands (these tables are empty):
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
```

To enable forwarding and add the NAT rule, enter
```bash
### Enable IPv4/IPv6 Forwarding
$ sudo sysctl -w net.ipv4.ip_forward=1
$ sudo sysctl -w net.ipv6.conf.all.forwarding=1

### Add NAT Rule
$ sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
$ sudo ip6tables -t nat -A POSTROUTING -s 2001:db8:cafe::/48 ! -o ogstun -j MASQUERADE
```


## 5. Turn on your eNB/gNB and UE
---

First, connect your eNB/gNB to the Open5GS core:
* Make sure the PLMN and TAC of the eNB/gNB matches the settings in your MME/AMF
* Connect your eNB/gNB to the IP of your server via the standard S1AP/NGAP SCTP port 36412/38412 (for MME/AMF)
* Your eNB/gNB should report a successful S1/NG connection - congrats, your core is fully working!
* You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
* You can view the log at `/var/log/open5gs/*.log`, eg:
```bash
### Watch the live MME log
tail -f /var/log/open5gs/mme.log
```

Next, try to attach a UE to the basestation:
* Insert your SIM card to the UE
* Set the UE's APN to match the APN you configured in the Open5GS WebUI
* Toggle the UE in and out of flight mode
* If it doesn't automatically connect, try manually searching for a network
* If the PLMN set on the SIM card does not match the PLMN being used by the radio, you will need to ensure 'data roaming' on the UE is switched on

## 6. Starting and Stopping Open5GS
---

When you install the software using the package manager, it is setup to run as a systemd service. You can stop and restart the components and network functions as follows:

```bash
$ sudo systemctl stop open5gs-mmed
$ sudo systemctl stop open5gs-sgwcd
$ sudo systemctl stop open5gs-smfd
$ sudo systemctl stop open5gs-amfd
$ sudo systemctl stop open5gs-sgwud
$ sudo systemctl stop open5gs-upfd
$ sudo systemctl stop open5gs-hssd
$ sudo systemctl stop open5gs-pcrfd
$ sudo systemctl stop open5gs-nrfd
$ sudo systemctl stop open5gs-ausfd
$ sudo systemctl stop open5gs-udmd
$ sudo systemctl stop open5gs-pcfd
$ sudo systemctl stop open5gs-nssfd
$ sudo systemctl stop open5gs-bsfd
$ sudo systemctl stop open5gs-udrd
$ sudo systemctl stop open5gs-webui
```

```bash
$ sudo systemctl restart open5gs-mmed
$ sudo systemctl restart open5gs-sgwcd
$ sudo systemctl restart open5gs-smfd
$ sudo systemctl restart open5gs-amfd
$ sudo systemctl restart open5gs-sgwud
$ sudo systemctl restart open5gs-upfd
$ sudo systemctl restart open5gs-hssd
$ sudo systemctl restart open5gs-pcrfd
$ sudo systemctl restart open5gs-nrfd
$ sudo systemctl restart open5gs-ausfd
$ sudo systemctl restart open5gs-udmd
$ sudo systemctl restart open5gs-pcfd
$ sudo systemctl restart open5gs-nssfd
$ sudo systemctl restart open5gs-bsfd
$ sudo systemctl restart open5gs-udrd
$ sudo systemctl restart open5gs-webui
```


## 7. Uninstall Open5GS and WebUI

To remove the Open5GS packages:

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
curl -fsSL {{ site.url }}{{ site.baseurl }}/assets/webui/uninstall | sudo -E bash -
```

