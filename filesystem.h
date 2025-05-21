#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

// Структура записи файла
typedef struct {
    char *path;
    char *content;
} FileEntry;

// Структура файловой системы
typedef struct {
    FileEntry *entries;
    size_t count;
} FileSystem;

// Открытие файла для чтения и записи
int open_file(const char *filename, FILE **fptr);

// Просмотр содержимого файла по имени
int view_file(const char *target, char **buffer);

// Создание нового файла в файловой системе
int fs_create(FileSystem *fs, const char *path, const char *data);

// Удаление файла по пути
int fs_delete(FileSystem *fs, const char *path);

// Обновление содержимого файла
int fs_update(FileSystem *fs, const char *path, const char *new_data);

#endif // FILESYSTEM_H
