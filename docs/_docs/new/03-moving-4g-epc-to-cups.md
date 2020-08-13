---
title: Moving 4G EPC to the CUPS
head_inline: "<style> .blue { color: blue; } </style>"
---

This post explains how to compile and install the source code on **Debian/Ubuntu** based Linux Distribution.
{: .blue}

### Getting MongoDB
---

Install MongoDB with package manager.  It is used as database for the HSS and PCRF.

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

Check whether the compilation is correct.
```bash
$ meson test
```

**Tip:** You can also check the result of `ninja -C build test` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || pfcp || gtp`.  You can see the virtually created packets. [[testcups.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testcups.pcapng)
{: .notice--info}

You need to perform the **installation process**.
```bash
$ cd build
$ ninja install
$ cd ../
```

### Configure Open5GS
---

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/mme.yaml.in) to set the S1AP IP address, PLMN ID, and TAC. 

In the below example we

- use MCC-MNC of 901-70, as this is the home network of the default IMSIs of the sysmoUSIM-SJS1 cards.
- use 192.168.0.100 for the S1AP/GTP-U connection of MME/SGW-U to the eNB

```diff
diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.yaml	2018-04-15 19:53:10.000000000 +0900
@@ -204,20 +204,20 @@ logger:
 mme:
     freeDiameter: @sysconfdir@/freeDiameter/mme.conf
     s1ap:
-      addr: 127.0.0.1
+      addr: 192.168.0.100
     gtpc:
       addr: 127.0.0.1
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
-      tac: 1
+        mcc: 901
+        mnc: 70
+      tac: 7
     security:
         integrity_order : [ EIA1, EIA2, EIA0 ]
         ciphering_order : [ EEA0, EEA1, EEA2 ]

```

Modify [install/etc/open5gs/sgwu.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgwu.yaml.in) to set the GTP-U IP address.  
```diff
diff -u /etc/open5gs/sgwu.yaml.old /etc/open5gs/sgwu.yaml
--- sgwu.yaml.old	2018-04-15 18:30:25.000000000 +0900
+++ sgwu.yaml	2018-04-15 18:30:30.000000000 +0900
@@ -51,7 +51,7 @@ logger:
 #
 sgwu:
     gtpu:
-      addr: 127.0.0.7
+      addr: 192.168.0.100
     pfcp:
       addr: 127.0.0.7
```

If you modify the config files while Open5GS daemons are running, please restart them


### Running Open5GS
---

Specify the absolute path to the sharead library as follows.

```bash
$ echo $(cd $(dirname ./install/lib/x86_64-linux-gnu/) && pwd -P)/$(basename ./install/lib/x86_64-linux-gnu/)
/home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu
$ export LD_LIBRARY_PATH=/home/acetcom/Documents/git/open5gs/install/lib/x86_64-linux-gnu
$ ldd ./install/bin/open5gs-mmed
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
$ ./install/bin/open5gs-nrfd
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:34:39.277: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/nrf.yaml' (../src/main.c:54)
08/12 19:34:39.277: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/nrf.log' (../src/main.c:57)
08/12 19:34:39.278: [app] INFO: NRF initialize...done (../src/nrf/app.c:31)
08/12 19:34:39.278: [sbi] INFO: sbi_server() [127.0.0.1]:7777 (../lib/sbi/server.c:298)
08/12 19:34:39.278: [sbi] INFO: sbi_server() [::1]:7777 (../lib/sbi/server.c:298

$ ./install/bin/open5gs-mmed 
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:35:00.505: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../src/main.c:54)
08/12 19:35:00.506: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/mme.log' (../src/main.c:57)
08/12 19:35:00.560: [gtp] INFO: gtp_server() [127.0.0.1]:2123 (../lib/gtp/path.c:32)
08/12 19:35:00.560: [app] INFO: MME initialize...done (../src/mme/app-init.c:33)
08/12 19:35:00.560: [gtp] INFO: gtp_connect() [127.0.0.6]:2123 (../lib/gtp/path.c:59)
08/12 19:35:00.560: [mme] INFO: s1ap_server() [127.0.0.1]:36412 (../src/mme/s1ap-sctp.c:57)

$ ./install/bin/open5gs-sgwcd 
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:35:15.866: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwc.yaml' (../src/main.c:54)
08/12 19:35:15.866: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwc.log' (../src/main.c:57)
08/12 19:35:15.872: [app] INFO: SGW-C initialize...done (../src/sgwc/app.c:31)
08/12 19:35:15.873: [gtp] INFO: gtp_server() [127.0.0.6]:2123 (../lib/gtp/path.c:32)
08/12 19:35:15.873: [pfcp] INFO: pfcp_server() [127.0.0.6]:8805 (../lib/pfcp/path.c:32)
08/12 19:35:15.873: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.7]:8805 (../lib/pfcp/path.c:60)

$ ./install/bin/open5gs-sgwud 
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:35:37.768: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/sgwu.yaml' (../src/main.c:54)
08/12 19:35:37.768: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgwc.log' (../src/main.c:57)
08/12 19:35:37.771: [pfcp] INFO: pfcp_server() [127.0.0.7]:8805 (../lib/pfcp/path.c:32)
08/12 19:35:37.771: [app] INFO: SGW-U initialize...done (../src/sgwu/app.c:31)
08/12 19:35:37.773: [gtp] INFO: gtp_server() [127.0.0.7]:2152 (../lib/gtp/path.c:32)

$ ./install/bin/open5gs-smfd 
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:35:50.948: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/smf.yaml' (../src/main.c:54)
08/12 19:35:50.948: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/smf.log' (../src/main.c:57)
08/12 19:35:50.999: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (../lib/gtp/path.c:32)
08/12 19:35:50.999: [app] INFO: SMF initialize...done (../src/smf/app.c:31)
08/12 19:35:50.999: [gtp] INFO: gtp_server() [::1]:2123 (../lib/gtp/path.c:32)
08/12 19:35:50.999: [pfcp] INFO: pfcp_server() [127.0.0.3]:8805 (../lib/pfcp/path.c:32)
08/12 19:35:50.999: [pfcp] INFO: pfcp_server() [::1]:8805 (../lib/pfcp/path.c:32)
08/12 19:35:50.999: [pfcp] INFO: ogs_pfcp_connect() [127.0.0.4]:8805 (../lib/pfcp/path.c:60)
08/12 19:35:50.999: [sbi] INFO: sbi_server() [127.0.0.3]:7777 (../lib/sbi/server.c:298)

$ ./install/bin/open5gs-upfd 
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:36:03.980: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/upf.yaml' (../src/main.c:54)
08/12 19:36:03.981: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/upf.log' (../src/main.c:57)
08/12 19:36:03.984: [pfcp] INFO: pfcp_server() [127.0.0.4]:8805 (../lib/pfcp/path.c:32)
08/12 19:36:03.984: [app] INFO: UPF initialize...done (../src/upf/app.c:31)
08/12 19:36:03.984: [gtp] INFO: gtp_server() [127.0.0.4]:2152 (../lib/gtp/path.c:32)
08/12 19:36:03.984: [gtp] INFO: gtp_server() [::1]:2152 (../lib/gtp/path.c:32)

$ ./install/bin/open5gs-hssd
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:36:15.670: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/hss.yaml' (../src/main.c:54)
08/12 19:36:15.670: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/hss.log' (../src/main.c:57)
08/12 19:36:15.671: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
08/12 19:36:15.693: [app] INFO: HSS initialize...done (../src/hss/app-init.c:31)

$  ./install/bin/open5gs-pcrfd 
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:36:30.356: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/pcrf.yaml' (../src/main.c:54)
08/12 19:36:30.356: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcrf.log' (../src/main.c:57)
08/12 19:36:30.358: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
08/12 19:36:30.387: [app] INFO: PCRF initialize...done (../src/pcrf/app-init.c:31)
```

Several command line options are provided.

```bash
$ ./install/bin/open5gs-mmed -h
Usage: ./install/bin/open5gs-mmed [options]
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

For convenience, you can execute all network elements at once by using the following command.
```bash
$ cd build
$ vi configs/app.yaml ## check the configuration
$ ./test/app/app ## run all network elements
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

### Turn on your eNodeB and Phone
---

- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `$INSTALL_PREFIX/var/log/open5gs/*.log`.

### Troubleshooting
---

Debugging tools can help you troubleshoot problems.

- [GDB](https://www.gnu.org/software/gdb/) can be used as below:

  ```bash
  $ gdb ./open5gs-mmed
  ```

- On *Mac OS X*, you can use the [LLDB](https://lldb.llvm.org/).

  ```bash
  $ lldb ./open5gs-mmed
  ```

You can use the command line option[`-d`] to record more logs.

```bash
$ ./install/bin/open5gs-mmed -d
Open5GS daemon v1.3.0-194-gc674984+

08/12 19:38:40.968: [app] INFO: Configuration: '/home/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../src/main.c:54)
08/12 19:38:40.968: [app] INFO: File Logging: '/home/acetcom/Documents/git/open5gs/install/var/log/open5gs/mme.log' (../src/main.c:57)
08/12 19:38:40.968: [app] INFO: LOG-LEVEL: 'debug' (../src/main.c:60)
08/12 19:38:41.076: [thread] DEBUG: [0x7ff5d26adb18] worker signal (../lib/core/ogs-thread.c:66)
08/12 19:38:41.076: [mme] DEBUG: mme_state_initial(): INIT
 (../src/mme/mme-sm.c:82)
08/12 19:38:41.076: [thread] DEBUG: [0x7ff5d26adb18] thread started (../lib/core/ogs-thread.c:101)
08/12 19:38:41.076: [mme] DEBUG: mme_state_operational(): ENTRY
 (../src/mme/mme-sm.c:128)
08/12 19:38:41.076: [app] INFO: MME initialize...done (../src/mme/app-init.c:33)
08/12 19:38:41.076: [sock] DEBUG: socket create(2:2:17) (../lib/core/ogs-socket.c:92)
08/12 19:38:41.076: [sock] DEBUG: udp_socket() family:2 (../lib/core/ogs-udp.c:31)
08/12 19:38:41.076: [sock] DEBUG: socket bind 127.0.0.1:2123 (../lib/core/ogs-socket.c:117)
08/12 19:38:41.076: [sock] DEBUG: udp_server() [127.0.0.1]:2123 (../lib/core/ogs-udp.c:55)
08/12 19:38:41.076: [gtp] INFO: gtp_server() [127.0.0.1]:2123 (../lib/gtp/path.c:32)
08/12 19:38:41.076: [gtp] INFO: gtp_connect() [127.0.0.6]:2123 (../lib/gtp/path.c:59)
08/12 19:38:41.076: [sock] DEBUG: socket create(2:1:132) (../lib/core/ogs-socket.c:92)
08/12 19:38:41.076: [sock] DEBUG: socket bind 127.0.0.1:36412 (../lib/core/ogs-socket.c:117)
08/12 19:38:41.076: [sock] DEBUG: sctp_server() [127.0.0.1]:36412 (../lib/sctp/ogs-lksctp.c:100)
08/12 19:38:41.076: [mme] INFO: s1ap_server() [127.0.0.1]:36412 (../src/mme/s1ap-sctp.c:57)
```
