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

You need to perform the **installation process**.
```bash
➜  nextepc git:(master) ✗ make install
```

Check whether the installation is correct.
```bash
➜  nextepc git:(master) ✗ ./test/testcomplex
s1setup_test        : SUCCESS
attach_test         : SUCCESS
volte_test          : SUCCESS
handover_test       : SUCCESS
All tests passed.
```

**Tip:** You can also check the result of `./test/testcomplex` with a tool that captures packets. If you are running `wireshark`, select the `loopback` interface and set FILTER to `s1ap || gtpv2 || diameter || gtp`.  You can see the virtually created packets. [[testcomplex.pcapng]]({{ site.url }}{{ site.baseurl }}/assets/pcapng/testcomplex.pcapng)
{: .notice--info}

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
04/06 23:13:03.367: [core] INFO: NextEPC daemon start (main.c:169)

PID[6404]: '/home/acetcom/Documents/git/open5gs/nextepc/install/var/run/nextepc-epcd/pid'
File Logging: '/home/acetcom/Documents/git/open5gs/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
Configuration: '/home/acetcom/Documents/git/open5gs/nextepc/install/etc/nextepc/nextepc.conf'
04/06 23:13:03.369: [core] INFO: PCRF try to initialize (epc.c:37)
...
```

Several command line options are provided.

```bash
➜  nextepc git:(master) ✗ ./nextepc-epcd -h
Password:
NextEPC daemon v0.4.0.67-078c - Apr  6 2019 17:20:24
Usage: ./nextepc-epcd [arguments]

Arguments:
   -v                   Show version
   -h                   Show help
   -D                   Start as daemon
   -f                   Set configuration file name
   -l log_file          Log file path to be logged to
   -p pid_file          PID file path
   -d core:gtp:event    Enable debugging
   -t sock:mem:         Enable trace
```


### Building WebUI of NextEPC
---

[Node.js](https://nodejs.org/) is required to build WebUI of NextEPC

```bash
$ sudo apt install curl
$ curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
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
$ sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
$ sudo iptables -t nat -A POSTROUTING -o 'interface-name' -j MASQUERADE
$ sudo iptables -I INPUT -i pgwtun -j ACCEPT
```

**Note:** In the above command, you should replace `'interface-name'` with your interface name that can connect to the internet. (For example, `enp0s25`, `wls3`, and so on).
{: .notice--danger}

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
04/07 16:46:23.982: [core] INFO: NextEPC daemon start (main.c:169)

PID[5185]: '/Users/acetcom/Documents/git/open5gs/nextepc/install/var/run/nextepc-epcd/pid'
File Logging: '/Users/acetcom/Documents/git/open5gs/nextepc/install/var/log/nextepc/nextepc.log'
MongoDB URI: 'mongodb://localhost/nextepc'
Configuration: '/Users/acetcom/Documents/git/open5gs/nextepc/install/etc/nextepc/nextepc.conf'
04/07 16:46:23.996: [core] INFO: PCRF try to initialize (epc.c:37)
04/07 16:46:24.033: [core] INFO: PCRF initialize...done (epc.c:40)
04/07 16:46:24.035: [core] INFO: PGW try to initialize (epc.c:84)
04/07 16:46:24.132: [thread] DEBUG: [0x10d4df908] thread started (ogs-thread.c:101)
04/07 16:46:24.132: [thread] DEBUG: [0x10d4df908] worker signal (ogs-thread.c:66)
04/07 16:46:24.132: [fd] INFO: CONNECTED TO 'pgw.localdomain' (TCP,soc#11): (fd_logger.c:113)
04/07 16:46:24.133: [core] INFO: PGW initialize...done (epc.c:87)
04/07 16:46:24.133: [pgw] DEBUG: pgw_state_initial(): INIT (pgw_sm.c:15)
04/07 16:46:24.133: [fd] INFO: CONNECTED TO 'pcrf.localdomain' (TCP,soc#11): (fd_logger.c:113)
04/07 16:46:24.134: [pgw] DEBUG: pgw_state_operational(): ENTRY (pgw_sm.c:33)
04/07 16:46:24.135: [core] INFO: SGW try to initialize (epc.c:133)
04/07 16:46:24.136: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
04/07 16:46:24.136: [sock] DEBUG: udp socket(2) (ogs-udp.c:32)
04/07 16:46:24.137: [sock] DEBUG: socket bind 127.0.0.3:2123 (ogs-socket.c:107)
04/07 16:46:24.138: [sock] DEBUG: udp_server() [127.0.0.3]:2123 (ogs-udp.c:55)
04/07 16:46:24.139: [gtp] INFO: gtp_server() [127.0.0.3]:2123 (gtp_path.c:35)
04/07 16:46:24.140: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
04/07 16:46:24.140: [sock] DEBUG: udp socket(2) (ogs-udp.c:32)
04/07 16:46:24.141: [sock] DEBUG: socket bind 127.0.0.3:2152 (ogs-socket.c:107)
04/07 16:46:24.142: [sock] DEBUG: udp_server() [127.0.0.3]:2152 (ogs-udp.c:55)
04/07 16:46:24.143: [gtp] INFO: gtp_server() [127.0.0.3]:2152 (gtp_path.c:35)
04/07 16:46:24.158: [thread] DEBUG: [0x10d4df408] worker signal (ogs-thread.c:66)
04/07 16:46:24.158: [thread] DEBUG: [0x10d4df408] thread started (ogs-thread.c:101)
04/07 16:46:24.159: [sgw] DEBUG: sgw_state_initial(): INIT
 (sgw_sm.c:12)
04/07 16:46:24.159: [core] INFO: SGW initialize...done (epc.c:136)
04/07 16:46:24.160: [sgw] DEBUG: sgw_state_operational(): ENTRY
 (sgw_sm.c:30)
04/07 16:46:24.161: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
04/07 16:46:24.162: [sock] DEBUG: udp socket(2) (ogs-udp.c:32)
04/07 16:46:24.162: [sock] DEBUG: socket bind 127.0.0.2:2123 (ogs-socket.c:107)
04/07 16:46:24.162: [core] INFO: HSS try to initialize (epc.c:184)
04/07 16:46:24.163: [sock] DEBUG: udp_server() [127.0.0.2]:2123 (ogs-udp.c:55)
04/07 16:46:24.164: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (gtp_path.c:35)
04/07 16:46:24.164: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
04/07 16:46:24.165: [sock] DEBUG: udp socket(2) (ogs-udp.c:32)
04/07 16:46:24.166: [sock] DEBUG: socket bind 192.168.0.3:2152 (ogs-socket.c:107)
04/07 16:46:24.166: [sock] DEBUG: udp_server() [192.168.0.3]:2152 (ogs-udp.c:55)
04/07 16:46:24.167: [gtp] INFO: gtp_server() [192.168.0.3]:2152 (gtp_path.c:35)
04/07 16:46:24.254: [core] INFO: HSS initialize...done (epc.c:187)
04/07 16:46:24.255: [core] INFO: MME try to initialize (epc.c:217)
04/07 16:46:24.366: [fd] INFO: CONNECTED TO 'mme.localdomain' (TCP,soc#9): (fd_logger.c:113)
04/07 16:46:24.367: [fd] INFO: CONNECTED TO 'hss.localdomain' (TCP,soc#17): (fd_logger.c:113)
04/07 16:46:24.367: [thread] DEBUG: [0x10d4dfe08] thread started (ogs-thread.c:101)
04/07 16:46:24.367: [core] INFO: MME initialize...done (epc.c:220)
04/07 16:46:24.367: [thread] DEBUG: [0x10d4dfe08] worker signal (ogs-thread.c:66)


NextEPC daemon v0.4.0.67-078c - Apr  6 2019 17:20:24

04/07 16:46:24.368: [mme] DEBUG: mme_state_initial(): INIT
 (mme_sm.c:23)
04/07 16:46:24.368: [mme] DEBUG: mme_state_operational(): ENTRY
 (mme_sm.c:43)
04/07 16:46:24.368: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
04/07 16:46:24.368: [sock] DEBUG: udp socket(2) (ogs-udp.c:32)
04/07 16:46:24.368: [sock] DEBUG: socket bind 192.168.0.3:2123 (ogs-socket.c:107)
04/07 16:46:24.368: [sock] DEBUG: udp_server() [192.168.0.3]:2123 (ogs-udp.c:55)
04/07 16:46:24.368: [gtp] INFO: gtp_server() [192.168.0.3]:2123 (gtp_path.c:35)
04/07 16:46:24.368: [sock] DEBUG: socket create(2:2:17) (ogs-socket.c:82)
04/07 16:46:24.368: [sock] DEBUG: udp socket(2) (ogs-udp.c:32)
04/07 16:46:24.368: [sock] DEBUG: socket connect 127.0.0.2:2123
 (ogs-socket.c:132)
04/07 16:46:24.368: [sock] DEBUG: udp_client() [127.0.0.2]:2123 (ogs-udp.c:89)
04/07 16:46:24.368: [gtp] INFO: gtp_client() [127.0.0.2]:2123 (gtp_path.c:49)
04/07 16:46:24.368: [mme] DEBUG: Old INITMSG (numout:10 maxin:2048 maxattempt:8 maxinit_to:60000) (s1ap_usrsctp.c:283)
04/07 16:46:24.368: [mme] DEBUG: New INITMSG (numout:30 maxin:65535 maxattempt:4 maxinit_to:8000) (s1ap_usrsctp.c:311)
04/07 16:46:24.368: [mme] INFO: s1ap_server() [192.168.0.3]:36412 (s1ap_usrsctp.c:69)
04/07 16:46:24.609: [pgw] DEBUG: [PGW] PROTO:17 SRC:2d2d0001 2d2d0001 d683d683 010f2296 (pgw_ipfw.c:277)
04/07 16:46:24.610: [pgw] DEBUG: [PGW] HLEN:20  DST:2d2d0001 d683d683 010f2296 0053756b (pgw_ipfw.c:280)
04/07 16:46:25.611: [pgw] DEBUG: [PGW] PROTO:17 SRC:2d2d0001 2d2d0001 d683d683 010f3a6d (pgw_ipfw.c:277)
04/07 16:46:25.612: [pgw] DEBUG: [PGW] HLEN:20  DST:2d2d0001 d683d683 010f3a6d 0053756b (pgw_ipfw.c:280)

04/07 16:46:26.607: [pgw] DEBUG: [PGW] PROTO:17 SRC:2d2d0001 2d2d0001 d683d683 010fa451 (pgw_ipfw.c:277)
04/07 16:46:26.608: [pgw] DEBUG: [PGW] HLEN:20  DST:2d2d0001 d683d683 010fa451 0053756b (pgw_ipfw.c:280)
...
...
...
^C04/07 16:46:27.013: [core] INFO: SIGINT received (main.c:60)
04/07 16:46:27.013: [core] INFO: NextEPC daemon terminating... (main.c:185)
04/07 16:46:27.013: [core] INFO: DB-Client try to terminate (application.c:116)
04/07 16:46:27.019: [core] INFO: DB-Client terminate...done (application.c:118)
04/07 16:46:27.019: [core] INFO: MME try to terminate (epc.c:229)
04/07 16:46:27.019: [event] DEBUG: interrupt all (ogs-queue.c:260)
04/07 16:46:27.019: [thread] DEBUG: [0x10d4dfe08] thread running(1) (ogs-thread.c:111)
04/07 16:46:27.019: [mme] DEBUG: mme_state_operational(): EXIT
 (mme_sm.c:43)
04/07 16:46:27.020: [mme] DEBUG: mme_state_final(): INIT
 (mme_sm.c:32)
04/07 16:46:27.020: [thread] DEBUG: [0x10d4dfe08] worker done (ogs-thread.c:72)
04/07 16:46:27.021: [thread] DEBUG: [0x10d4dfe08] thread destroy (ogs-thread.c:123)
04/07 16:46:27.021: [thread] DEBUG: [0x10d4dfe08] thread join (ogs-thread.c:132)
04/07 16:46:27.021: [thread] DEBUG: [0x10d4dfe08] thread done (ogs-thread.c:138)
04/07 16:46:27.022: [fd] INFO: freeDiameter[6]: Initiating freeDiameter shutdown sequence (3) (fd_init.c:131)
04/07 16:46:27.148: [core] INFO: MME terminate...done (epc.c:231)
04/07 16:46:27.148: [core] INFO: HSS try to terminate (epc.c:194)
04/07 16:46:27.149: [fd] INFO: freeDiameter[6]: Initiating freeDiameter shutdown sequence (3) (fd_init.c:131)
04/07 16:46:27.257: [core] INFO: HSS terminate...done (epc.c:196)
04/07 16:46:27.257: [core] INFO: SGW try to terminate (epc.c:143)
04/07 16:46:27.258: [event] DEBUG: interrupt all (ogs-queue.c:260)
04/07 16:46:27.258: [thread] DEBUG: [0x10d4df408] thread running(1) (ogs-thread.c:111)
04/07 16:46:27.258: [sgw] DEBUG: sgw_state_operational(): EXIT
 (sgw_sm.c:30)
04/07 16:46:27.259: [sgw] DEBUG: sgw_state_final(): INIT
 (sgw_sm.c:21)
04/07 16:46:27.260: [thread] DEBUG: [0x10d4df408] worker done (ogs-thread.c:72)
04/07 16:46:27.260: [thread] DEBUG: [0x10d4df408] thread destroy (ogs-thread.c:123)
04/07 16:46:27.261: [thread] DEBUG: [0x10d4df408] thread join (ogs-thread.c:132)
04/07 16:46:27.261: [thread] DEBUG: [0x10d4df408] thread done (ogs-thread.c:138)
04/07 16:46:27.263: [core] INFO: SGW terminate...done (epc.c:145)
04/07 16:46:27.263: [core] INFO: PGW try to terminate (epc.c:94)
04/07 16:46:27.264: [event] DEBUG: interrupt all (ogs-queue.c:260)
04/07 16:46:27.265: [thread] DEBUG: [0x10d4df908] thread running(1) (ogs-thread.c:111)
04/07 16:46:27.265: [pgw] DEBUG: pgw_state_operational(): EXIT (pgw_sm.c:33)
04/07 16:46:27.266: [pgw] DEBUG: pgw_state_final(): INIT (pgw_sm.c:24)
04/07 16:46:27.267: [thread] DEBUG: [0x10d4df908] worker done (ogs-thread.c:72)
04/07 16:46:27.268: [thread] DEBUG: [0x10d4df908] thread destroy (ogs-thread.c:123)
04/07 16:46:27.269: [thread] DEBUG: [0x10d4df908] thread join (ogs-thread.c:132)
04/07 16:46:27.270: [thread] DEBUG: [0x10d4df908] thread done (ogs-thread.c:138)
04/07 16:46:27.271: [fd] INFO: freeDiameter[6]: Initiating freeDiameter shutdown sequence (3) (fd_init.c:131)
04/07 16:46:27.401: [core] INFO: PGW terminate...done (epc.c:96)
04/07 16:46:27.402: [core] INFO: PCRF try to terminate (epc.c:47)
04/07 16:46:27.403: [fd] INFO: freeDiameter[6]: Initiating freeDiameter shutdown sequence (3) (fd_init.c:131)
04/07 16:46:27.514: [core] INFO: PCRF terminate...done (epc.c:49)
```
