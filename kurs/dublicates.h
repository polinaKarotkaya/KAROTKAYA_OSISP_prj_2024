#ifndef DUPLICATE_FINDER_H
#define DUPLICATE_FINDER_H

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

int find_duplicate_files(FileInfo **file_infos, size_t file_count, int *processed_files);
void delete_duplicates(int duplicate_counter, FileInfo **file_infos, size_t file_count, int *processed_files);

#endif