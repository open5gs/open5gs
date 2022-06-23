---
title: Now in the Github Issue
head_inline: "<style> .blue { color: blue; } </style>"
---

<style>
  img {
    max-width: 100%;
    height: auto;
  }
</style>

#### MME sends Attach reject(EMM-Cause:15) with Diameter error(Result-Code:3002)

If you see the Attach reject(EMM-Cause:15] with Diameter error(Result-Code:3002), it means that HSS is not running.

```
...
5/08 18:22:23.584: [diam] ERROR: pid:Routing-OUT (0x563969fc2060) in md_hook_cb_tree@dbg_msg_dumps.c:113:         AVP: 'Vendor-Specific-Application-Id'(260) l=8 f=-M val=(grouped)
 ((null):0)
05/08 18:22:23.584: [diam] ERROR: pid:Routing-OUT (0x563969fc2060) in md_hook_cb_tree@dbg_msg_dumps.c:113:            AVP: 'Vendor-Id'(266) l=12 f=-M val=10415 (0x28af)
 ((null):0)
05/08 18:22:23.584: [diam] ERROR: pid:Routing-OUT (0x563969fc2060) in md_hook_cb_tree@dbg_msg_dumps.c:113:            AVP: 'Auth-Application-Id'(258) l=12 f=-M val=16777251 (0x1000023)
 ((null):0)
05/08 18:22:23.584: [mme] INFO:     Result Code: 3002 (../src/mme/mme-fd-path.c:301)
05/08 18:22:23.585: [mme] INFO: [001010123456792] Attach reject [EMM_CAUSE:15] (../src/mme/mme-sm.c:448)
05/08 18:22:23.612: [mme] INFO: UE Context Release [Action:3] (../src/mme/s1ap-handler.c:1328)
05/08 18:22:23.612: [mme] INFO:     ENB_UE_S1AP_ID[1] MME_UE_S1AP_ID[1] (../src/mme/s1ap-handler.c:1330)
05/08 18:22:23.612: [mme] INFO:     IMSI[001010123456792] (../src/mme/s1ap-handler.c:1332)
05/08 18:22:23.612: [mme] INFO: [Removed] Number of eNB-UEs is now 0 (../src/mme/mme-context.c:3228)
...
```

Please check the status of HSS and restart it.
```
$ sudo systemctl status open5gs-hssd.service
● open5gs-hssd.service - Open5GS HSS Daemon
     Loaded: loaded (/lib/systemd/system/open5gs-hssd.service; disabled; vendor preset: enabled)
     Active: inactive (dead)
sudo systemctl status open5gs-hssd.service
● open5gs-hssd.service - Open5GS HSS Daemon
     Loaded: loaded (/lib/systemd/system/open5gs-hssd.service; disabled; vendor preset: enabled)
     Active: active (running) since Sun 2021-05-09 18:36:49 KST; 1s ago
   Main PID: 6011 (open5gs-hssd)
      Tasks: 37 (limit: 19047)
     Memory: 14.6M
     CGroup: /system.slice/open5gs-hssd.service
             └─6011 /usr/bin/open5gs-hssd -c /etc/open5gs/hss.yaml

May 09 18:36:49 open5gs systemd[1]: Started Open5GS HSS Daemon.
May 09 18:36:49 open5gs open5gs-hssd[6011]: Open5GS daemon v2.2.7
May 09 18:36:49 open5gs open5gs-hssd[6011]: 05/09 18:36:49.987: [app] INFO: Configuration: '/etc/ope>
May 09 18:36:49 open5gs open5gs-hssd[6011]: 05/09 18:36:49.987: [app] INFO: File Logging: '/var/log/>
May 09 18:36:49 open5gs open5gs-hssd[6011]: 05/09 18:36:49.994: [dbi] INFO: MongoDB URI: 'mongodb://>
May 09 18:36:50 open5gs open5gs-hssd[6011]: 05/09 18:36:50.116: [app] INFO: HSS initialize...done
```

#### MME Diameter-Error with HSS-crash using v2.2.x package

If the following MME log occurs while connecting to the UE, it means that you may use the old format DB schema.

```
04/14 20:14:21.981: [diam] ERROR: pid:PSM/hss.localdomain in fd_psm_change_state@p_psm.c:287: 'STATE_OPEN' -> 'STATE_CLOSED' 'hss.localdomain'
((null):0)
04/14 20:14:21.982: [diam] ERROR: pid:PSM/hss.localdomain in md_hook_cb_tree@dbg_msg_dumps.c:89: FAILOVER from 'hss.localdomain':
((null):0)
04/14 20:14:21.982: [diam] ERROR: pid:PSM/hss.localdomain in md_hook_cb_tree@dbg_msg_dumps.c:90: 'Update-Location-Request'
((null):0)
```

In this case, the HSS may crash as shown below.

```
04/12 10:13:45.025: [app] INFO: Configuration: '/home/open5gs/install/etc/open5gs/hss.yaml' (../lib/app/ogs-init.c:129)
04/12 10:13:45.025: [app] INFO: File Logging: '/home/open5gs/install/var/log/open5gs/hss.log' (../lib/app/ogs-init.c:132)
04/12 10:13:45.028: [dbi] INFO: MongoDB URI: 'mongodb://localhost/open5gs' (../lib/dbi/ogs-mongoc.c:129)
04/12 10:13:45.068: [diam] INFO: CONNECTED TO 'mme.epc.mnc001.mcc001.3gppnetwork.org' (SCTP,soc#17): (../lib/diameter/common/logger.c:108)
04/12 10:13:45.069: [app] INFO: HSS initialize...done (../src/hss/app-init.c:31)
04/12 10:14:27.167: [core] FATAL: ogs_slice_find_by_s_nssai: Assertion `num_of_slice_data' failed. (../lib/core/ogs-3gpp-types.c:529)
04/12 10:14:27.168: [core] FATAL: backtrace() returned 10 addresses (../lib/core/ogs-abort.c:37)
/home/open5gs/install/lib/x86_64-linux-gnu/libogscore.so.2(ogs_slice_find_by_s_nssai+0xd2) [0x7f3b720a126e]
./install/bin/open5gs-hssd(+0xd12e) [0x55a57bb6f12e]
/home/open5gs/install/lib/x86_64-linux-gnu/libfdproto.so.7(fd_disp_call_cb_int+0x270) [0x7f3b7135acb3]
/home/open5gs/install/lib/x86_64-linux-gnu/libfdproto.so.7(fd_msg_dispatch+0xdca) [0x7f3b7137442f]
home/open5gs/install/lib/x86_64-linux-gnu/libfdcore.so.7(+0x67c3c) [0x7f3b715f9c3c]
/home/open5gs/install/lib/x86_64-linux-gnu/libfdcore.so.7(+0x6ca99) [0x7f3b715fea99]
/open5gs/install/lib/x86_64-linux-gnu/libfdcore.so.7(+0x6cd06) [0x7f3b715fed06]
/lib/x86_64-linux-gnu/libpthread.so.0(+0x76db) [0x7f3b70d016db]
/lib/x86_64-linux-gnu/libc.so.6(clone+0x3f) [0x7f3b70a2a71f]
```

At this time, you need to check the DB schema is in the form below by using the command the below.

```
$ mongo
> use open5gs
> db.subscribers.find().pretty()
{
	"_id" : ObjectId("60969fe79459f8b40d8d3f68"),
	"imsi" : "999700000000001",
	"__v" : 0,
	"access_restriction_data" : 32,
	"ambr" : {
		"uplink" : {
			"value" : 1,
			"unit" : 3
		},
		"downlink" : {
			"value" : 1,
			"unit" : 3
		}
	},
	"network_access_mode" : 2,
	"security" : {
		"k" : "465b5ce8b199b49faa5f0a2ee238a6bc",
		"amf" : "8000",
		"op" : null,
		"opc" : "e8ed289deba952e4283b54e88e6183ca",
		"sqn" : NumberLong(97)
	},
	"slice" : [
		{
			"sst" : 1,
			"default_indicator" : true,
			"_id" : ObjectId("60969fe7de8743b3c7b1a973"),
			"session" : [
				{
					"name" : "internet",
					"type" : 3,
					"_id" : ObjectId("60969fe7de8743b3c7b1a974"),
					"pcc_rule" : [ ],
					"ambr" : {
						"uplink" : {
							"value" : 1,
							"unit" : 3
						},
						"downlink" : {
							"value" : 1,
							"unit" : 3
						}
					},
					"qos" : {
						"index" : 9,
						"arp" : {
							"priority_level" : 8,
							"pre_emption_capability" : 1,
							"pre_emption_vulnerability" : 1
						}
					}
				}
			]
		}
	],
	"subscribed_rau_tau_timer" : 12,
	"subscriber_status" : 0
}
```

If you see below, you are using the old format DB schema.

```
$ mongo
> use open5gs
> db.subscribers.find().pretty()
{
	"_id" : ObjectId("609715fda08851a0744e6ae7"),
	"imsi" : "999700000021309",
	"__v" : 0,
	"access_restriction_data" : 32,
	"ambr" : {
		"downlink" : NumberLong(1024000),
		"uplink" : NumberLong(1024000)
	},
	"network_access_mode" : 2,
	"pdn" : [
		{
			"apn" : "internet",
			"_id" : ObjectId("609715fd455bcd38c884ce85"),
			"pcc_rule" : [ ],
			"ambr" : {
				"downlink" : NumberLong(1024000),
				"uplink" : NumberLong(1024000)
			},
			"qos" : {
				"qci" : 9,
				"arp" : {
					"priority_level" : 8,
					"pre_emption_vulnerability" : 1,
					"pre_emption_capability" : 0
				}
			},
			"type" : 0
		}
	],
	"security" : {
		"k" : "70D49A71DD1A2B806A25ABE0EF749F1E",
		"amf" : "8000",
		"op" : null,
		"opc" : "6F1BF53D624B3A43AF6592854E2444C7"
	},
	"subscribed_rau_tau_timer" : 12,
	"subscriber_status" : 0
}
```

If you are using old format DB schema, please perform the following step.

1. First of all, it is recommended to use the following command to remove all existing subscription DB.
```
$ mongo
> use open5gs
switched to db open5gs
> db.subscribers.drop()
true
```
2. Then, if you are using a version of WebUI prior to v2.1.7, you need to do a WebUI logout from your web browser.
3. Finally, install the latest version of WebUI with the following command.
```
$ curl -fsSL https://open5gs.org/open5gs/assets/webui/install | sudo -E bash -
```
4. Log in to the new WebUI and add new subscriber information using your web browser.
5. Make sure it is a new DB schema as below:
```
$ mongo
> use open5gs
> db.subscribers.find().pretty()
{
...
	"slice" : [
		{
			"sst" : 1,
			"default_indicator" : true,
			"_id" : ObjectId("60969fe7de8743b3c7b1a973"),
			"session" : [
...
}
```

If the above problem still occurs, we recommend that you delete all Open5GS and start from scratch.

#### 5G Core test failed (e.g. `./build/tests/registration/registration`)

The AUSF(or SMF) has already been started, but NRF-discovery fails and the test program does not work as shown below.

```
$ ./build/tests/registration/registration

...
04/12 14:26:25.885: [app] ERROR: Cannot discover [AUSF] (../lib/sbi/path.c:119)
...
04/12 14:27:25.666: [app] ERROR: Cannot discover [SMF] (../lib/sbi/path.c:119)
```

This is a test program bug and has not yet been resolved.
{: .blue}

To restart the test program, first remove all subscriber information using MongoDB Client
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
$ sudo pkill -9 open5gs-mmed
$ sudo pkill -9 open5gs-sgwcd
$ sudo pkill -9 open5gs-smfd
$ sudo pkill -9 open5gs-amfd
$ sudo pkill -9 open5gs-sgwud
$ sudo pkill -9 open5gs-upfd
$ sudo pkill -9 open5gs-hssd
$ sudo pkill -9 open5gs-pcrfd
$ sudo pkill -9 open5gs-nrfd
$ sudo pkill -9 open5gs-ausfd
$ sudo pkill -9 open5gs-udmd
$ sudo pkill -9 open5gs-pcfd
$ sudo pkill -9 open5gs-nssfd
$ sudo pkill -9 open5gs-bsfd
$ sudo pkill -9 open5gs-udrd
```

Run `./build/tests/registration/registration` again
```
$ ./build/tests/registration/registration
guti-test           : SUCCESS
auth-test           : SUCCESS
idle-test           : SUCCESS
dereg-test          : SUCCESS
paging-test         : SUCCESS
identity-test       : SUCCESS
gmm-status-test     : SUCCESS
ue-context-test     : SUCCESS
reset-test          : SUCCESS
All tests passed.
```

In general, if this test program succeeds at least once, you can assume that you have successfully built the development environment.

#### How to use a different Slice for each SMF

To add a slice with SST of 1 and SD of 000080, you need to update the configuration file as shown below.

```diff
### amf.yaml

$ diff --git a/configs/open5gs/amf.yaml.in b/configs/open5gs/amf.yaml.in
index 7e939e81..dfe4456d 100644
--- a/configs/open5gs/amf.yaml.in
+++ b/configs/open5gs/amf.yaml.in
@@ -199,6 +199,12 @@ amf:
           mnc: 70
         s_nssai:
           - sst: 1
+      - plmn_id:
+          mcc: 999
+          mnc: 70
+        s_nssai:
+          - sst: 1
+            sd: 000080
     security:
         integrity_order : [ NIA2, NIA1, NIA0 ]
         ciphering_order : [ NEA0, NEA1, NEA2 ]

### FIRST smf.yaml

$ diff --git a/configs/open5gs/smf.yaml.in b/configs/open5gs/smf.yaml.in
index d45aa60f..701ee533 100644
--- a/configs/open5gs/smf.yaml.in
+++ b/configs/open5gs/smf.yaml.in
@@ -317,6 +317,11 @@ logger:
 # 
 
 smf:
+    info:
+      - s_nssai:
+          - sst: 1
+            dnn:
+              - internet
     sbi:
       - addr: 127.0.0.4
         port: 7777

### SECOND smf.yaml

$ diff --git a/configs/open5gs/smf.yaml.in b/configs/open5gs/smf.yaml.in
index d45aa60f..949da220 100644
--- a/configs/open5gs/smf.yaml.in
+++ b/configs/open5gs/smf.yaml.in
@@ -317,6 +317,12 @@ logger:
 #

 smf:
+    info:
+      - s_nssai:
+          - sst: 1
+            sd: 000080
+            dnn:
+              - internet
     sbi:
       - addr: 127.0.0.4
         port: 7777

### nssf.yaml
$ diff --git a/configs/open5gs/nssf.yaml.in b/configs/open5gs/nssf.yaml.in
index ecd4f7e2..04d9c4ba 100644
--- a/configs/open5gs/nssf.yaml.in
+++ b/configs/open5gs/nssf.yaml.in
@@ -119,6 +119,11 @@ nssf:
         port: 7777
         s_nssai:
           sst: 1
+      - addr: 127.0.0.10
+        port: 7777
+        s_nssai:
+          sst: 1
+          sd: 000080
 
 #
 # nrf:
```

Then add a slice to MongoDB's subscriber info.

![Subscriber Info]({{ site.url }}{{ site.baseurl }}/assets/images/subscriber_info_with_two_slice.png)


#### Can I disable specific services if 5G functionally is not needed?


From v2.0.x, SGW was divided into SGW-C and SGW-U, and PGW function was seperated into SMF and UPF.

In order to use 4G only, you need to run the process below.
```bash
$ open5gs-mmed
$ open5gs-sgwcd
$ open5gs-smfd
$ open5gs-sgwud
$ open5gs-upfd
$ open5gs-hssd
$ open5gs-pcrfd
```

And the process below is only used in 5G, so there is no need to run it.

```bash
$ open5gs-nrfd
$ open5gs-amfd
$ open5gs-ausfd
$ open5gs-udmd
$ open5gs-pcfd
$ open5gs-nssfd
$ open5gs-bsfd
$ open5gs-udrd
```

However, among these, SMF and UPF are used by both 4G EPC and 5G Core. And SMF has a protocol stack to interact with 5G NRF. Therefore, if you run SMF without running 5G NRF, the following WARNING occurs in SMF.

```
10/08 14:44:03.045: [sbi] WARNING: [7] Failed to connect to ::1 port 7777: Connection refused (../lib/sbi/client.c:450)
10/08 14:44:03.045: [smf] INFO: PFCP associated (../src/smf/pfcp-sm.c:174)
10/08 14:44:03.046: [diam] INFO: CONNECTED TO 'pcrf.localdomain' (SCTP,soc#16): (../lib/diameter/common/logger.c:108)
10/08 14:44:06.046: [smf] WARNING: [3c85dd06-0996-41eb-a985-476fa905aefc] Retry to registration with NRF (../src/smf/nf-sm.c:161)
10/08 14:44:06.047: [sbi] WARNING: [7] Failed to connect to ::1 port 7777: Connection refused (../lib/sbi/client.c:450)
```

To prevent SMF from attempting to access the 5G NRF, you need to modify the SMF configuration file as below.

```diff
$ diff -u ./install/etc/open5gs/smf.yaml.old ./install/etc/open5gs/smf.yaml
--- ./install/etc/open5gs/smf.yaml.old 2020-10-08 14:43:20.599734045 -0400
+++ ./install/etc/open5gs/smf.yaml 2020-10-08 14:44:21.864952687 -0400
@@ -168,9 +168,9 @@
 #      - ::1
 #
 smf:
-    sbi:
-      - addr: 127.0.0.4
-        port: 7777
+#    sbi:
+#      - addr: 127.0.0.4
+#        port: 7777
     gtpc:
       - addr: 127.0.0.4
       - addr: ::1
@@ -214,12 +214,12 @@
 #        - 127.0.0.10
 #        - fe80::1%lo
 #
-nrf:
-    sbi:
-      - addr:
-          - 127.0.0.10
-          - ::1
-        port: 7777
+#nrf:
+#    sbi:
+#      - addr:
+#          - 127.0.0.10
+#          - ::1
+#        port: 7777

 #
 # upf:
```

If you set as above and run SMF, you do not need to run NRF. Seven daemons operate in 4G only state.

#### How to change UE IP Pool

The Open5GS package contains a systemd-networkd configuration file for `ogstun`. Therefore, you must first modify the configuration file as follows.

```diff
$ diff -u /etc/systemd/network/99-open5gs.network /etc/systemd/network/99-open5gs.network.new
--- /etc/systemd/network/99-open5gs.network	2020-09-17 09:29:09.137392040 -0400
+++ /etc/systemd/network/99-open5gs.network.new	2020-09-17 09:29:03.375719620 -0400
@@ -2,5 +2,5 @@
 Name=ogstun

 [Network]
-Address=10.45.0.1/16
+Address=10.46.0.1/16
 Address=2001:db8:cafe::1/48
```

Restart systemd-networkd
```
$ sudo systemctl restart systemd-networkd
```

And then, you need to chanage NAT table as below.
```
$ sudo iptables -t nat -A POSTROUTING -s 10.46.0.0/16 ! -o ogstun -j MASQUERADE
```

Now, you need to modify the configuration file of Open5GS to adjust the UE IP Pool. UE IP Pool can be allocated by SMF or UPF, but in this tutorial, we will modify both configuration files.

```diff
$ diff -u smf.yaml smf.yaml.new
--- smf.yaml	2020-09-17 09:31:16.547882093 -0400
+++ smf.yaml.new	2020-09-17 09:32:18.267726844 -0400
@@ -190,7 +190,7 @@
       - addr: 127.0.0.4
       - addr: ::1
     subnet:
-      - addr: 10.45.0.1/16
+      - addr: 10.46.0.1/16
       - addr: 2001:db8:cafe::1/48
     dns:
       - 8.8.8.8
```

```diff
$ diff -u upf.yaml upf.yaml.new
--- upf.yaml	2020-09-17 09:31:16.547882093 -0400
+++ upf.yaml.new	2020-09-17 09:32:25.199619989 -0400
@@ -139,7 +139,7 @@
     gtpu:
       - addr: 127.0.0.7
     subnet:
-      - addr: 10.45.0.1/16
+      - addr: 10.46.0.1/16
       - addr: 2001:db8:cafe::1/48

 #
```

Restart SMF/UPF
```
$ sudo systemctl restart open5gs-smfd.service
$ sudo systemctl restart open5gs-upfd.service
```

#### Wireshark cannot decode NAS-5GS

By default, wireshark cannot decode NAS-5GS message when the security header type is "Integrity protected and ciphered".

![Wireshark cannot decode]({{ site.url }}{{ site.baseurl }}/assets/images/wireshark_cannot_decode_nas_5gs.png)

You need to turn on "Try to detect and decode 5G-EA0 ciphered messages" in the wireshark perference menu.

![Wireshark perference]({{ site.url }}{{ site.baseurl }}/assets/images/wireshark_preference.png)

Now, you can see the NAS-5GS message in the wireshark.

![Wireshark can decode]({{ site.url }}{{ site.baseurl }}/assets/images/wireshark_can_decode_nas_5gs.png)

#### Test failed (e.g. `meson test -v`)

If MongoDB server is not started, you may get a message like this:

```
$ meson test -v

...
4/10 open5gs:unit / unit OK 0.06 s
09/30 01:12:37.829: [core] FATAL: test_5gc_init: Assertion ogs_dbi_init(ogs_app()->db_uri) == OGS_OK' failed. (../tests/app/5gc-init.c:100)
09/30 01:12:37.830: [core] FATAL: backtrace() returned 8 addresses (../lib/core/ogs-abort.c:37)
/home/open5gs/build/tests/registration/registration(+0x1bfd0) [0x55af96a05fd0]
/home/open5gs/build/tests/registration/registration(+0x3c2e) [0x55af969edc2e]
/home/open5gs/build/tests/registration/registration(+0x25151) [0x55af96a0f151]
/home/open5gs/build/tests/registration/registration(+0x251a5) [0x55af96a0f1a5]
/home/open5gs/build/tests/registration/registration(+0x3cde) [0x55af969edcde]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xe7) [0x7f3c2bb97b97]
/home/open5gs/build/tests/registration/registration(+0x39aa) [0x55af969ed9aa]
5/10 open5gs:5gc / registration FAIL 0.27 s
09/30 01:12:38.073: [core] FATAL: test_epc_init: Assertion ogs_dbi_init(ogs_app()->db_uri) == OGS_OK' failed. (../tests/app/epc-init.c:105)
09/30 01:12:38.073: [core] FATAL: backtrace() returned 8 addresses (../lib/core/ogs-abort.c:37)
/home/open5gs/build/tests/attach/attach(+0x12362) [0x55ef42081362]
/home/open5gs/build/tests/attach/attach(+0x367e) [0x55ef4207267e]
/home/open5gs/build/tests/attach/attach(+0x1b4e3) [0x55ef4208a4e3]
/home/open5gs/build/tests/attach/attach(+0x1b537) [0x55ef4208a537]
/home/open5gs/build/tests/attach/attach(+0x372e) [0x55ef4207272e]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xe7) [0x7f10b2e30b97]
/home/open5gs/build/tests/attach/attach(+0x33fa) [0x55ef420723fa]
6/10 open5gs:epc / attach FAIL 0.17 s
09/30 01:12:38.239: [core] FATAL: test_epc_init: Assertion `ogs_dbi_init(ogs_app()->db_uri) == OGS_OK' failed. (../tests/app/epc-init.c:105)
09/30 01:12:38.239: [core] FATAL: backtrace() returned 8 addresses (../lib/core/ogs-abort.c:37)
/home/open5gs/build/tests/volte/volte(+0x22272) [0x555df9643272]
/home/open5gs/build/tests/volte/volte(+0x1210a) [0x555df963310a]
/home/open5gs/build/tests/volte/volte(+0x2b3f3) [0x555df964c3f3]
/home/open5gs/build/tests/volte/volte(+0x2b447) [0x555df964c447]
/home/open5gs/build/tests/volte/volte(+0x12221) [0x555df9633221]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xe7) [0x7fd1df6b6b97]
/home/open5gs/build/tests/volte/volte(+0x4daa) [0x555df9625daa]
7/10 open5gs:epc / volte FAIL 0.17 s
...
```

Please make sure that MongoDB server daemon is running.
{: .blue}

Then, remove all subscriber information using MongoDB Client
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
$ sudo pkill -9 open5gs-mmed
$ sudo pkill -9 open5gs-sgwcd
$ sudo pkill -9 open5gs-smfd
$ sudo pkill -9 open5gs-amfd
$ sudo pkill -9 open5gs-sgwud
$ sudo pkill -9 open5gs-upfd
$ sudo pkill -9 open5gs-hssd
$ sudo pkill -9 open5gs-pcrfd
$ sudo pkill -9 open5gs-nrfd
$ sudo pkill -9 open5gs-ausfd
$ sudo pkill -9 open5gs-udmd
$ sudo pkill -9 open5gs-pcfd
$ sudo pkill -9 open5gs-nssfd
$ sudo pkill -9 open5gs-bsfd
$ sudo pkill -9 open5gs-udrd
```

Run `meson test -v` again
```
$ meson test -v
ninja: Entering directory `/home/acetcom/Documents/git/open5gs/build'
ninja: no work to do.
list-test           : SUCCESS
pool-test           : SUCCESS
strings-test        : SUCCESS
time-test           : SUCCESS
conv-test           : SUCCESS
log-test            : SUCCESS
pkbuf-test          : SUCCESS
memory-test         : SUCCESS
rbtree-test         : SUCCESS
timer-test          : SUCCESS
thread-test         : SUCCESS
socket-test         : SUCCESS
queue-test          : SUCCESS
poll-test           : SUCCESS
tlv-test            : SUCCESS
fsm-test            : SUCCESS
hash-test           : SUCCESS
uuid-test           : SUCCESS
All tests passed.
 1/10 open5gs:unit / core                     OK       6.89 s
aes-test            : SUCCESS
sha-test            : SUCCESS
base64-test         : SUCCESS
All tests passed.
 2/10 open5gs:unit / crypt                    OK       0.12 s
sctp-test           : SUCCESS
All tests passed.
 3/10 open5gs:app / sctp                      OK       0.02 s
s1ap-message-test   : SUCCESS
nas-message-test    : SUCCESS
gtp-message-test    : SUCCESS
sbi-message-test    : SUCCESS
security-test       : SUCCESS
crash-test          : SUCCESS
All tests passed.
 4/10 open5gs:unit / unit                     OK       0.03 s
guti-test           : SUCCESS
auth-test           : SUCCESS
idle-test           : SUCCESS
dereg-test          : SUCCESS
identity-test       : SUCCESS
gmm-status-test     : SUCCESS
ue-context-test     : SUCCESS
All tests passed.
 5/10 open5gs:5gc / registration              OK       4.98 s
s1setup-test        : SUCCESS
guti-test           : SUCCESS
auth-test           : SUCCESS
idle-test           : SUCCESS
emm-status-test     : SUCCESS
reset-test          : SUCCESS
ue-context-test     : SUCCESS
All tests passed.
 6/10 open5gs:epc / attach                    OK       4.68 s
bearer-test         : SUCCESS
session-test        : SUCCESS
rx-test             : SUCCESS
All tests passed.
 7/10 open5gs:epc / volte                     OK       4.33 s
mo-idle-test        : SUCCESS
mt-idle-test        : SUCCESS
mo-active-test      : SUCCESS
mt-active-test      : SUCCESS
mo-sms-test         : SUCCESS
mt-sms-test         : SUCCESS
crash-test          : SUCCESS
All tests passed.
 8/10 open5gs:epc / csfb                      OK       4.08 s
epc-test            : SUCCESS
All tests passed.
 9/10 open5gs:app / 310014                    OK       2.37 s
epc-x2-test         : SUCCESS
epc-s1-test         : SUCCESS
All tests passed.
10/10 open5gs:app / handover                  OK       4.78 s

Ok:                   10
Expected Fail:         0
Fail:                  0
Unexpected Pass:       0
Skipped:               0
Timeout:               0
```

#### Is it possible to setup IP/NAT table along with Docker?

Enable IPv4/IPv6 Forward.
```
$ sudo sysctl -w net.ipv4.ip_forward=1
$ sudo sysctl -w net.ipv6.conf.all.forwarding=1
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

Docker doesn't have IPv6 NAT rules. In this case, you just add the NAT rule as below.
```
$ sudo ip6tables -t nat -A POSTROUTING -s 2001:db8:cafe::/48 ! -o ogstun -j MASQUERADE
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

### Enable IPv4/IPv6 Forwarding
$ sudo sysctl -w net.ipv4.ip_forward=1
$ sudo sysctl -w net.ipv6.conf.all.forwarding=1

### Add NAT Rule
$ sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
$ sudo ip6tables -t nat -A POSTROUTING -s 2001:db8:cafe::/48 ! -o ogstun -j MASQUERADE
```

#### How to use a different DNN/APN for each SMF

By default, MME selects the SMF as the first SMF node. To use a different DNN/APN for each SMF, specify gtpc.dnn as the DNN/APN name. If the HSS uses WebUI to set the SMF IP for each UE, you can use a specific SMF node for each UE.

See the following example.

```
### For reference, see `smf.yaml`
#
# smf:
#
#  <GTP-C Client>
#
#  o By default, the SMF uses the first SMF node.
#    - To use a different DNN/APN for each SMF, specify gtpc.dnn
#      as the DNN/APN name.
#    - If the HSS uses WebUI to set the SMF IP for each UE,
#      you can use a specific SMF node for each UE.
#
#  o Two SMF are defined. 127.0.0.4:2123 is used.
#    [fe80::3%lo]:2123 is ignored.
#    gtpc:
#      - addr: 127.0.0.4
#      - addr: fe80::3%lo
#
#  o One SMF is defined. if prefer_ipv4 is not true,
#    [fe80::3%lo] is selected.
#    gtpc:
#      - addr:
#        - 127.0.0.4
#        - fe80::3%lo
#
#  o Two SMF are defined with a different DNN/APN.
#    - Note that if SMF IP for UE is configured in HSS,
#      the following configurion for this UE is ignored.
#    gtpc:
#      - addr: 127.0.0.4
#        dnn: internet
#      - addr: 127.0.0.5
#        dnn: volte
#
#  o If DNN/APN is omitted, the default DNN/APN uses the first SMF node.
#    gtpc:
#      - addr: 127.0.0.4
#      - addr: 127.0.0.5
#        dnn: volte
```

The IP address of the UE can also use a different UE pool depending on the DNN/APN.

```
### For reference, see `smf.yaml`
#  <Subnet for UE Pool>
#
#  o IPv4 Pool
#    $ sudo ip addr add 10.45.0.1/16 dev ogstun
#
#    subnet:
#      addr: 10.45.0.1/16
#
#  o IPv4/IPv6 Pool
#    $ sudo ip addr add 10.45.0.1/16 dev ogstun
#    $ sudo ip addr add 2001:db8:cafe::1/48 dev ogstun
#
#    subnet:
#      - addr: 10.45.0.1/16
#      - addr: 2001:db8:cafe::1/48
#
#
#  o Specific DNN/APN(e.g 'volte') uses 10.46.0.1/16, 2001:db8:babe::1/48
#    All other DNNs/APNs use 10.45.0.1/16, 2001:db8:cafe::1/48
#    $ sudo ip addr add 10.45.0.1/16 dev ogstun
#    $ sudo ip addr add 10.46.0.1/16 dev ogstun
#    $ sudo ip addr add 2001:db8:cafe::1/48 dev ogstun
#    $ sudo ip addr add 2001:db8:babe::1/48 dev ogstun
#
#    subnet:
#      - addr: 10.45.0.1/16
#      - addr: 2001:db8:cafe::1/48
#      - addr: 10.46.0.1/16
#        dnn: volte
#      - addr: 2001:db8:babe::1/48
#        dnn: volte
#
#  o Pool Range Sample
#    subnet:
#      - addr: 10.45.0.1/24
#        range: 10.45.0.100-10.45.0.200
#
#    subnet:
#      - addr: 10.45.0.1/24
#        range:
#          - 10.45.0.5-10.45.0.50
#          - 10.45.0.100-
#
#    subnet:
#      - addr: 10.45.0.1/24
#        range:
#          - -10.45.0.200
#          - 10.45.0.210-10.45.0.220
#
#    subnet:
#      - addr: 10.45.0.1/16
#        range:
#          - 10.45.0.100-10.45.0.200
#          - 10.45.1.100-10.45.1.200
#      - addr: 2001:db8:cafe::1/48
#        range:
#          - 2001:db8:cafe:a0::0-2001:db8:cafe:b0::0
#          - 2001:db8:cafe:c0::0-2001:db8:cafe:d0::0
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

#### I have some error when running `./build/tests/attach/attach`

Did you see the following error after executing `./build/tests/attach/attach`?
```bash
$ ./build/tests/attach/attach
s1setup_test        : SUCCESS  
attach_test         : -Line 134: Condition is false, but expected true  
\04/09 15:49:09.285: [esm] FATAL: esm_handle_pdn_connectivity_request: Assertion `SECURITY_CONTEXT_IS_VALID(mme_ue)' failed. (esm_handler.c:29)  
/home/acetcom/Documents/git/open5gs/open5gs/lib/ogslib/src/core/.libs/libogscore-1.0.so.0(ogs_abort+0x2b)[0x7f608518271b]  
/home/acetcom/Documents/git/open5gs/open5gs/test/.libs/simple(+0x92121)[0x55dc9e274121]
/home/acetcom/Documents/git/open5gs/open5gs/test/.libs/simple(+0x4f5b9)[0x55dc9e2315b9]
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
$ ps -ef | grep attach
$ ps -ef | grep open5gs
$ sudo pkill -9 attach
$ sudo pkill -9 open5gs-mmed ...
```

Execute `./build/tests/attach/attach`
```bash
$ ./build/tests/attach/attach
```

#### My gNB/eNB does not support IPv6.

Your eNodeB don't have to support IPv6.

If the sgwu.gtpu configuration does not have an IPv6 address, the gNB/eNB can use IPv4 to connect to the MME and SGW-U. If the sgwu.gtpu setting has an IPv6 address, you can disable the IPv6 address as shown below.

```yaml
parameter:
    no_ipv6: true
```

**Note:** This parameter `no_ipv6` is only applied to NFs such as AMF, MME, SGW-C, and so on. The parameter `no_ipv6` does not affect to UE. So, IPv6-enabled UE can connect to Open5GS LTE network.
{: .notice--warning}

#### Unable to add new user by WebUI 

It might be a CSRF token mismatch error.
Please, delete corresponding cookies, cache, session data etc.

#### Change Domain from localdomain to anything else

You should configure the domain name on your computer. Otherwise, freeDiameter raise an error.

#### How many of UEs can Open5GS support?

See the [lib/core/ogs-3gpp-types.h](https://github.com/{{ site.github_username }}/open5gs/blob/main/lib/core/ogs-3gpp-types.h).

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
* MongoDB : 127.0.0.1
* MME : 127.0.0.2
* SGW-C : 127.0.0.3
* SMF : 127.0.0.4
* AMF : 127.0.0.5
* SGW-U : 127.0.0.6
* UPF : 127.0.0.7
* HSS : 127.0.0.8
* PCRF : 127.0.0.9
* NRF : 127.0.0.10
* AUSF : 127.0.0.11
* UDM : 127.0.0.12
* PCF : 127.0.0.13
* NSSF : 127.0.0.14
* BSF : 127.0.0.15
* UDR : 127.0.0.20
```

- AMF_ID, TAC and S_NSSAI

```
* AMF_ID
  PLMN ID - MNC: 999, MCC: 70
  Region : 2
  Set : 1

* TAI
  PLMN ID - MNC: 999, MCC: 70
  TAC : 1

* S_NSSASI
  SST : 1
```

- GUMMEI, PLMN and TAC

```
* GUMMEI
  PLMN ID - MNC: 999, MCC: 70
  MME Group : 2
  MME Code : 1

* TAI
  PLMN ID - MNC: 999, MCC: 70
  TAC : 1
```

- Security

```
* Integrity : NIA1/EIA1 - Snow 3G
* Ciphering : NEA0/EEA0 - Nothing
```

- UE Network

```
* IPv4 : 10.45.0.1/16
* IPv6 : 2001:db8:cafe::1/48
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
$ git clone https://github.com/{{ site.github_username }}/open5gs
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
$ git clone https://github.com/{{ site.github_username }}/open5gs
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

#### Cannot open shared object file when running daemon

An error occurred when running as follows.

```
$ ./install/bin/open5gs-nrfd
./install/bin/open5gs-nrfd: error while loading shared libraries: libogscrypt.so.2: cannot open shared object file: No such file or directory
```

You need to specify the absolute path to the shared library as follows.

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
