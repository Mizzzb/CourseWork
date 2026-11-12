#include <iostream>
#include <vector>
#include <cassert>
#include <cstdio>
#include "file_io.h"
#include "errors.h"

void test_file_io() {
    std::string test_file = "test_data.bin";
    
    // 1. Создаем тестовые данные
    std::vector<int64_t> test_results = {10, 20, 30, 40, 50};
    
    // 2. Тестируем запись
    try {
        DataWriter::write(test_file, test_results);
        std::cout << "[PASS] Запись в файл прошла успешно.\n";
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] Ошибка записи: " << e.what() << "\n";
        return;
    }

    // 3. Тестируем чтение
    try {
        FILE* f = fopen(test_file.c_str(), "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            fclose(f);
            assert(size > 0);
            std::cout << "[PASS] Файл создан, размер: " << size << " байт.\n";
        } else {
            std::cerr << "[FAIL] Файл не создан.\n";
        }
        
        // Удаляем тестовый файл
        std::remove(test_file.c_str());
    } catch (const std::exception& e) {
        std::cerr << "[FAIL] Ошибка проверки файла: " << e.what() << "\n";
    }
}
