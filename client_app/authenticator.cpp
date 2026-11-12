#include "authenticator.h"
#include <openssl/md5.h>
#include <cstdio>

std::string Authenticator::calculate_md5(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    char hex[MD5_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        sprintf(&hex[i * 2], "%02X", (unsigned int)digest[i]);
    return std::string(hex);
}
