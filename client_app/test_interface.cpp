#include <iostream>
#include <string>
#include "interface.h"
#include "errors.h"

void test_interface() {
    // Тест 1: Корректные аргументы
    const char* argv1[] = {"./client", "192.168.1.1", "-i", "in.bin", "-o", "out.bin"};
    int argc1 = 6;
    try {
        Config cfg = UserInterface::parse_args(argc1, const_cast<char**>(argv1));
        if (cfg.host == "192.168.1.1" && cfg.input_file == "in.bin" && cfg.output_file == "out.bin") {
            std::cout << "[PASS] Парсинг аргументов верен.\n";
        } else {
            std::cerr << "[FAIL] Данные распарсены неверно.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] Неожиданная ошибка: " << e.what() << "\n";
    }

    // Тест 2: Отсутствует host (должно выбросить исключение)
    const char* argv2[] = {"./client", "-i", "in.bin", "-o", "out.bin"};
    int argc2 = 5;
    try {
        UserInterface::parse_args(argc2, const_cast<char**>(argv2));
        std::cerr << "[FAIL] Отсутствие host не вызвало ошибку!\n";
    } catch (const ProgramError& e) {
        std::cout << "[PASS] Отсутствие host отловлено верно: " << e.what() << "\n";
    }
}
