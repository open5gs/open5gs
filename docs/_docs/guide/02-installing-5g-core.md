---
title: Installing 5G Core from Sources
head_inline: "<style> .blue { color: blue; } </style>"
---

This post explains how to compile and install the source code on **Debian/Ubuntu** based Linux Distribution.
{: .blue}

### Getting MongoDB
---

Install MongoDB with package manager.  It is used as database for the UDR.

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
$ sudo ip addr add cafe::1/64 dev ogstun
$ sudo ip link set ogstun up
```

**Tip:** The script provided in [$GIT_REPO/misc/netconf.sh](https://github.com/{{ site.github_username }}/open5gs/blob/master/misc/netconf.sh) makes it easy to configure the TUN device as follows:  
`$ sudo ./misc/netconf.sh`
{: .notice--info}

### Building Open5GS
---

Install the dependencies for building the source code.

```bash
$ sudo apt install python3-pip python3-setuptools python3-wheel ninja-build build-essential flex bison git libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev libidn11-dev libmongoc-dev libbson-dev libyaml-dev libmicrohttpd-dev libcurl4-gnutls-dev meson
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

Check whether the compilation is correct in your environment.
```bash
$ cd build
$ ./test/registration/registration
```

**Tip:** You can also check the result of `./test/registration/registration` with a tool that captures packets. If you are running `wireshark`, select the `any` interface and set FILTER to `ngap || http || pfcp || gtp`.
{: .notice--info}

You need to perform the **installation process**.
```bash
$ cd build
$ ninja install
$ cd ../
```

### Configure Open5GS
---

Modify [install/etc/open5gs/amf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/amf.yaml.in) to set the NGAP IP address, PLMN ID, TAC and NSSAI. 

In the below example we

- use MCC-MNC of 901-70, as this is the home network of the default IMSIs of the sysmoUSIM-SJS1 cards.
- use 192.168.0.100 for the NGAP connection of AMF to the gNB

```diff
diff -u amf.yaml.old amf.yaml
--- amf.yaml.old	2020-06-21 23:34:14.643114779 -0400
+++ amf.yaml	2020-06-21 23:34:28.718482095 -0400
@@ -67,25 +67,25 @@
       - addr: 127.0.0.2
         port: 7777
     ngap:
+      - addr: 192.168.0.100
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
-        tac: 1
+          mcc: 901
+          mnc: 70
+        tac: 7
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

Modify [install/etc/open5gs/upf.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/upf.yaml.in) to set the GTP-U IP address.  
```diff
diff -u upf.yaml.old upf.yaml
--- upf.yaml.old	2020-06-21 23:35:54.378631781 -0400
+++ upf.yaml	2020-06-21 23:36:02.978245251 -0400
@@ -61,6 +61,7 @@
     pfcp:
       - addr: 127.0.0.4
     gtpu:
+      - addr: 192.168.0.100
     pdn:
```

If you modify the config files while Open5GS daemons are running, please restart them


### Running Open5GS
---

Specify the absolute path to the sharead library as follows.

```bash
$ echo $(cd $(dirname ./install/lib/x86_64-linux-gnu/) && pwd -P)/$(basename ./install/lib/x86_64-linux-gnu/)
/home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu
$ export LD_LIBRARY_PATH=/home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu
$ ldd ./install/bin/open5gs-amfd
...
	libogsapp.so.1 => /home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu/libogsapp.so.1 (0x00007f161ab51000)
	libogscore.so.1 => /home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu/libogscore.so.1 (0x00007f161a922000)
	libogssctp.so.1 => /home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu/libogssctp.so.1 (0x00007f161a71d000)
	libogss1ap.so.1 => /home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu/libogss1ap.so.1 (0x00007f161a519000)
...
```

If you want to set the shared library path permanently, you can use ldconfig.
```bash
$ sudo sh -c "echo /home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu > /etc/ld.so.conf.d/open5gs.conf"
$ sudo ldconfig
```

Now let's get started.

```bash
$ cd install/bin/
$ ./install/bin/open5gs-nrfd 
Open5GS daemon v1.3.0

06/21 23:37:46.749: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nrf.yaml' (../src/main.c:54)
06/21 23:37:46.749: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nrf.log' (../src/main.c:57)
06/21 23:37:46.749: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)
06/21 23:37:46.749: [sbi] INFO: sbi_server() [127.0.0.1]:7777 (../lib/sbi/server.c:291)
06/21 23:37:46.749: [sbi] INFO: sbi_server() [::1]:7777 (../lib/sbi/server.c:291


$ ./install/bin/open5gs-amfd 
Open5GS daemon v1.3.0

06/21 23:38:15.834: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/amf.yaml' (../src/main.c:54)
06/21 23:38:15.834: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/amf.log' (../src/main.c:57)
06/21 23:38:15.876: [app] INFO: AMF initialize...done (../src/amf/app.c:33)
06/21 23:38:15.876: [sbi] INFO: sbi_server() [127.0.0.2]:7777 (../lib/sbi/server.c:291)
06/21 23:38:15.877: [amf] INFO: ngap_server() [192.168.0.11]:38412 (../src/amf/ngap-sctp.c:56)
06/21 23:38:15.877: [amf] INFO: ngap_server() [172.19.0.1]:38412 (../src/amf/ngap-sctp.c:56)
06/21 23:38:15.877: [amf] INFO: ngap_server() [172.17.0.1]:38412 (../src/amf/ngap-sctp.c:56)
06/21 23:38:15.877: [amf] INFO: ngap_server() [2601:144:4100:a220:ee:f149:8f28:9572]:38412 (../src/amf/ngap-sctp.c:56)
06/21 23:38:15.877: [amf] INFO: ngap_server() [2601:144:4100:a220:c643:4b3a:b0ee:c446]:38412 (../src/amf/ngap-sctp.c:56)
06/21 23:38:15.877: [amf] INFO: [ce785804-b439-41ea-9c93-399fce5a75a5] NF registered (../src/amf/nf-sm.c:209)

$ ./install/bin/open5gs-smfd
Open5GS daemon v1.3.0

06/21 23:38:35.313: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/smf.yaml' (../src/main.c:54)
06/21 23:38:35.313: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/smf.log' (../src/main.c:57)
06/21 23:38:35.350: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (../lib/gtp/path.c:32)
06/21 23:38:35.350: [gtp] INFO: gtp_server() [::1]:2123 (../lib/gtp/path.c:32)
06/21 23:38:35.350: [app] INFO: SMF initialize...done (../src/smf/app.c:31)
06/21 23:38:35.350: [pfcp] INFO: pfcp_server() [127.0.0.3]:8805 (../lib/pfcp/path.c:32)
06/21 23:38:35.350: [pfcp] INFO: pfcp_server() [::1]:8805 (../lib/pfcp/path.c:32)
06/21 23:38:35.350: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.4]:8805 (../lib/pfcp/path.c:60)
06/21 23:38:35.351: [sbi] INFO: sbi_server() [127.0.0.3]:7777 (../lib/sbi/server.c:291)
06/21 23:38:35.352: [smf] INFO: [da15f3e2-b439-41ea-bd58-c3fe61467c94] NF registered (../src/smf/nf-sm.c:210)

$ ./install/bin/open5gs-upfd 
Open5GS daemon v1.3.0

06/21 23:38:52.832: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/upf.yaml' (../src/main.c:54)
06/21 23:38:52.832: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/upf.log' (../src/main.c:57)
06/21 23:38:52.844: [app] INFO: UPF initialize...done (../src/upf/app.c:31)
06/21 23:38:52.844: [pfcp] INFO: pfcp_server() [127.0.0.4]:8805 (../lib/pfcp/path.c:32)
06/21 23:38:52.844: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.3]:8805 (../lib/pfcp/path.c:60)
06/21 23:38:52.846: [gtp] INFO: gtp_server() [192.168.0.11]:2152 (../lib/gtp/path.c:32)
06/21 23:38:52.846: [gtp] INFO: gtp_server() [172.19.0.1]:2152 (../lib/gtp/path.c:32)
06/21 23:38:52.846: [gtp] INFO: gtp_server() [172.17.0.1]:2152 (../lib/gtp/path.c:32)
06/21 23:38:52.846: [gtp] INFO: gtp_server() [2601:144:4100:a220:ee:f149:8f28:9572]:2152 (../lib/gtp/path.c:32)
06/21 23:38:52.846: [gtp] INFO: gtp_server() [2601:144:4100:a220:c643:4b3a:b0ee:c446]:2152 (../lib/gtp/path.c:32)
06/21 23:38:52.848: [upf] INFO: PFCP associated (../src/upf/pfcp-sm.c:171)
06/21 23:38:53.358: [upf] WARNING: PFCP[REQ] has already been associated (../src/upf/pfcp-sm.c:201)

$ ./install/bin/open5gs-ausfd 
Open5GS daemon v1.3.0

06/21 23:39:11.641: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/ausf.yaml' (../src/main.c:54)
06/21 23:39:11.641: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/ausf.log' (../src/main.c:57)
06/21 23:39:11.641: [app] INFO: AUSF initialize...done (../src/ausf/app.c:31)
06/21 23:39:11.642: [sbi] INFO: sbi_server() [127.0.0.5]:7777 (../lib/sbi/server.c:291)
06/21 23:39:11.643: [ausf] INFO: [efbbbe5c-b439-41ea-b95c-039d7a609671] NF registered (../src/ausf/nf-sm.c:209)

$ ./install/bin/open5gs-udmd
Open5GS daemon v1.3.0

06/21 23:39:28.792: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udm.yaml' (../src/main.c:54)
06/21 23:39:28.792: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udm.log' (../src/main.c:57)
06/21 23:39:28.792: [app] INFO: UDM initialize...done (../src/udm/app.c:31)
06/21 23:39:28.793: [sbi] INFO: sbi_server() [127.0.0.6]:7777 (../lib/sbi/server.c:291)
06/21 23:39:29.390: [udm] INFO: [f9f4ce22-b439-41ea-bf29-c309f36c787f] NF registered (../src/udm/nf-sm.c:209)

$ ./install/bin/open5gs-udrd
Open5GS daemon v1.3.0

06/21 23:39:41.763: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/udr.yaml' (../src/main.c:54)
06/21 23:39:41.763: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/udr.log' (../src/main.c:57)
06/21 23:39:41.765: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
06/21 23:39:41.765: [app] INFO: UDR initialize...done (../src/udr/app.c:31)
06/21 23:39:41.765: [sbi] INFO: sbi_server() [127.0.0.7]:7777 (../lib/sbi/server.c:291)
06/21 23:39:41.767: [udr] INFO: [01b0191e-b43a-41ea-9abb-df3b42196e0a] NF registered (../src/udr/nf-sm.c:209)

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
$ cd build
$ vi configs/5gc.yaml ## check the configuration
$ ./test/app/5gc ## run all NFs
```

### Building the WebUI of Open5GS
---

[Node.js](https://nodejs.org/) is required to build WebUI of Open5GS

```bash
$ sudo apt install curl
$ curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
$ sudo apt install nodejs
```

Install the dependencies to run WebUI

```bash
$ cd webui
$ npm install
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
$ npm run dev
```

### Register Subscriber Information
---

Connect to `http://localhost:3000` and login with **admin** account.

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

### Enable IPv4 Forwarding
$ sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"

### Add NAT Rule
$ sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
```

**Note:** The above assumes you do not have any existing rules in the filter and nat tables. If a program such as docker has already set up rules, you may need to add the Open5GS related rules differently.
{: .notice--danger}

### Turn on your gNB and Phone
---

- You can view the log at `$INSTALL_PREFIX/var/log/open5gs/*.log`.

### Troubleshooting
---

If you run into any problems, please post the github issue. Providing packets using wireshark can help a lot.
{: .blue}

To fix the problem yourself, debugging tools can help you troubleshoot problems.

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
$ ./test/app/5gc -d
Open5GS daemon v1.3.0

06/21 23:44:54.500: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/5gc.yaml' (../src/main.c:54)
06/21 23:44:54.500: [thread] DEBUG: [0x7f3342a7d018] worker signal (../lib/core/ogs-thread.c:66)
06/21 23:44:54.500: [thread] DEBUG: [0x7f3342a7d018] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v1.3.0

06/21 23:44:54.508: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/5gc.yaml' (../src/main.c:54)
06/21 23:44:54.508: [thread] DEBUG: [0x7f31d3620818] worker signal (../lib/core/ogs-thread.c:66)
06/21 23:44:54.508: [nrf] DEBUG: nrf_state_initial(): INIT (../src/nrf/nrf-sm.c:25)
06/21 23:44:54.508: [nrf] DEBUG: nrf_state_operational(): ENTRY (../src/nrf/nrf-sm.c:50)
06/21 23:44:54.508: [thread] DEBUG: [0x7f31d3620818] thread started (../lib/core/ogs-thread.c:101)
06/21 23:44:54.508: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)
06/21 23:44:54.509: [sbi] INFO: sbi_server() [127.0.0.1]:7777 (../lib/sbi/server.c:291)
06/21 23:44:54.509: [sbi] INFO: sbi_server() [::1]:7777 (../lib/sbi/server.c:291)
06/21 23:44:54.551: [thread] DEBUG: [0x7f3342a7d098] worker signal (../lib/core/ogs-thread.c:66)
06/21 23:44:54.551: [thread] DEBUG: [0x7f3342a7d098] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v1.3.0

06/21 23:44:54.575: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/5gc.yaml' (../src/main.c:54)
06/21 23:44:54.601: [thread] DEBUG: [0x7f3342a7d118] worker signal (../lib/core/ogs-thread.c:66)
06/21 23:44:54.601: [thread] DEBUG: [0x7f3342a7d118] thread started (../lib/core/ogs-thread.c:101)
Open5GS daemon v1.3.0

06/21 23:44:54.607: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/build/configs/5gc.yaml' (../src/main.c:54)
06/21 23:44:54.608: [thread] DEBUG: [0x7f996e1b2918] worker signal (../lib/core/ogs-thread.c:66)
06/21 23:44:54.608: [ausf] DEBUG: ausf_state_initial(): INIT (../src/ausf/ausf-sm.c:25)
```
