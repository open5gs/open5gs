---
title: Now in the Github Issue
head_inline: "<style> .blue { color: blue; } </style>"
---

#### Test failed (e.g. `meson test`)

Sometimes you may get a message like the one below due to a problem with the freeDiameter library.

```
$ meson test

...
5/8 open5gs:system / simple                 OK       7.69 s 

--- command ---
08:06:23 /home/parallels/open5gs/build/tests/simple/simple
--- stdout ---
s1setup-test        :  SUCCESS
attach-test         :  SUCCESS
volte-test          :  SUCCESS
handover-test       :  SUCCESS
crash-test          :  SUCCESS
All tests passed.
-------

6/8 open5gs:system / mnc3                   FAIL    12.80 s (exit status 1)

--- command ---
08:06:31 /home/parallels/open5gs/build/tests/mnc3/mnc3
--- stdout ---
mnc3-test           :  ERROR: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:112 ROUTING ERROR 'No remaining suitable candidate to route the message to' for:  (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113      'Credit-Control-Request' (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113        Version: 0x01 (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113        Length: 20 (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113        Flags: 0xC0 (RP--) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113        Command Code: 272 (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113        ApplicationId: 16777238 (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113        Hop-by-Hop Identifier: 0x00000000 (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113        End-to-End Identifier: 0x2875502B (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         {internal data}: src:(nil)(0) rwb:(nil) rt:0 cb:0x564527642502,(nil)(0x7f71a86df418) qry:(nil) asso:0 sess:0x7f7158001140 (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Session-Id'(263) l=8 f=-M val="pgw.localdomain;1585209991;1;app_gx" (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Origin-Host'(264) l=8 f=-M val="pgw.localdomain" (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Origin-Realm'(296) l=8 f=-M val="localdomain" (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Destination-Realm'(283) l=8 f=-M val="localdomain" (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Auth-Application-Id'(258) l=12 f=-M val=16777238 (0x1000016) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'CC-Request-Type'(416) l=12 f=-M val='INITIAL_REQUEST' (1 (0x1)) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'CC-Request-Number'(415) l=12 f=-M val=0 (0x0) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Subscription-Id'(443) l=8 f=-M val=(grouped) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'Subscription-Id-Type'(450) l=12 f=-M val='END_USER_IMSI' (1 (0x1)) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'Subscription-Id-Data'(444) l=8 f=-M val="310014987654004" (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Supported-Features'(628) vend='3GPP'(10415) l=12 f=V- val=(grouped) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'Feature-List-ID'(629) vend='3GPP'(10415) l=16 f=V- val=1 (0x1) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'Feature-List'(630) vend='3GPP'(10415) l=16 f=V- val=11 (0xb) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Network-Request-Support'(1024) vend='3GPP'(10415) l=16 f=VM val=1 (0x1) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Framed-IP-Address'(8) l=8 f=-M val=<0A 2D 00 02> (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'IP-CAN-Type'(1027) vend='3GPP'(10415) l=16 f=VM val=5 (0x5) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'RAT-Type'(1032) vend='3GPP'(10415) l=16 f=V- val=1004 (0x3ec) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'QoS-Information'(1016) vend='3GPP'(10415) l=12 f=VM val=(grouped) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'APN-Aggregate-Max-Bitrate-UL'(1041) vend='3GPP'(10415) l=16 f=V- val=1024000000 (0x3d090000) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'APN-Aggregate-Max-Bitrate-DL'(1040) vend='3GPP'(10415) l=16 f=V- val=1024000000 (0x3d090000) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Default-EPS-Bearer-QoS'(1049) vend='3GPP'(10415) l=12 f=V- val=(grouped) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'QoS-Class-Identifier'(1028) vend='3GPP'(10415) l=16 f=VM val=9 (0x9) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113            AVP: 'Allocation-Retention-Priority'(1034) vend='3GPP'(10415) l=12 f=V- val=(grouped) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113               AVP: 'Priority-Level'(1046) vend='3GPP'(10415) l=16 f=V- val=15 (0xf) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113               AVP: 'Pre-emption-Capability'(1047) vend='3GPP'(10415) l=16 f=V- val=1 (0x1) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113               AVP: 'Pre-emption-Vulnerability'(1048) vend='3GPP'(10415) l=16 f=V- val=0 (0x0) (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: '3GPP-User-Location-Info'(22) vend='3GPP'(10415) l=12 f=VM val=<82 13 00 41 00 33 13 00 41 08 D0 1B 78> (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: '3GPP-MS-TimeZone'(23) vend='3GPP'(10415) l=12 f=VM val=<23 00> (../lib/diameter/common/init.c:116)
: ../subprojects/freeDiameter/extensions/dbg_msg_dumps/dbg_msg_dumps.c:113         AVP: 'Called-Station-Id'(30) l=8 f=-M val=<73 74 61 72 65 6E 74 2E 63 6F 6D> (../lib/diameter/common/init.c:116)
: no_CC-Request-Number (../src/pgw/pgw-fd-path.c:505)
/home/parallels/open5gs/build/src/pgw/../../lib/core/libogscore.so.1(ogs_abort+0x2e)[0x7f71abdbdbf8]
/home/parallels/open5gs/build/src/pgw/open5gs-pgwd(+0x1cb8f)[0x564527642b8f]
/home/parallels/open5gs/build/src/pgw/../../subprojects/freeDiameter/libfdcore/libfdcore.so.7(+0x492d7)[0x7f71ab7702d7]
/home/parallels/open5gs/build/src/pgw/../../subprojects/freeDiameter/libfdcore/libfdcore.so.7(+0x4cb63)[0x7f71ab773b63]
/home/parallels/open5gs/build/src/pgw/../../subprojects/freeDiameter/libfdcore/libfdcore.so.7(+0x4ccef)[0x7f71ab773cef]
/lib/x86_64-linux-gnu/libpthread.so.0(+0x76db)[0x7f71aae686db]
/lib/x86_64-linux-gnu/libc.so.6(clone+0x3f)[0x7f71aab9188f]
03/26 16:06:43.674ERROR: GTP Timeout : IMSI[310014987654004] Message-Type[32] (../src/sgw/sgw-s11-handler.c:39)
FAILED 1 of 1
Failed Tests   		Total	Fail	Failed %
===================================================
mnc3-test      		    1	   1	100.00%
--- stderr ---
03/26 16:06:43.681: [ERROR: GTP Timeout : IMSI[310014987654004] Message-Type[32] (../src/mme/mme-gtp-path.c:110)
Line 202: Condition is false, but expected true
-------

7/8 open5gs:system / volte                  FAIL    10.57 s (killed by signal 6 SIGABRT)

...
```

I cannot solve the problem exactly at this point.
{: .blue}

Remove all subscriber information using MongoDB Client
```
$ mongo
> use open5gs
switched to db open5gs
> db.subscribers.find()  ### Check the test subscriber
> db.subscribers.drop()  ### Remove all subscriber
> db.subscribers.find()  ### Check that all subscribers are empty
```

Kill all processes.
```bash
$ ps -ef | grep open5gs
$ sudo pkill -9 open5gs-hssd
$ sudo pkill -9 open5gs-pcrfd
$ sudo pkill -9 open5gs-pgwd
$ sudo pkill -9 open5gs-sgwd
$ sudo pkill -9 open5gs-mmed
```

Run `meson test` again
```
$ meson test
ninja: Entering directory `/Users/acetcom/Documents/git/open5gs/build'
ninja: no work to do.
1/8 open5gs:unit / core                     OK       6.93 s
2/8 open5gs:unit / crypt                    OK       0.08 s
3/8 open5gs:system / sctp                   OK       1.09 s
4/8 open5gs:unit / unit                     OK       0.04 s
5/8 open5gs:system / simple                 OK       5.07 s
6/8 open5gs:system / mnc3                   OK       1.18 s
7/8 open5gs:system / volte                  OK       2.99 s
8/8 open5gs:system / csfb                   OK       6.53 s

Ok:                    8
Expected Fail:         0
Fail:                  0
Unexpected Pass:       0
Skipped:               0
Timeout:               0

Full log written to /Users/acetcom/Documents/git/open5gs/build/meson-logs/testlog.txt
```

**Note:** If your test result more than once is same as above, there should be no problem with code you modified.
{: .notice--danger}

#### Is it possible to setup IP/NAT table along with Docker?

Enable IP Forward.
```
$ sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
```

The following is the default docker IP/NAT table.

```
$ sudo iptables-save
# Generated by iptables-save v1.6.1 on Sat Jun  1 23:43:50 2019
*nat
:PREROUTING ACCEPT [74:4820]
:INPUT ACCEPT [4:208]
:OUTPUT ACCEPT [49:3659]
:POSTROUTING ACCEPT [49:3659]
:DOCKER - [0:0]
-A PREROUTING -m addrtype --dst-type LOCAL -j DOCKER
-A OUTPUT ! -d 127.0.0.0/8 -m addrtype --dst-type LOCAL -j DOCKER
-A POSTROUTING -s 172.17.0.0/16 ! -o docker0 -j MASQUERADE
-A DOCKER -i docker0 -j RETURN
COMMIT
# Completed on Sat Jun  1 23:43:50 2019
# Generated by iptables-save v1.6.1 on Sat Jun  1 23:43:50 2019
*filter
:INPUT ACCEPT [651:514108]
:FORWARD DROP [70:4612]
:OUTPUT ACCEPT [590:63846]
:DOCKER - [0:0]
:DOCKER-ISOLATION-STAGE-1 - [0:0]
:DOCKER-ISOLATION-STAGE-2 - [0:0]
:DOCKER-USER - [0:0]
-A FORWARD -j DOCKER-USER
-A FORWARD -j DOCKER-ISOLATION-STAGE-1
-A FORWARD -o docker0 -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
-A FORWARD -o docker0 -j DOCKER
-A FORWARD -i docker0 ! -o docker0 -j ACCEPT
-A FORWARD -i docker0 -o docker0 -j ACCEPT
-A DOCKER-ISOLATION-STAGE-1 -i docker0 ! -o docker0 -j DOCKER-ISOLATION-STAGE-2
-A DOCKER-ISOLATION-STAGE-1 -j RETURN
-A DOCKER-ISOLATION-STAGE-2 -o docker0 -j DROP
-A DOCKER-ISOLATION-STAGE-2 -j RETURN
-A DOCKER-USER -j RETURN
COMMIT
# Completed on Sat Jun  1 23:43:50 2019
```

Create **newtables** file as below.

```diff
$ diff -u oldtables newtables
--- oldtables	2019-06-01 23:43:50.354974226 +0900
+++ newtables	2019-06-01 23:44:16.110931684 +0900
@@ -8,6 +8,7 @@
 -A PREROUTING -m addrtype --dst-type LOCAL -j DOCKER
 -A OUTPUT ! -d 127.0.0.0/8 -m addrtype --dst-type LOCAL -j DOCKER
 -A POSTROUTING -s 172.17.0.0/16 ! -o docker0 -j MASQUERADE
+-A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
 -A DOCKER -i docker0 -j RETURN
 COMMIT
 # Completed on Sat Jun  1 23:43:50 2019
@@ -26,6 +27,10 @@
 -A FORWARD -o docker0 -j DOCKER
 -A FORWARD -i docker0 ! -o docker0 -j ACCEPT
 -A FORWARD -i docker0 -o docker0 -j ACCEPT
+-A FORWARD -o ogstun -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
+-A FORWARD -o ogstun -j DOCKER
+-A FORWARD -i ogstun ! -o ogstun -j ACCEPT
+-A FORWARD -i ogstun -o ogstun -j ACCEPT
 -A DOCKER-ISOLATION-STAGE-1 -i docker0 ! -o docker0 -j DOCKER-ISOLATION-STAGE-2
 -A DOCKER-ISOLATION-STAGE-1 -j RETURN
 -A DOCKER-ISOLATION-STAGE-2 -o docker0 -j DROP
```

And then, apply **newtables** as below.
```
$ sudo iptables-restore < newtables
```

The above operation is the same as described in the following manuals.
```
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
$ sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
```

#### How to use a different APN for each PGW

By default, MME selects the PGW as the first PGW node. To use a different APN for each PGW, specify gtpc.apn as the APN name. If the HSS uses WebUI to set the PGW IP for each UE, you can use a specific PGW node for each UE.

See the following example.

```
### For reference, see `pgw.yaml`
#------------------------ MME --------------------------
#
#  o Two PGW are defined. 127.0.0.3:2123 is used.
#    [fe80::3%lo]:2123 is ignored.
#    gtpc:
#      - addr: 127.0.0.3
#      - addr: fe80::3%lo
#
#  o One PGW is defined. if prefer_ipv4 is not true,
#    [fe80::3%lo] is selected.
#    gtpc:
#      - addr:
#        - 127.0.0.3
#        - fe80::3%lo
#
#  o Two PGW are defined with a different APN.
#    - Note that if PGW IP for UE is configured in HSS,
#      the following configurion for this UE is ignored.
#    gtpc:
#      - addr: 127.0.0.3
#        apn: internet
#      - addr: 127.0.0.5
#        apn: volte
#
#  o If APN is omitted, the default APN uses the first PGW node.
#    gtpc:
#      - addr: 127.0.0.3
#      - addr: 127.0.0.5
#        apn: volte
#
```

The IP address of the UE can also use a different UE pool depending on the APN.

```
### For reference, see `pgw.yaml`
#
#  <UE Pool>
#
#  o IPv4 Pool
#    $ sudo ip addr add 10.45.0.1/16 dev ogstun
#
#    ue_pool:
#      addr: 10.45.0.1/16
#
#  o IPv4/IPv6 Pool
#    $ sudo ip addr add 10.45.0.1/16 dev ogstun
#    $ sudo ip addr add cafe:1::1/64 dev ogstun
#
#    ue_pool:
#      - addr: 10.45.0.1/16
#      - addr: cafe:1::1/64
#
#
#  o Specific APN(e.g 'volte') uses 10.46.0.1/16, cafe:2::1/64
#    All other APNs use 10.45.0.1/16, cafe:1::1/64
#    $ sudo ip addr add 10.45.0.1/16 dev ogstun
#    $ sudo ip addr add 10.46.0.1/16 dev ogstun
#    $ sudo ip addr add cafe:1::1/64 dev ogstun
#    $ sudo ip addr add cafe:2::1/64 dev ogstun
#
#    ue_pool:
#      - addr: 10.45.0.1/16
#      - addr: cafe:1::1/64
#      - addr: 10.46.0.1/16
#        apn: volte
#      - addr: cafe:2::1/64
#        apn: volte
#
#  o Multiple Devices (default: ogstun)
#    $ sudo ip addr add 10.45.0.1/16 dev ogstun
#    $ sudo ip addr add cafe:1::1/64 dev ogstun2
#    $ sudo ip addr add 10.46.0.1/16 dev ogstun3
#    $ sudo ip addr add cafe:2::1/64 dev ogstun3
#
#    ue_pool:
#      - addr: 10.45.0.1/16
#      - addr: cafe:1::1/64
#        dev: ogstun2
#      - addr: 10.46.0.1/16
#        apn: volte
#        dev: ogstun3
#      - addr: cafe:2::1/64
#        apn: volte
#        dev: ogstun3
#
```

#### Failing to run `./open5gs-mmed`

You might be getting the following error after running `./open5gs-mmed`.
```bash
$ ./build/src/mme/open5gs-mmed
10/27 16:45:41.912: [app] FATAL: cannot open file `/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/mme.yaml` (../lib/app/ogs-config.c:67)
10/27 16:45:41.912: [app] FATAL: Open5GS initialization failed. Aborted (../src/main.c:211)
```

You should perform **the installation process**.

```bash
$ ninja -C build install
```

#### How to connect MongoDB server

If you do not start MongoDB, you will get the following error:

```bash
$ ./install/bin/open5gs-hssd
Open5GS daemon v1.0.0

10/27 16:45:00.371: [app] INFO: Configuration: '/Users/acetcom/Documents/git/open5gs/install/etc/open5gs/hss.yaml' (../src/main.c:54)
10/27 16:45:00.372: [app] INFO: File Logging: '/Users/acetcom/Documents/git/open5gs/install/var/log/open5gs/hss.log' (../src/main.c:57)
10/27 16:45:00.377: [dbi] ERROR: Failed to connect to server [mongodb://localhost/open5gs] (../lib/dbi/ogs-mongoc.c:91)
10/27 16:45:00.377: [app] ERROR: Failed to intialize HSS (../src/hss/app-init.c:28)
```

You can start MongoDB using systemctl.
```bash
$ sudo systemctl start mongodb
```

#### I have some error when running `./build/test/simple/simple`

Did you see the following error after executing `./build/test/simple/simple`?
```bash
$ ./build/test/simple/simple
s1setup_test        : SUCCESS  
attach_test         : -Line 134: Condition is false, but expected true  
\04/09 15:49:09.285: [esm] FATAL: esm_handle_pdn_connectivity_request: Assertion `SECURITY_CONTEXT_IS_VALID(mme_ue)' failed. (esm_handler.c:29)  
/home/acetcom/Documents/git/open5gs/open5gs/lib/ogslib/src/core/.libs/libogscore-1.0.so.0(ogs_abort+0x2b)[0x7f608518271b]  
/home/acetcom/Documents/git/open5gs/open5gs/test/.libs/testcomplex(+0x92121)[0x55dc9e274121]  
/home/acetcom/Documents/git/open5gs/open5gs/test/.libs/testcomplex(+0x4f5b9)[0x55dc9e2315b9]  
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
$ ps -ef | grep simple
$ ps -ef | grep open5gs
$ sudo pkill -9 simple
$ sudo pkill -9 open5gs-mmed ...
```

Execute `./build/test/simple/simple`
```bash
$ ./build/test/simple/simple
```

#### My eNB does not support IPv6.

Your eNodeB don't have to support IPv6.

If the sgw.gtpu configuration does not have an IPv6 address, the eNodeB can use IPv4 to connect to the MME and SGW. If the sgw.gtpu setting has an IPv6 address, you can disable the IPv6 address as shown below.

```yaml
parameter:
    no_ipv6: true
```

**Note:** This parameter `no_ipv6` is only applied to EPC Elements such as MME, SGW, and so on. The parameter `no_ipv6` does not affect to UE. So, IPv6-enabled UE can connect to Open5GS LTE network.
{: .notice--warning}

#### Unable to add new user by WebUI 

It might be a CSRF token mismatch error.
Please, delete corresponding cookies, cache, session data etc.

#### Change Domain from localdomain to anything else

You should configure the domain name on your computer. Otherwise, freeDiameter raise an error.

#### How many of UEs can Open5GS support?

See the [lib/core/ogs-3gpp-types.h](https://github.com/{{ site.github_username }}/open5gs/blob/master/lib/core/ogs-3gpp-types.h).

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
* IPv4 : 10.45.0.1/16
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

YAML forbids tabs. You should use space instead of tab in Open5GS configuration file.

```markdown
YAML FAQ: Why does YAML forbid tabs?

Tabs have been outlawed since they are treated differently by different editors and tools. And since indentation is so critical to proper interpretation of YAML, this issue is just too tricky to even attempt. Indeed Guido van Rossum of Python has acknowledged that allowing TABs in Python source is a headache for many people and that were he to design Python again, he would forbid them.
```

#### Cross compilation setup

By default, Open5GS is designed to support the Embedding System. To do so, we introduced pool-based memory management. Unfortunately, we have not tested Open5GS in an embedded environment. I tried to compile on the ARM architecture using Docker and run it with QEMU for your reference.

- We'll use Debian Docker Environment.

```bash
$ git clone https://github.com/acetcom/open5gs
$ cd open5gs/docker
$ DIST=debian TAG=stretch docker-compose run dev
```

- In Docker Container

```bash
$ sudo dpkg --add-architecture armel
$ sudo apt update
$ sudo apt install libsctp-dev:armel libyaml-dev:armel libgnutls28-dev:armel libgcrypt-dev:armel libidn11-dev:armel libssl-dev:armel libmongoc-dev:armel libbson-dev:armel
$ sudo apt install crossbuild-essential-armel
$ sudo apt install qemu
$ git clone https://github.com/acetcom/open5gs
$ cd open5gs/

$ cat << EOF > cross_file.txt
[host_machine]
system = 'linux'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'

[binaries]
c = '/usr/bin/arm-linux-gnueabi-gcc'
ar = '/usr/bin/arm-linux-gnueabi-ar'
strip = '/usr/bin/arm-linux-gnueabi-strip'
pkgconfig = '/usr/bin/arm-linux-gnueabi-pkg-config'

[properties]
needs_exe_wrapper = true
EOF

$ meson build --prefix=`pwd`/install --cross-file cross_file.txt
$ ninja -C build
$ qemu-arm ./install/bin/open5gs-sgwd
Open5GS daemon v1.0.0

10/28 06:47:36.649: [app] INFO: Configuration: '/home/acetcom/git/open5gs/install/etc/open5gs/sgw.yaml' (../src/main.c:54)
10/28 06:47:36.652: [app] INFO: File Logging: '/home/acetcom/git/open5gs/install/var/log/open5gs/sgw.log' (../src/main.c:57)
Unknown host QEMU_IFLA type: 50
Unknown host QEMU_IFLA type: 51
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 47
Unknown host QEMU_IFLA type: 48
Unknown host QEMU_IFLA type: 43
Unknown host QEMU_IFLA type: 50
Unknown host QEMU_IFLA type: 51
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 47
Unknown host QEMU_IFLA type: 48
Unknown host QEMU_IFLA type: 43
Unknown QEMU_IFLA_INFO_KIND tun
Unknown host QEMU_IFLA type: 50
Unknown host QEMU_IFLA type: 51
Unknown host QEMU_IFLA type: 40
Unknown host QEMU_IFLA type: 41
Unknown host QEMU_IFLA type: 47
Unknown host QEMU_IFLA type: 48
Unknown host QEMU_IFLA type: 43
10/28 06:47:36.683: [app] INFO: SGW initialize...done (../src/sgw/app-init.c:31)
10/28 06:47:36.696: [gtp] INFO: gtp_server() [127.0.0.2]:2123 (../lib/gtp/path.c:32)
10/28 06:47:36.697: [gtp] INFO: gtp_server() [172.18.0.2]:2152 (../lib/gtp/path.c:32)
^C10/28 06:47:37.324: [app] INFO: SIGINT received (../src/main.c:73)
10/28 06:47:37.325: [app] INFO: Open5GS daemon terminating... (../src/main.c:229)
10/28 06:47:37.337: [app] INFO: SGW terminate...done (../src/sgw/app-init.c:39)

$ qemu-arm ./install/bin/open5gs-hssd
Open5GS daemon v1.0.0

10/28 06:47:39.021: [app] INFO: Configuration: '/home/acetcom/git/open5gs/install/etc/open5gs/hss.yaml' (../src/main.c:54)
10/28 06:47:39.024: [app] INFO: File Logging: '/home/acetcom/git/open5gs/install/var/log/open5gs/hss.log' (../src/main.c:57)
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
qemu: Unsupported syscall: 345
10/28 06:47:39.044: [dbi] INFO: MongoDB URI: 'mongodb://mongodb/open5gs' (../lib/dbi/ogs-mongoc.c:99)


getsockopt level=132 optname=0 not yet supported
10/28 06:47:41.917: [diam] ERROR: ../subprojects/freediameter/libfdcore/sctp.c:86 ERROR: in '(getsockopt(sk, IPPROTO_SCTP, 0, &rtoinfo, &sz))' :    Operation not supported (../lib/diameter/common/init.c:116)
10/28 06:47:41.919: [diam] ERROR: ../subprojects/freediameter/libfdcore/sctp.c:778 ERROR: in '((fd_setsockopt_prebind(*sock)))' :   Operation not supported (../lib/diameter/common/init.c:116)
10/28 06:47:41.919: [diam] ERROR: ../subprojects/freediameter/libfdcore/cnxctx.c:181 ERROR: in '(fd_sctp_create_bind_server( &cnx->cc_socket, cnx->cc_family, ep_list, port ))' :   Operation not supported (../lib/diameter/common/init.c:116)
10/28 06:47:41.919: [diam] ERROR: ../subprojects/freediameter/libfdcore/server.c:355 ERROR: in '(s->conn = fd_cnx_serv_sctp(fd_g_config->cnf_port, empty_conf_ep ? ((void *)0) : &fd_g_config->cnf_endpoints))' :   Transport endpoint is not connected (../lib/diameter/common/init.c:116)
10/28 06:47:41.919: [diam] ERROR: ../subprojects/freediameter/libfdcore/core.c:300 ERROR: in '((fd_servers_start()))' : Transport endpoint is not connected (../lib/diameter/common/init.c:116)
10/28 06:47:41.920: [diam] ERROR: ../lib/diameter/common/init.c:62 ERROR: in '(fd_core_start())' :  Transport endpoint is not connected (../lib/diameter/common/init.c:116)
10/28 06:47:41.920: [diam] INFO: [6]: ../subprojects/freediameter/libfdcore/core.c:329 Initiating freeDiameter shutdown sequence (2) (../lib/diameter/common/init.c:127)
10/28 06:47:42.057: [hss] FATAL: hss_fd_init: Assertion `ret == 0' failed. (../src/hss/hss-fd-path.c:681)
```

The SCTP module is not included in the QEMU kernel. I believe that if the Linux kernel installed on your target platform contains an SCTP module, it will work normally.
{: .notice--warning}
