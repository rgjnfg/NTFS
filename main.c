#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

#define MAX_INPUT_LENGTH 1024
#define MAX_PARAMS 3

// Выводит справку по доступным командам
void print_help() {
    printf("\nДоступные команды:\n");
    printf("создать <путь> <содержимое> - Создать новый файл\n");
    printf("удалить <путь> - Удалить файл\n");
    printf("изменить <путь> <новое_содержимое> - Изменить содержимое файла\n");
    printf("просмотреть <путь> - Просмотреть содержимое файла\n");
    printf("демо - Показать демонстрацию работы всех функций\n");
    printf("выход - Завершить программу\n");
    printf("помощь - Показать эту справку\n");
}

// Демонстрация всех операций с файловой системой
void run_demo() {
    printf("\n=== Демонстрация работы файловой системы ===\n");

    FileSystem fs = {NULL, 0};  // Инициализация пустой файловой системы

    printf("\n1. Создаём файл /demo.txt с содержимым 'Привет мир!'\n");
    if (fs_create(&fs, "/demo.txt", "Привет мир!") == 0) {
        printf("Файл успешно создан\n");
    } else {
        printf("Ошибка при создании файла\n");
    }

    // Просмотр содержимого файла
    printf("\n2. Просматриваем содержимое /demo.txt\n");
    char *content = NULL;
    if (view_file("/demo.txt", &content) == 0) {
        printf("Содержимое: %s\n", content);
        free(content);
    } else {
        printf("Ошибка при чтении файла\n");
    }

    // Изменение содержимого
    printf("\n3. Изменяем содержимое на 'Новое содержимое!'\n");
    if (fs_update(&fs, "/demo.txt", "Новое содержимое!") == 0) {
        printf("Содержимое успешно обновлено\n");
    } else {
        printf("Ошибка при обновлении файла\n");
    }

    // Проверка изменения
    printf("\n4. Проверка обновлённого содержимого\n");
    if (view_file("/demo.txt", &content) == 0) {
        printf("Новое содержимое: %s\n", content);
        free(content);
    } else {
        printf("Ошибка при чтении файла\n");
    }

    // Удаление файла
    printf("\n5. Удаляем файл /demo.txt\n");
    if (fs_delete(&fs, "/demo.txt") == 0) {
        printf("Файл успешно удалён\n");
    } else {
        printf("Ошибка при удалении файла\n");
    }

    // Попытка прочитать удалённый файл
    printf("\n6. Пытаемся прочитать удалённый файл\n");
    if (view_file("/demo.txt", &content) != 0) {
        printf("Файл не найден (как и ожидалось)\n");
    } else {
        printf("Файл найден: %s\n", content);
        free(content);
    }

    // Очистка ресурсов
    for (size_t i = 0; i < fs.count; ++i) {
        free(fs.entries[i].path);
        free(fs.entries[i].content);
    }
    free(fs.entries);

    printf("\n=== Демонстрация завершена ===\n");
}

// Обработка команды, введённой пользователем
void process_command(FileSystem *fs, char *command, char **params, int param_count) {
    if (strcmp(command, "создать") == 0 && param_count == 2) {
        if (fs_create(fs, params[0], params[1]) == 0) {
            printf("Файл '%s' успешно создан\n", params[0]);
        } else {
            printf("Ошибка при создании файла '%s'\n", params[0]);
        }

    } else if (strcmp(command, "удалить") == 0 && param_count == 1) {
        if (fs_delete(fs, params[0]) == 0) {
            printf("Файл '%s' успешно удалён\n", params[0]);
        } else {
            printf("Файл '%s' не найден\n", params[0]);
        }

    } else if (strcmp(command, "изменить") == 0 && param_count == 2) {
        if (fs_update(fs, params[0], params[1]) == 0) {
            printf("Файл '%s' успешно изменён\n", params[0]);
        } else {
            printf("Файл '%s' не найден\n", params[0]);
        }

    } else if (strcmp(command, "просмотреть") == 0 && param_count == 1) {
        char *content = NULL;
        if (view_file(params[0], &content) == 0) {
            printf("Содержимое файла '%s':\n%s\n", params[0], content);
            free(content);
        } else {
            printf("Файл '%s' не найден или ошибка чтения\n", params[0]);
        }

    } else if (strcmp(command, "демо") == 0) {
        run_demo();

    } else if (strcmp(command, "помощь") == 0) {
print_help();

    } else {
        printf("Неизвестная команда или неверные параметры. Введите 'помощь' для справки.\n");
    }
}

int main() {
    FileSystem fs = {NULL, 0}; 
    char input[MAX_INPUT_LENGTH]; // Строка ввода пользователя
    char *params[MAX_PARAMS];     // Параметры команды
    char *token;
    int param_count;

    printf("Файловая система готова к работе!\n");
    print_help();

    while (1) {
        printf("\n> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Ошибка ввода. Выход.\n");
            break;
        }

        // Удаление символа новой строки
        input[strcspn(input, "\n")] = '\0';

        // Проверка команды "выход"
        if (strcmp(input, "выход") == 0) {
            break;
        }

        // Разделение строки на команду и параметры
        token = strtok(input, " ");
        if (!token) continue;

        char *command = token;
        param_count = 0;

        while ((token = strtok(NULL, " ")) != NULL && param_count < MAX_PARAMS) {
            params[param_count++] = token;
        }

        // Обработка команды
        process_command(&fs, command, params, param_count);
    }

    // Очистка ресурсов при завершении программы
    for (size_t i = 0; i < fs.count; ++i) {
        free(fs.entries[i].path);
        free(fs.entries[i].content);
    }
    free(fs.entries);

    printf("\nЗавершение работы файловой системы.\n");
    return 0;
}
