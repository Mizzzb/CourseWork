#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>

class ProgramError : public std::runtime_error {
public:
    explicit ProgramError(const std::string& msg) : std::runtime_error(msg) {}
};

class IOError : public ProgramError {
public:
    explicit IOError(const std::string& msg) : ProgramError("File I/O Error: " + msg) {}
};

class NetworkError : public ProgramError {
public:
    explicit NetworkError(const std::string& msg) : ProgramError("Network Error: " + msg) {}
};

class AuthError : public ProgramError {
public:
    explicit AuthError(const std::string& msg) : ProgramError("Auth Error: " + msg) {}
};

#endif
