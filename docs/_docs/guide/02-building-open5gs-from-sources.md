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

Modify [install/etc/open5gs/nrf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/nrf.yaml.in) to set the NGAP IP address, PLMN ID, TAC and NSSAI.

```diff
$ diff --git a/configs/open5gs/nrf.yaml.in b/configs/open5gs/nrf.yaml.in
index cd9e45feb..58e8cbbce 100644
--- a/configs/open5gs/nrf.yaml.in
+++ b/configs/open5gs/nrf.yaml.in
@@ -10,8 +10,8 @@ global:
 nrf:
   serving:  # 5G roaming requires PLMN in NRF
     - plmn_id:
-        mcc: 999
-        mnc: 70
+        mcc: 001
+        mnc: 01
   sbi:
     server:
       - address: 127.0.0.10
```

Modify [install/etc/open5gs/amf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/amf.yaml.in) to set the NGAP IP address, PLMN ID, TAC and NSSAI.

```diff
$ diff --git a/configs/open5gs/amf.yaml.in b/configs/open5gs/amf.yaml.in
index 938917e32..35d0ab5aa 100644
--- a/configs/open5gs/amf.yaml.in
+++ b/configs/open5gs/amf.yaml.in
@@ -18,27 +18,27 @@ amf:
           - uri: http://127.0.0.200:7777
     ngap:
       server:
-        - address: 127.0.0.5
+        - address: 10.10.0.5
     metrics:
       server:
         - address: 127.0.0.5
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
$ diff --git a/configs/open5gs/upf.yaml.in b/configs/open5gs/upf.yaml.in
index e78b018f1..35a54419e 100644
--- a/configs/open5gs/upf.yaml.in
+++ b/configs/open5gs/upf.yaml.in
@@ -15,7 +15,7 @@ upf:
 #          - address: 127.0.0.4
     gtpu:
       server:
-        - address: 127.0.0.7
+        - address: 10.11.0.7
     session:
       - subnet: 10.45.0.1/16
       - subnet: 2001:db8:cafe::1/48
```
##### 4G EPC

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC.

```diff
$ diff --git a/configs/open5gs/mme.yaml.in b/configs/open5gs/mme.yaml.in
index db2cdaef1..2010f6691 100644
--- a/configs/open5gs/mme.yaml.in
+++ b/configs/open5gs/mme.yaml.in
@@ -10,7 +10,7 @@ mme:
     freeDiameter: @sysconfdir@/freeDiameter/mme.conf
     s1ap:
       server:
-        - address: 127.0.0.2
+        - address: 10.10.0.2
     gtpc:
       server:
         - address: 127.0.0.2
@@ -25,14 +25,14 @@ mme:
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
index 7266e47fb..d640f0357 100644
--- a/configs/open5gs/sgwu.yaml.in
+++ b/configs/open5gs/sgwu.yaml.in
@@ -15,7 +15,7 @@ sgwu:
 #          - address: 127.0.0.3
     gtpu:
       server:
-        - address: 127.0.0.6
+        - address: 10.11.0.6

 ################################################################################
 # PFCP Server
```

If you modify the config files while Open5GS daemons are running, please restart them


### Running Open5GS
---

```bash
$ ./install/bin/open5gs-nrfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:15.476: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nrf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:15.476: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nrf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:15.478: [sbi] INFO: nghttp2_server() [http://127.0.0.10]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:15.478: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)

$ ./install/bin/open5gs-scpd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:17.105: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/scp.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:17.105: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/scp.log' (../lib/app/ogs-init.c:132)
10/02 10:09:17.107: [sbi] INFO: nghttp2_server() [http://127.0.0.200]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:17.107: [app] INFO: SCP initialize...done (../src/scp/app.c:31)

$ ./install/bin/open5gs-seppd -c ./install/etc/open5gs/sepp1.yaml
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:23.191: [app] INFO: Configuration: './install/etc/open5gs/sepp1.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:23.191: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sepp1.log' (../lib/app/ogs-init.c:132)
10/02 10:09:23.193: [sbi] INFO: nghttp2_server() [http://127.0.1.250]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:23.197: [sbi] INFO: nghttp2_server(n32f) [https://127.0.1.252]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:23.198: [sbi] INFO: nghttp2_server(sepp) [https://127.0.1.251]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:23.199: [app] INFO: SEPP initialize...done (../src/sepp/app.c:31)

$ ./install/bin/open5gs-amfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:26.041: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/amf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:26.041: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/amf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:26.045: [metrics] INFO: metrics_server() [http://127.0.0.5]:9090 (../lib/metrics/prometheus/context.c:299)
10/02 10:09:26.045: [sbi] INFO: NF Service [namf-comm] (../lib/sbi/context.c:1744)
10/02 10:09:26.045: [sbi] INFO: nghttp2_server() [http://127.0.0.5]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:26.045: [amf] INFO: ngap_server() [127.0.0.5]:38412 (../src/amf/ngap-sctp.c:61)
10/02 10:09:26.046: [sctp] INFO: AMF initialize...done (../src/amf/app.c:33)

$ ./install/bin/open5gs-smfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:27.887: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/smf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:27.887: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/smf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:27.947: [metrics] INFO: metrics_server() [http://127.0.0.4]:9090 (../lib/metrics/prometheus/context.c:299)
10/02 10:09:27.990: [gtp] INFO: gtp_server() [127.0.0.4]:2123 (../lib/gtp/path.c:30)
10/02 10:09:27.990: [gtp] INFO: gtp_server() [127.0.0.4]:2152 (../lib/gtp/path.c:30)
10/02 10:09:27.990: [pfcp] INFO: pfcp_server() [127.0.0.4]:8805 (../lib/pfcp/path.c:30)
10/02 10:09:27.990: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.7]:8805 (../lib/pfcp/path.c:61)
10/02 10:09:27.990: [sbi] INFO: NF Service [nsmf-pdusession] (../lib/sbi/context.c:1744)
10/02 10:09:27.990: [sbi] INFO: nghttp2_server() [http://127.0.0.4]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:27.990: [app] INFO: SMF initialize...done (../src/smf/app.c:31)

$ ./install/bin/open5gs-upfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:34.259: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/upf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:34.259: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/upf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:34.340: [metrics] INFO: metrics_server() [http://127.0.0.7]:9090 (../lib/metrics/prometheus/context.c:299)
10/02 10:09:34.340: [pfcp] INFO: pfcp_server() [127.0.0.7]:8805 (../lib/pfcp/path.c:30)
10/02 10:09:34.341: [gtp] INFO: gtp_server() [127.0.0.7]:2152 (../lib/gtp/path.c:30)
10/02 10:09:34.343: [app] INFO: UPF initialize...done (../src/upf/app.c:31)

$ ./install/bin/open5gs-ausfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:35.832: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/ausf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:35.832: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/ausf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:35.834: [sbi] INFO: NF Service [nausf-auth] (../lib/sbi/context.c:1744)
10/02 10:09:35.834: [sbi] INFO: nghttp2_server() [http://127.0.0.11]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:35.835: [app] INFO: AUSF initialize...done (../src/ausf/app.c:31)

$ ./install/bin/open5gs-udmd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:40.766: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udm.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:40.766: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udm.log' (../lib/app/ogs-init.c:132)
10/02 10:09:40.768: [sbi] INFO: NF Service [nudm-ueau] (../lib/sbi/context.c:1744)
10/02 10:09:40.768: [sbi] INFO: NF Service [nudm-uecm] (../lib/sbi/context.c:1744)
10/02 10:09:40.768: [sbi] INFO: NF Service [nudm-sdm] (../lib/sbi/context.c:1744)
10/02 10:09:40.768: [sbi] INFO: nghttp2_server() [http://127.0.0.12]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:40.769: [app] INFO: UDM initialize...done (../src/udm/app.c:31)

$ ./install/bin/open5gs-pcfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:43.335: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/pcf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:43.335: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:43.337: [metrics] INFO: metrics_server() [http://127.0.0.13]:9090 (../lib/metrics/prometheus/context.c:299)
10/02 10:09:43.340: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/02 10:09:43.340: [sbi] INFO: NF Service [npcf-am-policy-control] (../lib/sbi/context.c:1744)
10/02 10:09:43.340: [sbi] INFO: NF Service [npcf-smpolicycontrol] (../lib/sbi/context.c:1744)
10/02 10:09:43.340: [sbi] INFO: NF Service [npcf-policyauthorization] (../lib/sbi/context.c:1744)
10/02 10:09:43.340: [sbi] INFO: nghttp2_server() [http://127.0.0.13]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:43.340: [app] INFO: PCF initialize...done (../src/pcf/app.c:31)

$ ./install/bin/open5gs-nssfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:44.960: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nssf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:44.960: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nssf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:44.962: [sbi] INFO: NF Service [nnssf-nsselection] (../lib/sbi/context.c:1744)
10/02 10:09:44.962: [sbi] INFO: nghttp2_server() [http://127.0.0.14]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:44.962: [app] INFO: NSSF initialize...done (../src/nssf/app.c:31)

$ ./install/bin/open5gs-bsfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:47.413: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/bsf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:47.413: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/bsf.log' (../lib/app/ogs-init.c:132)
10/02 10:09:47.415: [sbi] INFO: NF Service [nbsf-management] (../lib/sbi/context.c:1744)
10/02 10:09:47.415: [sbi] INFO: nghttp2_server() [http://127.0.0.15]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:47.415: [app] INFO: BSF initialize...done (../src/bsf/app.c:31)

$ ./install/bin/open5gs-udrd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:49.159: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udr.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:49.159: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udr.log' (../lib/app/ogs-init.c:132)
10/02 10:09:49.162: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/02 10:09:49.162: [sbi] INFO: NF Service [nudr-dr] (../lib/sbi/context.c:1744)
10/02 10:09:49.162: [sbi] INFO: nghttp2_server() [http://127.0.0.20]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:09:49.162: [app] INFO: UDR initialize...done (../src/udr/app.c:31)

$ ./install/bin/open5gs-mmed
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:54.793: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:54.793: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/mme.log' (../lib/app/ogs-init.c:132)
10/02 10:09:54.796: [metrics] INFO: metrics_server() [http://127.0.0.2]:9090 (../lib/metrics/prometheus/context.c:299)
10/02 10:09:54.844: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (../lib/gtp/path.c:30)
10/02 10:09:54.844: [gtp] INFO: gtp_connect() [127.0.0.3]:2123 (../lib/gtp/path.c:60)
10/02 10:09:54.844: [mme] INFO: s1ap_server() [127.0.0.2]:36412 (../src/mme/s1ap-sctp.c:62)
10/02 10:09:54.844: [sctp] INFO: MME initialize...done (../src/mme/app-init.c:33)

$ ./install/bin/open5gs-sgwcd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:09:57.549: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwc.yaml' (../lib/app/ogs-init.c:129)
10/02 10:09:57.549: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwc.log' (../lib/app/ogs-init.c:132)
10/02 10:09:57.607: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (../lib/gtp/path.c:30)
10/02 10:09:57.607: [pfcp] INFO: pfcp_server() [127.0.0.3]:8805 (../lib/pfcp/path.c:30)
10/02 10:09:57.607: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.6]:8805 (../lib/pfcp/path.c:61)
10/02 10:09:57.607: [app] INFO: SGW-C initialize...done (../src/sgwc/app.c:31)

$ ./install/bin/open5gs-sgwud
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:10:01.720: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwu.yaml' (../lib/app/ogs-init.c:129)
10/02 10:10:01.720: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwu.log' (../lib/app/ogs-init.c:132)
10/02 10:10:01.775: [pfcp] INFO: pfcp_server() [127.0.0.6]:8805 (../lib/pfcp/path.c:30)
10/02 10:10:01.775: [gtp] INFO: gtp_server() [127.0.0.6]:2152 (../lib/gtp/path.c:30)
10/02 10:10:01.776: [app] INFO: SGW-U initialize...done (../src/sgwu/app.c:31)

$ ./install/bin/open5gs-hssd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:10:05.765: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/hss.yaml' (../lib/app/ogs-init.c:129)
10/02 10:10:05.765: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/hss.log' (../lib/app/ogs-init.c:132)
10/02 10:10:05.766: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/02 10:10:05.853: [app] INFO: HSS initialize...done (../src/hss/app-init.c:31)

$ ./install/bin/open5gs-pcrfd
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:10:07.843: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/pcrf.yaml' (../lib/app/ogs-init.c:129)
10/02 10:10:07.843: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcrf.log' (../lib/app/ogs-init.c:132)
10/02 10:10:07.844: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:130)
10/02 10:10:07.879: [app] INFO: PCRF initialize...done (../src/pcrf/app-init.c:31)
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
$ ./build/tests/app/epc -c ./build/configs/srsenb.yaml ## EPC Only with ./build/configs/srsenb.yaml
$ ./build/tests/app/app ## Both 5G Core and EPC with ./build/configs/sample.yaml
```

### Building the WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to build WebUI of Open5GS

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

Connect to `http://127.0.0.1:9999` and login with **admin** account.

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
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:13:57.009: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/sample.yaml' (../lib/app/ogs-init.c:129)
10/02 10:13:57.009: [thread] DEBUG: [0x560e00a20420] worker signal (../lib/core/ogs-thread.c:66)
10/02 10:13:57.009: [thread] DEBUG: [0x560e00a20420] thread started (../lib/core/ogs-thread.c:104)
Open5GS daemon v2.6.4-59-g204ad57+

10/02 10:13:57.015: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/sample.yaml' (../lib/app/ogs-init.c:129)
10/02 10:13:57.017: [sock] DEBUG: addr:127.0.0.10, port:7777 (../lib/core/ogs-sockaddr.c:143)
10/02 10:13:57.017: [sock] DEBUG: socket create(2:1:6) (../lib/core/ogs-socket.c:97)
10/02 10:13:57.017: [sock] DEBUG: Turn on TCP_NODELAY (../lib/core/ogs-sockopt.c:139)
10/02 10:13:57.017: [sock] DEBUG: Turn on SO_REUSEADDR (../lib/core/ogs-sockopt.c:120)
10/02 10:13:57.017: [sock] DEBUG: socket bind 127.0.0.10:7777 (../lib/core/ogs-socket.c:122)
10/02 10:13:57.017: [sock] DEBUG: tcp_server() [127.0.0.10]:7777 (../lib/core/ogs-tcp.c:60)
10/02 10:13:57.017: [sbi] INFO: nghttp2_server() [http://127.0.0.10]:7777 (../lib/sbi/nghttp2-server.c:414)
10/02 10:13:57.017: [thread] DEBUG: [0x559919881ac0] worker signal (../lib/core/ogs-thread.c:66)
10/02 10:13:57.017: [nrf] DEBUG: nrf_state_initial(): INIT (../src/nrf/nrf-sm.c:25)
10/02 10:13:57.017: [nrf] DEBUG: nrf_state_operational(): ENTRY (../src/nrf/nrf-sm.c:52)
10/02 10:13:57.017: [thread] DEBUG: [0x559919881ac0] thread started (../lib/core/ogs-thread.c:104)
10/02 10:13:57.017: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)
10/02 10:13:57.060: [thread] DEBUG: [0x560e00a3a880] worker signal (../lib/core/ogs-thread.c:66)
10/02 10:13:57.060: [thread] DEBUG: [0x560e00a3a880] thread started (../lib/core/ogs-thread.c:104)
Open5GS daemon v2.6.4-59-g204ad57+
...

```
