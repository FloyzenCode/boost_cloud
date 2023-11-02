#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void handle_client(tcp::socket &socket)
{
    // Получение размера файла от клиента
    std::uint32_t file_size_net;
    boost::asio::read(socket, boost::asio::buffer(&file_size_net, sizeof(file_size_net)));
    std::uint32_t file_size = ntohl(file_size_net);

    // Получение имени файла от клиента
    boost::asio::streambuf filename_buffer;
    char character;
    while (boost::asio::read(socket, boost::asio::buffer(&character, 1)))
    {
        if (character == '\0')
        {
            break;
        }
        boost::asio::buffer_cast<char *>(boost::asio::buffer(filename_buffer.prepare(1)))[0] = character;
        filename_buffer.commit(1);
    }

    std::istream filename_stream(&filename_buffer);
    std::string filename;
    std::getline(filename_stream, filename, '\0');

    // Получение файла от клиента
    std::ofstream output_file(filename, std::ios::binary);
    if (!output_file.is_open())
    {
        std::cerr << "Невозможно открыть файл для записи: " << filename << std::endl;
        return;
    }

    std::vector<char> buffer(1024);
    std::size_t bytes_transferred = 0;
    while (bytes_transferred < file_size)
    {
        std::size_t read_size = std::min(buffer.size(), file_size - bytes_transferred);
        std::size_t received = boost::asio::read(socket, boost::asio::buffer(buffer.data(), read_size));
        output_file.write(buffer.data(), received);
        bytes_transferred += received;
    }

    output_file.close();
    std::cout << "Файл \"" << filename << "\" сохранен на сервере." << std::endl;
}

auto main(int argc, char *argv[]) -> int
{
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
    std::cout << "Сервер запущен. Ожидание подключений..." << std::endl;

    while (true)
    {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Клиент подключен: " << socket.remote_endpoint() << std::endl;

        try
        {
            handle_client(socket);
        }
        catch (std::exception &e)
        {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}
