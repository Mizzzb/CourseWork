#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include "client.h"

// Структура для хранения настроек из командной строки
struct Config {
    std::string host;
    int port = 33333;
    std::string login = "guest";
    std::string password = "12345";
    std::string input_file = "input.bin";
    std::string output_file = "output.bin";
    int timeout = 10;
};

void print_help() {
    std::cout << "Использование: ./client <host> [опции]\n"
              << "Обязательный параметр:\n"
              << "  <host>                   Сетевой адрес сервера\n\n"
              << "Опции:\n"
              << "  -h, --help               Справка\n"
              << "  -p, --port <N>           Порт сервера (1-65535). По умолчанию: 33333\n"
              << "  -l, --login <STR>        Логин для аутентификации. По умолчанию: guest\n"
              << "  -s, --password <STR>     Пароль для аутентификации. По умолчанию: 12345\n"
              << "  -i, --input <FILE>       Входной бинарный файл. По умолчанию: input.bin\n"
              << "  -o, --output <FILE>      Выходной бинарный файл. По умолчанию: output.bin\n"
              << "  -t, --timeout <N>        Таймаут ожидания (сек). По умолчанию: 10\n";
}

void error_exit(const std::string& msg) {
    std::cerr << "Ошибка: " << msg << "\n";
    std::cerr << "Используйте -h для получения справки.\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    Config cfg;

    // 1. Приоритет: -h, --help
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        }
    }

    // 2. Парсинг аргументов
    bool host_found = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-p" || arg == "--port") {
            if (i + 1 >= argc) error_exit("не указано значение для -p");
            try {
                cfg.port = std::stoi(argv[++i]);
                if (cfg.port < 1 || cfg.port > 65535) error_exit("порт должен быть от 1 до 65535");
            } catch (...) { error_exit("неверный формат порта"); }
        }
        else if (arg == "-l" || arg == "--login") {
            if (i + 1 >= argc) error_exit("не указано значение для -l");
            cfg.login = argv[++i];
        }
        else if (arg == "-s" || arg == "--password") {
            if (i + 1 >= argc) error_exit("не указано значение для -s");
            cfg.password = argv[++i];
        }
        else if (arg == "-i" || arg == "--input") {
            if (i + 1 >= argc) error_exit("не указано значение для -i");
            cfg.input_file = argv[++i];
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 >= argc) error_exit("не указано значение для -o");
            cfg.output_file = argv[++i];
        }
        else if (arg == "-t" || arg == "--timeout") {
            if (i + 1 >= argc) error_exit("не указано значение для -t");
            try {
                cfg.timeout = std::stoi(argv[++i]);
                if (cfg.timeout < 1 || cfg.timeout > 60) error_exit("таймаут должен быть от 1 до 60");
            } catch (...) { error_exit("неверный формат таймаута"); }
        }
        else if (arg[0] == '-') {
            error_exit("неизвестный параметр " + arg);
        }
        else {
            // Позиционный аргумент - это host
            if (!host_found) {
                cfg.host = arg;
                host_found = true;
            } else {
                error_exit("лишний позиционный аргумент: " + arg);
            }
        }
    }

    if (!host_found) error_exit("не указан сетевой адрес сервера");

    // 3. Выполнение программы
    try {
        std::cout << "Сервер: " << cfg.host << ":" << cfg.port << std::endl;
        std::cout << "Входной файл: " << cfg.input_file << std::endl;
        
        // Чтение векторов
        std::vector<std::vector<int64_t>> vectors = read_vectors(cfg.input_file);
        std::cout << "Прочитано векторов: " << vectors.size() << std::endl;

        // Работа с сетью
        Client client(cfg.host, cfg.port, cfg.timeout);
        client.connect_to_server();
        client.authenticate(cfg.login, cfg.password);
        std::vector<int64_t> results = client.process_vectors(vectors);
        client.disconnect();

        // Запись результатов
        write_results(cfg.output_file, results);
        std::cout << "Результаты сохранены в " << cfg.output_file << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
