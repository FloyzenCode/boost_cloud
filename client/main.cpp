#include <iostream>
#include "src/client.hpp"

auto main(int argc, char *argv[]) -> int
{
    cloud::Client client;
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("localhost", "12345");

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    if (socket.is_open())
    {
        std::string file_path;
        std::cout << "Введите путь к файлу для отправки на сервер: ";
        std::cin >> file_path;

        client.send_file(socket, file_path);
    }
    else
    {
        std::cerr << "Ошибка: не удалось подключиться к серверу." << std::endl;
    }

    return 0;
}
