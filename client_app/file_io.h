#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
#include <cstdint>

class DataReader {
public:
    static std::vector<std::vector<int64_t>> read(const std::string& filename);
};

class DataWriter {
public:
    static void write(const std::string& filename, const std::vector<int64_t>& results);
};

#endif
