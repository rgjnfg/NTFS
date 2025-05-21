#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h" // Предполагается, что в этом заголовке описаны FileSystem и FileEntry

int main() {
    FileSystem fs = { NULL, 0 }; // Инициализация пустой файловой системы

    printf("Создание файлов...\n");
    fs_create(&fs, "/docs/readme.txt", "Это README файл.\n");
    fs_create(&fs, "/docs/info.txt", "Информация о проекте.\n");

    printf("Обновление файла /docs/info.txt...\n");
    fs_update(&fs, "/docs/info.txt", "Обновлённая информация.\n");

    printf("Удаление файла /docs/readme.txt...\n");
    fs_delete(&fs, "/docs/readme.txt");

    // Сохраняем текущее состояние в файл
    FILE *fs_file;
    if (open_file("filesystem.txt", &fs_file) == 0) {
        for (size_t i = 0; i < fs.count; ++i) {
            fprintf(fs_file, "%s", fs.entries[i].path);
            if (fs.entries[i].path[strlen(fs.entries[i].path) - 1] != '\n') {
                fprintf(fs_file, "\n");
            }
            fprintf(fs_file, "%s", fs.entries[i].content);
            if (fs.entries[i].content[strlen(fs.entries[i].content) - 1] != '\n') {
                fprintf(fs_file, "\n");
            }
        }
        fclose(fs_file);
    } else {
        fprintf(stderr, "Не удалось сохранить файловую систему.\n");
    }

    // Просмотр содержимого файла
    char *content = NULL;
    if (view_file("/docs/info.txt\n", &content) == 0) {
        printf("Содержимое /docs/info.txt:\n%s\n", content);
        free(content);
    } else {
        printf("Файл /docs/info.txt не найден.\n");
    }

    // Освобождение памяти
    for (size_t i = 0; i < fs.count; ++i) {
        free(fs.entries[i].path);
        free(fs.entries[i].content);
    }
    free(fs.entries);

    return 0;
}
