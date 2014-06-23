//
//  main.c
//  mb2parse
//
//  Created by Sam Marshall on 6/12/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <stdio.h>
#include "mbdb.h"
#include "keybag.h"

#define hashprint(name) \
for (int i = 0; i < 20; i++) { \
	printf("%x",name[i]); \
} \
printf("\n");

int main(int argc, const char * argv[]) {
	if (argc == 2) {
		struct mbdb_file *test = ParseMBDBFile((char*)argv[1]);
		
		for (int index = 0; index < test->record_count; index++) {
			struct mbdb_record *record = &(test->record[index]);
			if (record->domain.name.length) {
				printf("%s\n",record->domain.name.string);
			}
			if (record->path.name.length) {
				printf("\tFile Name: %s\n",record->path.name.string);
			}
			if (record->path.name.string != NULL) {
				struct mbdb_file_name *file_name = FindFileForFromManifestRecord(record);
				if (file_name->has_file) {
					printf("\tHash Name: ");
					hashprint(file_name->name);
				}
			}
		}
	}
	else {
		printf("please provide the path to a Manifest.mbdb file. They can be found at: /Users/*/Library/Application Support/MobileSync/Backup/<UDID>/Manifest.mbdb\n");
		printf("usage: ./mb2parse <path to Manifest.mbdb>\n");
	}
	
    return 0;
}

