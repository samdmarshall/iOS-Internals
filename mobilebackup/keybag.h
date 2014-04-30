//
//  keybag.h
//  sup3rv1s0r
//
//  Created by Sam Marshall on 4/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef sup3rv1s0r_keybag_h
#define sup3rv1s0r_keybag_h

#include <CoreFoundation/CoreFoundation.h>
#include "Core.h"

struct keybag_block_header {
	uint32_t type;
	uint32_t length;
} ATR_PACK;

struct keybag_block {
	struct keybag_block_header header;
	uint8_t * data;
} ATR_PACK;

struct keybag {
	struct keybag_block * block;
	uint32_t block_count;
} ATR_PACK;

struct keybag * ParseKeybag(BufferRef bag);

#endif
