#include "file_io.h"
#include "errors.h"
#include <fstream>

std::vector<std::vector<int64_t>> DataReader::read(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw IOError("Не удалось открыть файл: " + filename);

    uint32_t N;
    file.read(reinterpret_cast<char*>(&N), sizeof(N));
    
    std::vector<std::vector<int64_t>> vectors(N);
    for (uint32_t i = 0; i < N; ++i) {
        uint32_t S;
        file.read(reinterpret_cast<char*>(&S), sizeof(S));
        vectors[i].resize(S);
        file.read(reinterpret_cast<char*>(vectors[i].data()), S * sizeof(int64_t));
    }
    return vectors;
}

void DataWriter::write(const std::string& filename, const std::vector<int64_t>& results) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw IOError("Не удалось создать файл: " + filename);

    uint32_t N = results.size();
    file.write(reinterpret_cast<const char*>(&N), sizeof(N));
    file.write(reinterpret_cast<const char*>(results.data()), N * sizeof(int64_t));
}
