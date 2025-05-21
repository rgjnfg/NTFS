#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

int main() {
    // Инициализация пустой файловой системы
    FileSystem fs;
    fs.entries = NULL;
    fs.count = 0;

    // Создание нового логического файла
    if (fs_create(&fs, "/example.txt", "Привет из файловой системы!") == 0) {
        printf("Файл успешно создан: /example.txt\n");
        printf("Содержимое: %s\n", fs.entries[0].content);
    } else {
        printf("Ошибка при создании файла.\n");
    }

    // Освобождение памяти
    for (size_t i = 0; i < fs.count; ++i) {
        free(fs.entries[i].path);
        free(fs.entries[i].content);
    }
    free(fs.entries);

    return 0;
}
