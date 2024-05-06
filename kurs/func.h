#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <magic.h>
#include <openssl/md5.h>
#include <dirent.h>
#include <openssl/evp.h>
#include <pthread.h>
#include "fileinfo.h"


void get_file_mime_type(const char *filepath, char *mime_type, size_t mime_type_len);
void get_file_md5(const char *filepath, unsigned char *md5_digest);
void *process_file(void *arg);

#endif
