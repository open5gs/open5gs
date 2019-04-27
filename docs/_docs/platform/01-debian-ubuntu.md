---
title: Build on Ubuntu/Debian
---

### TUN device permissions 
---

To run nextepc with least privilege, the TUN device privilege should be a `crw-rw-rw-`(666). Otherwise, you need to run nextepc daemon as root. If the permission is not `crw-rw-rw-`(666), you may need to install [udev](https://mirrors.edge.kernel.org/pub/linux/utils/kernel/hotplug/udev/udev.html) package.

```bash
$ ls -al /dev/net/tun
crw-rw---- 1 root 28 10, 200 Feb 11 05:13 /dev/net/tun

$ sudo apt install udev
$ sudo systemctl start systemd-udevd (if '/lib/systemd/systemd-udevd' is not running)

$ ls -al /dev/net/tun
crw-rw-rw- 1 root 28 10, 200 Feb 11 05:13 /dev/net/tun
```

Nevertheless, if the permission do not change, you can run nextepc with root privileges or change the permission using [chmod](https://www.gnu.org/software/coreutils/manual/html_node/chmod-invocation.html) as follows:

```bash
$ sudo chmod 666 /dev/net/tun
```

### Making TUN Permanent
---

Write a configuration file for the TUN deivce.
```bash
$ sudo sh -c "cat << EOF > /etc/systemd/network/99-nextepc.netdev
[NetDev]
Name=pgwtun
Kind=tun
EOF"
```

Create a TUN device. The interface name will be `pgwtun`.
```bash
$ sudo systemctl enable systemd-networkd
$ sudo systemctl restart systemd-networkd

$ sudo apt install net-tools
$ ifconfig pgwtun
pgwtun: flags=4241<UP,POINTOPOINT,NOARP,MULTICAST>  mtu 1500
        inet6 fe80::e86e:86d8:ea24:f8ee  prefixlen 64  scopeid 0x20<link>
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 500  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 2  bytes 255 (255.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

Then, to support IPv6-enabled UEs, you must configure your TUN device to support IPv6.

```bash
$ sysctl -n net.ipv6.conf.pgwtun.disable_ipv6
1

$ sudo sh -c "echo 'net.ipv6.conf.pgwtun.disable_ipv6=0' > /etc/sysctl.d/30-nextepc.conf"
$ sudo sysctl -p /etc/sysctl.d/30-nextepc.conf

$ sysctl -n net.ipv6.conf.pgwtun.disable_ipv6
0
```

**Note:** If your TUN device already supports IPv6, you can skip this steps above.
{: .notice--warning}


You are now ready to set the IP address on TUN device. 

```bash
$ sudo sh -c "cat << EOF > /etc/systemd/network/99-nextepc.network
[Match]
Name=pgwtun
[Network]
Address=45.45.0.1/16
Address=cafe::1/64
EOF"
```

Restart the TUN device

```bash
$ sudo systemctl restart systemd-networkd
```

Make sure it is set up properly.


```bash
$ ifconfig pgwtun
pgwtun: flags=4305<UP,POINTOPOINT,RUNNING,NOARP,MULTICAST>  mtu 1500
        inet 45.45.0.1  netmask 255.255.0.0  destination 45.45.0.1
        inet6 cafe::1  prefixlen 64  scopeid 0x0<global>
        inet6 fe80::e86e:86d8:ea24:f8ee  prefixlen 64  scopeid 0x20<link>
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 500  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 20  bytes 2019 (2.0 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

**Note:** On *Linux*, you should run `nextepc-pgwd` first to correctly verify the IP address of TUN device. Otherwise, the TUN device's IP address is not displayed by the [ifconfig](http://net-tools.sourceforge.net/man/ifconfig.8.html) command.
{: .notice--warning}

