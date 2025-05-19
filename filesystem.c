#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <filesystem.h>

// Поиск индекса файла по пути
static int locate_file(const FileSystem *fs, const char *target_path) {
    for (size_t i = 0; i < fs->count; ++i) {
        if (strcmp(fs->entries[i].path, target_path) == 0) {
            return (int)i;
        }
    }
    return -1; // Не найден
}
// Открытие файла на чтение/запись (создаётся при необходимости)
int open_file(const char *filename, FILE **fptr) {
    *fptr = fopen(filename, "a+");
    return (*fptr != NULL) ? 0 : -1;
}
// Просмотр содержимого файла по имени
int view_file(const char *target, char **buffer) {
    FILE *fs_file;
    if (open_file("filesystem.txt", &fs_file) != 0) {
        return -1; // Ошибка при открытии
    }

    char *line = NULL;
    size_t size = 0;
    ssize_t len;
    int is_target = 0;

    *buffer = malloc(1024);
    if (!*buffer) {
        fclose(fs_file);
        return -1;
    }
    (*buffer)[0] = '\0';

    while ((len = getline(&line, &size, fs_file)) != -1) {
        if (strcmp(line, target) == 0) {
            is_target = 1;
            continue;
        }
        if (is_target) {
            if (line[0] == '/') break;
            strcat(*buffer, line);
        }
    }

    free(line);
    fclose(fs_file);
    return is_target ? 0 : -1;
}

// Создание нового файла в файловой системе
int fs_create(FileSystem *fs, const char *path, const char *data) {
    FileEntry *new_entries = realloc(fs->entries, (fs->count + 1) * sizeof(FileEntry));
    if (!new_entries) return -1;

    fs->entries = new_entries;
    fs->entries[fs->count].path = strdup(path);
    fs->entries[fs->count].content = strdup(data);
    fs->count++;
    return 0;
}

// Удаление файла по пути
int fs_delete(FileSystem *fs, const char *path) {
    int index = locate_file(fs, path);
    if (index == -1) return -1;

    free(fs->entries[index].path);
    free(fs->entries[index].content);

    for (size_t i = index; i < fs->count - 1; ++i) {
        fs->entries[i] = fs->entries[i + 1];
    }
    fs->count--;
    return 0;
}

// Обновление содержимого файла
int fs_update(FileSystem *fs, const char *path, const char *new_data) {
    int index = locate_file(fs, path);
    if (index == -1) return -1;

    free(fs->entries[index].content);
    fs->entries[index].content = strdup(new_data);
    return 0;
}
