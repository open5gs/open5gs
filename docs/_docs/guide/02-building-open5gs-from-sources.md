---
title: Building Open5GS from Sources
head_inline: "<style> .blue { color: blue; } </style>"
---

This post explains how to compile and install the source code on **Debian/Ubuntu** based Linux Distribution.
{: .blue}

**Note:** Ubuntu 16.04(xenial) and earlier versions, and Debian 9(stretch) and earlier versions are not supported.
{: .notice--danger}


### Getting MongoDB
---

Import the public key used by the package management system.

```bash
$ sudo apt update
$ sudo apt install gnupg
$ curl -fsSL https://pgp.mongodb.com/server-6.0.asc | sudo gpg -o /usr/share/keyrings/mongodb-server-6.0.gpg --dearmor
```

Create the list file /etc/apt/sources.list.d/mongodb-org-6.0.list for your version of Ubuntu.

On ubuntu 22.04 (Jammy)
```bash
$ echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-6.0.gpg] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/6.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-6.0.list
```

Install the MongoDB packages.
```bash
$ sudo apt update
$ sudo apt install -y mongodb-org
$ sudo systemctl start mongod (if '/usr/bin/mongod' is not running)
$ sudo systemctl enable mongod (ensure to automatically start it on system boot)
```

**Tip:** MongoDB is used as database for NRF/PCF/UDR and PCRF/HSS.
{: .notice--info}

### Setting up TUN device (not persistent after rebooting)
---

Create the TUN device with the interface name `ogstun`.

```bash
$ sudo ip tuntap add name ogstun mode tun
$ sudo ip addr add 10.45.0.1/16 dev ogstun
$ sudo ip addr add 2001:db8:cafe::1/48 dev ogstun
$ sudo ip link set ogstun up
```

**Tip:** The script provided in [$GIT_REPO/misc/netconf.sh](https://github.com/{{ site.github_username }}/open5gs/blob/main/misc/netconf.sh) makes it easy to configure the TUN device as follows:
`$ sudo ./misc/netconf.sh`
{: .notice--info}

### Building Open5GS
---

Install the dependencies for building the source code.

```bash
$ sudo apt install python3-pip python3-setuptools python3-wheel ninja-build build-essential flex bison git cmake libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev libidn11-dev libmongoc-dev libbson-dev libyaml-dev libnghttp2-dev libmicrohttpd-dev libcurl4-gnutls-dev libnghttp2-dev libtins-dev libtalloc-dev meson
```

Git clone.

```bash
$ git clone https://github.com/{{ site.github_username }}/open5gs
```

To compile with meson:

```bash
$ cd open5gs
$ meson build --prefix=`pwd`/install
$ ninja -C build
```

Please free up enough memory space on the VM and run the test program.

The test program has been run on [VirtualBox - CPU: 1, Memory: 4.00 GB] and [Docker for Mac - CPU: 2, Memory: 2.00 GB] with default setting (max.ue: 4,096, pool.packet: 32,768).
{: .notice--danger}

Check whether the compilation is correct.
```bash
$ ./build/tests/attach/attach ## EPC Only
$ ./build/tests/registration/registration ## 5G Core Only
```

Run all test programs as below.
```bash
$ cd build
$ meson test -v
```

**Tip:** You can also check the result of `ninja -C build test` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || pfcp || diameter || gtp || ngap || http2.data.data || http2.headers`.  You can see the virtually created packets. [testattach.pcapng]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testattach.pcapng)/[testregistration.pcapng]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testregistration.pcapng)
{: .notice--info}

You need to perform the **installation process**.
```bash
$ cd build
$ ninja install
$ cd ../
```

### Configure Open5GS
---

##### 5G Core

Modify [install/etc/open5gs/amf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/amf.yaml.in) to set the NGAP IP address, PLMN ID, TAC and NSSAI.

```diff
$ diff -u /etc/open5gs/amf.yaml.old /etc/open5gs/amf.yaml
--- amf.yaml    2020-09-05 20:52:28.652234967 -0400
+++ amf.yaml.new    2020-09-05 20:55:07.453114885 -0400
@@ -293,26 +293,26 @@ amf:
       - addr: 127.0.0.5
         port: 7777
     ngap:
-      - addr: 127.0.0.5
+      - addr: 10.10.0.5
     metrics:
         addr: 127.0.0.5
         port: 9090
     guami:
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 001
+          mnc: 01
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 001
+          mnc: 01
         tac: 1
     plmn_support:
       - plmn_id:
-          mcc: 999
-          mnc: 70
+          mcc: 001
+          mnc: 01
         s_nssai:
           - sst: 1
     security:
```

Modify [install/etc/open5gs/upf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/upf.yaml.in) to set the GTP-U and PFCP IP address.
```diff
$ diff -u /etc/open5gs/upf.yaml.old /etc/open5gs/upf.yaml
--- upf.yaml    2020-09-05 20:52:28.652234967 -0400
+++ upf.yaml.new    2020-09-05 20:52:55.279052142 -0400
@@ -168,7 +168,7 @@ upf:
     pfcp:
       - addr: 127.0.0.7
     gtpu:
-      - addr: 127.0.0.7
+      - addr: 10.11.0.7
     subnet:
       - addr: 10.45.0.1/16
       - addr: 2001:db8:cafe::1/48
```
##### 4G EPC

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC.

```diff
$ diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml    2020-09-05 20:52:28.648235143 -0400
+++ mme.yaml.new    2020-09-05 20:56:05.434484208 -0400
@@ -253,20 +253,20 @@ mme:
     s1ap:
-      - addr: 127.0.0.2
+      - addr: 10.10.0.2
     gtpc:
       - addr: 127.0.0.2
     metrics:
       addr: 127.0.0.2
       port: 9090
     gummei:
       plmn_id:
-        mcc: 999
-        mnc: 70
+        mcc: 001
+        mnc: 01
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 999
-        mnc: 70
+        mcc: 001
+        mnc: 01
       tac: 1
     security:
         integrity_order : [ EIA2, EIA1, EIA0 ]
```

Modify [install/etc/open5gs/sgwu.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/sgwu.yaml.in) to set the GTP-U IP address.
```diff
$ diff --git a/configs/open5gs/sgwu.yaml.in b/configs/open5gs/sgwu.yaml.in
index 8ccf94378..25b6884a3 100644
--- a/configs/open5gs/sgwu.yaml.in
+++ b/configs/open5gs/sgwu.yaml.in
@@ -100,7 +100,7 @@ sgwu:
     pfcp:
       - addr: 127.0.0.6
     gtpu:
-      - addr: 127.0.0.6
+      - addr: 10.11.0.6

 #
 # sgwc:
```

If you modify the config files while Open5GS daemons are running, please restart them


### Running Open5GS
---

```bash
$ ./install/bin/open5gs-nrfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:43:43.669: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nrf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:43:43.669: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nrf.log' (../lib/app/ogs-init.c:129)
10/22 10:43:43.670: [sbi] INFO: nghttp2_server() [127.0.0.10]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:43:43.670: [sbi] INFO: nghttp2_server() [::1]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:43:43.671: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)

$ ./install/bin/open5gs-scpd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:43:45.709: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/scp.yaml' (../lib/app/ogs-init.c:126)
10/22 10:43:45.709: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/scp.log' (../lib/app/ogs-init.c:129)
10/22 10:43:45.711: [sbi] INFO: nghttp2_server() [127.0.1.10]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:43:45.712: [app] INFO: SCP initialize...done (../src/scp/app.c:31)

$ ./install/bin/open5gs-amfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:01.416: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/amf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:01.416: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/amf.log' (../lib/app/ogs-init.c:129)
10/22 10:44:01.419: [sbi] INFO: NF Service [namf-comm] (../lib/sbi/context.c:1401)
10/22 10:44:01.420: [sbi] INFO: nghttp2_server() [127.0.0.5]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:01.420: [amf] INFO: ngap_server() [127.0.0.5]:38412 (../src/amf/ngap-sctp.c:61)
10/22 10:44:01.420: [sctp] INFO: AMF initialize...done (../src/amf/app.c:33)

$ ./install/bin/open5gs-smfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:03.217: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/smf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:03.217: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/smf.log' (../lib/app/ogs-init.c:129)
10/22 10:44:03.269: [gtp] INFO: gtp_server() [127.0.0.4]:2123 (../lib/gtp/path.c:30)
10/22 10:44:03.269: [gtp] INFO: gtp_server() [::1]:2123 (../lib/gtp/path.c:30)
10/22 10:44:03.269: [gtp] INFO: gtp_server() [127.0.0.4]:2152 (../lib/gtp/path.c:30)
10/22 10:44:03.269: [gtp] INFO: gtp_server() [::1]:2152 (../lib/gtp/path.c:30)
10/22 10:44:03.269: [pfcp] INFO: pfcp_server() [127.0.0.4]:8805 (../lib/pfcp/path.c:30)
10/22 10:44:03.269: [pfcp] INFO: pfcp_server() [::1]:8805 (../lib/pfcp/path.c:30)
10/22 10:44:03.269: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.7]:8805 (../lib/pfcp/path.c:61)
10/22 10:44:03.269: [sbi] INFO: NF Service [nsmf-pdusession] (../lib/sbi/context.c:1401)
10/22 10:44:03.270: [sbi] INFO: nghttp2_server() [127.0.0.4]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:03.270: [app] INFO: SMF initialize...done (../src/smf/app.c:31)

$ ./install/bin/open5gs-upfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:06.884: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/upf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:06.884: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/upf.log' (../lib/app/ogs-init.c:129)
10/22 10:44:06.892: [pfcp] INFO: pfcp_server() [127.0.0.7]:8805 (../lib/pfcp/path.c:30)
10/22 10:44:06.892: [gtp] INFO: gtp_server() [127.0.0.7]:2152 (../lib/gtp/path.c:30)
10/22 10:44:06.892: [app] INFO: UPF initialize...done (../src/upf/app.c:31)

$./install/bin/open5gs-ausfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:08.747: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/ausf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:08.747: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/ausf.log' (../lib/app/ogs-init.c:129)
10/22 10:44:08.748: [sbi] INFO: NF Service [nausf-auth] (../lib/sbi/context.c:1401)
10/22 10:44:08.749: [sbi] INFO: nghttp2_server() [127.0.0.11]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:08.749: [app] INFO: AUSF initialize...done (../src/ausf/app.c:31)

$ ./install/bin/open5gs-udmd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:10.724: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udm.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:10.724: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udm.log' (../lib/app/ogs-init.c:129)
10/22 10:44:10.726: [sbi] INFO: NF Service [nudm-ueau] (../lib/sbi/context.c:1401)
10/22 10:44:10.726: [sbi] INFO: NF Service [nudm-uecm] (../lib/sbi/context.c:1401)
10/22 10:44:10.726: [sbi] INFO: NF Service [nudm-sdm] (../lib/sbi/context.c:1401)
10/22 10:44:10.727: [sbi] INFO: nghttp2_server() [127.0.0.12]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:10.727: [app] INFO: UDM initialize...done (../src/udm/app.c:31)

$./install/bin/open5gs-pcfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:14.265: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/pcf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:14.265: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcf.log' (../lib/app/ogs-init.c:129)
10/22 10:44:14.269: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/22 10:44:14.269: [sbi] INFO: NF Service [npcf-am-policy-control] (../lib/sbi/context.c:1401)
10/22 10:44:14.269: [sbi] INFO: NF Service [npcf-smpolicycontrol] (../lib/sbi/context.c:1401)
10/22 10:44:14.269: [sbi] INFO: NF Service [npcf-policyauthorization] (../lib/sbi/context.c:1401)
10/22 10:44:14.269: [sbi] INFO: nghttp2_server() [127.0.0.13]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:14.269: [app] INFO: PCF initialize...done (../src/pcf/app.c:31)

$ ./install/bin/open5gs-nssfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:16.250: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nssf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:16.250: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nssf.log' (../lib/app/ogs-init.c:129)
10/22 10:44:16.252: [sbi] INFO: NF Service [nnssf-nsselection] (../lib/sbi/context.c:1401)
10/22 10:44:16.252: [sbi] INFO: nghttp2_server() [127.0.0.14]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:16.252: [app] INFO: NSSF initialize...done (../src/nssf/app.c:31)

$ ./install/bin/open5gs-bsfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:18.836: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/bsf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:18.836: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/bsf.log' (../lib/app/ogs-init.c:129)
10/22 10:44:18.837: [sbi] INFO: NF Service [nbsf-management] (../lib/sbi/context.c:1401)
10/22 10:44:18.837: [sbi] INFO: nghttp2_server() [127.0.0.15]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:18.837: [app] INFO: BSF initialize...done (../src/bsf/app.c:31)

$ ./install/bin/open5gs-udrd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:24.018: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udr.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:24.018: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udr.log' (../lib/app/ogs-init.c:129)
10/22 10:44:24.021: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/22 10:44:24.021: [sbi] INFO: NF Service [nudr-dr] (../lib/sbi/context.c:1401)
10/22 10:44:24.021: [sbi] INFO: nghttp2_server() [127.0.0.20]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:44:24.021: [app] INFO: UDR initialize...done (../src/udr/app.c:31)

$ ./install/bin/open5gs-mmed
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:26.011: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:26.011: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/mme.log' (../lib/app/ogs-init.c:129)
10/22 10:44:26.062: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (../lib/gtp/path.c:30)
10/22 10:44:26.063: [gtp] INFO: gtp_connect() [127.0.0.3]:2123 (../lib/gtp/path.c:60)
10/22 10:44:26.063: [mme] INFO: s1ap_server() [127.0.0.2]:36412 (../src/mme/s1ap-sctp.c:62)
10/22 10:44:26.063: [sctp] INFO: MME initialize...done (../src/mme/app-init.c:33)

$ ./install/bin/open5gs-sgwcd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:29.050: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwc.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:29.050: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwc.log' (../lib/app/ogs-init.c:129)
10/22 10:44:29.058: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (../lib/gtp/path.c:30)
10/22 10:44:29.058: [pfcp] INFO: pfcp_server() [127.0.0.3]:8805 (../lib/pfcp/path.c:30)
10/22 10:44:29.058: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.6]:8805 (../lib/pfcp/path.c:61)
10/22 10:44:29.058: [app] INFO: SGW-C initialize...done (../src/sgwc/app.c:31)

$ ./install/bin/open5gs-sgwud
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:44:36.178: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwu.yaml' (../lib/app/ogs-init.c:126)
10/22 10:44:36.178: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwu.log' (../lib/app/ogs-init.c:129)
10/22 10:44:36.185: [pfcp] INFO: pfcp_server() [127.0.0.6]:8805 (../lib/pfcp/path.c:30)
10/22 10:44:36.185: [gtp] INFO: gtp_server() [127.0.0.6]:2152 (../lib/gtp/path.c:30)
10/22 10:44:36.185: [app] INFO: SGW-U initialize...done (../src/sgwu/app.c:31)

$ ./install/bin/open5gs-hssd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:49:22.963: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/hss.yaml' (../lib/app/ogs-init.c:126)
10/22 10:49:22.963: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/hss.log' (../lib/app/ogs-init.c:129)
10/22 10:49:22.965: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/22 10:49:22.996: [app] INFO: HSS initialize...done (../src/hss/app-init.c:31)
10/22 10:49:22.997: [diam] INFO: CONNECTED TO 'mme.localdomain' (SCTP,soc#15): (../lib/diameter/common/logger.c:108)

$ ./install/bin/open5gs-pcrfd
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:49:26.089: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/pcrf.yaml' (../lib/app/ogs-init.c:126)
10/22 10:49:26.089: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcrf.log' (../lib/app/ogs-init.c:129)
10/22 10:49:26.091: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/22 10:49:26.124: [app] INFO: PCRF initialize...done (../src/pcrf/app-init.c:31)
10/22 10:49:26.125: [diam] INFO: CONNECTED TO 'smf.localdomain' (SCTP,soc#15): (../lib/diameter/common/logger.c:108)
```

Several command line options are provided.

```bash
$ ./install/bin/open5gs-amfd -h
Usage: ./install/bin/open5gs-amfd [options]
Options:
   -c filename    : set configuration file
   -l filename    : set logging file
   -e level       : set global log-level (default:info)
   -m domain      : set log-domain (e.g. mme:sgw:gtp)
   -d             : print lots of debugging information
   -t             : print tracing information for developer
   -D             : start as a daemon
   -v             : show version number and exit
   -h             : show this message and exit
```

You can also copy the binaries to /usr/bin to be able to run them from anywhere on the system.
```bash
$ cp open5gs* /usr/bin/
```

For convenience, you can execute all NFs at once by using the following command.
```bash
$ ./build/tests/app/5gc ## 5G Core Only with ./build/configs/sample.yaml
$ ./build/tests/app/epc -c ./build/configs/srslte.yaml ## EPC Only with ./build/configs/srslte.yaml
$ ./build/tests/app/app ## Both 5G Core and EPC with ./build/configs/sample.yaml
```

### Building the WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to build WebUI of Open5GS

```bash
$ sudo apt install curl
$ curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
$ sudo apt install nodejs
```

Install the dependencies to run WebUI

```bash
$ cd webui
$ npm ci
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
$ npm run dev
```

Server listening can be changed by setting the environment variable HOSTNAME or PORT as below.

```bash
$ HOSTNAME=192.168.0.11 npm run dev
$ PORT=7777 npm run dev
```

### Register Subscriber Information
---

Connect to `http://127.0.0.1:3000` and login with **admin** account.

> Username : admin  
> Password : 1423

**Note:**
You can change the password in _Account_ Menu.
{: .notice--info}

To add subscriber information, you can do WebUI operations in the following order:

  1. Go to `Subscriber` Menu.
  2. Click `+` Button to add a new subscriber.
  3. Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  4. Click `SAVE` Button

**Note:** Subscribers added with this tool immediately register in the Open5GS HSS/UDR without the need to restart any daemon. However, if you use the WebUI to change subscriber profile, you must restart the Open5GS AMF/MME daemon for the changes to take effect.
{: .notice--warning}

#### Adding a route for the UE to have WAN connectivity {#UEInternet}
---

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

## 5. Turn on your eNB/gNB and UE
---

- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `$INSTALL_PREFIX/var/log/open5gs/*.log`.

### Re-install with updated source code

You need to update the Open5GS source and all subprojects.

```bash
$ cd open5gs
$ git pull ## Update the Open5GS source
$ meson subprojects update ## Update all subprojects
```

To compile with meson:

```bash
$ ninja -C build
```

Check whether the compilation is correct.
```bash
$ cd build
$ meson test -v
```

Install Open5GS without making any configuration file changes.
```bash
$ ninja install
```

**Note:** The configuration files in the installation directory are not changed. If you want to initialize the configuration files as well, you need to delete the existing installation directory and install it.
{: .notice--danger}

Install Open5GS and initialize configuration files.
```bash
$ rm -Rf ../install
$ ninja install
```

### Troubleshooting
---

Debugging tools can help you troubleshoot problems.

- [GDB](https://www.gnu.org/software/gdb/) can be used as below:

  ```bash
  $ gdb ./open5gs-amfd
  ```

- On *Mac OS X*, you can use the [LLDB](https://lldb.llvm.org/).

  ```bash
  $ lldb ./open5gs-amfd
  ```

You can use the command line option[`-d`] to record more logs.

```bash
$ ./tests/app/app -d
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:59:03.813: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/sample.yaml' (../lib/app/ogs-init.c:126)
10/22 10:59:03.813: [thread] DEBUG: [0x55fab86aff40] worker signal (../lib/core/ogs-thread.c:66)
10/22 10:59:03.813: [thread] DEBUG: [0x55fab86aff40] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v2.4.11-100-gbea24d7

10/22 10:59:03.818: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/sample.yaml' (../lib/app/ogs-init.c:126)
10/22 10:59:03.819: [sock] DEBUG: addr:127.0.0.10, port:7777 (../lib/core/ogs-sockaddr.c:140)
10/22 10:59:03.819: [sock] DEBUG: addr:::1, port:7777 (../lib/core/ogs-sockaddr.c:140)
10/22 10:59:03.819: [sock] DEBUG: addr:127.0.1.10, port:7777 (../lib/core/ogs-sockaddr.c:140)
10/22 10:59:03.819: [sock] DEBUG: socket create(2:1:6) (../lib/core/ogs-socket.c:92)
10/22 10:59:03.819: [sock] DEBUG: Turn on TCP_NODELAY (../lib/core/ogs-sockopt.c:139)
10/22 10:59:03.819: [sock] DEBUG: Turn on SO_REUSEADDR (../lib/core/ogs-sockopt.c:120)
10/22 10:59:03.819: [sock] DEBUG: socket bind 127.0.0.10:7777 (../lib/core/ogs-socket.c:117)
10/22 10:59:03.819: [sock] DEBUG: tcp_server() [127.0.0.10]:7777 (../lib/core/ogs-tcp.c:60)
10/22 10:59:03.819: [sbi] INFO: nghttp2_server() [127.0.0.10]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:59:03.819: [sock] DEBUG: socket create(10:1:6) (../lib/core/ogs-socket.c:92)
10/22 10:59:03.819: [sock] DEBUG: Turn on TCP_NODELAY (../lib/core/ogs-sockopt.c:139)
10/22 10:59:03.819: [sock] DEBUG: Turn on SO_REUSEADDR (../lib/core/ogs-sockopt.c:120)
10/22 10:59:03.819: [sock] DEBUG: socket bind ::1:7777 (../lib/core/ogs-socket.c:117)
10/22 10:59:03.819: [sock] DEBUG: tcp_server() [::1]:7777 (../lib/core/ogs-tcp.c:60)
10/22 10:59:03.819: [sbi] INFO: nghttp2_server() [::1]:7777 (../lib/sbi/nghttp2-server.c:150)
10/22 10:59:03.819: [thread] DEBUG: [0x5601b141e520] worker signal (../lib/core/ogs-thread.c:66)
10/22 10:59:03.819: [nrf] DEBUG: nrf_state_initial(): INIT (../src/nrf/nrf-sm.c:25)
10/22 10:59:03.819: [nrf] DEBUG: nrf_state_operational(): ENTRY (../src/nrf/nrf-sm.c:50)
10/22 10:59:03.819: [thread] DEBUG: [0x5601b141e520] thread started (../lib/core/ogs-thread.c:101)
10/22 10:59:03.819: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)
10/22 10:59:03.864: [thread] DEBUG: [0x55fab86d2100] worker signal (../lib/core/ogs-thread.c:66)
10/22 10:59:03.864: [thread] DEBUG: [0x55fab86d2100] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v2.4.11-100-gbea24d7
...

```
