//
//  keybag.c
//  sup3rv1s0r
//
//  Created by Sam Marshall on 4/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef sup3rv1s0r_keybag_c
#define sup3rv1s0r_keybag_c

#include "keybag.h"

#define kVERS 0x56455253
#define kTYPE 0x54595045
#define kUUID 0x55554944
#define kHMCK 0x484d434b
#define kWRAP 0x57524150
#define kSALT 0x53414c54
#define kITER 0x49544552
#define kCLAS 0x434c4153
#define kKTYP 0x4b545950
#define kWPKY 0x57504b59

#define BYTES(a) (a&0xff000000)>>24,(a&0x00ff0000)>>16,(a&0x0000ff00)>>8,(a&0x000000ff)

struct keybag * ParseKeybag(BufferRef bag) {
	struct keybag * keybag = calloc(1, sizeof(struct keybag));
	if (bag) {
		keybag->block = calloc(1, sizeof(struct keybag_block));
		uint64_t offset = 0;
		while (offset < bag->length) {
			keybag->block = realloc(keybag->block, sizeof(struct keybag_block)*(keybag->block_count+1));
			memset(&(keybag->block[keybag->block_count]), 0, sizeof(struct keybag_block));
			memcpy(&(keybag->block[keybag->block_count].header), &(bag->data[offset]), sizeof(struct keybag_block_header));
			keybag->block[keybag->block_count].header.type = be32toh(keybag->block[keybag->block_count].header.type);
			keybag->block[keybag->block_count].header.length = be32toh(keybag->block[keybag->block_count].header.length);
			offset += sizeof(struct keybag_block_header);
			if (keybag->block[keybag->block_count].header.length > 0) {
				keybag->block[keybag->block_count].data = calloc(keybag->block[keybag->block_count].header.length, sizeof(char));
				memcpy(keybag->block[keybag->block_count].data, &(bag->data[offset]), keybag->block[keybag->block_count].header.length);
			}
			offset += keybag->block[keybag->block_count].header.length;
			keybag->block_count++;
		}
	}
	return keybag;
}

#endif