#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string get_filename(const std::string &file_path)
{
    size_t pos = file_path.find_last_of("/\\");
    return (pos == std::string::npos) ? file_path : file_path.substr(pos + 1);
}

void send_file(tcp::socket &socket, const std::string &file_path)
{
    try
    {
        std::string filename = get_filename(file_path);

        std::ifstream input_file(file_path, std::ios::binary);
        if (!input_file.is_open())
        {
            std::cerr << "Невозможно открыть файл: " << file_path << std::endl;
            return;
        }

        // Отправка размера файла серверу
        input_file.seekg(0, std::ios::end);
        int32_t file_size = static_cast<int32_t>(input_file.tellg());
        input_file.seekg(0, std::ios::beg);
        int32_t file_size_net = htonl(file_size);
        boost::asio::write(socket, boost::asio::buffer(reinterpret_cast<const char *>(&file_size_net), sizeof(file_size_net)));

        // Отправка имени файла серверу
        boost::asio::write(socket, boost::asio::buffer(filename + '\0'));

        // Отправка файла серверу
        std::vector<char> buffer(1024);
        while (!input_file.eof())
        {
            input_file.read(buffer.data(), buffer.size());
            std::size_t bytes_read = static_cast<std::size_t>(input_file.gcount());
            if (bytes_read > 0)
                boost::asio::write(socket, boost::asio::buffer(buffer.data(), bytes_read));
        }

        input_file.close();

        if (socket.is_open())
            socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

        std::cout << "Файл \"" << file_path << "\" успешно отправлен на сервер." << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}

auto main(int argc, char *argv[]) -> int
{
    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("localhost", "12345");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    if (socket.is_open())
    {
        std::string file_path;
        std::cout << "Введите путь к файлу для отправки на сервер: ";
        std::cin >> file_path;

        send_file(socket, file_path);
    }
    else
    {
        std::cerr << "Ошибка: не удалось подключиться к серверу." << std::endl;
    }

    return 0;
}
