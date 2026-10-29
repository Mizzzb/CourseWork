#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <cstdint>

// Функции для работы с файлами (можно вынести в отдельный файл)
std::vector<std::vector<int64_t>> read_vectors(const std::string& filename);
void write_results(const std::string& filename, const std::vector<int64_t>& results);

class Client {
public:
    Client(const std::string& host, int port, int timeout_sec);
    ~Client();

    void connect_to_server();
    void authenticate(const std::string& login, const std::string& password);
    std::vector<int64_t> process_vectors(const std::vector<std::vector<int64_t>>& vectors);
    void disconnect();

private:
    std::string host_;
    int port_;
    int sockfd_;
    struct timeval timeout_;

    void send_all(const void* data, size_t len);
    void recv_all(void* data, size_t len);
    std::string calculate_md5(const std::string& input);
};

#endif
