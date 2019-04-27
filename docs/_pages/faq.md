---
layout: page
title: FAQ
permalink: /faq/
---

#### How to connect MongoDB server

If you do not start MongoDB, you will get the following error:

```bash
➜  nextepc git:(master) ✗ ./test/testepc  
04/09 15:42:34.817: [core] ERROR: Failed to conect to server [mongodb://localhost/nextepc] (context.c:326)  
04/09 15:42:34.817: [core] ERROR: app_initialize() failed (basic/abts-main.c:91)  
s1ap_message_test   : -04/09 15:42:34.830: [s1ap] ERROR: Failed to decode S1AP-PDU[-1] (s1ap_encoder.c:41)  
04/09 15:42:34.830: [core] FATAL: ogs_log_vprintf: Assertion `domain' failed. (ogs-log.c:347)  
```

You can start MongoDB using systemctl.
```bash
$ sudo systemctl start mongodb
```

#### Failing to run `./nextepc-epcd`

You might be getting the following error after running `./nextepc-epcd`.
```bash
➜  nextepc git:(master) ./nextepc-epcd  
04/09 15:41:02.600: [core] INFO: NextEPC daemon start (main.c:169)  
04/09 15:41:02.601: [core] ERROR: CHECK PERMISSION of Installation Directory... (application.c:144)  
04/09 15:41:02.601: [core] ERROR: Cannot create PID file:`/home/acetcom/Documents/git/open5gs/nextepc/install/var/run/nextepc-epcd/pid` (application.c:145)  
04/09 15:41:02.601: [core] WARNING: log_pid: should not be reached. (application.c:146)  
/home/acetcom/Documents/git/open5gs/nextepc/lib/ogslib/src/core/.libs/libogscore-1.0.so.0(ogs_abort+0x2b)[0x7f9d5d26d71b]  
/home/acetcom/Documents/git/open5gs/nextepc/.libs/nextepc-epcd(+0x9606)[0x563a4ba23606]  
/home/acetcom/Documents/git/open5gs/nextepc/.libs/nextepc-epcd(+0x8640)[0x563a4ba22640]  
/home/acetcom/Documents/git/open5gs/nextepc/.libs/nextepc-epcd(+0x81f3)[0x563a4ba221f3]  
[1]    9635 abort (core dumped)  ./nextepc-epcd
```

You should perform **the installation process**.

```bash
$ make install
```

#### I have some error when running `./test/testepc`

Did you see the following error after executing `testepc`?
```bash
➜  nextepc git:(master) ✗ ./test/testepc  
s1ap_message_test   : SUCCESS  
nas_message_test    : SUCCESS  
gtp_message_test    : SUCCESS  
security_test       : SUCCESS  
s1setup_test        : SUCCESS  
attach_test         : -Line 134: Condition is false, but expected true  
\04/09 15:49:09.285: [esm] FATAL: esm_handle_pdn_connectivity_request: Assertion `SECURITY_CONTEXT_IS_VALID(mme_ue)' failed. (esm_handler.c:29)  
/home/acetcom/Documents/git/open5gs/nextepc/lib/ogslib/src/core/.libs/libogscore-1.0.so.0(ogs_abort+0x2b)[0x7f608518271b]  
/home/acetcom/Documents/git/open5gs/nextepc/test/.libs/testepc(+0x92121)[0x55dc9e274121]  
/home/acetcom/Documents/git/open5gs/nextepc/test/.libs/testepc(+0x4f5b9)[0x55dc9e2315b9]  
```


Remove all subscriber information using MongoDB Client
```
$ mongo
> db.subscribers.find()  ### Check the test subscriber
> db.subscribers.drop()  ### Remove all subscriber
> db.subscribers.find()  ### Check that all subscribers are empty
```

Kill all processes.
```bash
$ ps -ef | grep testepc
$ ps -ef | grep nextepc
$ sudo pkill -9 testepc
$ sudo pkill -9 nextepc-epcd ...
```

Execute `testepc`
```bash
$ ./test/testepc
```

#### My eNB does not support IPv6.

Your eNodeB don't have to support IPv6.

If the sgw.gtpu configuration does not have an IPv6 address, the eNodeB can use IPv4 to connect to the MME and SGW. If the sgw.gtpu setting has an IPv6 address, you can disable the IPv6 address as shown below.

```yaml
parameter:
    no_ipv6: true
```

**Note:** This parameter `no_ipv6` is only applied to EPC Elements such as MME, SGW, and so on. The parameter `no_ipv6` does not affect to UE. So, IPv6-enabled UE can connect to NextEPC LTE network.
{: .notice--warning}

#### Unable to add new user by WebUI 

It might be a CSRF token mismatch error.
Please, delete corresponding cookies, cache, session data etc.

#### Change Domain from localdomain to anything else

You should configure the domain name on your computer. Otherwise, freeDiameter raise an error.

#### How many of UEs can NextEPC support?

See the [lib/base/types.h](https://github.com/{{ site.github_username }}/nextepc/blob/master/lib/base/types.h).

```
#define MAX_NUM_OF_ENB              128
#define MAX_NUM_OF_UE               128
#define MAX_NUM_OF_SESS             4
#define MAX_NUM_OF_BEARER           4
#define MAX_NUM_OF_TUNNEL           3   /* Num of Tunnel per Bearer */
#define MAX_NUM_OF_PF               16  /* Num of Packet Filter per Bearer */

#define MAX_POOL_OF_UE              (MAX_NUM_OF_ENB * MAX_NUM_OF_UE)
#define MAX_POOL_OF_SESS            (MAX_POOL_OF_UE * MAX_NUM_OF_SESS)
#define MAX_POOL_OF_BEARER          (MAX_POOL_OF_SESS * MAX_NUM_OF_BEARER)
#define MAX_POOL_OF_TUNNEL          (MAX_POOL_OF_BEARER * MAX_NUM_OF_TUNNEL)
#define MAX_POOL_OF_PF              (MAX_POOL_OF_BEARER * MAX_NUM_OF_PF)
#define MAX_POOL_OF_DIAMETER_SESS   (MAX_POOL_OF_UE * MAX_NUM_OF_SESS)
```

Currently, the number of UE is limited to `128*128`.

#### What is the Default Configuration?

- Network

```
* MME
  S1AP: listen on all address avaiable in system
  GTP-C: listen on the first IP address in system
  DIAMETER: 127.0.0.2 (No TLS)

* SGW
  GTP-C: 127.0.0.2
  GTP-U: listen on the first IP address in system

* PGW
  GTP-C: Both 127.0.0.3 and [::1]
  GTP-U: Both 127.0.0.3 and [::1]
  DIAMETER: 127.0.0.3 (No TLS)

* HSS
  DIAMETER: 127.0.0.4 (No TLS)

* PCRF
  DIAMETER: 127.0.0.5 (No TLS)
```

- GUMMEI, PLMN and TAC

```
* GUMMEI
  PLMN ID - MNC: 001, MCC: 01
  MME Group : 2
  MME Code : 1

* TAI
  PLMN ID - MNC: 001, MCC: 01
  TAC : 12345
```

- Security

```
* Integrity : EIA1 - Snow 3G
* Ciphering : EEA0 - Nothing
```

- UE Network

```
* IPv4 : 45.45.0.1/16
* IPv6 : cafe::1/64
```

- DNS

```
* IPv4
  Primary : 8.8.8.8 
  Secondary : 8.8.4.4

* IPv6
  Primary : 2001:4860:4860::8888
  Secondary : 2001:4860:4860::8844
```

#### The parsing errors are caused by tab spaces in the configuration files.

YAML forbids tabs. You should use space instead of tab in NextEPC configuration file.

```markdown
YAML FAQ: Why does YAML forbid tabs?

Tabs have been outlawed since they are treated differently by different editors and tools. And since indentation is so critical to proper interpretation of YAML, this issue is just too tricky to even attempt. Indeed Guido van Rossum of Python has acknowledged that allowing TABs in Python source is a headache for many people and that were he to design Python again, he would forbid them.
```

#### High CPU usage on idle

This issue will not occur after v0.4.x version.

#### Cross compilation setup

By default, NextEPC is designed to support the Embedding System. To do so, we introduced pool-based memory management. Unfortunately, we have not tested NextEPC in an embedded environment. I tried to compile on the ARM architecture using Docker and run it with QEMU for your reference.

- We'll use Debian Docker Environment.

```bash
$ git clone -r https://github.com/acetcom/nextepc
$ cd nextepc/docker
$ DIST=debian docker-compose run dev
```

- In Docker Container

```bash
acetcom@nextepc-dev:~$ sudo dpkg --add-architecture armel
acetcom@nextepc-dev:~$ sudo apt-get install libsctp-dev:armel libyaml-dev:armel libgnutls28-dev:armel libgcrypt-dev:armel libssl-dev:armel libmongoc-dev:armel libbson-dev:armel
acetcom@nextepc-dev:~$ sudo apt-get install crossbuild-essential-armel
acetcom@nextepc-dev:~$ sudo apt-get install qemu

acetcom@nextepc-dev:~$ mkdir git
acetcom@nextepc-dev:~$ cd git/
acetcom@nextepc-dev:~/git$ git clone https://github.com/acetcom/nextepc
acetcom@nextepc-dev:~/git$ cd nextepc/
acetcom@nextepc-dev:~/git/nextepc$ autoreconf -if;./configure --prefix=`pwd`/install --host=arm-linux-gnueabi;make -j 2
acetcom@nextepc-dev:~/git/nextepc$ make install
acetcom@nextepc-dev:~/git/nextepc$ qemu-arm .libs/nextepc-mmed
NextEPC daemon v0.3.10 - Oct  4 2018 13:24:24

[10/04 13:38:06.329] WARN: pid file /home/acetcom/git/nextepc/install/var/run/nextepc-mmed/pid overwritten -- Unclean shutdown of previous NextEPC run? (application.c:113)
  PID[55780] : '/home/acetcom/git/nextepc/install/var/run/nextepc-mmed/pid'
  File Logging : '/home/acetcom/git/nextepc/install/var/log/nextepc/nextepc.log'
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
  MongoDB URI : 'mongodb://mongodb/nextepc'
  Configuration : '/home/acetcom/git/nextepc/install/etc/nextepc/nextepc.conf'
[10/04 13:38:06.400] MME try to initialize
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown QEMU_IFLA_INFO_KIND ipip
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown QEMU_IFLA_INFO_KIND ip6tnl
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown QEMU_IFLA_INFO_KIND ipip
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown QEMU_IFLA_INFO_KIND ip6tnl
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
[10/04 13:38:08.693] gtp_server() [172.20.0.2]:2123
[10/04 13:38:08.696] gtp_client() [127.0.0.2]:2123
Unsupported setsockopt level=132 optname=11
[10/04 13:38:08.697] ERRR: Unable to subscribe to SCTP events: (92:Protocol not available) (unix/sctp.c:291)
[10/04 13:38:08.699] ASSERT: !(rv == CORE_OK).  (unix/sctp.c:33)
[10/04 13:38:08.700] ERRR: sctp_server() [172.20.0.2]:36412 failed(92:Protocol not available) (unix/sctp.c:98)
[10/04 13:38:08.701] ASSERT: !(rv == CORE_OK).  (s1ap_sctp.c:35)
[10/04 13:38:08.702] ASSERT: !(rv == CORE_OK).  (s1ap_path.c:53)
[10/04 13:38:08.703] ASSERT: !(rv == CORE_OK).  (s1ap_path.c:28)
[10/04 13:38:08.704] ERRR: Can't establish S1AP path (mme_sm.c:63)
[10/04 13:38:08.708] MME initialize...done


[10/04 13:38:08.710] INFO: NextEPC daemon start (main.c:157)
```

The SCTP module is not included in the QEMU kernel. I believe that if the Linux kernel installed on your target platform contains an SCTP module, it will work normally.
{: .notice--warning}
