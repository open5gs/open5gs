---
title: Open5GS Splitting Network Elements
head_inline: "<style> .blue { color: blue; } </style>"
---

In a production network, NFs would typically not all be on the same machine, as is the default example that ships with Open5GS.

Open5GS is designed to be standards compliant, so in theory you can connect any core network function from Open5GS or any other vendor to form a functioning network, so long as they are 3GPP compliant. 

To demonstrate this we will cover isolating each network element onto it's on machine and connect each network element to the other. For some interfaces specifying multiple interfaces is supported to allow connection to multiple

In these examples we'll be connecting Open5GS NFs together, but it could just as easily be NFs from a different vendor in the place of any Open5GS network functions.

| Service        | IP           | Identity |
| ------------- |:-------------:|:-------------:|
| SMF  | 10.0.1.121 | smf.localdomain |
| SGW-C | 10.0.1.122 | |
| PCRF | 10.0.1.123 | pcrf.localdomain |
| MME | 10.0.1.124 | mme.localdomain |
| HSS | 10.0.1.118 | hss.localdomain |


# External SMF
In it's simplest from the SMF has 3 interfaces:
 * S5 - Connection to home network SGW-C (GTP-C)
 * Gx - Connection to PCRF (Diameter)
 * Sgi - Connection to external network (Generally the Internet via standard TCP/IP)
 
### S5 Interface Configuration
Edit ```/etc/open5gs/smf.yaml``` and change the address to IP of the server running the SMF for the listener on GTP-C interfaces.

```
smf:
    freeDiameter: /etc/freeDiameter/smf.conf
    gtpc: 
      addr:
        - 10.0.1.121
```




### Gx Interface Configuration
Edit ```/etc/freeDiameter/smf.conf```

Update ```ListenOn``` address to IP of the server running the SMF:

``` ListenOn = "10.0.1.121"; ``` 

Update ConnectPeer to connect to the PCRF on it's IP.

```ConnectPeer = "pcrf.localdomain" { ConnectTo = "10.0.1.123"; No_TLS; };```


### Restart Services
Restart Open5GS SMF Daemon:

``` $ sudo systemctl restart open5gs-smfd.service ```


# External SGW-C
In it's simplest form the SGW-C has 2 interfaces:
 * S11 - Connection to MME (GTP-C)
 * S5 - Connection to the home network SMF (GTP-C)

### S5 Interface Configuration
Edit ```/etc/open5gs/sgwc.yaml``` and change the address to IP of the server running the SGW-C for the listener on GTP-C interface.

```
sgwc:
    gtpc: 
      addr:
        - 10.0.1.122
```
 
 Restart Open5GS SGW-C Daemon:

``` $ sudo systemctl restart open5gs-sgwcd.service ```


# External PCRF
In it's simplest from the PCRF has 1 network interface:
 * Gx - Connection to SMF (Diameter)

### Gx Interface Configuration
Edit ```/etc/freeDiameter/pcrf.conf```

Update ```ListenOn``` address to IP of the server running the HSS on it's IP:

``` ListenOn = "10.0.1.123"; ``` 

Update ConnectPeer to connect to the MME.

```ConnectPeer = "smf.localdomain" { ConnectTo = "10.0.1.121"; No_TLS; };```

### MongoDB Interface Configuration
Edit the ```db_uri:``` to point at the Open5GS: ```db_uri: mongodb://10.0.1.118/open5gs``` 

Restart Open5GS PCRF Daemon:

``` $ sudo systemctl restart open5gs-pcrfd.service ```

# External HSS
In it's simplest form the HSS has 1 network interface:
 * S6a - Connection to MME (Diameter)

### S6a Interface Configuration
Edit ```/etc/freeDiameter/hss.conf```

Update ```ListenOn``` address to IP of the server running the HSS on it's IP:

``` ListenOn = "10.0.1.118"; ``` 

Update ConnectPeer to connect to the MME.

```ConnectPeer = "mme.localdomain" { ConnectTo = "10.0.1.124"; No_TLS; };```

Restart Open5GS HSS Daemon:

``` $ sudo systemctl restart open5gs-hssd.service ```

### MongoDB Interface Configuration (Open5GS specific)
If you are using Open5GS's HSS you may need to enable MongoDB access from the PCRF. This is done by editing ''/etc/mongodb.conf'' and changing the bind IP to:
``` bind_ip = 0.0.0.0 ```

Restart MongoDB for changes to take effect.

``` $ /etc/init.d/mongodb restart ```

# External MME
In it's simplest form the MME has 3 interfaces:
 * S1AP - Connections from eNodeBs
 * S6a - Connection to HSS (Diameter)
 * S11 - Connection to SGW-C (GTP-C)
 
### S11 Interface Configuration
Edit ```/etc/open5gs/mme.yaml``` and filling the IP address of the SGW-C and SMF servers.
```
sgwc:
    gtpc:
      addr: 10.0.1.122

smf:
    gtpc:
      addr:
        - 10.0.1.121
```

### S6a Interface Configuration
Edit ```/etc/freeDiameter/mme.conf```

Update ```ListenOn``` address to IP of the server running the MME:

``` ListenOn = "10.0.1.124"; ``` 

Update ConnectPeer to connect to the PCRF on it's IP.

```ConnectPeer = "hss.localdomain" { ConnectTo = "10.0.1.118"; No_TLS; };```


### Restart Services
Restart Open5GS MME Daemon:

``` $ sudo systemctl restart open5gs-mmed.service ```
 
