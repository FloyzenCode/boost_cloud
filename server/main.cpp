#include <iostream>
#include "src/server.hpp"

auto main(int argc, char *argv[]) -> int
{
    server::Server server;
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));
    std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;

    while (true)
    {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Клиент подключен: " << socket.remote_endpoint() << std::endl;

        try
        {
            server.handle_client(socket);
        }
        catch (std::exception &e)
        {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}
