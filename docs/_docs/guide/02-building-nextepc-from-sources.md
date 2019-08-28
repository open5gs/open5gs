---
title: Building nextepc from Sources
head_inline: "<style> .blue { color: blue; } </style>"
---

This post explains how to compile and install the source code on **Debian/Ubuntu** based Linux Distribution.
{: .blue}

### Getting MongoDB
---

Install MongoDB with package manager.

```bash
$ sudo apt update
$ sudo apt install mongodb
$ sudo systemctl start mongodb (if '/usr/bin/mongod' is not running)
```

### Setting up TUN device (No persistent after rebooting)
---

Create the TUN device with the interface name `pgwtun`.

```bash
$ sudo ip tuntap add name pgwtun mode tun
$ sudo ip addr add 45.45.0.1/16 dev pgwtun
$ sudo ip addr add cafe::1/64 dev pgwtun
$ sudo ip link set pgwtun up
```

The script provided in [$GIT_REPO/support/network/restart.sh](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/network/restart.sh) makes it easy to configure the TUN device as follows:  
`$ sudo ./support/network/restart.sh`
{: .notice--info}

### Building NextEPC
---

Install the depedencies for building the source code.

```bash
$ sudo apt install autoconf libtool gcc pkg-config git flex bison libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev libidn11-dev libmongoc-dev libbson-dev libyaml-dev
```

Git clone with `--recursive` option.

```bash
➜  open5gs git clone --recursive https://github.com/{{ site.github_username }}/nextepc
```

To compile with autotools:

```bash
➜  open5gs cd nextepc
➜  nextepc git:(master) ✗ autoreconf -iv
➜  nextepc git:(master) ✗ ./configure --prefix=`pwd`/install
➜  nextepc git:(master) ✗ make -j `nproc`
```

Check whether the compilation is correct.
```bash
➜  nextepc git:(master) ✗ make check
```

**Tip:** You can also check the result of `make check` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || gtp`.  You can see the virtually created packets. [[testcomplex.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testcomplex.pcapng)
{: .notice--info}

You need to perform the **installation process**.
```bash
➜  nextepc git:(master) ✗ make install
```

### Configure NextEPC
---

**Note:** In the developer environment, all settings can be managed in one place, such as [$INSTALL_PREFIX/install/etc/nextepc/nextepc.conf](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/config/nextepc.conf.in).
{: .notice--danger}

Modify [$INSTALL_PREFIX/install/etc/nextepc/nextepc.conf](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/config/nextepc.conf.in) to set the S1AP/GTP-C/GTP-U IP address, PLMN ID, and TAC

```diff
diff -u ./install/etc/nextepc/nextepc.conf.old ./install/etc/nextepc/nextepc.conf
--- nextepc.conf.old	2018-04-15 18:28:31.000000000 +0900
+++ nextepc.conf	2018-04-15 19:53:10.000000000 +0900
@@ -74,6 +74,7 @@ mme:
 #      dev: eth0
 #
     s1ap:
+      addr: 192.168.0.100

 #
 #  <GTP-C Server>>
@@ -87,6 +88,7 @@ mme:
 #      - addr: ::1
 #
     gtpc:
+      addr: 192.168.0.100

 #
 #  <GUMMEI>
@@ -110,8 +112,8 @@ mme:
 #
     gummei:
       plmn_id:
-        mcc: 001
-        mnc: 01
+        mcc: 901
+        mnc: 70
       mme_gid: 2
       mme_code: 1

@@ -149,9 +151,9 @@ mme:
 #
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
@@ -242,6 +244,7 @@ sgw:
 #    gtpu:
 #
     gtpu:
+      addr: 192.168.0.100

 pgw:
     freeDiameter: pgw.conf
```

### Running NextEPC
---

For developers, it provides `nextepc-epcd` daemon that includes both *MME*, *SGW*, *PGW*, *HSS*, and *PCRF*.

```bash
➜  nextepc git:(master) ✗ ./nextepc-epcd
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:40:18.194: [core] INFO: MME try to initialize (epc.c:100)
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:40:18.439: [gtp] INFO: gtp_server() [192.168.0.3]:2123 (gtp-path.c:36)
08/25 20:40:18.439: [gtp] INFO: gtp_connect() [127.0.0.2]:2123 (gtp-path.c:61)
08/25 20:40:18.439: [mme] INFO: s1ap_server() [192.168.0.3]:36412 (s1ap-usrpath.c:47)
08/25 20:40:18.439: [core] INFO: MME initialize...done (epc.c:104)
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:40:18.517: [pcrf] INFO: PCRF initialize...done (pcrf.c:24)
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:40:18.606: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (gtp-path.c:36)
08/25 20:40:18.606: [gtp] INFO: gtp_server() [192.168.0.3]:2152 (gtp-path.c:36)
08/25 20:40:18.613: [sgw] INFO: SGW initialize...done (sgw.c:24)
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:40:18.706: [pgw] INFO: PGW initialize...done (pgw.c:24)
08/25 20:40:18.706: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (gtp-path.c:36)
08/25 20:40:18.707: [fd] INFO: CONNECTED TO 'pgw.localdomain' (TCP,soc#15): (fd-logger.c:113)
08/25 20:40:18.707: [gtp] INFO: gtp_server() [127.0.0.3]:2152 (gtp-path.c:36)
08/25 20:40:18.707: [fd] INFO: CONNECTED TO 'pcrf.localdomain' (TCP,soc#19): (fd-logger.c:113)
08/25 20:40:18.742: [fd] INFO: CONNECTED TO 'hss.localdomain' (TCP,soc#24): (fd-logger.c:113)
08/25 20:40:18.743: [fd] INFO: CONNECTED TO 'mme.localdomain' (TCP,soc#21): (fd-logger.c:113)
08/25 20:40:18.744: [hss] INFO: HSS initialize...done (hss.c:24)
...
```

Several command line options are provided.

```bash
➜  nextepc git:(master) ✗ ./nextepc-epcd -h
Usage: /Users/acetcom/Documents/git/nextepc/.libs/nextepc-epcd [options]
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


### Building WebUI of NextEPC
---

[Node.js](https://nodejs.org/) is required to build WebUI of NextEPC

```bash
$ sudo apt install curl
$ curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
$ sudo apt install nodejs
```

Install the dependencies to run WebUI

```bash
➜  nextepc git:(master) ✗ cd webui
➜  webui git:(master) ✗ npm install
```

The WebUI runs as an [npm](https://www.npmjs.com/) script.

```bash
➜  webui git:(master) ✗ npm run dev
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

**Tip:** This addition immediately affects NextEPC without restaring any daemon.
{: .notice--warning}

### Adding a route for UE to have internet connectivity
---

If your phone can connect to internet, you must run the following command in NextEPC-PGW installed host. 

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
$ sudo iptables -t nat -A POSTROUTING -s 45.45.0.0/16 ! -o pgwtun -j MASQUERADE
```

**Note:** It is a good condition if you do not have any rules in the IP/NAT tables. If a program such as docker has already set up a rule, you will need to add a rule differently.

### Turn on your eNodeB and Phone
---

- You can see actual traffic through wireshark -- [[srsenb.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/srsenb.pcapng).
- You can view the log at `$INSTALL_PREFIX/var/log/nextepc/*.log`.

### Troubleshooting
---

Debugging tools can help you troubleshoot problems.

- [GDB](https://www.gnu.org/software/gdb/) can be used as below:

  ```bash
  ➜  nextepc git:(master) ✗ sudo apt install gdb
  ➜  nextepc git:(master) ✗ ./libtool --mode=execute gdb ./nextepc-epcd
  ```

- On *Mac OS X*, you can use the [LLDB](https://lldb.llvm.org/).

  ```bash
  ➜  nextepc git:(master) ✗ ./libtool --mode=execute sudo lldb ./nextepc-epcd
  ```

You can use the command line option[`-d`] to record more logs.

```bash
➜  nextepc git:(master) ✗ ./nextepc-epcd -d
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
LOG-LEVEL: 'debug'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:41:53.302: [thread] DEBUG: [0x1022da008] thread started (ogs-thread.c:101)
08/25 20:41:53.302: [thread] DEBUG: [0x1022da008] worker signal (ogs-thread.c:66)
08/25 20:41:53.353: [thread] DEBUG: [0x1022da108] thread started (ogs-thread.c:101)
08/25 20:41:53.353: [thread] DEBUG: [0x1022da108] worker signal (ogs-thread.c:66)
08/25 20:41:53.408: [thread] DEBUG: [0x1022da208] thread started (ogs-thread.c:101)
08/25 20:41:53.408: [thread] DEBUG: [0x1022da208] worker signal (ogs-thread.c:66)
08/25 20:41:53.460: [thread] DEBUG: [0x1022da308] worker signal (ogs-thread.c:66)
08/25 20:41:53.461: [thread] DEBUG: [0x1022da308] thread started (ogs-thread.c:101)
08/25 20:41:53.516: [core] INFO: MME try to initialize (epc.c:100)
08/25 20:41:53.723: [thread] DEBUG: [0x1022db108] worker signal (ogs-thread.c:66)
08/25 20:41:53.723: [thread] DEBUG: [0x1022db108] thread started (ogs-thread.c:101)
08/25 20:41:53.723: [core] INFO: MME initialize...done (epc.c:104)
08/25 20:41:53.723: [mme] DEBUG: mme_state_initial(): INIT
 (mme-sm.c:43)
08/25 20:41:53.724: [mme] DEBUG: mme_state_operational(): ENTRY
 (mme-sm.c:88)
08/25 20:41:53.724: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
08/25 20:41:53.724: [sock] DEBUG: udp_socket() family:2 (ogs-udp.c:31)
08/25 20:41:53.724: [sock] DEBUG: socket bind 192.168.0.3:2123 (ogs-socket.c:107)
08/25 20:41:53.724: [sock] DEBUG: udp_server() [192.168.0.3]:2123 (ogs-udp.c:55)
08/25 20:41:53.724: [gtp] INFO: gtp_server() [192.168.0.3]:2123 (gtp-path.c:36)
08/25 20:41:53.724: [gtp] INFO: gtp_connect() [127.0.0.2]:2123 (gtp-path.c:61)
08/25 20:41:53.726: [core] DEBUG: Old INITMSG (numout:10 maxin:2048 maxattempt:8 maxinit_to:60000) (ogs-usrsctp.c:130)
08/25 20:41:53.726: [core] DEBUG: New INITMSG (numout:30 maxin:65535 maxattempt:4 maxinit_to:8000) (ogs-usrsctp.c:150)
08/25 20:41:53.726: [core] DEBUG: sctp_bind() [192.168.0.3]:36412 (ogs-usrsctp.c:259)
08/25 20:41:53.726: [mme] INFO: s1ap_server() [192.168.0.3]:36412 (s1ap-usrpath.c:47)
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
LOG-LEVEL: 'debug'
MongoDB URI: 'mongodb://localhost/nextepc'
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
LOG-LEVEL: 'debug'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:41:53.833: [thread] DEBUG: [0x105506608] worker signal (ogs-thread.c:66)
08/25 20:41:53.834: [thread] DEBUG: [0x105506608] thread started (ogs-thread.c:101)
08/25 20:41:53.834: [sgw] DEBUG: sgw_state_initial(): INIT
 (sgw-sm.c:31)
08/25 20:41:53.834: [sgw] INFO: SGW initialize...done (sgw.c:24)
08/25 20:41:53.834: [sgw] DEBUG: sgw_state_operational(): ENTRY
 (sgw-sm.c:55)
08/25 20:41:53.837: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
08/25 20:41:53.837: [sock] DEBUG: udp_socket() family:2 (ogs-udp.c:31)
08/25 20:41:53.837: [sock] DEBUG: socket bind 127.0.0.2:2123 (ogs-socket.c:107)
08/25 20:41:53.837: [sock] DEBUG: udp_server() [127.0.0.2]:2123 (ogs-udp.c:55)
08/25 20:41:53.837: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (gtp-path.c:36)
08/25 20:41:53.837: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
08/25 20:41:53.837: [sock] DEBUG: udp_socket() family:2 (ogs-udp.c:31)
08/25 20:41:53.837: [sock] DEBUG: socket bind 192.168.0.3:2152 (ogs-socket.c:107)
08/25 20:41:53.837: [sock] DEBUG: udp_server() [192.168.0.3]:2152 (ogs-udp.c:55)
08/25 20:41:53.837: [gtp] INFO: gtp_server() [192.168.0.3]:2152 (gtp-path.c:36)
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
LOG-LEVEL: 'debug'
MongoDB URI: 'mongodb://localhost/nextepc'
NextEPC daemon v0.5.2.12-0aae

Configuration: '/Users/acetcom/Documents/git/nextepc/install/etc/nextepc/nextepc.conf'
File Logging: '/Users/acetcom/Documents/git/nextepc/install/var/log/nextepc/nextepc.log'
LOG-LEVEL: 'debug'
MongoDB URI: 'mongodb://localhost/nextepc'
08/25 20:41:53.964: [pcrf] INFO: PCRF initialize...done (pcrf.c:24)
08/25 20:41:54.043: [thread] DEBUG: [0x100d7cb08] thread started (ogs-thread.c:101)
08/25 20:41:54.043: [thread] DEBUG: [0x100d7cb08] worker signal (ogs-thread.c:66)
08/25 20:41:54.044: [fd] INFO: CONNECTED TO 'pgw.localdomain' (TCP,soc#15): (fd-logger.c:113)
08/25 20:41:54.044: [pgw] DEBUG: pgw_state_initial(): INIT (pgw-sm.c:34)
08/25 20:41:54.044: [pgw] DEBUG: pgw_state_operational(): ENTRY (pgw-sm.c:63)
08/25 20:41:54.044: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
08/25 20:41:54.044: [sock] DEBUG: udp_socket() family:2 (ogs-udp.c:31)
08/25 20:41:54.044: [sock] DEBUG: socket bind 127.0.0.3:2123 (ogs-socket.c:107)
08/25 20:41:54.044: [sock] DEBUG: udp_server() [127.0.0.3]:2123 (ogs-udp.c:55)
08/25 20:41:54.044: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (gtp-path.c:36)
08/25 20:41:54.044: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
08/25 20:41:54.044: [sock] DEBUG: udp_socket() family:2 (ogs-udp.c:31)
08/25 20:41:54.045: [sock] DEBUG: socket bind 127.0.0.3:2152 (ogs-socket.c:107)
08/25 20:41:54.045: [sock] DEBUG: udp_server() [127.0.0.3]:2152 (ogs-udp.c:55)
08/25 20:41:54.045: [fd] INFO: CONNECTED TO 'pcrf.localdomain' (TCP,soc#19): (fd-logger.c:113)
08/25 20:41:54.045: [gtp] INFO: gtp_server() [127.0.0.3]:2152 (gtp-path.c:36)
08/25 20:41:54.046: [pgw] INFO: PGW initialize...done (pgw.c:24)
08/25 20:41:54.273: [hss] INFO: HSS initialize...done (hss.c:24)
08/25 20:41:54.274: [fd] INFO: CONNECTED TO 'hss.localdomain' (TCP,soc#24): (fd-logger.c:113)
08/25 20:41:54.275: [fd] INFO: CONNECTED TO 'mme.localdomain' (TCP,soc#21): (fd-logger.c:113)
08/25 20:41:54.686: [pgw] DEBUG: [PGW] PROTO:17 SRC:2d2d0001 2d2d0001 d683d683 010fd314 (pgw-ipfw.c:284)
08/25 20:41:54.686: [pgw] DEBUG: [PGW] HLEN:20  DST:2d2d0001 d683d683 010fd314 0053756b (pgw-ipfw.c:287)
08/25 20:41:55.688: [pgw] DEBUG: [PGW] PROTO:17 SRC:2d2d0001 2d2d0001 d683d683 010f98b5 (pgw-ipfw.c:284)
08/25 20:41:55.689: [pgw] DEBUG: [PGW] HLEN:20  DST:2d2d0001 d683d683 010f98b5 0053756b (pgw-ipfw.c:287)
```
