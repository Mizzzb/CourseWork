#include <iostream>
#include "interface.h"
#include "file_io.h"
#include "network.h"
#include "authenticator.h"
#include "errors.h"

int main(int argc, char* argv[]) {
    try {
        // 1. Парсинг аргументов
        Config cfg = UserInterface::parse_args(argc, argv);
        
        // 2. Чтение данных
        auto vectors = DataReader::read(cfg.input_file);
        std::cout << "Прочитано векторов: " << vectors.size() << std::endl;

        // 3. Сеть и аутентификация
        Communicator net(cfg.host, cfg.port);
        net.connect();

        // Аутентификация
        net.send_bytes(cfg.login.c_str(), cfg.login.length());
        char salt16[17] = {0};
        net.recv_bytes(salt16, 16);
        if (std::string(salt16) == "ERR") throw AuthError("Логин неверен");

        std::string hash = Authenticator::calculate_md5(std::string(salt16) + cfg.password);
        net.send_bytes(hash.c_str(), hash.length());
        
        char ok[3] = {0};
        net.recv_bytes(ok, 2);
        if (std::string(ok) != "OK") throw AuthError("Пароль неверен");

        // 4. Передача данных
        uint32_t N = vectors.size();
        net.send_bytes(&N, sizeof(N));

        std::vector<int64_t> results;
        for (const auto& vec : vectors) {
            uint32_t S = vec.size();
            net.send_bytes(&S, sizeof(S));
            net.send_bytes(vec.data(), S * sizeof(int64_t));
            
            int64_t res;
            net.recv_bytes(&res, sizeof(res));
            results.push_back(res);
        }
        net.disconnect();

        // 5. Запись результатов
        DataWriter::write(cfg.output_file, results);
        std::cout << "Результаты сохранены в " << cfg.output_file << std::endl;

    } catch (const ProgramError& e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "НЕИЗВЕСТНАЯ ОШИБКА: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
