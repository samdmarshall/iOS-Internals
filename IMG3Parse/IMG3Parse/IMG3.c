//
//  IMG3.c
//  IMG3Parse
//
//  Created by Sam Marshall on 1/31/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef IMG3Parse_IMG3_c
#define IMG3Parse_IMG3_c

#include "IMG3.h"

struct IMG3_File * ParseIMG3FileAtPath(char *path) {
	struct IMG3_File *parsedFile = (struct IMG3_File *)calloc(0x1, S(struct IMG3_File));
	if (path) {
		parsedFile->fileContents = CreateBufferFromFilePath(path);
		parsedFile->header = PtrCast(parsedFile->fileContents->data,struct IMG3_Header*);
		
	}
	return parsedFile;
}

#endif