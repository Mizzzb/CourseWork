#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <vector>
#include <cstdint>

class Communicator {
public:
    Communicator(const std::string& host, int port);
    ~Communicator();

    void connect();
    void send_bytes(const void* data, size_t len);
    void recv_bytes(void* data, size_t len);
    void disconnect();

private:
    std::string host_;
    int port_;
    int sockfd_;
};

#endif
