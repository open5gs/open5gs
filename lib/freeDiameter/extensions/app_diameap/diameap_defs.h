/*****************************************************************************************************
 * Software License Agreement (BSD License)
 * Author : Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>
 *
 * Copyright (c) 2009-2010, Souheil Ben Ayed, Teraoka Laboratory of Keio University, and the WIDE Project
 * All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by Souheil Ben Ayed <souheil@tera.ics.keio.ac.jp>.
 *
 * 4. Neither the name of Souheil Ben Ayed, Teraoka Laboratory of Keio University or the WIDE Project nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************************/


#ifndef DIAMEAP_DEFS_H_
#define DIAMEAP_DEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/************************************************/
/*		Data Type Definition 					*/
/************************************************/

/* boolean Type */
typedef enum
{
	FALSE = 0, TRUE = 1
} boolean;

/* Exact-width integer types */
typedef int8_t s8;		/* signed char */
typedef uint8_t u8;		/* unsigned char */
typedef int16_t s16;	/* signed int */
typedef uint16_t u16;	/* unsigned int */
typedef int32_t s32;	/* signed long int */
typedef uint32_t u32;	/* unsigned long int */
typedef int64_t s64;	/* signed long long int */
typedef uint64_t u64;	/* unsigned long long int */

/************************************************/
/*		Macros									*/
/************************************************/

/* Macros for manipulating data*/

/* Retrieve signed/unsigned exact-width integer types */
#define G8(v) (u8) (* (v) )

#define G16BIGE(v) ( 	( (u16) (* (v)   ) <<  8 ) ^ \
		      	( (u16) (*((v)+1))       ) )
#define G16LITE(v) (	( (u16) (*((v)+1)) <<  8 ) ^ \
		      	( (u16) (* (v)   )       ) )

#define G24BIGE(v) (	( (u32) (* (v)   ) << 16 ) ^ \
			( (u32) (*((v)+1)) <<  8 ) ^ \
			( (u32) (*((v)+2))       ) )
#define G24LITE(v) ( 	( (u32) (*((v)+2)) << 16 ) ^ \
			( (u32) (*((v)+1)) <<  8 ) ^ \
			( (u32) (* (v)   )       ) )

#define G32BIGE(v) ( 	( (u32) (* (v)   ) << 24 ) ^ \
			( (u32) (*((v)+1)) << 16 ) ^ \
			( (u32) (*((v)+2)) <<  8 ) ^ \
			( (u32) (*((v)+3))       ) )
#define G32LITE(v) (	( (u32) (*((v)+3)) << 24 ) ^ \
			( (u32) (*((v)+2)) << 16 ) ^ \
			( (u32) (*((v)+1)) <<  8 ) ^ \
			( (u32) (* (v)   )       ) )

#define G64BIGE(v) ( 	( (u64) (* (v)   ) << 56 ) ^ \
			( (u64) (*((v)+1)) << 48 ) ^ \
			( (u64) (*((v)+2)) << 40 ) ^ \
			( (u64) (*((v)+3)) << 32 ) ^ \
			( (u64) (*((v)+4)) << 24 ) ^ \
			( (u64) (*((v)+5)) << 16 ) ^ \
			( (u64) (*((v)+6)) <<  8 ) ^ \
			( (u64) (*((v)+7))       ) )
#define G64LITE(v) (	( (u64) (*((v)+7)) << 56 ) ^ \
			( (u64) (*((v)+6)) << 48 ) ^ \
			( (u64) (*((v)+5)) << 40 ) ^ \
			( (u64) (*((v)+4)) << 32 ) ^ \
			( (u64) (*((v)+3)) << 24 ) ^ \
			( (u64) (*((v)+2)) << 16 ) ^ \
			( (u64) (*((v)+1)) <<  8 ) ^ \
			( (u64) (* (v)   )       ) )

/* Insert signed/unsigned exact-width integer types */
/* v : pointer where to insert the data
 * b : pointer of data to be inserted
 */
/* Insertion format
 * BIGE : BIG ENDIAN
 * LITE : LITTLE ENDIAN
 */

#define P8(v,b) do { \
		u8 x = (b); \
		u8 *d = (v); \
		d[0] = (x&0xffU); \
	} while (0)

#define P16BIGE(v,b) do { \
		u16 x = (b); \
		u8 *d = (v); \
		d[0] = ((x >> 8)&0xffU); \
		d[1] = (x&0xffU); \
	} while (0)

#define P16LITE(v,b) do { \
		u16 x = (b); \
		u8 *d = (v); \
		d[0] = (x&0xffU); \
		d[1] = ((x >> 8)&0xffU); \
	} while (0)

#define P24BIGE(v, b)  do { \
		u32 x = (b); \
		u8 *d = (v); \
		d[0] = ((x >> 16)&0xffU); \
		d[1] = ((x >> 8)&0xffU); \
		d[2] = ((x)&0xffU); \
	} while (0)

#define P24LITE(v, b)  do { \
		u32 x = (b); \
		u8 *d = (v); \
		d[0] = ((x)&0xffU); \
		d[1] = ((x >> 8)&0xffU); \
		d[2] = ((x >> 16)&0xffU); \
	} while (0)

#define P32BIGE(v, b)  do { \
		u32 x = (b); \
		u8 *d = (v); \
		d[0] = ((x >> 24)&0xffU); \
		d[1] = ((x >> 16)&0xffU); \
		d[2] = ((x >> 8)&0xffU); \
		d[3] = ((x)&0xffU); \
	} while (0)

#define P32LITE(v, b)  do { \
		u32 x = (b); \
		u8 *d = (v); \
		d[0] = ((x)&0xffU); \
		d[1] = ((x >> 8)&0xffU); \
		d[2] = ((x >> 16)&0xffU); \
		d[3] = ((x >> 24)&0xffU); \
	} while (0)

#define P64BIGE(v, b) do { \
		u64 x = (b); \
		u8 *d = (v); \
		d[0] = ((x >> 56)&0xffU); \
		d[1] = ((x >> 48)&0xffU); \
		d[2] = ((x >> 40)&0xffU); \
		d[3] = ((x >> 32)&0xffU); \
		d[4] = ((x >> 24)&0xffU); \
		d[5] = ((x >> 16)&0xffU); \
		d[6] = ((x >> 8)&0xffU); \
		d[7] = ((x)&0xffU); \
	} while (0)

#define P64LITE(v, b) do { \
		u64 x = (b); \
		u8 *d = (v); \
		d[0] = ((x)&0xffU); \
		d[1] = ((x >> 8)&0xffU); \
		d[2] = ((x >> 16)&0xffU); \
		d[3] = ((x >> 24)&0xffU); \
		d[4] = ((x >> 32)&0xffU); \
		d[5] = ((x >> 40)&0xffU); \
		d[6] = ((x >> 48)&0xffU); \
		d[7] = ((x >> 56)&0xffU); \
	} while (0)

/*
 * Insert data in a specified position
 * a : (u8*) pointer where to insert the data
 * f : (int) insert from this position
 * l : (int) length of the data in byte to insert
 * b : (u8*) pointer of data to be inserted
 */

#define U8COPY(a,f,l,b) do{ \
			u8 * x = (a); \
			u8 * y = (b); \
			int i; \
			for(i=0;i<l;i++) \
			x[i+f]= y[i]; \
		} while (0)


#endif /*DIAMEAP_DEFS_H_*/
