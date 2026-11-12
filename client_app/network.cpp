#include "network.h"
#include "errors.h"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

Communicator::Communicator(const std::string& host, int port) : host_(host), port_(port), sockfd_(-1) {}
Communicator::~Communicator() { disconnect(); }

void Communicator::connect() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) throw NetworkError("Ошибка создания сокета");

    struct hostent* server = gethostbyname(host_.c_str());
    if (!server) throw NetworkError("Сервер не найден: " + host_);

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port_);

    if (::connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        throw NetworkError("Ошибка подключения к " + host_);
    }
}

void Communicator::send_bytes(const void* data, size_t len) {
    const char* ptr = static_cast<const char*>(data);
    while (len > 0) {
        ssize_t sent = send(sockfd_, ptr, len, 0);
        if (sent < 0) throw NetworkError("Ошибка отправки данных");
        ptr += sent;
        len -= sent;
    }
}

void Communicator::recv_bytes(void* data, size_t len) {
    char* ptr = static_cast<char*>(data);
    while (len > 0) {
        ssize_t received = recv(sockfd_, ptr, len, 0);
        if (received <= 0) throw NetworkError("Соединение закрыто или ошибка приема");
        ptr += received;
        len -= received;
    }
}

void Communicator::disconnect() {
    if (sockfd_ >= 0) {
        close(sockfd_);
        sockfd_ = -1;
    }
}
