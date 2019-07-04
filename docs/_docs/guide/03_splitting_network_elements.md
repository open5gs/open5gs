# NextEPC Splitting Network Elements
In a production network network elements would typically not all be on the same machine, as is the default example that ships with NextEPC.

NextEPC is designed to be standards compliant, so in theory you can connect any core network element (MME, PGW, SGW, PCRF, HSS) from NextEPC or any other vendor to form a functioning network, so long as they are 3GPP compliant. 

To demonstrate this we will cover isolating each network element onto it's on machine and connect each network element to the other. For some interfaces specifying multiple interfaces is supported to allow connection to multiple

In these examples we'll be connecting NextEPC elements together, but it could just as easily be EPC elements from a different vendor in the place of any NextEPC network element.

| Service        | IP           | Identity |
| ------------- |:-------------:|:-------------:|
| P-GW | 10.0.1.121 | pgw.localdomain |
| S-GW | 10.0.1.122 | |
| PCRF | 10.0.1.123 | pcrf.localdomain |
| MME | 10.0.1.124 | mme.localdomain |
| HSS | 10.0.1.118 | hss.localdomain |

# External MME
In it's simplest form the MME has 3 interfaces:
 * S1AP - Connections from eNodeBs
 * S6a - Connection to HSS (Diameter)
 * S11 - Connection to S-GW (GTP-C)



# External P-GW
In it's simplest from the P-GW has 3 interfaces:
 * S5 - Connection to home network S-GW (GTP-C)
 * Gx - Connection to PCRF (Diameter)
 * Sgi - Connection to external network (Generally the Internet via standard TCP/IP)
 
### S5 Interface Configuration
Edit ```/etc/nextepc/pgw.conf```and change the address to IP of the server running the P-GW for the listener on GTP-C and GTP-U interfaces.

```

pgw:

    freeDiameter: pgw.conf
      
    gtpc: 
    
      addr:
      
        - 10.0.1.121
        
     gtpu: 
    
      addr:
      
        - 10.0.1.121
        
 ```




### Gx Interface Configuration
Edit ```/etc/nextepc/freeDiameter/pgwd.conf```

Update ```ListenOn``` address to IP of the server running the P-GW:

``` ListenOn = "10.0.1.121"; ``` 

Update ConnectPeer to connect to the PCRF on it's IP.

```ConnectPeer = "pcrf.localdomain" { ConnectTo = "10.0.1.123"; No_TLS; };```


### Restart Services
Restart NextEPC PGW Daemon:

``` $ sudo systemctl restart nextepc-pgwd ```


# External S-GW
In it's simplest form the S-GW has 2 interfaces:
 * S11 - Connection to MME (GTP-C)
 * S5 - Connection to the home network P-GW (GTP-C)

### S5 Interface Configuration
Edit ```/etc/nextepc/sgw.conf```and change the address to IP of the server running the S-GW for the listener on GTP-C interface.

```
sgw:

    freeDiameter: pgw.conf
      
    gtpc: 
    
      addr:
      
        - 10.0.1.122
        
 ```
 
 Restart NextEPC SGW Daemon:

``` $ sudo systemctl restart nextepc-sgwd ```


# External PCRF
In it's simplest from the PCRF has 1 network interface:
 * Gx - Connection to P-GW (Diameter)

### Gx Interface Configuration
Edit ```/etc/nextepc/freeDiameter/hss.conf```

Update ```ListenOn``` address to IP of the server running the HSS on it's IP:

``` ListenOn = "10.0.1.123"; ``` 

Update ConnectPeer to connect to the MME.

```ConnectPeer = "pgw.localdomain" { ConnectTo = "10.0.1.121"; No_TLS; };```

### MongoDB Interface Configuration (NextEPC HSS only)
Edit ```/etc/nextepc/freeDiameter/hss.conf``` and change the ```db_uri:``` to point at the HSS: ```db_uri: mongodb://10.0.1.118/nextepc``` 

Restart NextEPC PCRF Daemon:

``` $ sudo systemctl restart nextepc-pcrfd ```

# External HSS
In it's simplest form the HSS has 1 network interface:
 * S6a - Connection to MME (Diameter)

### S6a Interface Configuration
Edit ```/etc/nextepc/freeDiameter/hss.conf```

Update ```ListenOn``` address to IP of the server running the HSS on it's IP:

``` ListenOn = "10.0.1.118"; ``` 

Update ConnectPeer to connect to the MME.

```ConnectPeer = "mme.localdomain" { ConnectTo = "10.0.1.124"; No_TLS; };```

Restart NextEPC HSS Daemon:

``` $ sudo systemctl restart nextepc-hssd ```

### MongoDB Interface Configuration (NextEPC specific)
If you are using NextEPC's HSS you may need to enable MongoDB access from the PCRF. This is done by editing ''/etc/mongodb.conf'' and changing the bind IP to:
``` bind_ip = 0.0.0.0 ```

Restart MongoDB for changes to take effect.

``` $ /etc/init.d/mongodb restart ```


# External MME
### S11 Interface Configuration
Edit ```/etc/nextepc/mme.conf```, filling the IP address of the S-GW and P-GW servers.
```
sgw:

    gtpc:
    
      addr: 10.0.1.122


pgw:

    gtpc:
    
      addr:
      
        - 10.0.1.121
```

### S6a Interface Configuration
Edit ```/etc/nextepc/freeDiameter/mme.conf```

Update ```ListenOn``` address to IP of the server running the MME:

``` ListenOn = "10.0.1.124"; ``` 

Update ConnectPeer to connect to the PCRF on it's IP.

```ConnectPeer = "hss.localdomain" { ConnectTo = "10.0.1.118"; No_TLS; };```


### Restart Services
Restart NextEPC MME Daemon:

``` $ sudo systemctl restart nextepc-mmed ```
 
