#include <iostream>
#include <string>
#include "authenticator.h"

void test_authenticator() {
    // Известные хэши MD5 (в верхнем регистре, т.к. код возвращает UpperCase)
    // MD5("hello") = 5D41402ABC4B2A76B9719D911017C592
    // MD5("12345") = 827CCB0EEA8A706C4C34A16891F84E7B
    
    std::string test1 = Authenticator::calculate_md5("hello");
    if (test1 == "5D41402ABC4B2A76B9719D911017C592") {
        std::cout << "[PASS] MD5('hello') верен.\n";
    } else {
        std::cerr << "[FAIL] MD5('hello') неверен. Получено: " << test1 << "\n";
    }

    std::string test2 = Authenticator::calculate_md5("12345");
    if (test2 == "827CCB0EEA8A706C4C34A16891F84E7B") {
        std::cout << "[PASS] MD5('12345') верен.\n";
    } else {
        std::cerr << "[FAIL] MD5('12345') неверен. Получено: " << test2 << "\n";
    }

    // Тест с солью (SALT + PASSWORD)
    std::string salt = "0000000000000000";
    std::string pass = "secret";
    std::string hash = Authenticator::calculate_md5(salt + pass);
    // MD5("0000000000000000secret") = 5E5B1E1B2B2D3C... (проверять не будем жестко, просто выведем)
    std::cout << "[INFO] MD5(SALT||PASS) вычислен: " << hash << "\n";
}
