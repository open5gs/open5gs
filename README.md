CellWire is an implementation of 3GPP EPC(MME, SGW, PGW, HSS)

The implementation consists in several components:
  - lib/core : the library contains the core of the framework.
      memory pool, thread, lock, timer, ...

  - lib/logger : the logging library

  - lib/s1ap : the S1AP message encoding/decoding library

  - lib/nas : the NAS message encoding/decoding library

  - lib/s6a : the freeDiameter wrapper for S6A

  - lib/gtp : the GTPv2-C/GTP-U protocol stack library

  - src/mme : the heart of MME protocol stack

  - src/hss : the heart of HSS protocol stack

  - src/sgw : the heart of SGW protocol stack

  - src/pgw : the heart of PGW protocol stack


See INSTALL.md for information on building and using this software.
