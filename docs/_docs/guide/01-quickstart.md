---
title: Qucikstart
---

**Note:** NextEPC supports installation of packages in *Debian/Ubuntu and openSUSE* environments. *CentOS, Fedora, FreeBSD, and Mac OSX* require you to [build with source code]({{ site.url }}{{ site.baseurl }}/docs/guide/02-building-nextepc-from-sources)
{: .notice--warning}

### Install NextEPC with a Package Manager
---

The nextepc package is available on the recent versions of *Ubuntu*.

```bash
$ sudo apt update
$ sudo apt install software-properties-common
$ sudo add-apt-repository ppa:acetcom/nextepc
$ sudo apt update
$ sudo apt install nextepc
```

>The NextEPC package is also available on [OBS](https://build.opensuse.org/package/show/home:acetcom:open5gs:snapshot/nextepc). First, install the authentication key as shown below.
```bash
$ sudo apt install wget
$ wget https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.10/Release.key
$ sudo apt install gnupg
$ sudo apt-key add Release.key
```
In Debian 9.0, you can install it as follows:
```bash
$ sudo sh -c "echo 'deb https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_9.0/ ./' > /etc/apt/sources.list.d/open5gs.list"
$ sudo apt update
$ sudo apt install nextepc
```
Other Linux distributions can be installed by changing the path.
```
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Raspbian_9.0/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_16.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_17.10/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.10/
```
{: .notice--success}

[Martin Hauke](https://build.opensuse.org/user/show/mnhauke) packaged NextEPC for *openSUSE* on [OBS](https://build.opensuse.org/package/show/home:mnhauke:nextepc/nextepc).

```bash
$ sudo zypper addrepo -f obs://home:mnhauke:nextepc home:mnhauke:nextepc
$ sudo zypper install nextepc
$ sudo zypper install mongodb-server mongodb-shell
```

### Configure NextEPC
---

Modify [/etc/nextepc/mme.conf](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/config/mme.conf.in) to set the S1AP/GTP-C IP address, PLMN ID, and TAC

```diff
diff -u /etc/nextepc/mme.conf.old /etc/nextepc/mme.conf
--- mme.conf.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.conf	2018-04-15 19:53:10.000000000 +0900
@@ -8,18 +8,20 @@ parameter:
 mme:
     freeDiameter: mme.conf
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

Modify [/etc/nextepc/sgw.conf](https://github.com/{{ site.github_username }}/nextepc/blob/master/support/config/sgw.conf.in) to set the GTP-U IP address.  
```diff
diff -u /etc/nextepc/sgw.conf.old /etc/nextepc/sgw.conf
--- sgw.conf.old	2018-04-15 18:30:25.000000000 +0900
+++ sgw.conf	2018-04-15 18:30:30.000000000 +0900
@@ -14,3 +14,4 @@
     gtpc:
       addr: 127.0.0.2
     gtpu:
+      addr: 192.168.0.100
```

After changing conf files, please restart NextEPC daemons.

```bash
$ sudo systemctl restart nextepc-mmed
$ sudo systemctl restart nextepc-sgwd
```

### Install WebUI of NextEPC
---

[Node.js](https://nodejs.org/) is required to install WebUI of NextEPC

1. *Debian and Ubuntu* based Linux distributions can install [Node.js](https://nodejs.org/) as follows:

    ```bash
    $ sudo apt install curl
    $ curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -
    $ sudo apt install nodejs
    ```

2. To install [Node.js](https://nodejs.org/) on *openSUSE*, run the following:

    ```bash
    $ sudo zypper install nodejs8
    ```

You can now install WebUI of NextEPC.

```bash
$ curl -sL {{ site.url }}/static/webui/install | sudo -E bash -
```

### Register Subscriber Information
---

Connect to `http://localhost:3000` and login with **admin** account.

> Username : admin  
> Password : 1423

**Tip:** You can change the password in _Account_ Menu.
{: .notice--info}

To add subscriber information, you can do WebUI operations in the following order:

  1. Go to `Subscriber` Menu.
  2. Click `+` Button to add a new subscriber.
  3. Fill the IMSI, security context(K, OPc, AMF), and APN of the subscriber.
  4. Click `SAVE` Button

**Tip:** This addition immediately affects NextEPC without restaring any daemon.
{: .notice--info}


### Adding a route for UE to have Internet connectivity
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
- You can view the log at `/var/log/nextepc/*.log`.

### Troubleshooting
---

Problem with NextEPC can be filed as [GitHub Issues](https://github.com/open5gs/nextepc/issues). Please include the following to get help:

- Attach `*.pcapng` file created by wireskark.
- Attach configuration files at `/etc/nextepc/*.conf`.
- Attach log files at `/var/log/nextepc/*.log`.

You can modify the configuration file to record more logs.

```diff
diff -u /etc/nextepc/mme.conf.old /etc/nextepc/mme.conf
--- mme.conf.old	2018-04-15 18:28:31.000000000 +0900
+++ mme.conf	2018-04-15 19:53:10.000000000 +0900
@@ -2,6 +2,7 @@

 logger:
     file: @LOCALSTATE_DIR@/log/nextepc/mme.log
+    level: debug

 parameter:
```

After changing conf files, please restart NextEPC daemons.

```bash
$ sudo systemctl restart nextepc-mmed
$ sudo systemctl restart nextepc-sgwd
```

### Uninstall NextEPC and WebUI

How to remove NextEPC package:

1. On *Ubuntu*:

    ```bash
    $ sudo apt purge nextepc*
    ```

2. On *openSUSE*:

    ```bash
    $ sudo zypper rm nextepc
    ```

You may need to remove manually `/var/log/nextepc` unless it is empty.

```bash
$ sudo rm -Rf /var/log/nextepc
```

The WebUI of NextEPC can be removed as follows:

```bash
curl -sL {{ site.url }}{{ site.baseurl }}/assets/webui/uninstall | sudo -E bash -
```

