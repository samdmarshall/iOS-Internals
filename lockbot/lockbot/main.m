//
//  main.c
//  lockbot
//
//  Created by Sam Marshall on 2/19/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <Foundation/Foundation.h>
#include <pthread.h>
#include <syslog.h>
#include <launch.h>
#include <signal.h>
#include <sys/socket.h>
#include "lockdown_keys.h"

Boolean IsCFNumber(CFTypeRef value) {
	return (CFGetTypeID(value) == CFNumberGetTypeID());
}

Boolean IsCFString(CFTypeRef value) {
	return (CFGetTypeID(value) == CFStringGetTypeID());
}

Boolean IsCFArray(CFTypeRef value) {
	return (CFGetTypeID(value) == CFArrayGetTypeID());
}

Boolean IsCFDictionary(CFTypeRef value) {
	return (CFGetTypeID(value) == CFDictionaryGetTypeID());
}

Boolean IsCFData(CFTypeRef value) {
	return (CFGetTypeID(value) == CFDataGetTypeID());
}

Boolean IsCFDate(CFTypeRef value) {
	return (CFGetTypeID(value) == CFDateGetTypeID());
}

Boolean IsCFBoolean(CFTypeRef value) {
	return (CFGetTypeID(value) == CFBooleanGetTypeID());
}

void CFSafeRelease(CFTypeRef value) {
	if (value != NULL) {
		CFRelease(value);
	}
}

void WriteToSyslog(char *function, CFStringRef format, ...) {
	char function_copy[0x800] = {0};
	memcpy(function_copy, function, sizeof(char[0x800]));
	va_list args;
	va_start (args, format);
	CFStringRef syslog_message = CFStringCreateWithFormatAndArguments(kCFAllocatorDefault, NULL, format, args);
	char message[0x800] = {0};
	if (CFStringGetCString(syslog_message, message, 0x800, kCFStringEncodingUTF8)) {
		pthread_t current_thread = pthread_self();
		syslog(LOG_SYSLOG, "%08x %s: %s", (unsigned int)current_thread, function_copy, message);
	}	
}

Boolean CanSetPreferences(CFStringRef domain, CFStringRef key, CFTypeRef value) {
	Boolean can_set_pref = false, found_domain = false, found_key = false;
	uint32_t domain_index, key_index;
	for (domain_index = 0; domain_index < SDM_MD_Domain_Count; domain_index++) {
		CFStringRef known_domain = CFStringCreateWithCString(kCFAllocatorDefault, SDMMDKnownDomain[domain_index].domain, kCFStringEncodingUTF8);
		if (CFStringCompare(known_domain, domain, 0) == 0) {
			found_domain = true;
			break;
		}
	}
	
	if (found_domain) {
		for (key_index = 0; key_index < SDMMDKnownDomain[domain_index].keyCount; key_index++) {
			CFStringRef known_key = CFStringCreateWithCString(kCFAllocatorDefault, SDMMDKnownDomain[domain_index].keys[key_index], kCFStringEncodingUTF8);
			if (CFStringCompare(known_key, key, 0) == 0) {
				found_key = true;
				break;
			}
		}
	}
	
	if (found_domain && found_key) {
		if (key != NULL && domain != NULL) {
			CFPreferencesSetValue(key, value, domain, kCFPreferencesAnyUser, kCFPreferencesAnyHost);
			can_set_pref = true;
		}
	}
	
	return can_set_pref;
}

int SetFDSocketNoSigPipe(int socket) {
	int buf_size = 0x4;
    if (setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, &buf_size, sizeof(buf_size))) {
		WriteToSyslog("SetFDSocketNoSigPipe", CFSTR("setsockopt(SO_NOSIGPIPE) failed: %s"), strerror(errno));
    }
    return socket;
	
}

int SetFDSocketNoWake(int socket) {
	int buf_size = 0x4;
    if (setsockopt(socket, SOL_SOCKET, 0x10000, &buf_size, sizeof(buf_size))) {
		WriteToSyslog("SetFDSocketNoWake", CFSTR("setsockopt(SO_NOWAKEFROMSLEEP) failed: %s"), strerror(errno));
    }
    return socket;

}

int InitializeFD(launch_data_t data) {
	static int fd = -1;
	static dispatch_once_t onceToken;
	dispatch_once(&onceToken, ^{
		launch_data_t listening_fd_array = launch_data_dict_lookup(data, "thing");
		
		launch_data_t listening_fd = launch_data_array_get_index(listening_fd_array, 0);
		fd = launch_data_get_fd(listening_fd);
	});
	return fd;
}

xpc_object_t ConvertCFTypeToXPC(CFTypeRef value) {
	xpc_object_t xpc_data = nil;
	CFDataRef data_object = NULL;
	if (IsCFDictionary(value)) {
		data_object = CFPropertyListCreateData(kCFAllocatorDefault, value, kCFPropertyListBinaryFormat_v1_0, 0, NULL);
	} else if (IsCFData(value)) {
		data_object = value;
	}
	
	if (data_object) {
		CFIndex data_length = CFDataGetLength(data_object);
		const UInt8 *data_ptr = CFDataGetBytePtr(data_object);
		
		dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0);
		
		dispatch_data_t dispatch_data = dispatch_data_create(data_ptr, data_length, queue, ^{});
		
		xpc_data = xpc_data_create_with_dispatch_data(dispatch_data);
	}
	
	return xpc_data;
}

CFTypeRef ConvertXPCToCFType(xpc_object_t object) {
	CFTypeRef value = NULL;
	xpc_type_t type = xpc_get_type(object);
	if (type == XPC_TYPE_DATA) {
		const void* data_ptr = xpc_data_get_bytes_ptr(object);
		size_t data_length = xpc_data_get_length(object);
		
		CFDataRef data = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, data_ptr, data_length, kCFAllocatorDefault);
		
		value = CFPropertyListCreateWithData(kCFAllocatorDefault, data, 0, NULL, NULL);
		
		CFSafeRelease(data);
	}
	return value;
}

int main(int argc, const char * argv[]) {
	
	openlog("lockbot", LOG_PID, LOG_DAEMON);
	signal(SIGHUP, NULL);
	
	return 0;
}

