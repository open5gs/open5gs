---
title: Build on Ubuntu/Debian
---

### Using Meson

Install the depedencies for using Meson.
```bash
$ sudo apt install python3 python3-pip python3-setuptools python3-wheel ninja-build
```

Install Meson using Python
```bash
$ pip3 install --user meson
```

### Making TUN Permanent
---

Write a configuration file for the TUN deivce.
```bash
$ sudo sh -c "cat << EOF > /etc/systemd/network/99-open5gs.netdev
[NetDev]
Name=ogstun
Kind=tun
EOF"
```

Create a TUN device. The interface name will be `ogstun`.
```bash
$ sudo systemctl enable systemd-networkd
$ sudo systemctl restart systemd-networkd

$ sudo apt install net-tools
$ ifconfig ogstun
ogstun: flags=4241<UP,POINTOPOINT,NOARP,MULTICAST>  mtu 1500
        inet6 fe80::e86e:86d8:ea24:f8ee  prefixlen 64  scopeid 0x20<link>
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 500  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 2  bytes 255 (255.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

Then, to support IPv6-enabled UEs, you must configure your TUN device to support IPv6.

```bash
$ sysctl -n net.ipv6.conf.ogstun.disable_ipv6
1

$ sudo sh -c "echo 'net.ipv6.conf.ogstun.disable_ipv6=0' > /etc/sysctl.d/30-open5gs.conf"
$ sudo sysctl -p /etc/sysctl.d/30-open5gs.conf

$ sysctl -n net.ipv6.conf.ogstun.disable_ipv6
0
```

**Note:** If your TUN device already supports IPv6, you can skip this steps above.
{: .notice--warning}


You are now ready to set the IP address on TUN device. 

```bash
$ sudo sh -c "cat << EOF > /etc/systemd/network/99-open5gs.network
[Match]
Name=ogstun
[Network]
Address=10.45.0.1/16
Address=2001:db8:cafe::1/48
EOF"
```

Restart the TUN device

```bash
$ sudo systemctl restart systemd-networkd
```

Make sure it is set up properly.


```bash
$ ifconfig ogstun
ogstun: flags=4305<UP,POINTOPOINT,RUNNING,NOARP,MULTICAST>  mtu 1500
        inet 10.45.0.1  netmask 255.255.0.0  destination 10.45.0.1
        inet6 2001:db8:cafe::1  prefixlen 64  scopeid 0x0<global>
        inet6 fe80::e86e:86d8:ea24:f8ee  prefixlen 64  scopeid 0x20<link>
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 500  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 20  bytes 2019 (2.0 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

**Note:** On *Linux*, you should run `open5gs-pgwd` first to correctly verify the IP address of TUN device. Otherwise, the TUN device's IP address is not displayed by the [ifconfig](http://net-tools.sourceforge.net/man/ifconfig.8.html) command.
{: .notice--warning}

