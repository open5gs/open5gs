freeDiameter is an implementation of the Diameter protocol.

Diameter is a protocol designed to carry Authentication, Authorization and
Accounting (AAA) payload. It is an evolution of the RADIUS protocol (as the 
name suggests).

Diameter is an extensible protocol. RFC3588 (currently under revision) defines the
Base Protocol that all Diameter nodes must support, while other documents define
the additional protocol support for specific applications. Such applications include 
for example Network Access Servers (RFC4005), EAP (RFC4072), ...


The implementation consists in several components:
 - libfdproto : this shared library provides the functions to manipulate Diameter
      messages and dictionary. This library is meant to be re-used by other projects
      that would require parsing or manipulating Diameter messages.
      
 - libfdcore : this shared library contains the core of the framework. It establishes 
      the network connections with other Diameter peers and performs the tasks described 
      in Diameter Base Protocol, such as watchdogs, basic routing, ... It also handles 
      the loading of extensions (see below).
      
 - freeDiameterd : this simple daemon parses the command line and initializes the
      freeDiameter framework. Use it for your Diameter server & agent components.
      In case of Diameter clients, you probably will prefer linking the libfdcore 
      directly with your client application that must be made Diameter-aware.
      
 - extensions : the extensions provide the mean to augment the features of the
      freeDiameterd framework. Extensions can provide the handling of a Diameter 
      server application, but also advanced routing features, peer management, etc.


See http://www.freediameter.net/ for more information on the project.

freeDiameter was previously known as the "waaad" project (WIDE AAA Daemon)

This project is not related to the "freediameter" project from Sun on sourceforge.

Author: Sebastien Decugis.

See LICENSE file for legal information on this software.

See INSTALL for information on building and using this software.
