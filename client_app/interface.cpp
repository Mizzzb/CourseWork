#include "interface.h"
#include "errors.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>

void UserInterface::print_help() {
    std::cout << "Использование: ./client <host> -i <input> -o <output> [-p <port>] [-c <config>]\n";
}

Config UserInterface::parse_args(int argc, char* argv[]) {
    Config cfg;
    bool host_found = false;

    if (argc == 1) { // Запуск без параметров
        print_help();
        exit(0);
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_help();
            exit(0);
        } else if (arg == "-p" || arg == "--port") {
            if (i + 1 < argc) cfg.port = std::stoi(argv[++i]);
        } else if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) cfg.input_file = argv[++i];
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) cfg.output_file = argv[++i];
        } else if (arg[0] != '-') {
            if (!host_found) { cfg.host = arg; host_found = true; }
        }
    }

    if (!host_found || cfg.input_file.empty() || cfg.output_file.empty()) {
        throw ProgramError("Обязательные параметры не указаны (host, input, output)");
    }

    // Упрощенно: читаем из файла, если он есть.
    std::string conf_path = std::string(getenv("HOME")) + "/.config/vclient.conf";
    std::ifstream conf(conf_path);
    if (conf.is_open()) {
        std::getline(conf, cfg.login);
        std::getline(conf, cfg.password);
    } else {
        cfg.login = "guest";
        cfg.password = "12345";
    }

    return cfg;
}
