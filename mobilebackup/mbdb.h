//
//  mbdb.h
//  sup3rv1s0r
//
//  Created by Sam Marshall on 4/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef sup3rv1s0r_mbdb_h
#define sup3rv1s0r_mbdb_h

#include <CoreFoundation/CoreFoundation.h>
#include "Core.h"
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define kMBDB_MAGIC "mbdb"

#define HASH_LENGTH SHA_DIGEST_LENGTH

struct mbdb_file_name {
	bool has_file;
	unsigned char name[HASH_LENGTH];
	unsigned char hash[HASH_LENGTH];
} ATR_PACK;

struct mbdb_manifest {
	char magic[4];
	uint16_t version;
} ATR_PACK;

struct mbdb_string {
	uint16_t length;
	char * string;
} ATR_PACK;

struct mbdb_record_entry {
	struct mbdb_string name;
} ATR_PACK;

struct mbdb_record_info {
	uint16_t mode; // mode_t
	uint32_t unknown0;
	uint32_t unknown1;
	uint32_t user_id; // uid_t
	uint32_t group_id; // gid_t
	uint32_t atime; // time_t
	uint32_t mtime; // time_t
	uint32_t ctime; // time_t
	uint64_t file_length;
	uint8_t flag;
} ATR_PACK;

struct mbdb_record_property {
	struct mbdb_string name;
	struct mbdb_string value;
} ATR_PACK;

struct mbdb_record {
	struct mbdb_record_entry domain;
	struct mbdb_record_entry path;
	struct mbdb_record_entry target;
	struct mbdb_record_entry data_hash;
	struct mbdb_record_entry other;
	struct mbdb_record_info info;
	uint8_t property_count;
	struct mbdb_record_property *property;
} ATR_PACK;

struct mbdb_file {
	struct mbdb_manifest *manifest;
	struct mbdb_record *record;
	uint32_t record_count;
} ATR_PACK;

struct mbdb_file_name * FindFileForFromManifestRecord(struct mbdb_record * record);
struct mbdb_file * ParseMBDBData(CFDataRef file);
struct mbdb_file * ParseMBDBFile(char * path);
void MBDBFileRelease(struct mbdb_file * file);

#endif
