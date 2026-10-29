#include "client.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/md5.h>

// ================= ФАЙЛОВЫЕ ОПЕРАЦИИ =================
std::vector<std::vector<int64_t>> read_vectors(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Не удалось открыть файл: " + filename);

    uint32_t N;
    file.read(reinterpret_cast<char*>(&N), sizeof(N));
    if (!file) throw std::runtime_error("Ошибка чтения количества векторов");

    std::vector<std::vector<int64_t>> vectors(N);
    for (uint32_t i = 0; i < N; ++i) {
        uint32_t S;
        file.read(reinterpret_cast<char*>(&S), sizeof(S));
        vectors[i].resize(S);
        file.read(reinterpret_cast<char*>(vectors[i].data()), S * sizeof(int64_t));
    }
    return vectors;
}

void write_results(const std::string& filename, const std::vector<int64_t>& results) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Не удалось создать файл: " + filename);

    uint32_t N = results.size();
    file.write(reinterpret_cast<const char*>(&N), sizeof(N));
    file.write(reinterpret_cast<const char*>(results.data()), N * sizeof(int64_t));
}

// ================= СЕТЕВОЙ КЛИЕНТ =================
Client::Client(const std::string& host, int port, int timeout_sec) 
    : host_(host), port_(port), sockfd_(-1) {
    timeout_.tv_sec = timeout_sec;
    timeout_.tv_usec = 0;
}

Client::~Client() { disconnect(); }

void Client::connect_to_server() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) throw std::runtime_error("Ошибка создания сокета");

    // Установка таймаута
    setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout_, sizeof(timeout_));
    setsockopt(sockfd_, SOL_SOCKET, SO_SNDTIMEO, &timeout_, sizeof(timeout_));

    struct hostent* server = gethostbyname(host_.c_str());
    if (!server) throw std::runtime_error("Сервер не найден: " + host_);

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port_);

    if (connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Ошибка подключения (сервер недоступен или таймаут)");
    }
    std::cout << "Соединение установлено." << std::endl;
}

std::string Client::calculate_md5(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    char hex[MD5_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        sprintf(&hex[i * 2], "%02X", (unsigned int)digest[i]);
    return std::string(hex);
}

void Client::send_all(const void* data, size_t len) {
    const char* ptr = static_cast<const char*>(data);
    while (len > 0) {
        ssize_t sent = send(sockfd_, ptr, len, 0);
        if (sent < 0) throw std::runtime_error("Ошибка отправки данных (таймаут?)");
        ptr += sent;
        len -= sent;
    }
}

void Client::recv_all(void* data, size_t len) {
    char* ptr = static_cast<char*>(data);
    while (len > 0) {
        ssize_t received = recv(sockfd_, ptr, len, 0);
        if (received <= 0) throw std::runtime_error("Соединение закрыто или таймаут приема");
        ptr += received;
        len -= received;
    }
}

void Client::authenticate(const std::string& login, const std::string& password) {
    send_all(login.c_str(), login.length()); // 2. Передаем LOGIN

    char salt16[17] = {0};
    recv_all(salt16, 16); // 3. Получаем SALT
    std::string salt_str(salt16);
    
    if (salt_str == "ERR") throw std::runtime_error("Ошибка идентификации (логин неверен)");

    std::string hash_input = salt_str + password;
    std::string hash = calculate_md5(hash_input); // 4. Передаем хэш
    send_all(hash.c_str(), hash.length());

    char ok_resp[3] = {0};
    recv_all(ok_resp, 2); // 5. Получаем OK
    if (std::string(ok_resp) != "OK") throw std::runtime_error("Ошибка аутентификации (пароль неверен)");
    std::cout << "Аутентификация успешна." << std::endl;
}

std::vector<int64_t> Client::process_vectors(const std::vector<std::vector<int64_t>>& vectors) {
    uint32_t N = vectors.size();
    send_all(&N, sizeof(N)); // 6. Посылаем количество векторов

    std::vector<int64_t> results;
    for (size_t i = 0; i < vectors.size(); ++i) {
        uint32_t S = vectors[i].size();
        send_all(&S, sizeof(S)); // 7. Посылаем размер
        send_all(vectors[i].data(), S * sizeof(int64_t)); // 8. Посылаем значения

        int64_t result;
        recv_all(&result, sizeof(result)); // 9. Получаем результат
        results.push_back(result);
    }
    return results;
}

void Client::disconnect() {
    if (sockfd_ >= 0) {
        close(sockfd_);
        sockfd_ = -1;
    }
}
