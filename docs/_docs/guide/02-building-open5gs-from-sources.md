---
title: Building Open5GS from Sources
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
$ sudo apt install python3-pip python3-setuptools python3-wheel ninja-build build-essential flex bison git libsctp-dev libgnutls28-dev libgcrypt-dev libssl-dev libidn11-dev libmongoc-dev libbson-dev libyaml-dev meson
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
$ ninja -C build test
```

**Tip:** You can also check the result of `ninja -C build test` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || gtp`.  You can see the virtually created packets. [[testsimple.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testsimple.pcapng)
{: .notice--info}

You need to perform the **installation process**.
```bash
$ cd build
$ ninja install
$ cd ../
```

### Configure Open5GS
---

Modify [install/etc/open5gs/mme.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/mme.yaml.in) to set the S1AP/GTP-C IP address, PLMN ID, and TAC. 

In the below example we

- use MCC-MNC of 901-70, as this is the home network of the default IMSIs of the sysmoUSIM-SJS1 cards.
- use 192.168.0.100 for the S1AP +GTP-U connection of MME/SGW to the eNB

```diff
diff -u /etc/open5gs/mme.yaml.old /etc/open5gs/mme.yaml
--- mme.yaml.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.yaml	2018-04-15 19:53:10.000000000 +0900
@@ -8,18 +8,20 @@ parameter:
 mme:
     freeDiameter: /etc/freeDiameter/mme.conf
     s1ap:
+      addr: 192.168.0.100
     gtpc:
+      addr: 192.168.0.100
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

Modify [install/etc/open5gs/sgw.yaml](https://github.com/{{ site.github_username }}/open5gs/blob/master/configs/open5gs/sgw.yaml.in) to set the GTP-U IP address.  
```diff
diff -u /etc/open5gs/sgw.yaml.old /etc/open5gs/sgw.yaml
--- sgw.yaml.old	2018-04-15 18:30:25.000000000 +0900
+++ sgw.yaml	2018-04-15 18:30:30.000000000 +0900
@@ -14,3 +14,4 @@
     gtpc:
       addr: 127.0.0.2
     gtpu:
+      addr: 192.168.0.100
```

If you modify the config files while Open5GS daemons are running, please restart them


### Running Open5GS
---

```bash
$ cd install/bin/
$ ./open5gs-pcrfd
Open5GS daemon v1.0.0

10/27 15:47:55.821: [app] INFO: Configuration: '/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/pcrf.yaml' (../src/main.c:54)
10/27 15:47:55.822: [app] INFO: File Logging: '/Users/acetcom/Documents/git/open5gs/install/var/log/open5gs/pcrf.log' (../src/main.c:57)
10/27 15:47:55.868: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
10/27 15:47:55.965: [app] INFO: PCRF initialize...done (../src/pcrf/app-init.c:31)

$ ./open5gs-pgwd
Open5GS daemon v1.0.0

10/27 15:48:11.198: [app] INFO: Configuration: '/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/pgw.yaml' (../src/main.c:54)
10/27 15:48:11.199: [app] INFO: File Logging: '/Users/acetcom/Documents/git/open5gs/install/var/log/open5gs/pgw.log' (../src/main.c:57)
10/27 15:48:11.292: [app] INFO: PGW initialize...done (../src/pgw/app-init.c:31)
10/27 15:48:11.293: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (../lib/gtp/path.c:32)
10/27 15:48:11.293: [gtp] INFO: gtp_server() [::1]:2123 (../lib/gtp/path.c:32)
10/27 15:48:11.293: [gtp] INFO: gtp_server() [127.0.0.3]:2152 (../lib/gtp/path.c:32)
10/27 15:48:11.293: [gtp] INFO: gtp_server() [::1]:2152 (../lib/gtp/path.c:32)

$ ./open5gs-sgwd
Open5GS daemon v1.0.0

10/27 15:48:21.526: [app] INFO: Configuration: '/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/sgw.yaml' (../src/main.c:54)
10/27 15:48:21.527: [app] INFO: File Logging: '/Users/acetcom/Documents/git/open5gs/install/var/log/open5gs/sgw.log' (../src/main.c:57)
10/27 15:48:21.533: [app] INFO: SGW initialize...done (../src/sgw/app-init.c:31)
10/27 15:48:21.537: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (../lib/gtp/path.c:32)
10/27 15:48:21.537: [gtp] INFO: gtp_server() [192.168.0.3]:2152 (../lib/gtp/path.c:32)

$ ./open5gs-hssd
Open5GS daemon v1.0.0

10/27 15:48:32.802: [app] INFO: Configuration: '/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/hss.yaml' (../src/main.c:54)
10/27 15:48:32.803: [app] INFO: File Logging: '/Users/acetcom/Documents/git/open5gs/install/var/log/open5gs/hss.log' (../src/main.c:57)
10/27 15:48:32.815: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:99)
10/27 15:48:32.861: [app] INFO: HSS initialize...done (../src/hss/app-init.c:31)

$ ./open5gs-mmed
Open5GS daemon v1.0.0

10/27 15:46:23.539: [app] INFO: Configuration: '/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../src/main.c:54)
10/27 15:46:23.540: [app] INFO: File Logging: '/Users/acetcom/Documents/git/open5gs/install/var/log/open5gs/mme.log' (../src/main.c:57)
10/27 15:46:23.682: [app] INFO: MME initialize...done (../src/mme/app-init.c:33)
10/27 15:46:23.682: [gtp] INFO: gtp_server() [192.168.0.3]:2123 (../lib/gtp/path.c:32)
10/27 15:46:23.683: [gtp] INFO: gtp_connect() [127.0.0.2]:2123 (../lib/gtp/path.c:57)
10/27 15:46:23.683: [mme] INFO: s1ap_server() [192.168.0.3]:36412 (../src/mme/s1ap-usrpath.c:46)
```

Several command line options are provided.

```bash
$ ./open5gs-mmed -h
Usage: ./open5gs-mmed [options]
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
Open5GS daemon v1.0.0

10/27 15:50:45.170: [app] INFO: Configuration: '/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml' (../src/main.c:54)
10/27 15:50:45.171: [app] INFO: File Logging: '/Users/acetcom/Documents/git/open5gs/install/var/log/open5gs/mme.log' (../src/main.c:57)
10/27 15:50:45.171: [app] INFO: LOG-LEVEL: 'debug' (../src/main.c:60)
10/27 15:50:45.267: [thread] DEBUG: [0x10b52ed08] thread started (../lib/core/ogs-thread.c:101)
10/27 15:50:45.267: [thread] DEBUG: [0x10b52ed08] worker signal (../lib/core/ogs-thread.c:66)
10/27 15:50:45.268: [app] INFO: MME initialize...done (../src/mme/app-init.c:33)
10/27 15:50:45.268: [mme] DEBUG: mme_state_initial(): INIT
 (../src/mme/mme-sm.c:38)
10/27 15:50:45.268: [mme] DEBUG: mme_state_operational(): ENTRY
 (../src/mme/mme-sm.c:83)
10/27 15:50:45.269: [sock] DEBUG: socket create(2:2:17) (../lib/core/ogs-socket.c:92)
10/27 15:50:45.269: [sock] DEBUG: udp_socket() family:2 (../lib/core/ogs-udp.c:31)
10/27 15:50:45.269: [sock] DEBUG: socket bind 192.168.0.3:2123 (../lib/core/ogs-socket.c:117)
10/27 15:50:45.269: [sock] DEBUG: udp_server() [192.168.0.3]:2123 (../lib/core/ogs-udp.c:55)
10/27 15:50:45.269: [gtp] INFO: gtp_server() [192.168.0.3]:2123 (../lib/gtp/path.c:32)
10/27 15:50:45.269: [gtp] INFO: gtp_connect() [127.0.0.2]:2123 (../lib/gtp/path.c:57)
10/27 15:50:45.270: [sctp] DEBUG: Old INITMSG (numout:10 maxin:2048 maxattempt:8 maxinit_to:60000) (../lib/sctp/ogs-usrsctp.c:132)
10/27 15:50:45.271: [sctp] DEBUG: New INITMSG (numout:30 maxin:65535 maxattempt:4 maxinit_to:8000) (../lib/sctp/ogs-usrsctp.c:152)
10/27 15:50:45.271: [sctp] DEBUG: sctp_bind() [192.168.0.3]:36412 (../lib/sctp/ogs-usrsctp.c:261)
10/27 15:50:45.271: [mme] INFO: s1ap_server() [192.168.0.3]:36412 (../src/mme/s1ap-usrpath.c:46)
```
