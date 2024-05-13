#include "fileinfo.h"
#include "func.h"
#include "dublicates.h"

#define MAX_PATH_LENGTH 4096

int main() {
    
    char dirpath[MAX_PATH_LENGTH];

    printf("Enter directory path: ");
    fgets(dirpath, MAX_PATH_LENGTH, stdin);

    // Удаляем символ новой строки в конце
    dirpath[strcspn(dirpath, "\n")] = '\0';

    DIR *dir = opendir(dirpath);

    if (!dir) {
        perror("opendir");
        return 1;
    }

    pthread_t thread_id;
    FileInfo **file_infos = NULL; // Добавлен массив для хранения информации о файлах
    size_t file_count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filepath[4096];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
            pthread_create(&thread_id, NULL, process_file, (void *) filepath);
            FileInfo *file_info;
            pthread_join(thread_id, (void **)&file_info);
            file_infos = realloc(file_infos, (file_count + 1) * sizeof(FileInfo *));
            file_infos[file_count++] = file_info;
        }
    }

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