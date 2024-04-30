#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "func.h"
#include "dublicates.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s directory\n", argv[0]);
        return 1;
    }

    char *dirpath = argv[1];
    DIR *dir = opendir(dirpath);

    if (!dir) {
        perror("opendir");
        return 1;
    }

    size_t file_count = 0;
    FileInfo **file_infos = NULL; // Добавлен массив для хранения информации о файлах

    pthread_t *thread_ids = malloc(file_count * sizeof(pthread_t)); // Создаем массив идентификаторов потоков
    size_t thread_index = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filepath[4096];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
            pthread_create(&thread_ids[thread_index], NULL, process_file, (void *) filepath);
            thread_index++;
            file_count++;
        }
    }

    for (size_t i = 0; i < thread_index; i++) { // Дожидаемся завершения всех потоков
        FileInfo *file_info;
        pthread_join(thread_ids[i], (void **)&file_info);
        file_infos = realloc(file_infos, (file_count + 1) * sizeof(FileInfo *));
        file_infos[i] = file_info; // Сохраняем информацию о файле в массиве
    }

    free(thread_ids); // Освобождаем память, выделенную под массив идентификаторов потоков

    closedir(dir);

    int *processed_files = calloc(file_count, sizeof(int));

    int duplicate_counter = find_duplicate_files(file_infos, file_count, processed_files);

    delete_duplicates(duplicate_counter, file_infos, file_count, processed_files);

    // Освобождение памяти
    free(processed_files);

    for (size_t i = 0; i < file_count; i++) {
        free(file_infos[i]);
    }
    free(file_infos);

    return 0;
}