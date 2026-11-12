#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <string>

class Authenticator {
public:
    static std::string calculate_md5(const std::string& input);
};

#endif
