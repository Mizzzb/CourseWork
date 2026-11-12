#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>

struct Config {
    std::string host;
    int port = 33333;
    std::string input_file;
    std::string output_file;
    std::string login;
    std::string password;
};

class UserInterface {
public:
    static Config parse_args(int argc, char* argv[]);
    static void print_help();
};

#endif
