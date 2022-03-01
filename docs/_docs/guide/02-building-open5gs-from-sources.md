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

Install MongoDB with package manager. It is used as database for NRF/PCF/UDR and PCRF/HSS.

```bash
$ sudo apt update
$ sudo apt install mongodb
$ sudo systemctl start mongodb (if '/usr/bin/mongod' is not running)
$ sudo systemctl enable mongodb (ensure to automatically start it on system boot)
```

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
$ sudo apt install python3-pip python3-setuptools python3-wheel ninja-build build-essential flex bison git libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev libidn11-dev libmongoc-dev libbson-dev libyaml-dev libnghttp2-dev libmicrohttpd-dev libcurl4-gnutls-dev libnghttp2-dev libtins-dev libtalloc-dev meson
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
--- amf.yaml	2020-09-05 20:52:28.652234967 -0400
+++ amf.yaml.new	2020-09-05 20:55:07.453114885 -0400
@@ -165,23 +165,23 @@
       - addr: 127.0.0.5
         port: 7777
     ngap:
-      - addr: 127.0.0.5
+      - addr: 10.10.0.5
     guami:
       - plmn_id:
-          mcc: 901
-          mnc: 70
+          mcc: 001
+          mnc: 01
         amf_id:
           region: 2
           set: 1
     tai:
       - plmn_id:
-          mcc: 901
-          mnc: 70
-        tac: 1
+          mcc: 001
+          mnc: 01
+        tac: 2
     plmn_support:
       - plmn_id:
-          mcc: 901
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
--- upf.yaml	2020-09-05 20:52:28.652234967 -0400
+++ upf.yaml.new	2020-09-05 20:52:55.279052142 -0400
@@ -137,9 +137,7 @@
     pfcp:
       - addr: 127.0.0.7
     gtpu:
-      - addr:
-        - 127.0.0.7
-        - ::1
+      - addr: 10.11.0.7
     subnet:
       - addr: 10.45.0.1/16
       - addr: 2001:db8:cafe::1/48
```
##### 4G EPC

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC.

```diff
$ diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml	2020-09-05 20:52:28.648235143 -0400
+++ mme.yaml.new	2020-09-05 20:56:05.434484208 -0400
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
-        mcc: 901
-        mnc: 70
+        mcc: 001
+        mnc: 01
       mme_gid: 2
       mme_code: 1
     tai:
       plmn_id:
-        mcc: 901
-        mnc: 70
-      tac: 1
+        mcc: 001
+        mnc: 01
+      tac: 2
     security:
         integrity_order : [ EIA1, EIA2, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]
```

Modify [install/etc/open5gs/sgwu.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/main/configs/open5gs/sgwu.yaml.in) to set the GTP-U IP address.
```diff
$ diff -u /etc/open5gs/sgwu.yaml.old /etc/open5gs/sgwu.yaml
--- sgwu.yaml	2020-09-05 20:50:39.393022566 -0400
+++ sgwu.yaml.new	2020-09-05 20:51:06.667838823 -0400
@@ -51,7 +51,7 @@
 #
 sgwu:
     gtpu:
-      addr: 127.0.0.6
+      addr: 10.11.0.6
     pfcp:
       addr: 127.0.0.6

```

If you modify the config files while Open5GS daemons are running, please restart them


### Running Open5GS
---

```bash
$ ./install/bin/open5gs-mmed
Open5GS daemon v2.1.0

08/21 22:53:47.328: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../src/main.c:54)
08/21 22:53:47.328: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/mme.log' (../src/main.c:57)
08/21 22:53:47.365: [app] INFO: MME initialize...done (../src/mme/app-init.c:33)
08/21 22:53:47.365: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (../lib/gtp/path.c:32)
08/21 22:53:47.365: [gtp] INFO: gtp_connect() [127.0.0.3]:2123 (../lib/gtp/path.c:59)
08/21 22:53:47.366: [mme] INFO: s1ap_server() [127.0.0.2]:36412 (../src/mme/s1ap-sctp.c:57)

$ ./install/bin/open5gs-sgwcd
Open5GS daemon v2.1.0

08/21 22:54:43.059: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwc.yaml' (../src/main.c:54)
08/21 22:54:43.059: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwc.log' (../src/main.c:57)
08/21 22:54:43.065: [app] INFO: SGW-C initialize...done (../src/sgwc/app.c:31)
08/21 22:54:43.066: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (../lib/gtp/path.c:32)
08/21 22:54:43.066: [pfcp] INFO: pfcp_server() [127.0.0.3]:8805 (../lib/pfcp/path.c:32)
08/21 22:54:43.066: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.6]:8805 (../lib/pfcp/path.c:60)

$ ./install/bin/open5gs-smfd
Open5GS daemon v2.1.0

08/21 22:54:56.000: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/smf.yaml' (../src/main.c:54)
08/21 22:54:56.000: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/smf.log' (../src/main.c:57)
08/21 22:54:56.050: [gtp] INFO: gtp_server() [127.0.0.4]:2123 (../lib/gtp/path.c:32)
08/21 22:54:56.050: [app] INFO: SMF initialize...done (../src/smf/app.c:31)
08/21 22:54:56.050: [gtp] INFO: gtp_server() [::1]:2123 (../lib/gtp/path.c:32)
08/21 22:54:56.050: [pfcp] INFO: pfcp_server() [127.0.0.4]:8805 (../lib/pfcp/path.c:32)
08/21 22:54:56.050: [pfcp] INFO: pfcp_server() [::1]:8805 (../lib/pfcp/path.c:32)
08/21 22:54:56.050: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.7]:8805 (../lib/pfcp/path.c:60)
08/21 22:54:56.051: [sbi] INFO: sbi_server() [127.0.0.4]:7777 (../lib/sbi/server.c:298)

$ ./install/bin/open5gs-amfd
Open5GS daemon v2.1.0

08/21 22:55:14.015: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/amf.yaml' (../src/main.c:54)
08/21 22:55:14.015: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/amf.log' (../src/main.c:57)
08/21 22:55:14.039: [app] INFO: AMF initialize...done (../src/amf/app.c:33)
08/21 22:55:14.040: [sbi] INFO: sbi_server() [127.0.0.5]:7777 (../lib/sbi/server.c:298)
08/21 22:55:14.040: [amf] INFO: ngap_server() [127.0.0.5]:38412 (../src/amf/ngap-sctp.c:56)

$ ./install/bin/open5gs-sgwud
Open5GS daemon v2.1.0

08/21 22:54:10.357: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwu.yaml' (../src/main.c:54)
08/21 22:54:10.357: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwu.log' (../src/main.c:57)
08/21 22:54:10.360: [pfcp] INFO: pfcp_server() [127.0.0.6]:8805 (../lib/pfcp/path.c:32)
08/21 22:54:10.360: [app] INFO: SGW-U initialize...done (../src/sgwu/app.c:31)
08/21 22:54:10.361: [gtp] INFO: gtp_server() [127.0.0.6]:2152 (../lib/gtp/path.c:32)

$ ./install/bin/open5gs-upfd
Open5GS daemon v2.1.0

08/21 22:54:21.596: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/upf.yaml' (../src/main.c:54)
08/21 22:54:21.596: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/upf.log' (../src/main.c:57)
08/21 22:54:21.601: [pfcp] INFO: pfcp_server() [127.0.0.7]:8805 (../lib/pfcp/path.c:32)
08/21 22:54:21.601: [app] INFO: UPF initialize...done (../src/upf/app.c:31)
08/21 22:54:21.601: [gtp] INFO: gtp_server() [127.0.0.7]:2152 (../lib/gtp/path.c:32)
08/21 22:54:21.601: [gtp] INFO: gtp_server() [::1]:2152 (../lib/gtp/path.c:32)

$ ./install/bin/open5gs-hssd
Open5GS daemon v2.1.0

08/21 22:57:17.450: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/hss.yaml' (../src/main.c:54)
08/21 22:57:17.450: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/hss.log' (../src/main.c:57)
08/21 22:57:17.451: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
08/21 22:57:17.519: [app] INFO: HSS initialize...done (../src/hss/app-init.c:31)

$ ./install/bin/open5gs-pcrfd
Open5GS daemon v2.1.0

08/21 22:57:45.894: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/pcrf.yaml' (../src/main.c:54)
08/21 22:57:45.894: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcrf.log' (../src/main.c:57)
08/21 22:57:45.896: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
08/21 22:57:45.997: [app] INFO: PCRF initialize...done (../src/pcrf/app-init.c:31)

$ ./install/bin/open5gs-nrfd
Open5GS daemon v2.1.0

08/21 22:56:35.472: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nrf.yaml' (../src/main.c:54)
08/21 22:56:35.472: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nrf.log' (../src/main.c:57)
08/21 22:56:35.472: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)
08/21 22:56:35.473: [sbi] INFO: sbi_server() [127.0.0.10]:7777 (../lib/sbi/server.c:298)
08/21 22:56:35.473: [sbi] INFO: sbi_server() [::1]:7777 (../lib/sbi/server.c:298

$ ./install/bin/open5gs-ausfd
Open5GS daemon v2.1.0

08/21 22:55:41.899: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/ausf.yaml' (../src/main.c:54)
08/21 22:55:41.899: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/ausf.log' (../src/main.c:57)
08/21 22:55:41.900: [app] INFO: AUSF initialize...done (../src/ausf/app.c:31)
08/21 22:55:41.900: [sbi] INFO: sbi_server() [127.0.0.11]:7777 (../lib/sbi/server.c:298)

$ ./install/bin/open5gs-udmd
Open5GS daemon v2.1.0

08/21 22:56:02.154: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udm.yaml' (../src/main.c:54)
08/21 22:56:02.154: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udm.log' (../src/main.c:57)
08/21 22:56:02.155: [app] INFO: UDM initialize...done (../src/udm/app.c:31)
08/21 22:56:02.155: [sbi] INFO: sbi_server() [127.0.0.12]:7777 (../lib/sbi/server.c:298)

$ ./install/bin/open5gs-pcfd
Open5GS daemon v2.1.0

08/21 22:56:02.154: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/pcf.yaml' (../src/main.c:54)
08/21 22:56:02.154: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcf.log' (../src/main.c:57)
08/21 22:56:02.155: [app] INFO: PCF initialize...done (../src/pcf/app.c:31)
08/21 22:56:02.155: [sbi] INFO: sbi_server() [127.0.0.12]:7777 (../lib/sbi/server.c:298)

$ ./install/bin/open5gs-nssfd
Open5GS daemon v2.1.0

08/21 22:56:02.154: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nssfd.yaml' (../src/main.c:54)
08/21 22:56:02.154: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nssfd.log' (../src/main.c:57)
08/21 22:56:02.155: [app] INFO: NSSF initialize...done (../src/nssfd/app.c:31)
08/21 22:56:02.155: [sbi] INFO: sbi_server() [127.0.0.12]:7777 (../lib/sbi/server.c:298)

$ ./install/bin/open5gs-bsfd
Open5GS daemon v2.1.0

08/21 22:56:02.154: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/bsf.yaml' (../src/main.c:54)
08/21 22:56:02.154: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/bsf.log' (../src/main.c:57)
08/21 22:56:02.155: [app] INFO: BSF initialize...done (../src/bsf/app.c:31)
08/21 22:56:02.155: [sbi] INFO: sbi_server() [127.0.0.12]:7777 (../lib/sbi/server.c:298)

$ ./install/bin/open5gs-udrd
Open5GS daemon v2.1.0

08/21 22:56:15.810: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udr.yaml' (../src/main.c:54)
08/21 22:56:15.810: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udr.log' (../src/main.c:57)
08/21 22:56:15.813: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
08/21 22:56:15.813: [app] INFO: UDR initialize...done (../src/udr/app.c:31)
08/21 22:56:15.813: [sbi] INFO: sbi_server() [127.0.0.13]:7777 (../lib/sbi/server.c:298)
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
$ curl -fsSL https://deb.nodesource.com/setup_14.x | sudo -E bash -
$ sudo apt install nodejs
```

Install the dependencies to run WebUI

```bash
$ cd webui
$ npm ci --no-optional
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

**Tip:** This addition immediately affects Open5GS without restarting any daemon.
{: .notice--warning}

### IP routing + NAT for UE internet connectivity
---

To allow your phones to connect to the internet, you must run the following command on the host running Open5GS-PGW:

```bash
### Check IP Table 'forward'
$ sudo iptables -L
Chain INPUT (policy ACCEPT)
target     prot opt source               destination

Chain FORWARD (policy ACCEPT)
target     prot opt source               destination

Chain OUTPUT (policy ACCEPT)
target     prot opt source               destination

### Check IP Table 'nat'
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

**Note:** The above assumes you do not have any existing rules in the filter and nat tables. If a program such as docker has already set up rules, you may need to add the Open5GS related rules differently.
{: .notice--danger}

### Turn on your eNodeB and Phone
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
Open5GS daemon v1.3.0-213-gd190548+

08/21 23:01:54.246: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/sample.yaml' (../src/main.c:54)
08/21 23:01:54.246: [thread] DEBUG: [0x7f8de4d25018] worker signal (../lib/core/ogs-thread.c:66)
08/21 23:01:54.246: [thread] DEBUG: [0x7f8de4d25018] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v1.3.0-213-gd190548+

08/21 23:01:54.254: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/sample.yaml' (../src/main.c:54)
08/21 23:01:54.254: [thread] DEBUG: [0x7ff8d13be818] worker signal (../lib/core/ogs-thread.c:66)
08/21 23:01:54.254: [nrf] DEBUG: nrf_state_initial(): INIT (../src/nrf/nrf-sm.c:25)
08/21 23:01:54.254: [nrf] DEBUG: nrf_state_operational(): ENTRY (../src/nrf/nrf-sm.c:50)
08/21 23:01:54.254: [thread] DEBUG: [0x7ff8d13be818] thread started (../lib/core/ogs-thread.c:101)
08/21 23:01:54.254: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)
08/21 23:01:54.255: [sbi] INFO: sbi_server() [127.0.0.10]:7777 (../lib/sbi/server.c:298)
08/21 23:01:54.255: [sbi] INFO: sbi_server() [::1]:7777 (../lib/sbi/server.c:298)
08/21 23:01:54.296: [thread] DEBUG: [0x7f8de4d25098] worker signal (../lib/core/ogs-thread.c:66)
08/21 23:01:54.296: [thread] DEBUG: [0x7f8de4d25098] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v1.3.0-213-gd190548+

08/21 23:01:54.315: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/sample.yaml' (../src/main.c:54)
08/21 23:01:54.319: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
08/21 23:01:54.332: [app] INFO: PCRF initialize...done (../src/pcrf/app-init.c:31)
08/21 23:01:54.347: [thread] DEBUG: [0x7f8de4d25118] worker signal (../lib/core/ogs-thread.c:66)
08/21 23:01:54.347: [thread] DEBUG: [0x7f8de4d25118] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v1.3.0-213-gd190548+

...

```
