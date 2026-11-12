#include <iostream>

// Объявления функций тестов
void test_file_io();
void test_authenticator();
void test_interface();

int main() {
    std::cout << "=== ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ ===\n\n";

    std::cout << "--- Тестирование File I/O ---\n";
    test_file_io();
    std::cout << "\n";

    std::cout << "--- Тестирование Authenticator ---\n";
    test_authenticator();
    std::cout << "\n";

    std::cout << "--- Тестирование Interface ---\n";
    test_interface();
    std::cout << "\n";

    std::cout << "=== ТЕСТИРОВАНИЕ ЗАВЕРШЕНО ===\n";
    return 0;
}
