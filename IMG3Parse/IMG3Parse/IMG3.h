//
//  IMG3.h
//  IMG3Parse
//
//  Created by Sam Marshall on 1/31/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef IMG3Parse_IMG3_h
#define IMG3Parse_IMG3_h

#include "Core.h"

/*
 
 VERS: iBoot version of the image
 SEPO: Security Epoch
 SDOM: Security Domain
 PROD: Production Mode
 CHIP: Chip to be used with. example: "0x8900" for S5L8900.
 BORD: Board to be used with
 KBAG: contains the KEY and IV required to decrypt encrypted with the GID Key
 SHSH: RSA encrypted SHA1 hash of the file
 CERT: Certificate
 ECID: Exclusive Chip ID unique to every device with iPhone OS.
 TYPE: Type of image, should contain the same string as 'iden' of the header
 DATA: Real content of the file

 */

enum IMG3_Tag_Magic {
	IMG3_Tag_Magic_Invalid = 0x0,
	IMG3_Tag_Magic_VERS,
	IMG3_Tag_Magic_SEPO,
	IMG3_Tag_Magic_SDOM,
	IMG3_Tag_Magic_PROD,
	IMG3_Tag_Magic_CHIP,
	IMG3_Tag_Magic_BORD,
	IMG3_Tag_Magic_KBAG,
	IMG3_Tag_Magic_SHSH,
	IMG3_Tag_Magic_CERT,
	IMG3_Tag_Magic_ECID,
	IMG3_Tag_Magic_TYPE,
	IMG3_Tag_Magic_DATA
};

struct IMG3_Header {
    uint32_t magic;
    uint32_t fullSize;
    uint32_t sizeNoPack;
    uint32_t sigCheckArea;
    uint32_t identifier;
};

struct IMG3_Tag {
	enum IMG3_Tag_Magic type;
	uint32_t totalLength;
	uint32_t dataLength;
	char *dataBuffer;
	char *padBuffer;
};

struct IMG3_File {
	struct IMG3_Header header;
	struct IMG3_Tag *tags;
	uint32_t tagCount;
};

struct IMG3_File * ParseIMG3FileAtPath(char *path);

#endif
